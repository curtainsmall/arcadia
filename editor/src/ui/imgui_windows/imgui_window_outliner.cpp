#include "imgui_window_outliner.hpp"

#include"core/file/pfd_header.hpp"
#include"function/ui/imgui_header.hpp"

void arcadia::imgui_window_outliner::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
        .dispatch<arcadia::event::open_imgui_window>(ARCADIA_BIND_MEMBER_FN(_on_open_imgui_window))
        .dispatch<arcadia::event::scene_activated>(ARCADIA_BIND_MEMBER_FN(_on_scene_activated))
        .dispatch<arcadia::event::scene_deactivated>(ARCADIA_BIND_MEMBER_FN(_on_scene_deactivated))
        .result();
}

void arcadia::imgui_window_outliner::on_update()
{
    if(!_open)
    {
        return;
    }

    auto has_scene = !_scene_wptr.expired();
    std::shared_ptr<const arcadia::scene> scene_sptr = _scene_wptr.lock();

    auto& event_queue = arcadia::event_queue::instance();

    auto imgui_title = has_scene
        ? _title + " - " + scene_sptr->get_name() + get_id_str()
        : _title + get_id_str();

    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_title.c_str(), &_open, window_flags))
    {
        if(has_scene && ImGui::BeginPopupContextWindow())
        {
            if(ImGui::Selectable("New Entity"))
            {
                event_queue.signal<arcadia::event::new_entity>();
            }
            ImGui::EndPopup();
        }

        if(!has_scene)
        {
            ImGui::Text("No scene to outline here");
        }
        else
        {
            for(const auto& [name, entity] : scene_sptr->get_name_entity_bimap())
            {
                if(_entity_old_name == name)
                {
                    _entity_new_name = _entity_old_name;

                    auto input_text_flags =
                        ImGuiInputTextFlags_AutoSelectAll;
                    ImGui::InputText("###rename_entity", &_entity_new_name, input_text_flags);
                    ImGui::SetItemDefaultFocus();
                    if(!ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsKeyPressed(ImGuiKey_Enter))
                    {
                        event_queue.signal<arcadia::event::rename_entity>(_entity_old_name, _entity_new_name);
                        _entity_old_name.clear();
                        _entity_new_name.clear();
                    }
                }
                else
                {
                    if(ImGui::Selectable(name.c_str(), _selected_entity == entity))
                    {
                        _selected_entity = entity;
                        event_queue.signal<arcadia::event::select_entity>(entity);
                    }

                    if(ImGui::BeginPopupContextItem())
                    {
                        if(ImGui::Selectable("Delete Entity"))
                        {
                            event_queue.signal<arcadia::event::delete_entity>(entity);
                        }
                        if(ImGui::Selectable("Rename Entity"))
                        {
                            _entity_old_name = name;
                        }
                        if(_selected_entity != entt::null)
                        {
                            ImGui::Separator();
                            if(ImGui::BeginMenu("Add Component"))
                            {
                                int item_count{ 0 };
                                _add_component_menu_item<arcadia::camera_component>(item_count);
                                _add_component_menu_item<arcadia::light_component>(item_count);
                                _add_component_menu_item<arcadia::model_component>(item_count);

                                if(item_count == 0)
                                {
                                    ImGui::Text("(No more component to add)");
                                }

                                ImGui::EndMenu();
                            }
                            if(ImGui::BeginMenu("Remove Component"))
                            {
                                int item_count{ 0 };
                                _remove_component_menu_item<arcadia::camera_component>(item_count);
                                _remove_component_menu_item<arcadia::light_component>(item_count);
                                _remove_component_menu_item<arcadia::model_component>(item_count);

                                if(item_count == 0)
                                {
                                    ImGui::Text("(No more component to remove)");
                                }

                                ImGui::EndMenu();
                            }
                        }

                        ImGui::EndPopup();
                    }
                }
            }
        }
    }
    ImGui::End();

}

void arcadia::imgui_window_outliner::_on_open_imgui_window(arcadia::event::open_imgui_window& e)
{
    const auto& [id_str] = e.data_tuple;
    if(id_str == get_id_str())
    {
        _open = true;
    }
}

void arcadia::imgui_window_outliner::_on_scene_activated(arcadia::event::scene_activated& e)
{
    const auto& [scene_wptr] = e.data_tuple;
    _scene_wptr = scene_wptr;
}

void arcadia::imgui_window_outliner::_on_scene_deactivated(arcadia::event::scene_deactivated& e)
{
    _scene_wptr.reset();
    _selected_entity = entt::null;
}
