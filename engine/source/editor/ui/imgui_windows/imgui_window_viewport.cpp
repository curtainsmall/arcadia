#include "imgui_window_viewport.hpp"

#include "core/assert.hpp"
#include "core/command.hpp"
#include "core/function.hpp"
#include "function/physics/physics_service.hpp"
#include "function/render/renderer_service.hpp"
#include "function/window/window_events.hpp"
#include "resource/components.hpp"
#include "resource/scene_service.hpp"

#include "project/project_service.hpp"

Arcadia::ImguiWindowViewport::ImguiWindowViewport(Engine& engine, bool open, std::string_view title):
    ImguiWindowInterface(engine, open, title)
{
}

void Arcadia::ImguiWindowViewport::OnEvent(EventBase& e)
{
    EventDispatcher{ e }
        .Dispatch<Events::InputCursorMove>(ACDA_BIND_MEMBER_FN(_OnInputCursorMove))
        .Dispatch<Events::OpenImguiWindow>(ACDA_BIND_MEMBER_FN(_OnOpenImguiWindow))
        .Dispatch<Events::SceneActivated>(ACDA_BIND_MEMBER_FN(_OnSceneActivated))
        .Dispatch<Events::SceneDeactivated>(ACDA_BIND_MEMBER_FN(_OnSceneDeactivated))
        .Dispatch<Events::SelectEntity>(ACDA_BIND_MEMBER_FN(_OnSelectEntity))
        .Dispatch<Events::DeleteEntity>(ACDA_BIND_MEMBER_FN(_OnDeleteEntity))
        .Dispatch<Events::ShowGizmo>(ACDA_BIND_MEMBER_FN(_OnShowGizmo))
        .Dispatch<Events::KeyboardInputOccupied>(ACDA_BIND_MEMBER_FN(_OnKeyboardInputOccupied))
        .IsDispatched();
}

