#include "imgui_window_viewport.hpp"

#include"core/app/app_context.hpp"
#include"function/ui/imgui_header.hpp"
#include"resource/component/camera_component/camera_component.hpp"
#include"resource/component/model_component/model_component.hpp"

void arcadia::imgui_window_viewport::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
        .dispatch<arcadia::event::input_cursor_move>(ARCADIA_BIND_MEMBER_FN(_on_input_cursor_move))
        .dispatch<arcadia::event::open_imgui_window>(ARCADIA_BIND_MEMBER_FN(_on_open_imgui_window))
        .dispatch<arcadia::event::project_built>(ARCADIA_BIND_MEMBER_FN(_on_project_built))
        .dispatch<arcadia::event::project_unbuilt>(ARCADIA_BIND_MEMBER_FN(_on_project_unbuilt))
        .dispatch<arcadia::event::scene_activated>(ARCADIA_BIND_MEMBER_FN(_on_scene_activated))
        .dispatch<arcadia::event::scene_deactivated>(ARCADIA_BIND_MEMBER_FN(_on_scene_deactivated))
        .dispatch<arcadia::event::renderer_built>(ARCADIA_BIND_MEMBER_FN(_on_renderer_built))
        .dispatch<arcadia::event::renderer_unbuilt>(ARCADIA_BIND_MEMBER_FN(_on_renderer_unbuilt))
        .dispatch<arcadia::event::physics_simulator_built>(ARCADIA_BIND_MEMBER_FN(_on_physics_simulator_built))
        .dispatch<arcadia::event::physics_simulator_unbuilt>(ARCADIA_BIND_MEMBER_FN(_on_physics_simulator_unbuilt))
        .result();
}

void arcadia::imgui_window_viewport::on_update()
{
    if(!_open)
    {
        return;
    }

    const auto& app_context = arcadia::app_context::instance();

    auto project_sptr = _project_wptr.lock();
    auto scene_sptr = _scene_wptr.lock();
    auto renderer_sptr = _renderer_wptr.lock();
    auto physics_simulator_sptr = _physics_simulator_wptr.lock();

    auto imgui_title = _title + get_id_str();

    ImGui::SetNextWindowSize(glm::vec2{ 1024,768 }, ImGuiCond_Once);
    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_title.c_str(), &_open, window_flags))
    {
        if(!scene_sptr)
        {
            ImGui::Text("No scene to render here");
        }
        else if(!renderer_sptr)
        {
            ImGui::Text("No renderer to use here");
        }
        else
        {
            //==== Physics Simulator ====// 
            if(physics_simulator_sptr)
            {
                physics_simulator_sptr->prepare();

                auto physics_comp_view = scene_sptr->view<arcadia::physics_component>();
                for(auto [entity, physics_comp] : physics_comp_view.each())
                {
                    physics_simulator_sptr->submit(physics_comp);
                }

                physics_simulator_sptr->finalize();

                physics_simulator_sptr->update();
                for(auto [entity, physics_comp] : physics_comp_view.each())
                {
                    physics_simulator_sptr->quary(physics_comp);
                }

            }

            //==== Renderer ====//
            auto& viewport_camera = project_sptr->viewport_camera;

            viewport_camera.viewport_size = ImGui::GetContentRegionAvail();
            //viewport_camera.should_display_grid = true;
            renderer_sptr->prepare();

            // Cameras
            renderer_sptr->submit(viewport_camera);

            // Lights
            for(auto [entity, light_comp] : scene_sptr->view<arcadia::light_component>().each())
            {
                if(scene_sptr->get_entity_info(entity).should_render_in_viewport)
                {
                    renderer_sptr->submit(light_comp);
                }
            }

            // Models
            for(auto [entity, model_comp] : scene_sptr->view<arcadia::model_component>().each())
            {
                if(scene_sptr->get_entity_info(entity).should_render_in_viewport)
                {
                    renderer_sptr->submit(model_comp);
                }
            }

            // Physcis simulator 
            if(physics_simulator_sptr)
            {
                for(auto [entity, physics_comp] : scene_sptr->view<arcadia::physics_component>().each())
                {
                    if(scene_sptr->get_entity_info(entity).should_render_in_viewport)
                    {
                        renderer_sptr->submit(physics_comp);
                    }
                }
            }

            renderer_sptr->finalize();
            renderer_sptr->draw();

            auto image_cursor_pos = ImGui::GetCursorPos();
            ImGui::Image(renderer_sptr->get_render_result_id(0), viewport_camera.viewport_size, { 0,1 }, { 1,0 });

            if(ImGui::IsItemHovered())
            {
                auto& io = ImGui::GetIO();

                // Scroll to zoom (move viewport_camera forwards or backwards along direction)
                auto mouse_wheel_offset = io.MouseWheel;
                viewport_camera.move(viewport_camera.get_forward_dir() * mouse_wheel_offset);

                if(ImGui::IsMouseDown(ImGuiMouseButton_Middle))
                {
                    if(ImGui::IsKeyDown(ImGuiKey_LeftShift))
                    {
                        viewport_camera.drag_view_move(_cursor_move * .05f);
                    }
                    else
                    {
                        viewport_camera.drag_view_rotate(_cursor_move * .005f);
                    }
                }

            }

            // Display viewport viewport_camera info
            ImGui::SetCursorPos(image_cursor_pos);
            ImGui::Text(std::format("Camera - Pos: {} - Direction: {}", viewport_camera.position, viewport_camera.get_forward_dir()).c_str());
            float fps = 1.f / std::chrono::duration_cast<std::chrono::duration<float>>(app_context.delta_time).count();
            ImGui::Text(std::format("FPS: {:.2f}", fps).c_str());
        }
    }
    ImGui::End();
}

