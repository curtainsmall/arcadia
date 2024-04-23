#include "imgui_window_state.hpp"

#include"core/file/pfd_header.hpp"
#include"ui/imgui_header.hpp"
#include"ui/imgui_wrapper.hpp"
#include"resource/components/camera_component.hpp"
#include"resource/components/light_component.hpp"
#include"resource/components/model_component.hpp"
#include"resource/components/physics_component.hpp"

void Arcadia::ImguiWindowStateScene::operator()(const Arcadia::Scene& scene)
{
    ImGui::Text(std::format("Entity Count: {}", scene.Count([&](const std::string&, const Arcadia::EntityInfo& info)->bool
    {
        return !info.Internal;
    })).c_str());
}

void Arcadia::ImguiWindowStateRenderer::operator()(const Arcadia::iRenderer& renderer)
{
    auto graphic_api_type_str = Arcadia::Match<std::string>(
        renderer.GetGraphicApiType(),
        [&](const Arcadia::GraphicApi::Opengl& api)
    {
        return std::format("OpenGL ({})", api.version);
    },
        [&](const Arcadia::GraphicApi::Directx& api)
    {
        return std::format("DirectX ({})", api.version);
    },
        [&](const Arcadia::GraphicApi::Vulkan& api)
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

void Arcadia::ImguiWindowStatePhysicsSimulator::operator()(Arcadia::PhysicsSimulator& physics_simulator)
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
    Arcadia::ImguiWrapper::DragInt(
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
    Arcadia::ImguiWrapper::DragInt(
        "            Updates per Second",
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(physics_simulator, GetJphPhysicsSystemUpdatesPerSecond),
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(physics_simulator, SetJphPhysicsSystemUpdatesPerSecond),
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

void Arcadia::ImguiWindowState::OnEvent(Arcadia::EventBase& event)
{
    Arcadia::EventDispatcher{ event }
        .Dispatch<Arcadia::Event::OpenImguiWindow>(ARCADIA_BIND_MEMBER_FN(_OnOpenImguiWindow))
        .Dispatch<Arcadia::Event::SceneActivated>(ARCADIA_BIND_MEMBER_FN(_OnSceneActivated))
        .Dispatch<Arcadia::Event::SceneDeactivated>(ARCADIA_BIND_MEMBER_FN(_OnSceneDeactivated))
        .Dispatch<Arcadia::Event::RendererBuilt>(ARCADIA_BIND_MEMBER_FN(_OnRendererBuilt))
        .Dispatch<Arcadia::Event::RendererUnbuilt>(ARCADIA_BIND_MEMBER_FN(_OnRendererUnbuilt))
        .Dispatch<Arcadia::Event::PhysicsSimulatorBuilt>(ARCADIA_BIND_MEMBER_FN(_OnPhysicsSimualtorBuilt))
        .Dispatch<Arcadia::Event::PhysicsSimulatorUnbuilt>(ARCADIA_BIND_MEMBER_FN(_OnPhysicsSimulatorUnbuilt))
        .Result();
}

void Arcadia::ImguiWindowState::OnUpdate()
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

void Arcadia::ImguiWindowState::_OnOpenImguiWindow(Arcadia::Event::OpenImguiWindow& e)
{
    _Open = true;
}

void Arcadia::ImguiWindowState::_OnSceneActivated(Arcadia::Event::SceneActivated& e)
{
    const auto& [scene] = e.data_tuple;
    _Scene = scene;
}

void Arcadia::ImguiWindowState::_OnSceneDeactivated(Arcadia::Event::SceneDeactivated& e)
{
    _Scene.reset();
}

void Arcadia::ImguiWindowState::_OnRendererBuilt(Arcadia::Event::RendererBuilt& e)
{
    const auto& [renderer] = e.data_tuple;
    _Renderer = renderer;
}

void Arcadia::ImguiWindowState::_OnRendererUnbuilt(Arcadia::Event::RendererUnbuilt& e)
{
    _Renderer.reset();
}

void Arcadia::ImguiWindowState::_OnPhysicsSimualtorBuilt(Arcadia::Event::PhysicsSimulatorBuilt& e)
{
    const auto& [physics_simulator] = e.data_tuple;
    _PhysicsSimulator = physics_simulator;
}

void Arcadia::ImguiWindowState::_OnPhysicsSimulatorUnbuilt(Arcadia::Event::PhysicsSimulatorUnbuilt& e)
{
    _PhysicsSimulator.reset();
}


