#include "imgui_window_property.hpp"

#include"function/ui/imgui_header.hpp"

void arcadia::imgui_window_property::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
        .dispatch<arcadia::event::open_imgui_window>(ARCADIA_BIND_MEMBER_FN(_on_open_imgui_window))
        .result();
}

void arcadia::imgui_window_property::on_update()
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

    }
    ImGui::End();
}

void arcadia::imgui_window_property::_on_open_imgui_window(arcadia::event::open_imgui_window& e)
{
    const auto& [id_str] = e.data_tuple;
    if(id_str == get_id_str())
    {
        _open = true;
    }
}