void arcadia::imgui_window_viewport::_on_input_cursor_move(arcadia::event::input_cursor_move& e)
{
    const auto& [wnd_ptr, cursor_move] = e.data_tuple;
    _cursor_move = cursor_move;
}

void arcadia::imgui_window_viewport::_on_open_imgui_window(arcadia::event::open_imgui_window& e)
{
    const auto& [id_str] = e.data_tuple;
    if(id_str == get_id_str())
    {
        _open = true;
    }
}

void arcadia::imgui_window_viewport::_on_project_built(arcadia::event::project_built& e)
{
    const auto& [project_wptr] = e.data_tuple;
    _project_wptr = project_wptr;
}

void arcadia::imgui_window_viewport::_on_project_unbuilt(arcadia::event::project_unbuilt& e)
{
    _project_wptr.reset();
}

void arcadia::imgui_window_viewport::_on_scene_activated(arcadia::event::scene_activated& e)
{
    const auto& [scene_wptr] = e.data_tuple;
    _scene_wptr = scene_wptr;
}

void arcadia::imgui_window_viewport::_on_scene_deactivated(arcadia::event::scene_deactivated& e)
{
    _scene_wptr.reset();
}

void arcadia::imgui_window_viewport::_on_renderer_built(arcadia::event::renderer_built& e)
{
    const auto& [renderer_wptr] = e.data_tuple;
    _renderer_wptr = renderer_wptr;
}

void arcadia::imgui_window_viewport::_on_renderer_unbuilt(arcadia::event::renderer_unbuilt& e)
{
    _renderer_wptr.reset();
}

void arcadia::imgui_window_viewport::_on_physics_simulator_built(arcadia::event::physics_simulator_built& e)
{
    const auto& [physics_simulator_wptr] = e.data_tuple;
    _physics_simulator_wptr = physics_simulator_wptr;
}

void arcadia::imgui_window_viewport::_on_physics_simulator_unbuilt(arcadia::event::physics_simulator_unbuilt& e)
{
    _physics_simulator_wptr.reset();
}
