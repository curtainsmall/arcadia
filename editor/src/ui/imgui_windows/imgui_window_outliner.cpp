#include "imgui_window_outliner.hpp"

#include"ui/imgui_header.hpp"

void arcadia::imgui_window_outliner::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
    .dispatch<arcadia::event::open_imgui_window>(ARCADIA_BIND_MEMBER_FN(_on_open_window));
}

void arcadia::imgui_window_outliner::on_update()
{
    if(!_open)
    {
        return;
    }

    auto& event_queue = arcadia::event_queue::instance();

    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(get_title().c_str(), &_open, window_flags))
    {
        if(ImGui::Button("New Entity"))
        {
            event_queue.signal<arcadia::event::new_entity>();
        }
    }
    ImGui::End();

}

void arcadia::imgui_window_outliner::_on_open_window(arcadia::event::open_imgui_window& e)
{
    const auto& [title] = e.data_tuple;
    if(title == get_title())
    {
        _open = true;
    }
}

void arcadia::imgui_window_outliner::_on_scene_built(arcadia::event::scene_built& e)
{
    const auto& [scene_ptr] = e.data_tuple;
    _scene_cptr = scene_ptr;
}

void arcadia::imgui_window_outliner::_on_scene_built(arcadia::event::scene_unbuilt& e)
{
    _scene_cptr = nullptr;
}
