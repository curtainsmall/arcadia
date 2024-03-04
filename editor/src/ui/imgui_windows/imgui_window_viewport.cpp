#include "imgui_window_viewport.hpp"

#include"ui/imgui_header.hpp"

void arcadia::imgui_window_viewport::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
    .dispatch<arcadia::event::open_imgui_window>(ARCADIA_BIND_MEMBER_FN(_on_open_window));
}

void arcadia::imgui_window_viewport::on_update()
{
    if(!_open)
    {
        return;
    }

    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(get_title().c_str(), &_open, window_flags))
    {
    }
    ImGui::End();
}

void arcadia::imgui_window_viewport::_on_open_window(arcadia::event::open_imgui_window& e)
{
    const auto& [title] = e.data_tuple;
    if(title == get_title())
    {
        _open = true;
    }
}
