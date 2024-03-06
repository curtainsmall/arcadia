#include "imgui_window_popup_create_scene.hpp"

#include"function/ui/imgui_header.hpp"

void arcadia::imgui_window_popup_create_scene::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
        .dispatch<arcadia::event::project_built>(ARCADIA_BIND_MEMBER_FN(_on_project_built))
        .dispatch<arcadia::event::project_unbuilt>(ARCADIA_BIND_MEMBER_FN(_on_project_unbuilt))
        .dispatch<arcadia::event::new_scene>(ARCADIA_BIND_MEMBER_FN(_on_new_scene));
}

void arcadia::imgui_window_popup_create_scene::on_update()
{
    if(!_open)
    {
        return;
    }

    auto project_sptr = _project_wptr.lock();

    auto imgui_title = _title + get_id_str();

    auto popup_flags =
        ImGuiPopupFlags_NoOpenOverExistingPopup;
    ImGui::OpenPopup(imgui_title.c_str(), popup_flags);

    ImGui::SetNextWindowSize({ 430,120 }, ImGuiCond_Once);

    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::BeginPopupModal(imgui_title.c_str(), &_open, window_flags))
    {
        auto input_text_flags =
            ImGuiInputTextFlags_AutoSelectAll;
        ImGui::Text("Scene name");
        if(ImGui::InputText("##scene_name", &_name, input_text_flags))
        {
            _name_available = !project_sptr->scene_sptr_umap.contains(_name);
            if(_name.empty())
            {
                ImGui::TextColored({ 204,80,69,255 }, "Scene name cannot empty");
            }
        }
        if(!_name_available)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, { 204, 80, 69, 255 });
            ImGui::Text("Scene named \"&s\" already exsits", _name.c_str());
            ImGui::PopStyleColor();
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

void arcadia::imgui_window_popup_create_scene::_on_project_built(arcadia::event::project_built& e)
{
    const auto& [project_wptr] = e.data_tuple;
    _project_wptr = project_wptr;
}

void arcadia::imgui_window_popup_create_scene::_on_project_unbuilt(arcadia::event::project_unbuilt& e)
{
    _project_wptr.reset();
}

void arcadia::imgui_window_popup_create_scene::_on_new_scene(arcadia::event::new_scene& e)
{
    _open = true;
}
