#include "imgui_window_viewport.hpp"

#include"function/ui/imgui_header.hpp"
#include"resource/component/camera_component/camera_component.hpp"
#include"resource/component/model_component/model_component.hpp"


void arcadia::imgui_window_viewport::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
        .dispatch<arcadia::event::open_imgui_window>(ARCADIA_BIND_MEMBER_FN(_on_open_window))
        .dispatch<arcadia::event::scene_activated>(ARCADIA_BIND_MEMBER_FN(_on_scene_activated))
        .dispatch<arcadia::event::scene_deactivated>(ARCADIA_BIND_MEMBER_FN(_on_scene_deactivated))
        .result();
}

void arcadia::imgui_window_viewport::on_update()
{
    if(!_open)
    {
        return;
    }

    auto imgui_title = _title + get_id_str();

    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_title.c_str(), &_open, window_flags))
    {
        if(!_scene_cptr)
        {
            ImGui::Text("No scene to render here");
        }
        else if(!_renderer_ptr)
        {
            ImGui::Text("No renderer to use here");
        }
        else
        {
            glm::ivec2 size{};

            _renderer_ptr->begin_frame();

            // Model
            const auto& model_comp_view = _scene_cptr->view<arcadia::model_component>();
            for(const auto& entity : model_comp_view)
            {
                const auto& [model_comp] = model_comp_view.get(entity);
                _renderer_ptr->submit(model_comp);
            }

            // Camera
            const auto& camera_comp_view = _scene_cptr->view<arcadia::camera_component>();
            for(const auto& entity : camera_comp_view)
            {
                const auto& [camera_comp] = camera_comp_view.get(entity);
                _renderer_ptr->submit(camera_comp);

                size = camera_comp.viewport_size;
            }

            _renderer_ptr->end_frame();

            _renderer_ptr->draw();

            ImGui::Image(_renderer_ptr->get_render_result_id(0), size);
        }
    }
    ImGui::End();
}

void arcadia::imgui_window_viewport::_on_open_window(arcadia::event::open_imgui_window& e)
{
    const auto& [id_str] = e.data_tuple;
    if(id_str == get_id_str())
    {
        _open = true;
    }
}

void arcadia::imgui_window_viewport::_on_scene_activated(arcadia::event::scene_activated& e)
{
    const auto& [scene_ptr] = e.data_tuple;
    _scene_cptr = scene_ptr;
}

void arcadia::imgui_window_viewport::_on_scene_deactivated(arcadia::event::scene_deactivated& e)
{
    _scene_cptr = nullptr;
}
