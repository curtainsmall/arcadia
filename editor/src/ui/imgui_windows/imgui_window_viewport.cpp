#include "imgui_window_viewport.hpp"

#include"core/app/app_context.hpp"
#include"core/assert.hpp"
#include"core/function.hpp"
#include"function/window/window_events.hpp"
#include"resource/components/camera_component.hpp"
#include"resource/components/model_component.hpp"
#include"resource/components/physics_component.hpp"
#include"resource/components/transform_component.hpp"

void Arcadia::ImguiWindowViewport::OnEvent(EventBase& e)
{
    EventDispatcher{ e }
        .Dispatch<Events::InputCursorMove>(ACDA_BIND_MEMBER_FN(_OnInputCursorMove))
        .Dispatch<Events::OpenImguiWindow>(ACDA_BIND_MEMBER_FN(_OnOpenImguiWindow))
        .Dispatch<Events::ProjectBuilt>(ACDA_BIND_MEMBER_FN(_OnProjectBuilt))
        .Dispatch<Events::ProjectUnbuilt>(ACDA_BIND_MEMBER_FN(_OnProjectUnbuilt))
        .Dispatch<Events::SceneActivated>(ACDA_BIND_MEMBER_FN(_OnSceneActivated))
        .Dispatch<Events::SceneDeactivated>(ACDA_BIND_MEMBER_FN(_OnSceneDeactivated))
        .Dispatch<Events::SelectEntity>(ACDA_BIND_MEMBER_FN(_OnSelectEntity))
        .Dispatch<Events::DeleteEntity>(ACDA_BIND_MEMBER_FN(_OnDeleteEntity))
        .Dispatch<Events::RendererBuilt>(ACDA_BIND_MEMBER_FN(_OnRendererBuilt))
        .Dispatch<Events::RendererUnbuilt>(ACDA_BIND_MEMBER_FN(_OnRendererUnbuilt))
        .Dispatch<Events::PhysicsSimulatorBuilt>(ACDA_BIND_MEMBER_FN(_OnPhysicsSimulatorBuilt))
        .Dispatch<Events::PhysicsSimulatorUnbuilt>(ACDA_BIND_MEMBER_FN(_OnPhysicsSimulatorUnbuilt))
        .Dispatch<Events::ShowGizmo>(ACDA_BIND_MEMBER_FN(_OnShowGizmo))
        .IsDispatched();
}

