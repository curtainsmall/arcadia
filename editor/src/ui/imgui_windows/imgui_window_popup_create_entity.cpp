#include "imgui_window_popup_create_entity.hpp"

#include"function/ui/imgui_header.hpp"

void arcadia::imgui_window_popup_create_entity::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
        .dispatch<arcadia::event::new_entity>(ARCADIA_BIND_MEMBER_FN(_on_new_entity))
        .dispatch<arcadia::event::scene_activated>(ARCADIA_BIND_MEMBER_FN(_on_scene_activated))
        .dispatch<arcadia::event::scene_deactivated>(ARCADIA_BIND_MEMBER_FN(_on_scene_deactivated))
        .result();
}

void arcadia::imgui_window_popup_create_entity::on_update()
{
    if(!_open)
    {
        return;
    }

    auto scene_sptr = _scene_wptr.lock();

    auto imgui_title = _title + get_id_str();

    auto popup_flags =
        ImGuiPopupFlags_NoOpenOverExistingPopup;
    ImGui::OpenPopup(imgui_title.c_str(), popup_flags);

    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::BeginPopupModal(imgui_title.c_str(), &_open, window_flags))
    {
        auto input_text_flags =
            ImGuiInputTextFlags_AutoSelectAll;
        ImGui::Text("Entity name");
        if(ImGui::InputText("##entity_name", &_name, input_text_flags))
        {
            _name_available = !scene_sptr->contains_entity(_name);
            if(_name.empty())
            {
                ImGui::TextColored({ 204,80,69,255 }, "Entity name cannot be empty");
            }
        }
        if(!_name_available)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, { 204, 80, 69, 255 });
            ImGui::Text("Scene named \"%s\" already exsits", _name.c_str());
            ImGui::PopStyleColor();
        }

        auto confirmed = ImGui::Button("Confirm") && !_name.empty() && _name_available;
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
            _name_available = true;
        }

        ImGui::EndPopup();
    }
}

void arcadia::imgui_window_popup_create_entity::_on_new_entity(arcadia::event::new_entity& e)
{
    _open = true;
}

void arcadia::imgui_window_popup_create_entity::_on_scene_activated(arcadia::event::scene_activated& e)
{
    const auto& [scene_wptr] = e.data_tuple;
    _scene_wptr = scene_wptr;
}

void arcadia::imgui_window_popup_create_entity::_on_scene_deactivated(arcadia::event::scene_deactivated& e)
{
    _scene_wptr.reset();
}
