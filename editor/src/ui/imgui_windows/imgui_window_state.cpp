#include "imgui_window_state.hpp"

#include"core/file/pfd_header.hpp"
#include"function/ui/imgui_header.hpp"

void arcadia::imgui_window_state_scene::operator()(const std::shared_ptr<arcadia::scene>& scene_sptr)
{
    const auto& scene_name_entity_bimap = scene_sptr->get_name_entity_bimap();
    ImGui::Text(std::format("Entity Count: {}", scene_name_entity_bimap.size()).c_str());
}

void arcadia::imgui_window_state_renderer::operator()(const std::shared_ptr<arcadia::renderer_interface>& renderer_sptr)
{
    auto graphic_api_type_str = arcadia::match<std::string>(
        renderer_sptr->get_graphic_api_type(),
        [&](const arcadia::graphic_api::opengl& api)
    {
        return std::format("OpenGL ({})", api.version);
    },
        [&](const arcadia::graphic_api::directx& api)
    {
        return std::format("DirectX ({})", api.version);
    },
        [&](const arcadia::graphic_api::vulkan& api)
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

void arcadia::imgui_window_state_physics_simulator::operator()(const std::shared_ptr<arcadia::physics_simulator>& physics_simulator_sptr)
{
    const auto& physics_simulator_jph_body_id_umap = physics_simulator_sptr->get_jph_body_id_umap();
    ImGui::Text(std::format("Body Count: {}", physics_simulator_sptr->get_jph_body_id_umap().size()).c_str());

    ImGui::NewLine();
    ImGui::SeparatorText("Physics Simulator Config");

    auto slider_flags =
        ImGuiSliderFlags_AlwaysClamp;

    ImGui::NewLine();
    ImGui::BeginDisabled();
    int temp_allocator_size_in_kib = physics_simulator_sptr->jph_temp_allocator_size / 1024;
    ImGui::Text("Temporary Allocator Size (KiB)"); ImGui::SameLine(); ImGui::DragInt("##tas", &temp_allocator_size_in_kib, 1.f, 64 /* 64 KiB*/, 16 * 1024 * 1024 /* 16 GiB */, "%d", slider_flags);
    physics_simulator_sptr->jph_temp_allocator_size = temp_allocator_size_in_kib * 1024;
    ImGui::EndDisabled();

    ImGui::NewLine();
    ImGui::Text("            Updates per Second"); ImGui::SameLine(); ImGui::DragInt("##ups", &physics_simulator_sptr->jph_physics_system_updates_per_second, 1.f, 0, (std::numeric_limits<int>::max)(), "%d", slider_flags);
    if(_link_ups_and_spu)
    {
        physics_simulator_sptr->jph_physics_system_collision_steps_per_update = physics_simulator_sptr->jph_physics_system_updates_per_second / 60;
    }
    ImGui::BeginDisabled();
    ImGui::Text("    Collision Steps per Update"); ImGui::SameLine(); ImGui::DragInt("##spu", &physics_simulator_sptr->jph_physics_system_collision_steps_per_update, 1.f, 0, (std::numeric_limits<int>::max)(), "%d", slider_flags);
    ImGui::EndDisabled();

    ImGui::NewLine();
    if(physics_simulator_sptr->should_update)
    {
        if(ImGui::Button("Stop"))
        {
            physics_simulator_sptr->should_update = false;
        }
    }
    else
    {
        if(ImGui::Button("Start"))
        {
            physics_simulator_sptr->should_update = true;
        }
    }
    ImGui::SameLine();
    if(ImGui::Button("Reset"))
    {
        physics_simulator_sptr->reset();
        physics_simulator_sptr->should_update = false;
    }

}

void arcadia::imgui_window_state::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
        .dispatch<arcadia::event::open_imgui_window>(ARCADIA_BIND_MEMBER_FN(_on_open_imgui_window))
        .dispatch<arcadia::event::scene_activated>(ARCADIA_BIND_MEMBER_FN(_on_scene_activated))
        .dispatch<arcadia::event::scene_deactivated>(ARCADIA_BIND_MEMBER_FN(_on_scene_deactivated))
        .dispatch<arcadia::event::renderer_built>(ARCADIA_BIND_MEMBER_FN(_on_renderer_built))
        .dispatch<arcadia::event::renderer_unbuilt>(ARCADIA_BIND_MEMBER_FN(_on_renderer_unbuilt))
        .dispatch<arcadia::event::physics_simulator_built>(ARCADIA_BIND_MEMBER_FN(_on_physics_simulator_built))
        .dispatch<arcadia::event::physics_simulator_unbuilt>(ARCADIA_BIND_MEMBER_FN(_on_physics_simulator_unbuilt))
        .result();

}

void arcadia::imgui_window_state::on_update()
{
    if(!_open)
    {
        return;
    }

    auto has_active_scene = !_scene_wptr.expired();
    auto active_scene_sptr = _scene_wptr.lock();

    auto has_renderer = !_renderer_wptr.expired();
    auto renderer_sptr = _renderer_wptr.lock();

    auto has_physics_simulator = !_physics_simulator_wptr.expired();
    auto physics_simulator_sptr = _physics_simulator_wptr.lock();

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
                if(has_active_scene)
                {
                    _imgui_window_state_scene(active_scene_sptr);
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
                if(has_renderer)
                {
                    _imgui_window_state_renderer(renderer_sptr);
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
                if(has_physics_simulator)
                {
                    _imgui_window_state_physics_simulator(physics_simulator_sptr);
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

void arcadia::imgui_window_state::_on_open_imgui_window(arcadia::event::open_imgui_window& e)
{
    _open = true;
}

void arcadia::imgui_window_state::_on_scene_activated(arcadia::event::scene_activated& e)
{
    const auto& [scene_wptr] = e.data_tuple;
    _scene_wptr = scene_wptr;
}

void arcadia::imgui_window_state::_on_scene_deactivated(arcadia::event::scene_deactivated& e)
{
    _scene_wptr.reset();
}

void arcadia::imgui_window_state::_on_renderer_built(arcadia::event::renderer_built& e)
{
    const auto& [renderer_wptr] = e.data_tuple;
    _renderer_wptr = renderer_wptr;
}

void arcadia::imgui_window_state::_on_renderer_unbuilt(arcadia::event::renderer_unbuilt& e)
{
    _renderer_wptr.reset();
}

void arcadia::imgui_window_state::_on_physics_simulator_built(arcadia::event::physics_simulator_built& e)
{
    const auto& [physics_simulator_wptr] = e.data_tuple;
    _physics_simulator_wptr = physics_simulator_wptr;
}

void arcadia::imgui_window_state::_on_physics_simulator_unbuilt(arcadia::event::physics_simulator_unbuilt& e)
{
    _physics_simulator_wptr.reset();
}


