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

    auto scene_sptr = _scene_wptr.lock();
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
            glm::ivec2 size{};

            renderer_sptr->begin_frame();

            // Model
            const auto& model_comp_view = scene_sptr->component_view<arcadia::model_component>();
            for(const auto& entity : model_comp_view)
            {
                const auto& [model_comp] = model_comp_view.get(entity);
                renderer_sptr->submit(model_comp);
            }

            // Camera
            const auto& camera_comp_view = scene_sptr->component_view<arcadia::camera_component>();
            for(const auto& entity : camera_comp_view)
            {
                const auto& [camera_comp] = camera_comp_view.get(entity);
                renderer_sptr->submit(camera_comp);

                size = camera_comp.viewport_size;
            }

            renderer_sptr->end_frame();

            renderer_sptr->draw();

            ImGui::Image(renderer_sptr->get_render_result_id(0), size);
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
    const auto& [scene_wptr] = e.data_tuple;
    _scene_wptr = scene_wptr;
}

void arcadia::imgui_window_viewport::_on_scene_deactivated(arcadia::event::scene_deactivated& e)
{
    _scene_wptr.reset();
}
