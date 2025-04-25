#include "imgui_window_outliner.hpp"

#include"core/command/command.hpp"
#include"core/event/event.hpp"
#include"core/file/pfd_header.hpp"
#include"core/function.hpp"
#include"resource/components/camera_component.hpp"
#include"resource/components/light_component.hpp"
#include"resource/components/model_component.hpp"
#include"resource/components/physics_component.hpp"
#include"resource/components/skybox_component.hpp"
#include"ui/imgui_header.hpp"

void Arcadia::ImguiWindowOutliner::OnEvent(EventBase& e)
{
    EventDispatcher(e)
        .Dispatch<Events::OpenImguiWindow>(ACDA_BIND_MEMBER_FN(_OnOpenImguiWindow))
        .Dispatch<Events::SceneActivated>(ACDA_BIND_MEMBER_FN(_OnSceneActivated))
        .Dispatch<Events::SceneDeactivated>(ACDA_BIND_MEMBER_FN(_OnSceneDeactivated))
        .IsDispatched();
}

void Arcadia::ImguiWindowOutliner::OnUpdate()
{
    if(!_Opened)
    {
        return;
    }

    std::shared_ptr<Scene> scene_sptr = _wpScene.lock();

    EventQueue& event_queue = EventQueue::Instance();

    std::string imgui_window_title = scene_sptr
        ? _Title + " - " + scene_sptr->GetName() + GetIdString()
        : _Title + GetIdString();

    ImGui::SetNextWindowSize(glm::vec2{ 1024,768 }, ImGuiCond_Once);
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_window_title.c_str(), &_Opened, window_flags))
    {
        if(scene_sptr && ImGui::BeginPopupContextWindow())
        {
            ImGui::SeparatorText("New");

            if(ImGui::Selectable("Actor"))
            {
                event_queue.Signal<Events::NewEntity>("actor");
            }

            if(ImGui::Selectable("Camera"))
            {
                event_queue.Signal<Events::NewEntity>("camera");
            }

            if(ImGui::Selectable("Light"))
            {
                event_queue.Signal<Events::NewEntity>("light");
            }

            /*if(ImGui::Selectable("Custom"))
            {
                event_queue.Signal<event::NewEntity>("");
            }*/
            ImGui::EndPopup();
        }

        if(!scene_sptr)
        {
            ImGui::Text("(No scene)");
        }
        else
        {
            for(auto& [entity_id, entity_info] : scene_sptr->GetEntityInfoStorage())
            {
                // Display text input
                if(_EntityOldName == entity_info.GetName())
                {
                    _EntityNewName = _EntityOldName;

                    ImGuiInputFlags input_text_flags =
                        ImGuiInputTextFlags_AutoSelectAll;
                    ImGui::InputText("##rename_entity", &_EntityNewName, input_text_flags);
                    ImGui::SetItemDefaultFocus();
                    if(!ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsKeyPressed(ImGuiKey_Enter))
                    {
                        if(_EntityOldName != _EntityNewName)
                        {
                            if(scene_sptr->IsEntityNameUsed(_EntityNewName))
                            {
                                pfd::message msg{
                                    "Arcadia - Rename Entity",
                                    std::format("Failed to rename {} to {}, because the new name is already used",_EntityOldName,_EntityNewName),
                                    pfd::choice::ok,
                                    pfd::icon::info
                                };
                            }
                            else
                            {
                                event_queue.Signal<Events::RenameEntity>(entity_id, _EntityNewName);
                            }
                        }
                        _EntityOldName.clear();
                        _EntityNewName.clear();
                    }
                }
                // Display selectable
                else
                {
                    // Internal entity will not be displayed, and thus not be selectable
                    if(entity_info.Internal)
                    {
                        continue;
                    }

                    ImGui::Checkbox(std::format("##render_in_viewport_{}", entity_id).c_str(), &entity_info.Displayed);
                    ImGui::SameLine();
                    if(ImGui::Selectable(entity_info.GetName().c_str(), _SelectedEntityId == entity_id))
                    {
                        _SelectedEntityId = entity_id;
                        event_queue.Signal<Events::SelectEntity>(entity_id);
                    }
                    if(ImGui::IsItemHovered())
                    {
                        ImGui::SetTooltip(entity_info.TypeString.c_str());
                    }

                    if(ImGui::BeginPopupContextItem())
                    {
                        if(ImGui::Selectable("Rename Entity"))
                        {
                            _EntityOldName = entity_info.GetName();
                        }
                        if(ImGui::Selectable("Delete Entity"))
                        {
                            event_queue.Signal<Events::DeleteEntity>(entity_id);
                        }

                    #if 0 // We do not allow custom entity for now
                        if(!_SelectedEntityId.empty())
                        {
                            ImGui::Separator();
                            if(ImGui::BeginMenu("Add Component"))
                            {
                                std::int32_t item_count = 0;
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
                                std::int32_t item_count = 0;
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

void Arcadia::ImguiWindowOutliner::_OnOpenImguiWindow(Events::OpenImguiWindow& e)
{
    if(e.IdString == GetIdString())
    {
        _Opened = true;
    }
}

void Arcadia::ImguiWindowOutliner::_OnSceneActivated(Events::SceneActivated& e)
{
    _wpScene = e.spScene;
}

void Arcadia::ImguiWindowOutliner::_OnSceneDeactivated(Events::SceneDeactivated& e)
{
    _wpScene.reset();
    _SelectedEntityId.SetNull();
}