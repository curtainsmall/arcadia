#include "imgui_window_outliner.hpp"

#include"core/command/command.hpp"
#include"core/event/event.hpp"
#include"core/file/pfd_header.hpp"
#include"function/ui/imgui_header.hpp"
#include"resource/components/camera_component.hpp"
#include"resource/components/light_component.hpp"
#include"resource/components/model_component.hpp"
#include"resource/components/physics_component.hpp"
#include"resource/components/skybox_component.hpp"

void Arcadia::ImguiWindowOutliner::OnEvent(Arcadia::EventBase& event)
{
    Arcadia::EventDispatcher{ event }
        .Dispatch<Arcadia::Event::OpenImguiWindow>(ARCADIA_BIND_MEMBER_FN(_OnOpenImguiWindow))
        .Dispatch<Arcadia::Event::SceneActivated>(ARCADIA_BIND_MEMBER_FN(_OnSceneActivated))
        .Dispatch<Arcadia::Event::SceneDeactivated>(ARCADIA_BIND_MEMBER_FN(_OnSceneDeactivated))
        .Result();
}

void Arcadia::ImguiWindowOutliner::OnUpdate()
{
    if(!_Open)
    {
        return;
    }

    auto scene = _Scene.lock();

    auto& event_queue = Arcadia::EventQueue::Instance();

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
            if(ImGui::Selectable("New Entity"))
            {
                event_queue.Signal<Arcadia::Event::NewEntity>("");
            }
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
                                scene->Rename(_EntityOldName, _EntityNewName);
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
                        event_queue.Signal<Arcadia::Event::SelectEntity>(name);
                    }

                    if(ImGui::BeginPopupContextItem())
                    {
                        if(ImGui::Selectable("Delete Entity"))
                        {
                            event_queue.Signal<Arcadia::Event::DeleteEntity>(name);
                        }
                        if(ImGui::Selectable("Rename Entity"))
                        {
                            _EntityOldName = name;
                        }
                        if(!_SelectedEntityName.empty())
                        {
                            ImGui::Separator();
                            if(ImGui::BeginMenu("Add Component"))
                            {
                                int item_count{ 0 };
                                _MenuItemAddComponent<Arcadia::CameraComponent>(item_count);
                                _MenuItemAddComponent<Arcadia::LightComponent>(item_count);
                                _MenuItemAddComponent<Arcadia::ModelComponent>(item_count);
                                _MenuItemAddComponent<Arcadia::PhysicsComponent>(item_count);

                                if(item_count == 0)
                                {
                                    ImGui::Text("(No more component to add)");
                                }

                                ImGui::EndMenu();
                            }
                            if(ImGui::BeginMenu("Remove Component"))
                            {
                                int item_count{ 0 };
                                _MenuItemRemoveComponent<Arcadia::CameraComponent>(item_count);
                                _MenuItemRemoveComponent<Arcadia::LightComponent>(item_count);
                                _MenuItemRemoveComponent<Arcadia::ModelComponent>(item_count);
                                _MenuItemRemoveComponent<Arcadia::PhysicsComponent>(item_count);

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

void Arcadia::ImguiWindowOutliner::_OnOpenImguiWindow(Arcadia::Event::OpenImguiWindow& e)
{
    const auto& [id_str] = e.data_tuple;
    if(id_str == GetIdStr())
    {
        _Open = true;
    }
}

void Arcadia::ImguiWindowOutliner::_OnSceneActivated(Arcadia::Event::SceneActivated& e)
{
    const auto& [scene] = e.data_tuple;
    _Scene = scene;
}

void Arcadia::ImguiWindowOutliner::_OnSceneDeactivated(Arcadia::Event::SceneDeactivated& e)
{
    _Scene.reset();
    _SelectedEntityName.clear();
}
