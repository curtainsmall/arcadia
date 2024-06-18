#include "imgui_window_state.hpp"

#include"core/file/pfd_header.hpp"
#include"ui/imgui_header.hpp"
#include"ui/imgui_wrapper.hpp"
#include"resource/components/camera_component.hpp"
#include"resource/components/light_component.hpp"
#include"resource/components/model_component.hpp"
#include"resource/components/physics_component.hpp"

void ImguiWindowStateScene::operator()(const Scene& scene)
{
    ImGui::Text(std::format("Entity Count: {}", scene.Count([&](const std::string&, const EntityInfo& info)->bool
    {
        return !info.Internal;
    })).c_str());
}

void ImguiWindowStateRenderer::operator()(const iRenderer& renderer)
{
    auto graphic_api_type_str = Match<std::string>(
        renderer.GetGraphicApiType(),
        [&](const GraphicApi::Opengl& api)
    {
        return std::format("OpenGL ({})", api.version);
    },
        [&](const GraphicApi::Directx& api)
    {
        return std::format("DirectX ({})", api.version);
    },
        [&](const GraphicApi::Vulkan& api)
    {
        return std::format("Vulkan ({})", api.version);
    }
    );

    ImGui::Text(std::format("Graphic API: {}", graphic_api_type_str).c_str());

    ImGui::NewLine();
    ImGui::SeparatorText("Renderer Config");

    ImGui::Text("Renderer Type"); ImGui::SameLine();
    if(ImGui::BeginCombo("##renderer_type", graphic_api_type_str.c_str()))
    {
        ImGui::Selectable(graphic_api_type_str.c_str());

        ImGui::EndCombo();
    }
}

void ImguiWindowStatePhysicsSimulator::operator()(PhysicsSimulator& physics_simulator)
{
    const auto& physics_simulator_jph_body_id_storage = physics_simulator.GetJphBodyIdStorage();
    ImGui::Text(std::format("Body Count: {}", physics_simulator_jph_body_id_storage.size()).c_str());

    ImGui::NewLine();
    ImGui::SeparatorText("Physics Simulator Config");

    auto slider_flags =
        ImGuiSliderFlags_AlwaysClamp;

    ImGui::NewLine();
    ImGui::BeginDisabled();
    int temp_allocator_size_in_kib = physics_simulator.GetJphTempAllocatorSize() / 1024;
    ImguiWrapper::DragInt(
        "Temporary Allocator Size (KiB)",
        temp_allocator_size_in_kib,
        1.f,
        64, // 64 KiB
        16 * 1024 * 1024, // 16 GiB
        "%d",
        slider_flags
    );
    physics_simulator.SetJphTempAllocatorSize(temp_allocator_size_in_kib * 1024);
    ImGui::EndDisabled();

    ImGui::NewLine();
    ImguiWrapper::DragInt(
        "            Updates per Second",
        ACDA_BIND_MEMBER_FN_ARBITRARY(physics_simulator, GetJphPhysicsSystemUpdatesPerSecond),
        ACDA_BIND_MEMBER_FN_ARBITRARY(physics_simulator, SetJphPhysicsSystemUpdatesPerSecond),
        1.f,
        0,
        (std::numeric_limits<int>::max)(),
        "%d",
        slider_flags
    );

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
    if(ImGui::Button("Reset"))
    {
        physics_simulator.Reset();
        physics_simulator.SetActive(false);
    }

}

void ImguiWindowState::OnEvent(EventBase& event)
{
    EventDispatcher{ event }
        .Dispatch<Event::OpenImguiWindow>(ACDA_BIND_MEMBER_FN(_OnOpenImguiWindow))
        .Dispatch<Event::SceneActivated>(ACDA_BIND_MEMBER_FN(_OnSceneActivated))
        .Dispatch<Event::SceneDeactivated>(ACDA_BIND_MEMBER_FN(_OnSceneDeactivated))
        .Dispatch<Event::RendererBuilt>(ACDA_BIND_MEMBER_FN(_OnRendererBuilt))
        .Dispatch<Event::RendererUnbuilt>(ACDA_BIND_MEMBER_FN(_OnRendererUnbuilt))
        .Dispatch<Event::PhysicsSimulatorBuilt>(ACDA_BIND_MEMBER_FN(_OnPhysicsSimualtorBuilt))
        .Dispatch<Event::PhysicsSimulatorUnbuilt>(ACDA_BIND_MEMBER_FN(_OnPhysicsSimulatorUnbuilt))
        .Result();
}

void ImguiWindowState::OnUpdate()
{
    if(!_Open)
    {
        return;
    }

    auto scene = _Scene.lock();
    auto renderer = _Renderer.lock();
    auto physics_simualtor = _PhysicsSimulator.lock();

    auto imgui_window_title = _Title + GetIdStr();

    ImGui::SetNextWindowSize(glm::vec2{ 1024,768 }, ImGuiCond_Once);
    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_window_title.c_str(), &_Open, window_flags))
    {
        auto tabbar_flags =
            ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;
        if(ImGui::BeginTabBar("##tab_bar", tabbar_flags))
        {
            ImGui::PushItemWidth(200.f);

            if(ImGui::BeginTabItem("Scene"))
            {
                ImGui::SeparatorText("Scene State");
                if(scene)
                {
                    _ImguiWindowStateScene(*scene);
                }
                else
                {
                    ImGui::Text("(No scene)");
                }

                ImGui::EndTabItem();
            }
            if(ImGui::BeginTabItem("Renderer"))
            {
                ImGui::SeparatorText("Renderer State");
                if(renderer)
                {
                    _ImguiWindowStateRenderer(*renderer);
                }
                else
                {
                    ImGui::Text("(No renderer)");
                }

                ImGui::EndTabItem();
            }
            if(ImGui::BeginTabItem("Physics Simulator"))
            {
                ImGui::SeparatorText("Physics Simulator State");
                if(physics_simualtor)
                {
                    _ImguiWindowStatePhysicsSimulator(*physics_simualtor);
                }
                else
                {
                    ImGui::Text("(No physics simulator)");
                }

                ImGui::EndTabItem();
            }

            ImGui::PopItemWidth();
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

void ImguiWindowState::_OnOpenImguiWindow(Event::OpenImguiWindow& e)
{
    _Open = true;
}

void ImguiWindowState::_OnSceneActivated(Event::SceneActivated& e)
{
    const auto& [scene] = e.data_tuple;
    _Scene = scene;
}

void ImguiWindowState::_OnSceneDeactivated(Event::SceneDeactivated& e)
{
    _Scene.reset();
}

void ImguiWindowState::_OnRendererBuilt(Event::RendererBuilt& e)
{
    const auto& [renderer] = e.data_tuple;
    _Renderer = renderer;
}

void ImguiWindowState::_OnRendererUnbuilt(Event::RendererUnbuilt& e)
{
    _Renderer.reset();
}

void ImguiWindowState::_OnPhysicsSimualtorBuilt(Event::PhysicsSimulatorBuilt& e)
{
    const auto& [physics_simulator] = e.data_tuple;
    _PhysicsSimulator = physics_simulator;
}

void ImguiWindowState::_OnPhysicsSimulatorUnbuilt(Event::PhysicsSimulatorUnbuilt& e)
{
    _PhysicsSimulator.reset();
}


