#include "imgui_window_state.hpp"

#include "core/function.hpp"
#include "core/match.hpp"
#include "core/pfd.hpp"
#include "resource/components.hpp"

#include "ui/imgui.hpp"
#include "ui/imgui_wrapper.hpp"

void Arcadia::ImguiWindowStateFunctor_Scene::operator()(const std::shared_ptr<SceneLayer>& scene_layer_sptr)
{
    ImGui::Text(
        std::format(
            "Entity Count: {}",
            scene_layer_sptr->ActiveScene_GetEntityCount(
                [&](EntityId, const EntityInfo& info)->bool
                {
                    return !info.Internal;
                }
            )
        )
        .c_str()
    );
}

void Arcadia::ImguiWindowStateFunctor_Renderer::operator()(const std::shared_ptr<RendererLayer>& renderer_layer_sptr)
{
    std::string graphic_api_type_string = MatchVariant<std::string>(
        renderer_layer_sptr->GetCurrentGraphicApiType(),
        [&](const GraphicApi::Opengl& api)
        {
            return std::format("OpenGL ({})", api.Version);
        },
        [&](const GraphicApi::Directx& api)
        {
            return std::format("DirectX ({})", api.Version);
        },
        [&](const GraphicApi::Vulkan& api)
        {
            return std::format("Vulkan ({})", api.Version);
        }
    );

    if(ImGui::BeginCombo("Graphic API", graphic_api_type_string.c_str()))
    {
        ImGui::Selectable(graphic_api_type_string.c_str());
        ImGui::EndCombo();
    }
}

void Arcadia::ImguiWindowStateFunctor_PhysicsSimulator::operator()(const std::shared_ptr<PhysicsLayer>& physics_layer_sptr)
{
    ImGui::Text(std::format("Body Count: {}", physics_layer_sptr->GetPhysicsBodyCount()).c_str());
    ImGui::Text(std::format("Temporary Allocator Size (KiB): {}", physics_layer_sptr->GetTempAllocatorSize()).c_str());
    ImGui::Text(std::format("Update per Second: {}", physics_layer_sptr->GetUpdatesPerSecondCount()).c_str());

    ImGui::NewLine();

    if(physics_layer_sptr->IsPhysicsSimulatorActive())
    {
        if(ImGui::Button("Stop"))
        {
            EventQueue::Instance().Signal<Events::PhysicsSimulatirSetActive>(false);
        }
    }
    else
    {
        if(ImGui::Button("Start"))
        {
            EventQueue::Instance().Signal<Events::PhysicsSimulatirSetActive>(true);
        }
    }
    ImGui::SameLine();
    ImGui::BeginDisabled();
    if(ImGui::Button("Reset"))
    {
        EventQueue::Instance().Signal<Events::PhysicsSimulatorReset>();
        EventQueue::Instance().Signal<Events::PhysicsSimulatirSetActive>(false);
    }
    ImGui::EndDisabled();
}

void Arcadia::ImguiWindowStateFunctor_ScriptInterpreter::operator()(const std::shared_ptr<ScriptLayer>& script_layer_sptr)
{
}

void Arcadia::ImguiWindowStateFunctor_PlayerController::operator()(const std::shared_ptr<PlayerLayer>& player_layer_sptr)
{
    EntityId entity_id = player_layer_sptr->GetActiveEntityId();

    if(!entity_id.IsNull())
    {
        ImGui::Text(std::format("Active Player Entity Id: {}", entity_id).c_str());
    }
    else
    {
        ImGui::Text("No Active Entity");
    }
}

Arcadia::ImguiWindowState::ImguiWindowState(bool open, std::string_view title):
    ImguiWindowInterface(open, title)
{
}

void Arcadia::ImguiWindowState::OnEvent(EventBase& e)
{
    EventDispatcher{ e }
        .Dispatch<Events::OpenImguiWindow>(ACDA_BIND_MEMBER_FN(_OnOpenImguiWindow))
        .IsDispatched();
}

void Arcadia::ImguiWindowState::OnUpdate()
{
    if(!_Opened)
    {
        return;
    }

    auto [
        scene_layer_sptr,
        renderer_layer_sptr,
        physcis_layer_sptr,
        player_layer_sptr
    ] = LayerStack::Instance().GetMultipleLayersShared<
        SceneLayer,
        RendererLayer,
        PhysicsLayer,
        PlayerLayer
    >();

    std::string imgui_window_title{};
    imgui_window_title
        .append(_Title)
        .append(GetIdString());

    ImGui::SetNextWindowSize(glm::vec2{ 1024, 768 }, ImGuiCond_Once);
    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_window_title.c_str(), &_Opened, window_flags))
    {
        auto tabbar_flags =
            ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;
        ImGui::PushItemWidth(200.f);

        _DisplayState(
            "Scene",
            true,
            [&]()
            {
                if(scene_layer_sptr->HasActiveScene())
                {
                    _ImguiWindowStateFunctor_Scene(scene_layer_sptr);
                }
                else
                {
                    ImGui::Text("(No Scene)");
                }
            }
        );

        _DisplayState(
            "Renderer",
            renderer_layer_sptr->IsRendererActive(),
            [&]()
            {
                if(renderer_layer_sptr)
                {
                    _ImguiWindowStateFunctor_Renderer(renderer_layer_sptr);
                }
                else
                {
                    ImGui::Text("(No renderer)");
                }
            }
        );

        _DisplayState(
            "Physics Simulator",
            physcis_layer_sptr->IsPhysicsSimulatorActive(),
            [&]()
            {
                if(physcis_layer_sptr)
                {
                    _ImguiWindowStateFunctor_PhysicsSimulator(physcis_layer_sptr);
                }
                else
                {
                    ImGui::Text("(No physics simulator)");
                }
            }
        );

        _DisplayState(
            "Player Controller",
            player_layer_sptr->IsPlayerControllerActive(),
            [&]()
            {
                if(player_layer_sptr)
                {
                    _ImguiWindowStateFunctor_PlayerController(player_layer_sptr);
                }
                else
                {
                    ImGui::Text("No Player Controller");
                }
            }
        );

        ImGui::PopItemWidth();
    }
    ImGui::End();
}

void Arcadia::ImguiWindowState::_OnOpenImguiWindow(Events::OpenImguiWindow& e)
{
    if(e.IdString == GetIdString())
    {
        _Opened = true;
    }
}

void Arcadia::ImguiWindowState::_DisplayState(std::string_view tab_name, bool is_activated, const std::function<void()>& display_fn) const
{
    static std::string_view state{};
    state = is_activated ? "Activated" : "Inactivated";
    if(ImGui::TreeNodeEx(std::format("{} | {}", tab_name, state).c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding))
    {
        ImGui::BeginGroup();
        display_fn();
        ImGui::EndGroup();
        ImGui::TreePop();
    }
}