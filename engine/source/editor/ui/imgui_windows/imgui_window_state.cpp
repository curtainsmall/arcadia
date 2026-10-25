#include "imgui_window_state.hpp"

#include "core/function.hpp"
#include "core/match.hpp"
#include "core/pfd.hpp"
#include "resource/components.hpp"

#include "ui/imgui.hpp"
#include "ui/imgui_wrapper.hpp"

void Arcadia::ImguiWindowStateFunctor_Scene::operator()(SceneService* scene_service)
{
    ImGui::Text(
        std::format(
            "Entity Count: {}",
            scene_service->ActiveScene_GetEntityCount(
                [&](EntityId, const EntityInfo& info)->bool
                {
                    return !info.Internal;
                }
            )
        )
        .c_str()
    );
}

void Arcadia::ImguiWindowStateFunctor_Renderer::operator()(RendererService* renderer_service)
{
    std::string graphic_api_type_string = MatchVariant<std::string>(
        renderer_service->GetCurrentGraphicApiType(),
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

void Arcadia::ImguiWindowStateFunctor_PhysicsSimulator::operator()(PhysicsService* physics_service)
{
    ImGui::Text(std::format("Body Count: {}", physics_service->GetPhysicsBodyCount()).c_str());
    ImGui::Text(std::format("Temporary Allocator Size (KiB): {}", physics_service->GetTempAllocatorSize()).c_str());
    ImGui::Text(std::format("Update per Second: {}", physics_service->GetUpdatesPerSecondCount()).c_str());

    ImGui::NewLine();
#if 0
    if(physics_layer_sptr->IsPhysicsSimulatorActive())
    {
        if(ImGui::Button("Stop"))
        {
            _Engine.events.Signal<Events::PhysicsSimulatorSetActive>(false);
        }
    }
    else
    {
        if(ImGui::Button("Start"))
        {
            _Engine.events.Signal<Events::PhysicsSimulatorSetActive>(true);
        }
    }
    ImGui::SameLine();
    ImGui::BeginDisabled();
    if(ImGui::Button("Reset"))
    {
        _Engine.events.Signal<Events::PhysicsSimulatorReset>();
        _Engine.events.Signal<Events::PhysicsSimulatorSetActive>(false);
    }
    ImGui::EndDisabled();
#endif
}

void Arcadia::ImguiWindowStateFunctor_ScriptInterpreter::operator()(ScriptService* script_service)
{
}

void Arcadia::ImguiWindowStateFunctor_PlayerController::operator()(PlayerService* player_service)
{
    EntityId entity_id = player_service->GetActiveEntityId();

    if(!entity_id.IsNull())
    {
        ImGui::Text(std::format("Active Player Entity Id: {}", entity_id).c_str());
    }
    else
    {
        ImGui::Text("No Active Entity");
    }
}

Arcadia::ImguiWindowState::ImguiWindowState(Engine& engine, bool open, std::string_view title):
    ImguiWindowInterface(engine, open, title)
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

    SceneService* scene_service = _Engine.scenes;
    RendererService* renderer_service = _Engine.renderer_service;
    PhysicsService* physics_service = _Engine.physics_service;
    PlayerService* player_service = _Engine.player_service;

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
                if(scene_service->HasActiveScene())
                {
                    _ImguiWindowStateFunctor_Scene(scene_service);
                }
                else
                {
                    ImGui::Text("(No Scene)");
                }
            }
        );

        _DisplayState(
            "Renderer",
            renderer_service->IsRendererActive(),
            [&]()
            {
                if(renderer_service)
                {
                    _ImguiWindowStateFunctor_Renderer(renderer_service);
                }
                else
                {
                    ImGui::Text("(No renderer)");
                }
            }
        );

        _DisplayState(
            "Physics Simulator",
            physics_service->IsPhysicsSimulatorActive(),
            [&]()
            {
                if(physics_service)
                {
                    _ImguiWindowStateFunctor_PhysicsSimulator(physics_service);
                }
                else
                {
                    ImGui::Text("(No physics simulator)");
                }
            }
        );

        _DisplayState(
            "Player Controller",
            player_service->IsPlayerControllerActive(),
            [&]()
            {
                if(player_service)
                {
                    _ImguiWindowStateFunctor_PlayerController(player_service);
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