#include "imgui_window_popup_create_project.hpp"

#include"core/file/pfd.hpp"

#include"ui/imgui_header.hpp"
#include"ui/ui_events.hpp"

void arcadia::imgui_window_popup_create_project::on_event(arcadia::event_base& event)
{
    arcadia::dispatch_event<arcadia::event::new_project>(
        event,
        [&](arcadia::event::new_project& e)
    {
        _open = true;
    }
    );
}

void arcadia::imgui_window_popup_create_project::on_update()
{
    if(!_open)
    {
        return;
    }

    auto popup_flags =
        ImGuiPopupFlags_NoOpenOverExistingPopup;
    ImGui::OpenPopup(_title.c_str(), popup_flags);

    ImGui::SetNextWindowSize({ 430,120 }, ImGuiCond_Once);

    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::BeginPopupModal(_title.c_str(), &_open, window_flags))
    {
        auto input_text_flags =
            ImGuiInputTextFlags_AutoSelectAll;
        ImGui::Text("Project name");
        ImGui::SameLine();
        if(ImGui::InputText("##project_name", &_name, input_text_flags))
        {
            _display_empty_name_waring = _name.empty();
        }
        if(_display_empty_name_waring)
        {
            ImGui::TextColored({ 204, 80, 69, 255 }, "Project name cannot be empty");
        }
        if(ImGui::Button("Project location"))
        {
            _filepath_str = pfd::save_file{
                "Select location for Project",
                std::format("{}.acdaprj",_name),
                {"Arcadia Project",".acdaprj"}
            }.result();
        }
        if(!_filepath_str.empty())
        {
            ImGui::TextColored({ 228, 228, 228, 255 }, "Location Selected: %s", _filepath_str.c_str());
        }

        auto confirmed = ImGui::Button("Confirm") && !_name.empty();
        if(confirmed)
        {
            arcadia::event_queue::instance()
                .signal<arcadia::event::create_project>(
                    _name,
                    _filepath_str
                );
        }
        ImGui::SameLine();
        if(confirmed || ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            _open = false;
            _name.clear();
            _filepath_str.clear();
        }

        ImGui::EndPopup();
    }
}
