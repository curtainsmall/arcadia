#include "imgui_window_state.hpp"

#include"core/file/pfd_header.hpp"
#include"core/function.hpp"
#include"core/match.hpp"
#include"resource/components/camera_component.hpp"
#include"resource/components/light_component.hpp"
#include"resource/components/model_component.hpp"
#include"resource/components/physics_component.hpp"
#include"ui/imgui_header.hpp"
#include"ui/imgui_wrapper.hpp"

void Arcadia::ImguiWindowStateFunctor_Scene::operator()(const Scene& scene)
{
    ImGui::Text(std::format("Entity Count: {}", scene.CountEntity([&](EntityId, const EntityInfo& info)->bool
    {
        return !info.Internal;
    })).c_str());
}

void Arcadia::ImguiWindowStateFunctor_Renderer::operator()(const RendererInterface& renderer)
{
    auto graphic_api_type_string = MatchVariant<std::string>(
        renderer.GetGraphicApiType(),
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

void Arcadia::ImguiWindowStateFunctor_PhysicsSimulator::operator()(PhysicsSimulator& physics_simulator)
{
    const auto& physics_simulator_jph_body_id_storage = physics_simulator.GetJphBodyIdStorage();
    ImGui::Text(std::format("Body Count: {}", physics_simulator_jph_body_id_storage.size()).c_str());

    auto slider_flags =
        ImGuiSliderFlags_AlwaysClamp;
    ImGui::BeginDisabled();
    std::int32_t temp_allocator_size_in_kib = physics_simulator.GetJphTempAllocatorSize() / 1024;
    ImguiWrappers::DragInt("Temporary Allocator Size (KiB)", temp_allocator_size_in_kib, 1.0f, 64 /*64 KiB*/, 16 * 1024 * 1024 /*16 GiB*/, "%d", slider_flags);
    physics_simulator.SetJphTempAllocatorSize(temp_allocator_size_in_kib * 1024);
    ImGui::EndDisabled();

    auto update_per_second = physics_simulator.GetJphPhysicsSystemUpdatesPerSecond();
    ImguiWrappers::DragInt("Update per Second", update_per_second, 1.0f, 0, INT_MAX, "%d", slider_flags);
    physics_simulator.SetJphPhysicsSystemUpdatesPerSecond(update_per_second);

    ImGui::NewLine();

    if(physics_simulator.IsActive())
    {
        if(ImGui::Button("Stop"))
        {
            physics_simulator.SetActive(false);
        }
    }
    else
    {
        if(ImGui::Button("Start"))
        {
            physics_simulator.SetActive(true);
        }
    }
    ImGui::SameLine();
    ImGui::BeginDisabled();
    if(ImGui::Button("Reset"))
    {
        physics_simulator.Reset();
        physics_simulator.SetActive(false);
    }
    ImGui::EndDisabled();
}

void Arcadia::ImguiWindowState::OnEvent(EventBase& e)
{
    EventDispatcher{ e }
        .Dispatch<Events::OpenImguiWindow>(ACDA_BIND_MEMBER_FN(_OnOpenImguiWindow))
        .Dispatch<Events::SceneActivated>(ACDA_BIND_MEMBER_FN(_OnSceneActivated))
        .Dispatch<Events::SceneDeactivated>(ACDA_BIND_MEMBER_FN(_OnSceneDeactivated))
        .Dispatch<Events::RendererBuilt>(ACDA_BIND_MEMBER_FN(_OnRendererBuilt))
        .Dispatch<Events::RendererUnbuilt>(ACDA_BIND_MEMBER_FN(_OnRendererUnbuilt))
        .Dispatch<Events::PhysicsSimulatorBuilt>(ACDA_BIND_MEMBER_FN(_OnPhysicsSimulatorBuilt))
        .Dispatch<Events::PhysicsSimulatorUnbuilt>(ACDA_BIND_MEMBER_FN(_OnPhysicsSimulatorUnbuilt))
        .IsDispatched();
}

void Arcadia::ImguiWindowState::OnUpdate()
{
    if(!_Opened)
    {
        return;
    }

    auto scene = _SceneWeakPtr.lock();
    auto renderer = _Renderer.lock();
    auto physics_simualtor = _PhysicsSimulator.lock();

    auto imgui_window_title = _Title + GetIdString();

    ImGui::SetNextWindowSize(glm::vec2{ 1024,768 }, ImGuiCond_Once);
    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_window_title.c_str(), &_Opened, window_flags))
    {
        auto tabbar_flags =
            ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;
        ImGui::PushItemWidth(200.f);

        if(ImGui::TreeNodeEx("Scene", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding))
        {
            if(scene)
            {
                _ImguiWindowStateFunctor_Scene(*scene);
            }
            else
            {
                ImGui::Text("(No scene)");
            }
            ImGui::TreePop();
        }
        if(ImGui::TreeNodeEx("Renderer", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding))
        {
            if(renderer)
            {
                _ImguiWindowStateFunctor_Renderer(*renderer);
            }
            else
            {
                ImGui::Text("(No renderer)");
            }
            ImGui::TreePop();
        }
        if(ImGui::TreeNodeEx("Physics Simulator", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding))
        {
            if(physics_simualtor)
            {
                _ImguiWindowStateFunctor_PhysicsSimulator(*physics_simualtor);
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
    _Opened = true;
}

void Arcadia::ImguiWindowState::_OnSceneActivated(Events::SceneActivated& e)
{
    _SceneWeakPtr = e.spScene;
}

void Arcadia::ImguiWindowState::_OnSceneDeactivated(Events::SceneDeactivated& e)
{
    _SceneWeakPtr.reset();
}

void Arcadia::ImguiWindowState::_OnRendererBuilt(Events::RendererBuilt& e)
{
    _Renderer = e.spRenderer;
}

void Arcadia::ImguiWindowState::_OnRendererUnbuilt(Events::RendererUnbuilt& e)
{
    _Renderer.reset();
}

void Arcadia::ImguiWindowState::_OnPhysicsSimulatorBuilt(Events::PhysicsSimulatorBuilt& e)
{
    _PhysicsSimulator = e.spPhysicsSimulator;
}

void Arcadia::ImguiWindowState::_OnPhysicsSimulatorUnbuilt(Events::PhysicsSimulatorUnbuilt& e)
{
    _PhysicsSimulator.reset();
}