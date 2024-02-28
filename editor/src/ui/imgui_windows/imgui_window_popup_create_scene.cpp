#include "imgui_window_popup_create_scene.hpp"

#include"project/project_events.hpp"
#include"ui/imgui_header.hpp"
#include"ui/ui_events.hpp"

void arcadia::imgui_window_popup_create_scene::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
        .bind_handler<arcadia::event::project_built>(
            [&](arcadia::event::project_built& e)
    {
        const auto& [project_ptr] = e.data_tuple;
        _project_ptr = project_ptr;
    }
        )
        .bind_handler<arcadia::event::project_unbuilt>(
            [&](arcadia::event::project_unbuilt&)
    {
        _project_ptr = nullptr;
    }
        )
        .bind_handler<arcadia::event::new_scene>(
            [&](arcadia::event::new_scene& e)
    {
        _open = true;
    }
        )
        .dispatch();
}

void arcadia::imgui_window_popup_create_scene::on_update()
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
        if(ImGui::InputText("Scene name", &_name, input_text_flags))
        {
            _name_available = !_project_ptr->scene_umap.contains(_name);
            _display_empty_name_waring = _name.empty();
        }
        if(!_name_available)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, { 204, 80, 69, 255 });
            ImGui::Text("Scene named \"&s\" already exsits", _name.c_str());
            ImGui::PopStyleColor();
        }
        if(_display_empty_name_waring)
        {
            ImGui::TextColored({ 204,80,69,255 }, "Scene name cannot empty");
        }
        ImGui::Checkbox("As current", &_as_current);

        auto confirmed = ImGui::Button("Confirm") && !_name.empty() && _name_available;
        if(confirmed)
        {
            arcadia::event_queue::instance()
                .signal<arcadia::event::create_scene>(
                    _name,
                    _as_current
                );
        }
        ImGui::SameLine();
        if(confirmed || ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            _open = false;
            _name.clear();
            _as_current = true;
            _name_available = true;
        }

        ImGui::EndPopup();
    }
}
