#include "imgui_window_popup_create_project.hpp"

#include"core/file/pfd_header.hpp"
#include"function/ui/imgui_header.hpp"

void arcadia::imgui_window_popup_create_project::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
    .dispatch<arcadia::event::new_project>(ARCADIA_BIND_MEMBER_FN(_on_new_project));
}

void arcadia::imgui_window_popup_create_project::on_update()
{
    if(!_open)
    {
        return;
    }

    auto imgui_window_title = _title + get_id_str();

    auto popup_flags =
        ImGuiPopupFlags_NoOpenOverExistingPopup;
    ImGui::OpenPopup(imgui_window_title.c_str(), popup_flags);

    ImGui::SetNextWindowSize({ 430,120 }, ImGuiCond_Once);

    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::BeginPopupModal(imgui_window_title.c_str(), &_open, window_flags))
    {
        auto input_text_flags =
            ImGuiInputTextFlags_AutoSelectAll;
        ImGui::Text("Project name");
        if(ImGui::InputText("##project_name", &_name, input_text_flags))
        {
            if(_name.empty())
            {
                ImGui::TextColored({ 204, 80, 69, 255 }, "Project name cannot be empty");
            }
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

void arcadia::imgui_window_popup_create_project::_on_new_project(arcadia::event::new_project& e)
{
    _open = true;
}