void Arcadia::ImguiWindowViewport::OnUpdate()
{
    if(!_Opened)
    {
        return;
    }

    std::shared_ptr<Scene> scene_sptr = _SceneWeakPtr.lock();
    std::shared_ptr<PhysicsSimulator> physics_simulator_sptr = _PhysicsSimulator.lock();
    std::shared_ptr<RendererInterface> renderer_sptr = _Renderer.lock();
    std::shared_ptr<Project> project_sptr = _Project.lock();

    const AppContext& app_context = AppContext::Instance();

    std::string imgui_title = _Title + GetIdString();

    ImGui::SetNextWindowSize(glm::vec2{ 1024,768 }, ImGuiCond_Once);
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_title.c_str(), &_Opened, window_flags))
    {
        if(!scene_sptr)
        {
            ImGui::Text("(No scene)");
        }
        else if(!renderer_sptr)
        {
            ImGui::Text("(No renderer)");
        }
        else
        {
            ACDA_ASSERT(physics_simulator_sptr);
            ACDA_ASSERT(renderer_sptr);

            physics_simulator_sptr->Prepare();
            renderer_sptr->Prepare();

            auto [viewport_camera_comp, viewport_transform_comp] = scene_sptr->GetComponent<CameraComponent, TransformComponent>(scene_sptr->GetEntityIdByName(_ViewportCameraEntityName));
            viewport_camera_comp.SetViewportSize(ImGui::GetContentRegionAvail());
            for(const auto& [entity_id, entity_info] : scene_sptr->GetEntityInfoStorage())
            {
                physics_simulator_sptr->Submit(*scene_sptr, entity_id);
            }
            physics_simulator_sptr->Finalize();
            physics_simulator_sptr->Update();
            for(const auto& [entity_id, entity_info] : scene_sptr->GetEntityInfoStorage())
            {
                physics_simulator_sptr->Query(*scene_sptr, entity_id);

                // We submit entity to renderer after query
                if(entity_info.Displayed)
                {
                    renderer_sptr->Submit(*scene_sptr, entity_id);
                }
            }

            renderer_sptr->Finalize();
            renderer_sptr->Draw();

            glm::vec2 image_cursor_pos = ImGui::GetCursorPos();
            ImGui::Image(renderer_sptr->GetRenderResultId(0), viewport_camera_comp.GetViewportSize(), { 0,1 }, { 1,0 });

            if(!_InViewportFreecamMode && ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Right))
            {
                EventQueue::Instance().Signal<Events::WindowSetCursorInputMode>(WindowCursorInputMode::Disabled);
                _InViewportFreecamMode = true;
            }
            if(_InViewportFreecamMode && !ImGui::IsMouseDown(ImGuiMouseButton_Right))
            {
                EventQueue::Instance().Signal<Events::WindowSetCursorInputMode>(WindowCursorInputMode::Normal);
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

                _CursorMoveDistance = GlmVec2::CreateZero();
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
            float fps = 1.f / std::chrono::duration_cast<std::chrono::duration<float>>(app_context.DeltaTime).count();
            ImGui::Text(std::format("FPS: {:.2f}", fps).c_str());

            ImGui::SetCursorPos(gizmo_options_cursor_pos);

            if(_ShowGizmo)
            {
                // Gizmo option
                std::uint32_t selected_color = IM_COL32(50, 50, 120, 255);
                std::uint32_t hovered_color = selected_color;
                std::uint32_t active_color = selected_color;

                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hovered_color);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, active_color);

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
                if(!_InViewportFreecamMode && (ImGui::IsItemClicked() || ImGui::IsKeyDown(ImGuiKey_Q)))
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
                if(!_InViewportFreecamMode && (ImGui::IsItemClicked() || ImGui::IsKeyDown(ImGuiKey_W)))
                {
                    _GizmoOption = GizmoOption::Translation;
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
                if(!_InViewportFreecamMode && (ImGui::IsItemClicked() || ImGui::IsKeyDown(ImGuiKey_E)))
                {
                    _GizmoOption = GizmoOption::Rotation;
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
                if(!_InViewportFreecamMode && (ImGui::IsItemClicked() || ImGui::IsKeyDown(ImGuiKey_R)))
                {
                    _GizmoOption = GizmoOption::Scale;
                }
                ImGui::SetItemTooltip("Set gizmo to scale mode (R)");

                ImGui::PopStyleColor(2);

                // Gizmo
                if(scene_sptr->ContainsEntity(_SelectedEntityId) && scene_sptr->ContainsAllComponents<TransformComponent>(_SelectedEntityId))
                {
                    ImGuizmo::SetDrawlist();

                    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, viewport_camera_comp.GetViewportSize().x, viewport_camera_comp.GetViewportSize().y);
                    glm::mat4 view_mat = viewport_camera_comp.GenerateViewMat4(viewport_transform_comp.GetPosition(), viewport_transform_comp.GetDirection());
                    glm::mat4 proj_mat = viewport_camera_comp.GenerateProjectiveMat4();

                    TransformComponent& transform_comp = scene_sptr->GetComponent<TransformComponent>(_SelectedEntityId);
                    glm::mat4 transform_mat = transform_comp.GetTransformMatrix();
                    ImGuizmo::Manipulate(
                        glm::value_ptr(view_mat),
                        glm::value_ptr(proj_mat),
                        static_cast<ImGuizmo::OPERATION>(_GizmoOption),
                        static_cast<ImGuizmo::MODE>(_GizmoMode),
                        glm::value_ptr(transform_mat)
                    );

                    // On use gizmo
                    if(ImGuizmo::IsUsing())
                    {
                        glm::vec3
                            scale{},
                            translation{},
                            rotation{};

                        Glm::Decompose(transform_mat, translation, rotation, scale);

                        if(transform_comp.GetPosition() != translation
                           || transform_comp.GetRotationEularAngle() != rotation
                           || transform_comp.GetScale() != scale)
                        {
                            _GizmoEdited = true;
                        }

                        glm::vec3 delta_rotation = rotation - transform_comp.GetRotationEularAngle();
                        transform_comp.IncreasePivot(translation - transform_comp.GetPosition());
                        transform_comp.SetPosition(translation);
                        transform_comp.IncreaseRotationEularAngle(delta_rotation);
                        transform_comp.SetScale(scale);
                    }

                    // On release gizmo
                    if(!ImGuizmo::IsUsingAny() && _GizmoEdited)
                    {
                        _GizmoEdited = false;

                        std::string description{};
                        switch(_GizmoOption)
                        {
                            case Arcadia::ImguiWindowViewport::GizmoOption::Translation:
                            {
                                description = "Translation";
                                break;
                            }
                            case Arcadia::ImguiWindowViewport::GizmoOption::Rotation:
                            {
                                description = "Rotation";
                                break;
                            }
                            case Arcadia::ImguiWindowViewport::GizmoOption::Scale:
                            {
                                description = "Scale";
                                break;
                            }
                            default:
                                break;
                        }
                        (void) description;
                    }
                }
            }
        }
    }
    ImGui::End();
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