void Arcadia::ImguiWindowViewport::OnUpdate()
{
    if(!_Opened)
    {
        return;
    }

    SceneService* scene_service = _Engine.scenes;
    PhysicsService* physics_service = _Engine.physics_service;
    RendererService* renderer_service = _Engine.renderer_service;

    std::string imgui_window_title{};
    imgui_window_title
        .append(_Title)
        .append(GetIdString());

    ImGui::SetNextWindowSize(glm::vec2{ 1024,768 }, ImGuiCond_Once);
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoCollapse;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, glm::vec2(2, 2));
    if(ImGui::Begin(imgui_window_title.c_str(), &_Opened, window_flags))
    {
        if(!scene_service->HasActiveScene())
        {
            ImGui::Text("(No scene)");
        }
        else if(!renderer_service->HasRenderer())
        {
            ImGui::Text("(No renderer)");
        }
        else if(!scene_service->ActiveScene_IsEntityNameUsed(_ViewportCameraEntityName))
        {
        }
        else
        {
            ACDA_ASSERT(physics_service);
            ACDA_ASSERT(renderer_service);

            auto [viewport_camera_comp, viewport_transform_comp] = scene_service->ActiveScene_GetComponent<CameraComponent, TransformComponent>(scene_service->ActiveScene_GetEntityIdByName(_ViewportCameraEntityName));
            viewport_camera_comp.SetViewportSize(ImGui::GetContentRegionAvail());
            _Engine.events
                .Signal<Events::RendererSetEntity>(
                    _ViewportCameraEntityId,
                    Events::RendererSetEntity::ActionType::Build
                );

            glm::vec2 image_cursor_pos = ImGui::GetCursorPos();
            if(renderer_service->HasRenderResult())
            {
                ImGui::Image(renderer_service->GetRenderResultId(_ViewportCameraEntityId), viewport_camera_comp.GetViewportSize(), { 0,1 }, { 1,0 });
            }

            if(!_InViewportFreecamMode && ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Right))
            {
                _Engine.events.Signal<Events::WindowSetCursorInputMode>(GlfwWindowCursorInputMode::Disabled);
                _InViewportFreecamMode = true;
            }
            if(_InViewportFreecamMode && !ImGui::IsMouseDown(ImGuiMouseButton_Right))
            {
                _Engine.events.Signal<Events::WindowSetCursorInputMode>(GlfwWindowCursorInputMode::Normal);
                _InViewportFreecamMode = false;
            }

            // Viewport camera control
            if(_InViewportFreecamMode)
            {
                // Scroll to zoom (move viewport_camera_comp forwards or backwards along direction)
                if(ImGui::IsKeyDown(ImGuiKey_W))
                {
                    viewport_transform_comp.IncreasePosition(viewport_transform_comp.GetDirection() * viewport_camera_comp.GetSpeed());
                }
                else if(ImGui::IsKeyDown(ImGuiKey_S))
                {
                    viewport_transform_comp.IncreasePosition(-viewport_transform_comp.GetDirection() * viewport_camera_comp.GetSpeed());
                }
                else if(ImGui::IsKeyDown(ImGuiKey_A))
                {
                    viewport_transform_comp.IncreasePosition(glm::cross(CameraComponent::GetUpAxis(), viewport_transform_comp.GetDirection()) * viewport_camera_comp.GetSpeed());
                }
                else if(ImGui::IsKeyDown(ImGuiKey_D))
                {
                    viewport_transform_comp.IncreasePosition(-glm::cross(CameraComponent::GetUpAxis(), viewport_transform_comp.GetDirection()) * viewport_camera_comp.GetSpeed());
                }
                else if(ImGui::IsKeyDown(ImGuiKey_E))
                {
                    viewport_transform_comp.IncreasePosition(glm::cross(viewport_transform_comp.GetDirection(), glm::cross(CameraComponent::GetUpAxis(), viewport_transform_comp.GetDirection())) * viewport_camera_comp.GetSpeed());
                }
                else if(ImGui::IsKeyDown(ImGuiKey_Q))
                {
                    viewport_transform_comp.IncreasePosition(-glm::cross(viewport_transform_comp.GetDirection(), glm::cross(CameraComponent::GetUpAxis(), viewport_transform_comp.GetDirection())) * viewport_camera_comp.GetSpeed());
                }

                // Rotate view
                glm::vec2 offset = _CursorMoveDistance * .005f;
                float x_angle_offset = -offset.x;
                viewport_transform_comp.SetDirection(glm::normalize(glm::angleAxis(x_angle_offset, viewport_camera_comp.GetUpAxis()) * viewport_transform_comp.GetDirection()));
                float pitch_angle = glm::half_pi<float>() - glm::angle(viewport_transform_comp.GetDirection(), viewport_camera_comp.GetUpAxis());
                float y_angle_offset = glm::clamp(-offset.y + pitch_angle, -glm::half_pi<float>() + viewport_camera_comp.GetUpAxisAngleEpsilon(), glm::half_pi<float>() - viewport_camera_comp.GetUpAxisAngleEpsilon()) - pitch_angle;
                viewport_transform_comp.SetDirection(glm::normalize(glm::angleAxis(y_angle_offset, glm::cross(viewport_transform_comp.GetDirection(), viewport_camera_comp.GetUpAxis())) * viewport_transform_comp.GetDirection()));

                _CursorMoveDistance = Glm::Vec2_CreateZero();
            }

            // Display viewport viewport_camera_comp info
            ImGui::SetCursorPos(image_cursor_pos);
            ImGui::Text(std::format(
                "Camera - Pos: {} | Direction: {} {}",
                viewport_transform_comp.GetPosition(),
                viewport_transform_comp.GetDirection(),
                _InViewportFreecamMode ? std::string("(Free Cam) ") : std::string{}
            ).c_str());
            float gizmo_option_position_offset_to_right = 350.f;
            ImGui::SameLine(ImGui::GetWindowWidth() - gizmo_option_position_offset_to_right);
            const glm::vec2 gizmo_options_cursor_pos = ImGui::GetCursorPos();
            ImGui::Dummy({ 0,0 });
            float fps = 1.f / std::chrono::duration_cast<std::chrono::duration<float>>(_Engine.frame.DeltaTime).count();
            ImGui::Text(std::format("FPS: {:.2f}", fps).c_str());

            ImGui::SetCursorPos(gizmo_options_cursor_pos);

            if(_ShowGizmo && !_SelectedEntityId.IsNull())
            {
                // Gizmo option
                std::uint32_t selected_color = IM_COL32(50, 50, 120, 255);
                std::uint32_t hovered_color = selected_color;
                std::uint32_t active_color = selected_color;

                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hovered_color);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, active_color);

                TransformComponent& transform_comp = scene_service->ActiveScene_GetComponent<TransformComponent>(_SelectedEntityId);

                if(_GizmoMode == GizmoMode::Local)
                {
                    if(ImGui::Button("Local"))
                    {
                        _GizmoMode = GizmoMode::World;
                    }
                }
                else if(_GizmoMode == GizmoMode::World)
                {
                    if(ImGui::Button("World"))
                    {
                        _GizmoMode = GizmoMode::Local;
                    }
                }
                else
                {
                    ACDA_UNREACHABLE("Invalid gizmo mode");
                }
                ImGui::SetItemTooltip("Toggle world/local mode");

                ImGui::SameLine();
                if(_GizmoOption == GizmoOption::None)
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, selected_color);
                }
                ImGui::Button("None");
                if(_GizmoOption == GizmoOption::None)
                {
                    ImGui::PopStyleColor();
                }
                if(!_InViewportFreecamMode && (ImGui::IsItemClicked() || _GizmoShortcutAvailable && ImGui::IsKeyDown(ImGuiKey_Q)))
                {
                    _GizmoOption = GizmoOption::None;
                }
                ImGui::SetItemTooltip("Turn off gizmo (Q)");

                ImGui::SameLine();
                if(_GizmoOption == GizmoOption::Translation)
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, selected_color);
                }
                ImGui::Button("Translation");
                if(_GizmoOption == GizmoOption::Translation)
                {
                    ImGui::PopStyleColor();
                }
                if(!_InViewportFreecamMode && (ImGui::IsItemClicked() || _GizmoShortcutAvailable && ImGui::IsKeyDown(ImGuiKey_W)))
                {
                    _GizmoOption = GizmoOption::Translation;
                    _GizmoOriginPosition = transform_comp.GetPosition();
                    _GizmoOriginPivot = transform_comp.GetPivot();
                }
                ImGui::SetItemTooltip("Set gizmo to translation mode (W)");

                ImGui::SameLine();
                if(_GizmoOption == GizmoOption::Rotation)
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, selected_color);
                }
                ImGui::Button("Rotation");
                if(_GizmoOption == GizmoOption::Rotation)
                {
                    ImGui::PopStyleColor();
                }
                if(!_InViewportFreecamMode && (ImGui::IsItemClicked() || _GizmoShortcutAvailable && ImGui::IsKeyDown(ImGuiKey_E)))
                {
                    _GizmoOption = GizmoOption::Rotation;
                    _GizmoOriginRotationEularAngle = transform_comp.GetRotationEularAngle();
                }
                ImGui::SetItemTooltip("Set gizmo to rotation mode (E)");

                ImGui::SameLine();
                if(_GizmoOption == GizmoOption::Scale)
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, selected_color);
                }
                ImGui::Button("Scale");
                if(_GizmoOption == GizmoOption::Scale)
                {
                    ImGui::PopStyleColor();
                }
                if(!_InViewportFreecamMode && (ImGui::IsItemClicked() || _GizmoShortcutAvailable && ImGui::IsKeyDown(ImGuiKey_R)))
                {
                    _GizmoOption = GizmoOption::Scale;
                    _GizmoOriginScale = transform_comp.GetScale();
                }
                ImGui::SetItemTooltip("Set gizmo to scale mode (R)");

                ImGui::PopStyleColor(2);

                // Gizmo
                if(scene_service->ActiveScene_ContainsEntity(_SelectedEntityId) && scene_service->ActiveScene_ContainsAllComponents<TransformComponent>(_SelectedEntityId))
                {
                    ImGuizmo::SetDrawlist();

                    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, viewport_camera_comp.GetViewportSize().x, viewport_camera_comp.GetViewportSize().y);
                    glm::mat4 view_mat = viewport_camera_comp.GenerateViewMat4(viewport_transform_comp.GetPosition(), viewport_transform_comp.GetDirection());
                    glm::mat4 proj_mat = viewport_camera_comp.GenerateProjectiveMat4();

                    glm::mat4 transform_mat = transform_comp.GetTransformMatrix();
                    ImGuizmo::Manipulate(
                        glm::value_ptr(view_mat),
                        glm::value_ptr(proj_mat),
                        static_cast<ImGuizmo::OPERATION>(_GizmoOption),
                        static_cast<ImGuizmo::MODE>(_GizmoMode),
                        glm::value_ptr(transform_mat)
                    );

                    glm::vec3 scale{};
                    glm::vec3 translation{};
                    glm::vec3 rotation{};

                    // On use gizmo
                    if(ImGuizmo::IsUsing())
                    {
                        Glm::Decompose(transform_mat, translation, rotation, scale);
                        _GizmoEditState = GizmoEditState::Editing;
                    }
                    else
                    {
                        translation = transform_comp.GetPosition();
                        rotation = transform_comp.GetRotationEularAngle();
                        scale = transform_comp.GetScale();
                    }

                    if(!ImGuizmo::IsUsingAny() && _GizmoEditState == GizmoEditState::Editing)
                    {
                        _GizmoEditState = GizmoEditState::Edited;
                    }

                    switch(_GizmoOption)
                    {
                        case Arcadia::ImguiWindowViewport::GizmoOption::Translation:
                        {
                            transform_comp.SetPosition(translation);
                            transform_comp.SetPivot(_GizmoOriginPivot + translation - _GizmoOriginPosition);
                            if(_GizmoEditState == GizmoEditState::Edited)
                            {
                                _Engine.commands->Emplace(
                                    "Transform - Position",
                                    [&comp = transform_comp, value = translation, origin_ptr = &_GizmoOriginPosition]() -> void
                                    {
                                        comp.SetPosition(value);
                                        *origin_ptr = value;
                                    },
                                    [&comp = transform_comp, origin = _GizmoOriginPosition, origin_ptr = &_GizmoOriginPosition]() -> void
                                    {
                                        comp.SetPosition(origin);
                                        *origin_ptr = origin;
                                    }
                                );
                                _GizmoEditState = GizmoEditState::None;
                                _GizmoOriginPosition = transform_comp.GetPosition();
                                _GizmoOriginPivot = transform_comp.GetPivot();
                            }
                            break;
                        }
                        case Arcadia::ImguiWindowViewport::GizmoOption::Rotation:
                        {
                            glm::vec3 delta_rotation = rotation - _GizmoOriginRotationEularAngle;
                            transform_comp.SetRotationEularAngle(rotation);
                            if(_GizmoEditState == GizmoEditState::Edited)
                            {
                                _Engine.commands->Emplace(
                                    "Transform - Rotation",
                                    [&comp = transform_comp, value = delta_rotation, origin_ptr = &_GizmoOriginPosition]() -> void
                                    {
                                        comp.IncreaseRotationEularAngle(value);
                                        *origin_ptr = value;
                                    },
                                    [&comp = transform_comp, origin_delta = -delta_rotation, origin = _GizmoOriginRotationEularAngle, origin_ptr = &_GizmoOriginPosition]() -> void
                                    {
                                        comp.IncreaseRotationEularAngle(origin_delta);
                                        *origin_ptr = origin;
                                    }
                                );
                                _GizmoEditState = GizmoEditState::None;
                                _GizmoOriginRotationEularAngle = transform_comp.GetRotationEularAngle();
                            }
                            break;
                        }
                        case Arcadia::ImguiWindowViewport::GizmoOption::Scale:
                        {
                            transform_comp.SetScale(scale);
                            if(_GizmoEditState == GizmoEditState::Edited)
                            {
                                _Engine.commands->Emplace(
                                    "Transform - Scale",
                                    [&comp = transform_comp, value = scale, origin_ptr = &_GizmoOriginPosition]() -> void
                                    {
                                        comp.SetScale(value);
                                        *origin_ptr = value;
                                    },
                                    [&comp = transform_comp, origin = _GizmoOriginScale, origin_ptr = &_GizmoOriginPosition]() -> void
                                    {
                                        comp.SetScale(origin);
                                        *origin_ptr = origin;
                                    }
                                );
                                _GizmoEditState = GizmoEditState::None;
                                _GizmoOriginScale = transform_comp.GetScale();
                            }
                            break;
                        }
                        case Arcadia::ImguiWindowViewport::GizmoOption::None:
                        default:
                            break;
                    }
                }
            }
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void Arcadia::ImguiWindowViewport::_OnInputCursorMove(Events::InputCursorMove& e)
{
    _CursorMoveDistance = e.CursorMoveDistance;
}

