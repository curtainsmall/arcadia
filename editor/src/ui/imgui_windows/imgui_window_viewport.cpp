#include "imgui_window_viewport.hpp"

#include"core/app/app_context.hpp"
#include"function/window/window_events.hpp"
#include"resource/components/camera_component.hpp"
#include"resource/components/model_component.hpp"
#include"resource/components/physics_component.hpp"
#include"resource/components/transform_component.hpp"

void ImguiWindowViewport::OnEvent(EventBase& e)
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

void ImguiWindowViewport::OnUpdate()
{
    if(!_Opened)
    {
        return;
    }

    auto scene = _Scene.lock();
    auto physics_simulator = _PhysicsSimulator.lock();
    auto renderer = _Renderer.lock();
    auto project = _Project.lock();

    const auto& app_context = AppContext::Instance();

    auto imgui_title = _Title + GetIdString();

    ImGui::SetNextWindowSize(glm::vec2{ 1024,768 }, ImGuiCond_Once);
    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_title.c_str(), &_Opened, window_flags))
    {
        if(!scene)
        {
            ImGui::Text("(No scene)");
        }
        else if(!renderer)
        {
            ImGui::Text("(No renderer)");
        }
        else
        {
            ACDA_ASSERT(physics_simulator);
            ACDA_ASSERT(renderer);

            physics_simulator->Prepare();
            renderer->Prepare();

            auto [viewport_camera_comp, viewport_transform_comp] = scene->GetComponent<CameraComponent, TransformComponent>(ViewportCameraEntityName);
            viewport_camera_comp.ViewportSize = ImGui::GetContentRegionAvail();
            for(const auto& [name, entity_info] : *scene)
            {
                physics_simulator->Submit(*scene, name);
            }
            physics_simulator->Finalize();
            physics_simulator->Update();
            for(const auto& [name, entity_info] : *scene)
            {
                physics_simulator->Query(*scene, name);

                // We submit entity to renderer after query
                if(entity_info.Display)
                {
                    renderer->Submit(*scene, name);
                }
            }

            renderer->Finalize();
            renderer->Draw();

            glm::vec2 image_cursor_pos = ImGui::GetCursorPos();
            ImGui::Image(renderer->GetRenderResultId(0), viewport_camera_comp.ViewportSize, { 0,1 }, { 1,0 });

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
                auto offset = _CursorMoveDistance * .005f;
                auto x_angle_offset = -offset.x;
                viewport_transform_comp.Direction = glm::normalize(glm::angleAxis(x_angle_offset, viewport_camera_comp.Up) * viewport_transform_comp.Direction);
                auto pitch_angle = glm::half_pi<float>() - glm::angle(viewport_transform_comp.Direction, viewport_camera_comp.Up);
                auto y_angle_offset = glm::clamp(-offset.y + pitch_angle, -glm::half_pi<float>() + viewport_camera_comp.UpEpsilon, glm::half_pi<float>() - viewport_camera_comp.UpEpsilon) - pitch_angle;
                viewport_transform_comp.Direction = glm::normalize(glm::angleAxis(y_angle_offset, glm::cross(viewport_transform_comp.Direction, viewport_camera_comp.Up)) * viewport_transform_comp.Direction);

                _CursorMoveDistance = Vec2::CreateZero();
            }

            // Display viewport viewport_camera_comp info
            ImGui::SetCursorPos(image_cursor_pos);
            ImGui::Text(std::format(
                "Camera - Pos: {} | Direction: {} {}",
                viewport_transform_comp.Position,
                viewport_transform_comp.Direction,
                _InViewportFreecamMode ? "(Free Cam) "s : ""s
            ).c_str());
            ImGui::SameLine(ImGui::GetWindowWidth() - 300.f);
            const auto gizmo_options_cursor_pos = ImGui::GetCursorPos();
            ImGui::Dummy({ 0,0 });
            float fps = 1.f / std::chrono::duration_cast<std::chrono::duration<float>>(app_context.DeltaTime).count();
            ImGui::Text(std::format("FPS: {:.2f}", fps).c_str());

            ImGui::SetCursorPos(gizmo_options_cursor_pos);

            if(_ShowGizmo)
            {
                // Gizmo option
                auto selected_color = IM_COL32(50, 50, 120, 255);
                auto hovered_color = selected_color;
                auto active_color = selected_color;

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
                if(!_SelectedEntityName.empty() && scene->ContainsAllComponents<TransformComponent>(_SelectedEntityName))
                {
                    ImGuizmo::SetDrawlist();

                    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, viewport_camera_comp.ViewportSize.x, viewport_camera_comp.ViewportSize.y);
                    glm::mat4 view_mat = viewport_camera_comp.GenerateViewMat4(viewport_transform_comp.Position, viewport_transform_comp.Direction);
                    glm::mat4 proj_mat = viewport_camera_comp.GenerateProjectiveMat4();

                    auto& transform_comp = scene->GetComponent<TransformComponent>(_SelectedEntityName);
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
                            case ImguiWindowViewport::GizmoOption::Translation:
                            {
                                description = "Translation";
                                break;
                            }
                            case ImguiWindowViewport::GizmoOption::Rotation:
                            {
                                description = "Rotation";
                                break;
                            }
                            case ImguiWindowViewport::GizmoOption::Scale:
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
                                    std::format("{} - {}", "Transform"s, description),
                                    [scene, this]() -> TransformComponent&
                            {
                                return scene->GetComponent<TransformComponent>(_SelectedEntityName);
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

void ImguiWindowViewport::_OnInputCursorMove(Events::InputCursorMove& e)
{
    const auto& [wnd_ptr, cursor_move] = e.DataTuple;
    _CursorMoveDistance = cursor_move;
}

void ImguiWindowViewport::_OnOpenImguiWindow(Events::OpenImguiWindow& e)
{
    const auto& [id_str] = e.DataTuple;
    if(id_str == GetIdString())
    {
        _Opened = true;
    }
}

void ImguiWindowViewport::_OnProjectBuilt(Events::ProjectBuilt& e)
{
    const auto& [project] = e.DataTuple;
    _Project = project;
}

void ImguiWindowViewport::_OnProjectUnbuilt(Events::ProjectUnbuilt& e)
{
    _Project.reset();
}

void ImguiWindowViewport::_OnSceneActivated(Events::SceneActivated& e)
{
    const auto& [scene] = e.DataTuple;
    if(!scene->ContainsEntity(ViewportCameraEntityName))
    {
        auto& entity_info = scene->CreateEntity(ViewportCameraEntityName, "camera");
        entity_info.Internal = true;
        scene->EmplaceComponent<CameraComponent>(ViewportCameraEntityName);
        auto& transform_comp = scene->EmplaceComponent<TransformComponent>(ViewportCameraEntityName);
        transform_comp.Position = glm::vec3{ 1.f,1.f,1.f };
        transform_comp.Direction = -transform_comp.Position;
    }
    _Scene = scene;
}

void ImguiWindowViewport::_OnSceneDeactivated(Events::SceneDeactivated& e)
{
    _Scene.reset();
}

void ImguiWindowViewport::_OnSelectEntity(Events::SelectEntity& e)
{
    const auto& [entity_name] = e.DataTuple;
    _SelectedEntityName = entity_name;
}

void ImguiWindowViewport::_OnRenameEntity(Events::RenameEntity& e)
{
    const auto& [old_name, new_name] = e.DataTuple;
    if(old_name == _SelectedEntityName)
    {
        _SelectedEntityName = new_name;
    }
}

void ImguiWindowViewport::_OnDeleteEntity(Events::DeleteEntity& e)
{
    const auto& [entity] = e.DataTuple;
    if(_SelectedEntityName == entity)
    {
        _SelectedEntityName.clear();
    }
}

void ImguiWindowViewport::_OnRendererBuilt(Events::RendererBuilt& e)
{
    const auto& [renderer] = e.DataTuple;
    _Renderer = renderer;
}

void ImguiWindowViewport::_OnRendererUnbuilt(Events::RendererUnbuilt& e)
{
    _Renderer.reset();
}

void ImguiWindowViewport::_OnPhysicsSimulatorBuilt(Events::PhysicsSimulatorBuilt& e)
{
    const auto& [physics_simulator] = e.DataTuple;
    _PhysicsSimulator = physics_simulator;
}

void ImguiWindowViewport::_OnPhysicsSimulatorUnbuilt(Events::PhysicsSimulatorUnbuilt& e)
{
    _PhysicsSimulator.reset();
}

void ImguiWindowViewport::_OnShowGizmo(Events::ShowGizmo& e)
{
    const auto& [show_gizmo] = e.DataTuple;
    _ShowGizmo = show_gizmo;
}