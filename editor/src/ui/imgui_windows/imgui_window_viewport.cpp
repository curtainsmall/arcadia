#include "imgui_window_viewport.hpp"

#include"function/ui/imgui_header.hpp"
#include"resource/component/camera_component/camera_component.hpp"
#include"resource/component/model_component/model_component.hpp"


arcadia::imgui_window_viewport::imgui_window_viewport(bool open, const std::string& title):
    imgui_window_interface(open, title)
{}

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
        .result();
}

void arcadia::imgui_window_viewport::on_update()
{
    if(!_open)
    {
        return;
    }

    auto project_sptr = _project_wptr.lock();
    std::shared_ptr<const arcadia::scene> scene_sptr = _scene_wptr.lock();
    auto renderer_sptr = _renderer_wptr.lock();

    auto imgui_title = _title + get_id_str();

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
            auto& camera = project_sptr->viewport_camera;

            camera.viewport_size = ImGui::GetContentRegionAvail();
            camera.should_display_grid = true;
            renderer_sptr->begin_frame();

            // Cameras
            renderer_sptr->submit(camera);

            // Lights
            const auto& light_comp_view = scene_sptr->component_view<arcadia::light_component>();
            for(const auto& entity : light_comp_view)
            {
                const auto& [light_comp] = light_comp_view.get(entity);
                renderer_sptr->submit(light_comp);
            }

            // Models
            renderer_sptr->submit(_grid);
            const auto& model_comp_view = scene_sptr->component_view<arcadia::model_component>();
            for(const auto& entity : model_comp_view)
            {
                const auto& [model_comp] = model_comp_view.get(entity);
                renderer_sptr->submit(model_comp);
            }

            renderer_sptr->end_frame();

            renderer_sptr->draw();

            auto image_cursor_pos = ImGui::GetCursorPos();
            ImGui::Image(renderer_sptr->get_render_result_id(0), camera.viewport_size, { 0,1 }, { 1,0 });

            if(ImGui::IsItemHovered())
            {
                auto& io = ImGui::GetIO();

                // Scroll to zoom (move camera forwards or backwards along direction)
                auto mouse_wheel_offset = io.MouseWheel;
                camera.move(camera.get_forward_dir() * mouse_wheel_offset);

                if(ImGui::IsMouseDown(ImGuiMouseButton_Middle))
                {
                    if(ImGui::IsKeyDown(ImGuiKey_LeftShift))
                    {
                        camera.drag_view_move(_cursor_move * .05f);
                    }
                    else
                    {
                        camera.drag_view_rotate(_cursor_move * .005f);
                    }
                }

                // Display viewport camera info
                ImGui::SetCursorPos(image_cursor_pos);
                ImGui::Text(std::format("Camera - Pos: {} - Direction: {}", camera.pos, camera.get_forward_dir()).c_str());
            }
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