void Arcadia::ImguiWindowViewport::_OnOpenImguiWindow(Events::OpenImguiWindow& e)
{
    if(e.IdString == GetIdString())
    {
        _Opened = true;
    }
}

void Arcadia::ImguiWindowViewport::_OnSceneActivated(Events::SceneActivated& e)
{
    const std::shared_ptr<Scene>& scene_sptr = e.spScene;
    if(!scene_sptr->IsEntityNameUsed(_ViewportCameraEntityName))
    {
        EntityId entity_id = scene_sptr->CreateEntity(_ViewportCameraEntityName, EntityType::Camera);
        EntityInfo& entity = scene_sptr->GetEntityInfo(entity_id);
        entity.Internal = true;
        scene_sptr->EmplaceComponent<CameraComponent>(entity_id);
        TransformComponent& transform_comp = scene_sptr->EmplaceComponent<TransformComponent>(entity_id);
        transform_comp.SetPosition(glm::vec3(1.f));
        transform_comp.IncreaseDirection(glm::vec3(-1.f));
    }
    _ViewportCameraEntityId = scene_sptr->GetEntityIdByName(_ViewportCameraEntityName);

    for(const auto& [entity_id, entity_info] : scene_sptr->GetEntityInfoStorage())
    {
        _Engine.events.Signal<Events::RendererSetEntity>(
            entity_id,
            Events::RendererSetEntity::ActionType::Build
        );
        _Engine.events.Signal<Events::PhysicsSimulatorSetEntity>(
            entity_id,
            Events::PhysicsSimulatorSetEntity::ActionType::Build
        );
    }
    _Engine.events.Signal<Events::RendererSetActive>(true);
}

