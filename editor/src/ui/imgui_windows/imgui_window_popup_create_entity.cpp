#include "imgui_window_popup_create_entity.hpp"

#include"ui/imgui_header.hpp"

void arcadia::imgui_window_popup_create_entity::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
    .dispatch<arcadia::event::new_entity>(ARCADIA_BIND_MEMBER_FN(_on_new_entity));
}

void arcadia::imgui_window_popup_create_entity::on_update()
{
    if(!_open)
    {
        return;
    }

    auto popup_flags =
        ImGuiPopupFlags_NoOpenOverExistingPopup;
    ImGui::OpenPopup(get_title().c_str(), popup_flags);

    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::BeginPopupModal(get_title().c_str(), &_open, window_flags))
    {
        auto input_text_flags =
            ImGuiInputTextFlags_AutoSelectAll;
        ImGui::Text("Entity name");
        if(ImGui::InputText("##entity_name", &_name, input_text_flags))
        {
            if(_name.empty())
            {
                ImGui::TextColored({ 204,80,69,255 }, "Entity name cannot be empty");
            }
        }

        auto confirmed = ImGui::Button("Confirm") && !_name.empty();
        if(confirmed)
        {
            arcadia::event_queue::instance()
                .signal<arcadia::event::create_entity>(
                    _name
                );
        }
        ImGui::SameLine();
        if(confirmed || ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            _open = false;
            _name.clear();
        }

        ImGui::EndPopup();
    }
}

void arcadia::imgui_window_popup_create_entity::_on_new_entity(arcadia::event::new_entity& e)
{
    _open = true;
}
