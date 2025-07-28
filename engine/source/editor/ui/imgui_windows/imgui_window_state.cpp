#include "imgui_window_state.hpp"

#include "core/function.hpp"
#include "core/match.hpp"
#include "core/pfd.hpp"
#include "resource/components.hpp"

#include "ui/imgui.hpp"
#include "ui/imgui_wrapper.hpp"

void Arcadia::ImguiWindowStateFunctor_Scene::operator()(const std::shared_ptr<SceneLayer>& scene_layer)
{
    ImGui::Text(
        std::format(
            "Entity Count: {}",
            scene_layer->ActiveScene_GetEntityCount(
                [&](EntityId, const EntityInfo& info)->bool
                {
                    return !info.Internal;
                }
            )
        )
        .c_str()
    );
}

void Arcadia::ImguiWindowStateFunctor_Renderer::operator()(const std::shared_ptr<RendererLayer>& renderer_layer)
{
    std::string graphic_api_type_string = MatchVariant<std::string>(
        renderer_layer->GetCurrentGraphicApiType(),
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

void Arcadia::ImguiWindowStateFunctor_PhysicsSimulator::operator()(const std::shared_ptr<PhysicsLayer>& physics_layer)
{
    ImGui::Text(std::format("Body Count: {}", physics_layer->GetPhysicsBodyCount()).c_str());
    ImGui::Text(std::format("Temporary Allocator Size (KiB)", physics_layer->GetTempAllocatorSize()).c_str());
    ImGui::Text(std::format("Update per Second", physics_layer->GetUpdatesPerSecondCount()).c_str());

    ImGui::NewLine();

    if(physics_layer->IsPhysicsSimulatorActive())
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

void Arcadia::ImguiWindowStateFunctor_ScriptInterpreter::operator()(const std::shared_ptr<ScriptLayer>& script_layer)
{
}

Arcadia::ImguiWindowState::ImguiWindowState(bool open, const std::string& title):
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

    auto [scene_layer_sptr, renderer_layer_sptr, physcis_layer] = LayerStack::Instance().GetMultipleLayersShared<SceneLayer, RendererLayer, PhysicsLayer>();

    std::string imgui_window_title = _Title + GetIdString();

    ImGui::SetNextWindowSize(glm::vec2{ 1024, 768 }, ImGuiCond_Once);
    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_window_title.c_str(), &_Opened, window_flags))
    {
        auto tabbar_flags =
            ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;
        ImGui::PushItemWidth(200.f);

        if(ImGui::TreeNodeEx("Scene", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding))
        {
            if(scene_layer_sptr->HasActiveScene())
            {
                _ImguiWindowStateFunctor_Scene(scene_layer_sptr);
            }
            else
            {
                ImGui::Text("(No scene)");
            }
            ImGui::TreePop();
        }
        if(ImGui::TreeNodeEx("Renderer", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding))
        {
            if(renderer_layer_sptr)
            {
                _ImguiWindowStateFunctor_Renderer(renderer_layer_sptr);
            }
            else
            {
                ImGui::Text("(No renderer)");
            }
            ImGui::TreePop();
        }
        if(ImGui::TreeNodeEx("Physics Simulator", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding))
        {
            if(physcis_layer)
            {
                _ImguiWindowStateFunctor_PhysicsSimulator(physcis_layer);
            }
            else
            {
                ImGui::Text("(No physics simulator)");
            }
            ImGui::TreePop();
        }

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