void Arcadia::ImguiWindowViewport::_OnProjectBuilt(Events::ProjectBuilt& e)
{
    _Project = e.Project;
}

void Arcadia::ImguiWindowViewport::_OnProjectUnbuilt(Events::ProjectUnbuilt& e)
{
    _Project.reset();
}

void Arcadia::ImguiWindowViewport::_OnSceneActivated(Events::SceneActivated& e)
{
    const std::shared_ptr<Scene>& scene_sptr = e.spScene;
    if(!scene_sptr->IsEntityNameUsed(_ViewportCameraEntityName))
    {
        EntityId entity_id = scene_sptr->CreateEntity(_ViewportCameraEntityName, "camera");
        EntityInfo& entity = scene_sptr->GetEntityInfo(entity_id);
        entity.Internal = true;
        scene_sptr->EmplaceComponent<CameraComponent>(entity_id);
        TransformComponent& transform_comp = scene_sptr->EmplaceComponent<TransformComponent>(entity_id);
        transform_comp.SetPosition(glm::vec3(1.f));
        transform_comp.IncreaseDirection(glm::vec3(-1.f));
    }
    _SceneWeakPtr = scene_sptr;
}

void Arcadia::ImguiWindowViewport::_OnSceneDeactivated(Events::SceneDeactivated& e)
{
    _SceneWeakPtr.reset();
}

void Arcadia::ImguiWindowViewport::_OnSelectEntity(Events::SelectEntity& e)
{
    _SelectedEntityId = e.EntityId;
}

void Arcadia::ImguiWindowViewport::_OnDeleteEntity(Events::DeleteEntity& e)
{
    if(_SelectedEntityId == e.EntityId)
    {
        _SelectedEntityId.SetNull();
    }
}

void Arcadia::ImguiWindowViewport::_OnRendererBuilt(Events::RendererBuilt& e)
{
    _Renderer = e.Renderer;
}

void Arcadia::ImguiWindowViewport::_OnRendererUnbuilt(Events::RendererUnbuilt& e)
{
    _Renderer.reset();
}

void Arcadia::ImguiWindowViewport::_OnPhysicsSimulatorBuilt(Events::PhysicsSimulatorBuilt& e)
{
    _PhysicsSimulator = e.PhysicsSimulator;
}

void Arcadia::ImguiWindowViewport::_OnPhysicsSimulatorUnbuilt(Events::PhysicsSimulatorUnbuilt& e)
{
    _PhysicsSimulator.reset();
}

void Arcadia::ImguiWindowViewport::_OnShowGizmo(Events::ShowGizmo& e)
{
    _ShowGizmo = e.ShouldShowGizmo;
}