void Arcadia::ImguiWindowViewport::_OnSceneDeactivated(Events::SceneDeactivated& e)
{
    _Engine.events.Signal<Events::RendererSetActive>(false);
    _Engine.events.Signal<Events::RendererReset>();
    _Engine.events.Signal<Events::PhysicsSimulatorSetActive>(false);
    _Engine.events.Signal<Events::PhysicsSimulatorReset>();
}

void Arcadia::ImguiWindowViewport::_OnSelectEntity(Events::SelectEntity& e)
{
    _SelectedEntityId = e.EntityId;
    SceneService* scene_service = _Engine.scenes;
    if(scene_service->ActiveScene_ContainsAllComponents<TransformComponent>(_SelectedEntityId))
    {
        const TransformComponent& transform_comp = scene_service->ActiveScene_GetComponent<TransformComponent>(_SelectedEntityId);
        _GizmoOriginPosition = transform_comp.GetPosition();
        _GizmoOriginRotationEularAngle = transform_comp.GetRotationEularAngle();
        _GizmoOriginScale = transform_comp.GetScale();
        _GizmoOriginPivot = transform_comp.GetPivot();
    }
}

void Arcadia::ImguiWindowViewport::_OnDeleteEntity(Events::DeleteEntity& e)
{
    if(_SelectedEntityId == e.EntityId)
    {
        _SelectedEntityId.SetNull();
    }
}

void Arcadia::ImguiWindowViewport::_OnShowGizmo(Events::ShowGizmo& e)
{
    _ShowGizmo = e.ShouldShowGizmo;
}

void Arcadia::ImguiWindowViewport::_OnKeyboardInputOccupied(Events::KeyboardInputOccupied& e)
{
    _GizmoShortcutAvailable = !e.Occupied;
}