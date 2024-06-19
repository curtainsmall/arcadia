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
    ImGui::Text(std::format("Entity Count: {}", scene.count([&](const std::string&, const EntityInfo& info)->bool
    {
        return !info.internal;
    })).c_str());
}

void ImguiWindowStateRenderer::operator()(const iRenderer& renderer)
{
    auto graphic_api_type_str = match<std::string>(
        renderer.graphic_api_type(),
        [&](const graphic_api::Opengl& api)
    {
        return std::format("OpenGL ({})", api.version);
    },
        [&](const graphic_api::Directx& api)
    {
        return std::format("DirectX ({})", api.version);
    },
        [&](const graphic_api::Vulkan& api)
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
    const auto& physics_simulator_jph_body_id_storage = physics_simulator.jph_body_id_storage();
    ImGui::Text(std::format("Body Count: {}", physics_simulator_jph_body_id_storage.size()).c_str());

    ImGui::NewLine();
    ImGui::SeparatorText("Physics Simulator Config");

    auto slider_flags =
        ImGuiSliderFlags_AlwaysClamp;

    ImGui::NewLine();
    ImGui::BeginDisabled();
    int temp_allocator_size_in_kib = physics_simulator.get_jph_temp_allocator_size() / 1024;
    imgui_wrapper::drag_int(
        "Temporary Allocator Size (KiB)",
        temp_allocator_size_in_kib,
        1.f,
        64, // 64 KiB
        16 * 1024 * 1024, // 16 GiB
        "%d",
        slider_flags
    );
    physics_simulator.set_jph_temp_allocator_size(temp_allocator_size_in_kib * 1024);
    ImGui::EndDisabled();

    ImGui::NewLine();
    imgui_wrapper::drag_int(
        "            Updates per Second",
        ACDA_BIND_MEMBER_FN_ARBITRARY(physics_simulator, get_jph_physics_system_updates_per_second),
        ACDA_BIND_MEMBER_FN_ARBITRARY(physics_simulator, set_jph_physics_system_updates_per_second),
        1.f,
        0,
        (std::numeric_limits<int>::max)(),
        "%d",
        slider_flags
    );

    ImGui::NewLine();
    if(physics_simulator.is_active())
    {
        if(ImGui::Button("Stop"))
        {
            physics_simulator.set_active(false);
        }
    }
    else
    {
        if(ImGui::Button("Start"))
        {
            physics_simulator.set_active(true);
        }
    }
    ImGui::SameLine();
    if(ImGui::Button("Reset"))
    {
        physics_simulator.reset();
        physics_simulator.set_active(false);
    }

}

void ImguiWindowState::on_event(EventBase& e)
{
    EventDispatcher{ e }
        .dispatch<event::OpenImguiWindow>(ACDA_BIND_MEMBER_FN(_on_open_imgui_window))
        .dispatch<event::SceneActivated>(ACDA_BIND_MEMBER_FN(_on_scene_activated))
        .dispatch<event::SceneDeactivated>(ACDA_BIND_MEMBER_FN(_on_scene_deactivated))
        .dispatch<event::RendererBuilt>(ACDA_BIND_MEMBER_FN(_on_renderer_built))
        .dispatch<event::RendererUnbuilt>(ACDA_BIND_MEMBER_FN(_on_renderer_unbuilt))
        .dispatch<event::PhysicsSimulatorBuilt>(ACDA_BIND_MEMBER_FN(_on_physics_simulator_built))
        .dispatch<event::PhysicsSimulatorUnbuilt>(ACDA_BIND_MEMBER_FN(_on_physics_simulator_unbuilt))
        .result();
}

void ImguiWindowState::on_update()
{
    if(!_open)
    {
        return;
    }

    auto scene = _scene.lock();
    auto renderer = _renderer.lock();
    auto physics_simualtor = _physics_simulator.lock();

    auto imgui_window_title = _title + get_id_str();

    ImGui::SetNextWindowSize(glm::vec2{ 1024,768 }, ImGuiCond_Once);
    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_window_title.c_str(), &_open, window_flags))
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
                    _imgui_window_state_scene(*scene);
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
                    _imgui_window_state_renderer(*renderer);
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
                    _imgui_window_state_physics_simulator(*physics_simualtor);
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

void ImguiWindowState::_on_open_imgui_window(event::OpenImguiWindow& e)
{
    _open = true;
}

void ImguiWindowState::_on_scene_activated(event::SceneActivated& e)
{
    const auto& [scene] = e.data_tuple;
    _scene = scene;
}

void ImguiWindowState::_on_scene_deactivated(event::SceneDeactivated& e)
{
    _scene.reset();
}

void ImguiWindowState::_on_renderer_built(event::RendererBuilt& e)
{
    const auto& [renderer] = e.data_tuple;
    _renderer = renderer;
}

void ImguiWindowState::_on_renderer_unbuilt(event::RendererUnbuilt& e)
{
    _renderer.reset();
}

void ImguiWindowState::_on_physics_simulator_built(event::PhysicsSimulatorBuilt& e)
{
    const auto& [physics_simulator] = e.data_tuple;
    _physics_simulator = physics_simulator;
}

void ImguiWindowState::_on_physics_simulator_unbuilt(event::PhysicsSimulatorUnbuilt& e)
{
    _physics_simulator.reset();
}


