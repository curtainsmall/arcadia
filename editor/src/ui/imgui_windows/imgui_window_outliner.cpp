#include "imgui_window_outliner.hpp"

#include"core/command/command.hpp"
#include"core/event/event.hpp"
#include"core/file/pfd_header.hpp"
#include"ui/imgui_header.hpp"
#include"resource/components/camera_component.hpp"
#include"resource/components/light_component.hpp"
#include"resource/components/model_component.hpp"
#include"resource/components/physics_component.hpp"
#include"resource/components/skybox_component.hpp"

void ImguiWindowOutliner::on_event(EventBase& e)
{
    EventDispatcher{ e }
        .dispatch<events::OpenImguiWindow>(ACDA_BIND_MEMBER_FN(_on_open_imgui_window))
        .dispatch<events::SceneActivated>(ACDA_BIND_MEMBER_FN(_on_scene_activated))
        .dispatch<events::SceneDeactivated>(ACDA_BIND_MEMBER_FN(_on_scene_deactivated))
        .dispatch<events::RenameEntity>(ACDA_BIND_MEMBER_FN(_on_rename_entity))
        .is_dispatched();
}

void ImguiWindowOutliner::on_update()
{
    if(!_open)
    {
        return;
    }

    auto scene = _scene.lock();

    auto& event_queue = EventQueue::instance();

    auto imgui_window_title = scene
        ? _title + " - " + scene->name + get_id_str()
        : _title + get_id_str();

    ImGui::SetNextWindowSize(glm::vec2{ 1024,768 }, ImGuiCond_Once);
    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_window_title.c_str(), &_open, window_flags))
    {
        if(scene && ImGui::BeginPopupContextWindow())
        {
            ImGui::SeparatorText("New");

            if(ImGui::Selectable("Actor"))
            {
                event_queue.signal<events::NewEntity>("actor");
            }

            if(ImGui::Selectable("Camera"))
            {
                event_queue.signal<events::NewEntity>("camera");
            }

            if(ImGui::Selectable("Light"))
            {
                event_queue.signal<events::NewEntity>("light");
            }

            /*if(ImGui::Selectable("Custom"))
            {
                event_queue.signal<event::NewEntity>("");
            }*/
            ImGui::EndPopup();
        }

        if(!scene)
        {
            ImGui::Text("(No scene)");
        }
        else
        {
            for(auto& [name, entity_info] : *scene)
            {
                // Display text input
                if(_EntityOldName == name)
                {
                    _EntityNewName = _EntityOldName;

                    auto input_text_flags =
                        ImGuiInputTextFlags_AutoSelectAll;
                    ImGui::InputText("##rename_entity", &_EntityNewName, input_text_flags);
                    ImGui::SetItemDefaultFocus();
                    if(!ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsKeyPressed(ImGuiKey_Enter))
                    {
                        if(_EntityOldName != _EntityNewName)
                        {
                            if(scene->contains(_EntityNewName))
                            {
                                pfd::message msg{
                                    "Rename Entity",
                                    std::format("Failed to rename {} to {}, because the new name is already used",_EntityOldName,_EntityNewName),
                                    pfd::choice::ok,
                                    pfd::icon::info
                                };
                            }
                            else
                            {
                                event_queue.signal<events::RenameEntity>(_EntityOldName, _EntityNewName);
                            }
                        }
                        _EntityOldName.clear();
                        _EntityNewName.clear();
                    }
                }
                // Display selectable
                else
                {
                    if(entity_info.internal)
                    {
                        continue;
                    }

                    ImGui::Checkbox(std::format("##render_in_viewport_{}", name).c_str(), &entity_info.display);
                    ImGui::SameLine();
                    if(ImGui::Selectable(name.c_str(), _selected_entity_name == name))
                    {
                        _selected_entity_name = name;
                        event_queue.signal<events::SelectEntity>(name);
                    }
                    if(ImGui::IsItemHovered())
                    {
                        ImGui::SetTooltip(entity_info.type.c_str());
                    }

                    if(ImGui::BeginPopupContextItem())
                    {
                        if(ImGui::Selectable("Rename Entity"))
                        {
                            _EntityOldName = name;
                        }
                        if(ImGui::Selectable("Delete Entity"))
                        {
                            event_queue.signal<events::DeleteEntity>(name);
                        }

                    #if 0 // We do not allow custom entity for now
                        if(!_selected_entity_name.empty())
                        {
                            ImGui::Separator();
                            if(ImGui::BeginMenu("Add Component"))
                            {
                                int item_count{ 0 };
                                _menu_item_add_component<CameraComponent>(item_count);
                                _menu_item_add_component<LightComponent>(item_count);
                                _menu_item_add_component<ModelComponent>(item_count);
                                _menu_item_add_component<PhysicsComponent>(item_count);

                                if(item_count == 0)
                                {
                                    ImGui::Text("(No more component to add)");
                                }

                                ImGui::EndMenu();
                            }
                            if(ImGui::BeginMenu("Remove Component"))
                            {
                                int item_count{ 0 };
                                _menu_item_remove_component<CameraComponent>(item_count);
                                _menu_item_remove_component<LightComponent>(item_count);
                                _menu_item_remove_component<ModelComponent>(item_count);
                                _menu_item_remove_component<PhysicsComponent>(item_count);

                                if(item_count == 0)
                                {
                                    ImGui::Text("(No more component to remove)");
                                }

                                ImGui::EndMenu();
                            }
                        }
                    #endif

                        ImGui::EndPopup();
                    }
                }
            }
        }
    }
    ImGui::End();

}

void ImguiWindowOutliner::_on_open_imgui_window(events::OpenImguiWindow& e)
{
    const auto& [id_str] = e.data_tuple;
    if(id_str == get_id_str())
    {
        _open = true;
    }
}

void ImguiWindowOutliner::_on_scene_activated(events::SceneActivated& e)
{
    const auto& [scene] = e.data_tuple;
    _scene = scene;
}

void ImguiWindowOutliner::_on_scene_deactivated(events::SceneDeactivated& e)
{
    _scene.reset();
    _selected_entity_name.clear();
}

void ImguiWindowOutliner::_on_rename_entity(events::RenameEntity& e)
{
    const auto& [old_name, new_name] = e.data_tuple;
    if(old_name == _selected_entity_name)
    {
        _selected_entity_name = new_name;
    }
}
