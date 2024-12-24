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
        .Dispatch<Events::RenameEntity>(ACDA_BIND_MEMBER_FN(_OnRenameEntity))
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
    std::shared_ptr<iRenderer> renderer_sptr = _Renderer.lock();
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

            auto [viewport_camera_comp, viewport_transform_comp] = scene_sptr->GetComponent<CameraComponent, TransformComponent>(ViewportCameraEntityName);
            viewport_camera_comp.ViewportSize = ImGui::GetContentRegionAvail();
            for(const auto& [name, entity_info] : scene_sptr->GetEntityInfoStorage())
            {
                physics_simulator_sptr->Submit(*scene_sptr, name);
            }
            physics_simulator_sptr->Finalize();
            physics_simulator_sptr->Update();
            for(const auto& [name, entity_info] : scene_sptr->GetEntityInfoStorage())
            {
                physics_simulator_sptr->Query(*scene_sptr, name);

                // We submit entity to renderer after query
                if(entity_info.Display)
                {
                    renderer_sptr->Submit(*scene_sptr, name);
                }
            }

            renderer_sptr->Finalize();
            renderer_sptr->Draw();

            glm::vec2 image_cursor_pos = ImGui::GetCursorPos();
            ImGui::Image(renderer_sptr->GetRenderResultId(0), viewport_camera_comp.ViewportSize, { 0,1 }, { 1,0 });

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
                    viewport_transform_comp.Position += viewport_transform_comp.Direction * viewport_camera_comp.Speed;
                }
                else if(ImGui::IsKeyDown(ImGuiKey_S))
                {
                    viewport_transform_comp.Position -= viewport_transform_comp.Direction * viewport_camera_comp.Speed;
                }
                else if(ImGui::IsKeyDown(ImGuiKey_A))
                {
                    viewport_transform_comp.Position += glm::cross(CameraComponent::Up, viewport_transform_comp.Direction) * viewport_camera_comp.Speed;
                }
                else if(ImGui::IsKeyDown(ImGuiKey_D))
                {
                    viewport_transform_comp.Position -= glm::cross(CameraComponent::Up, viewport_transform_comp.Direction) * viewport_camera_comp.Speed;
                }
                else if(ImGui::IsKeyDown(ImGuiKey_E))
                {
                    viewport_transform_comp.Position += glm::cross(viewport_transform_comp.Direction, glm::cross(CameraComponent::Up, viewport_transform_comp.Direction)) * viewport_camera_comp.Speed;
                }
                else if(ImGui::IsKeyDown(ImGuiKey_Q))
                {
                    viewport_transform_comp.Position -= glm::cross(viewport_transform_comp.Direction, glm::cross(CameraComponent::Up, viewport_transform_comp.Direction)) * viewport_camera_comp.Speed;
                }

                // Rotate view
                glm::vec2 offset = _CursorMoveDistance * .005f;
                float x_angle_offset = -offset.x;
                viewport_transform_comp.Direction = glm::normalize(glm::angleAxis(x_angle_offset, viewport_camera_comp.Up) * viewport_transform_comp.Direction);
                float pitch_angle = glm::half_pi<float>() - glm::angle(viewport_transform_comp.Direction, viewport_camera_comp.Up);
                float y_angle_offset = glm::clamp(-offset.y + pitch_angle, -glm::half_pi<float>() + viewport_camera_comp.UpEpsilon, glm::half_pi<float>() - viewport_camera_comp.UpEpsilon) - pitch_angle;
                viewport_transform_comp.Direction = glm::normalize(glm::angleAxis(y_angle_offset, glm::cross(viewport_transform_comp.Direction, viewport_camera_comp.Up)) * viewport_transform_comp.Direction);

                _CursorMoveDistance = GlmVec2::CreateZero();
            }

            // Display viewport viewport_camera_comp info
            ImGui::SetCursorPos(image_cursor_pos);
            ImGui::Text(std::format(
                "Camera - Pos: {} | Direction: {} {}",
                viewport_transform_comp.Position,
                viewport_transform_comp.Direction,
                _InViewportFreecamMode ? std::string("(Free Cam) ") : std::string{}
            ).c_str());
            ImGui::SameLine(ImGui::GetWindowWidth() - 300.f);
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
                if(!_SelectedEntityName.empty() && scene_sptr->ContainsAllComponents<TransformComponent>(_SelectedEntityName))
                {
                    ImGuizmo::SetDrawlist();

                    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, viewport_camera_comp.ViewportSize.x, viewport_camera_comp.ViewportSize.y);
                    glm::mat4 view_mat = viewport_camera_comp.GenerateViewMat4(viewport_transform_comp.Position, viewport_transform_comp.Direction);
                    glm::mat4 proj_mat = viewport_camera_comp.GenerateProjectiveMat4();

                    TransformComponent& transform_comp = scene_sptr->GetComponent<TransformComponent>(_SelectedEntityName);
                    glm::mat4 transform_mat = transform_comp.GenerateTransformMat4();
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
                        //_GizmoEdited = true;

                        glm::vec3
                            scale{},
                            translation{},
                            skew{};
                        glm::vec4 perspective{};
                        glm::quat rotation{};

                        glm::decompose(transform_mat, scale, rotation, translation, skew, perspective);

                        if(transform_comp.Position != translation
                           || transform_comp.Rotation != rotation
                           || transform_comp.Scale != scale)
                        {
                            _GizmoEdited = true;
                        }

                        if(transform_comp.Position != translation)
                        {
                            //transform_comp.pivot += translation - transform_comp.position;
                        }

                        transform_comp.Position = translation;
                        transform_comp.Rotation = rotation;
                        transform_comp.Scale = scale;
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
                        if(!description.empty())
                        {
                            MementoList::Instance()
                                .Snapshot<TransformComponent>(
                                    std::format("{} - {}", "Transform", description),
                                    [&]() -> TransformComponent&
                            {
                                return scene_sptr->GetComponent<TransformComponent>(_SelectedEntityName);
                            }
                                );
                        }
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
    const std::shared_ptr<Scene>& scene_sptr = e.Scene;
    if(!scene_sptr->ContainsEntity(ViewportCameraEntityName))
    {
        EntityInfo& entity_info = scene_sptr->CreateEntity(ViewportCameraEntityName, "camera");
        entity_info.Internal = true;
        scene_sptr->EmplaceComponent<CameraComponent>(ViewportCameraEntityName);
        TransformComponent& transform_comp = scene_sptr->EmplaceComponent<TransformComponent>(ViewportCameraEntityName);
        transform_comp.Position = glm::vec3(1.f, 1.f, 1.f);
        transform_comp.Direction = -transform_comp.Position;
    }
    _SceneWeakPtr = scene_sptr;
}

void Arcadia::ImguiWindowViewport::_OnSceneDeactivated(Events::SceneDeactivated& e)
{
    _SceneWeakPtr.reset();
}

void Arcadia::ImguiWindowViewport::_OnSelectEntity(Events::SelectEntity& e)
{
    _SelectedEntityName = e.EntityName;
}

void Arcadia::ImguiWindowViewport::_OnRenameEntity(Events::RenameEntity& e)
{
    if(e.OldName == _SelectedEntityName)
    {
        _SelectedEntityName = e.NewName;
    }
}

void Arcadia::ImguiWindowViewport::_OnDeleteEntity(Events::DeleteEntity& e)
{
    if(_SelectedEntityName == e.EntityName)
    {
        _SelectedEntityName.clear();
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