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

void ImguiWindowOutliner::OnEvent(EventBase& event)
{
    EventDispatcher{ event }
        .Dispatch<Event::OpenImguiWindow>(ACDA_BIND_MEMBER_FN(_OnOpenImguiWindow))
        .Dispatch<Event::SceneActivated>(ACDA_BIND_MEMBER_FN(_OnSceneActivated))
        .Dispatch<Event::SceneDeactivated>(ACDA_BIND_MEMBER_FN(_OnSceneDeactivated))
        .Dispatch<Event::RenameEntity>(ACDA_BIND_MEMBER_FN(_OnRenameEntity))
        .Result();
}

void ImguiWindowOutliner::OnUpdate()
{
    if(!_Open)
    {
        return;
    }

    auto scene = _Scene.lock();

    auto& event_queue = EventQueue::Instance();

    auto imgui_window_title = scene
        ? _Title + " - " + scene->Name + GetIdStr()
        : _Title + GetIdStr();

    ImGui::SetNextWindowSize(glm::vec2{ 1024,768 }, ImGuiCond_Once);
    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_window_title.c_str(), &_Open, window_flags))
    {
        if(scene && ImGui::BeginPopupContextWindow())
        {
            ImGui::SeparatorText("New");

            if(ImGui::Selectable("Actor"))
            {
                event_queue.Signal<Event::NewEntity>("actor");
            }

            if(ImGui::Selectable("Camera"))
            {
                event_queue.Signal<Event::NewEntity>("camera");
            }

            if(ImGui::Selectable("Light"))
            {
                event_queue.Signal<Event::NewEntity>("light");
            }

            /*if(ImGui::Selectable("Custom"))
            {
                event_queue.Signal<Event::NewEntity>("");
            }*/
            ImGui::EndPopup();
        }

        if(!scene)
        {
            ImGui::Text("No scene to outline here");
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
                    ImGui::InputText("###rename_entity", &_EntityNewName, input_text_flags);
                    ImGui::SetItemDefaultFocus();
                    if(!ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsKeyPressed(ImGuiKey_Enter))
                    {
                        if(_EntityOldName != _EntityNewName)
                        {
                            if(scene->Contains(_EntityNewName))
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
                                event_queue.Signal<Event::RenameEntity>(_EntityOldName, _EntityNewName);
                            }
                        }
                        _EntityOldName.clear();
                        _EntityNewName.clear();
                    }
                }
                // Display selectable
                else
                {
                    if(entity_info.Internal)
                    {
                        continue;
                    }

                    ImGui::Checkbox(std::format("##render_in_viewport_{}", name).c_str(), &entity_info.Display);
                    ImGui::SameLine();
                    if(ImGui::Selectable(name.c_str(), _SelectedEntityName == name))
                    {
                        _SelectedEntityName = name;
                        event_queue.Signal<Event::SelectEntity>(name);
                    }
                    if(ImGui::IsItemHovered())
                    {
                        ImGui::SetTooltip(entity_info.Type.c_str());
                    }

                    if(ImGui::BeginPopupContextItem())
                    {
                        if(ImGui::Selectable("Rename Entity"))
                        {
                            _EntityOldName = name;
                        }
                        if(ImGui::Selectable("Delete Entity"))
                        {
                            event_queue.Signal<Event::DeleteEntity>(name);
                        }

                    #if 0 // We do not allow custom entity for now
                        if(!_SelectedEntityName.empty())
                        {
                            ImGui::Separator();
                            if(ImGui::BeginMenu("Add Component"))
                            {
                                int item_count{ 0 };
                                _MenuItemAddComponent<CameraComponent>(item_count);
                                _MenuItemAddComponent<LightComponent>(item_count);
                                _MenuItemAddComponent<ModelComponent>(item_count);
                                _MenuItemAddComponent<PhysicsComponent>(item_count);

                                if(item_count == 0)
                                {
                                    ImGui::Text("(No more component to add)");
                                }

                                ImGui::EndMenu();
                            }
                            if(ImGui::BeginMenu("Remove Component"))
                            {
                                int item_count{ 0 };
                                _MenuItemRemoveComponent<CameraComponent>(item_count);
                                _MenuItemRemoveComponent<LightComponent>(item_count);
                                _MenuItemRemoveComponent<ModelComponent>(item_count);
                                _MenuItemRemoveComponent<PhysicsComponent>(item_count);

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

void ImguiWindowOutliner::_OnOpenImguiWindow(Event::OpenImguiWindow& e)
{
    const auto& [id_str] = e.data_tuple;
    if(id_str == GetIdStr())
    {
        _Open = true;
    }
}

void ImguiWindowOutliner::_OnSceneActivated(Event::SceneActivated& e)
{
    const auto& [scene] = e.data_tuple;
    _Scene = scene;
}

void ImguiWindowOutliner::_OnSceneDeactivated(Event::SceneDeactivated& e)
{
    _Scene.reset();
    _SelectedEntityName.clear();
}

void ImguiWindowOutliner::_OnRenameEntity(Event::RenameEntity& e)
{
    const auto& [old_name, new_name] = e.data_tuple;
    if(old_name == _SelectedEntityName)
    {
        _SelectedEntityName = new_name;
    }
}
