#include "imgui_window_outliner.hpp"

#include"function/ui/imgui_header.hpp"
#include"resource/component/meta_components/meta_componts.hpp"

void arcadia::imgui_window_outliner::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
        .dispatch<arcadia::event::open_imgui_window>(ARCADIA_BIND_MEMBER_FN(_on_open_window))
        .dispatch<arcadia::event::scene_activated>(ARCADIA_BIND_MEMBER_FN(_on_scene_activated))
        .dispatch<arcadia::event::scene_deactivated>(ARCADIA_BIND_MEMBER_FN(_on_scene_deactivated))
        .result();
}

void arcadia::imgui_window_outliner::on_update()
{
    if(!_open)
    {
        return;
    }

    auto has_scene = _scene_wptr.use_count();
    auto scene_sptr = _scene_wptr.lock();

    auto& event_queue = arcadia::event_queue::instance();

    auto imgui_title = has_scene
        ? _title + " - " + scene_sptr->get_name() + get_id_str()
        : _title + get_id_str();

    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_title.c_str(), &_open, window_flags))
    {
        if(!has_scene)
        {
            ImGui::Text("No scene to outline here");
        }
        else
        {
            if(ImGui::Button("New Entity"))
            {
                event_queue.signal<arcadia::event::new_entity>();
            }

            const auto& view = scene_sptr->view<arcadia::name_component>();
            for(const auto entity : view)
            {
                const auto& [name_comp] = view.get(entity);
                if(ImGui::Selectable(name_comp.get().c_str()))
                {
                    // TODO: Select entity here
                }

                if(ImGui::BeginPopupContextItem())
                {
                    if(ImGui::Selectable("Delete entity"))
                    {
                        event_queue.signal<arcadia::event::delete_entity>(entity);
                    }
                    if(ImGui::Selectable("Rename entity"))
                    {
                    }

                    ImGui::EndPopup();
                }
            }
        }
    }
    ImGui::End();

}

void arcadia::imgui_window_outliner::_on_open_window(arcadia::event::open_imgui_window& e)
{
    const auto& [id_str] = e.data_tuple;
    if(id_str == get_id_str())
    {
        _open = true;
    }
}

void arcadia::imgui_window_outliner::_on_scene_activated(arcadia::event::scene_activated& e)
{
    const auto& [scene_wptr] = e.data_tuple;
    _scene_wptr = scene_wptr;
}

void arcadia::imgui_window_outliner::_on_scene_deactivated(arcadia::event::scene_deactivated& e)
{
    _scene_wptr.reset();
}
