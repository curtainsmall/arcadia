#include "imgui_window_viewport.hpp"

#include"ui/imgui_header.hpp"
#include"ui/ui_events.hpp"

void arcadia::imgui_window_viewport::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
        .bind_handler<arcadia::event::focus_imgui_window>(
            [&](arcadia::event::focus_imgui_window& e)
    {
        const auto& [title] = e.data_tuple;
        if(title == get_title())
        {
            _open = true;
        }
    }
        )
        .dispatch();
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
        if(ImGui::Button("test"))
        {
        }
    }
    ImGui::End();
}
