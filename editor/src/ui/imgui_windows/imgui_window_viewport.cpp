#include "imgui_window_viewport.hpp"

#include"core/app/app_context.hpp"
#include"function/window/window_events.hpp"
#include"resource/components/camera_component.hpp"
#include"resource/components/model_component.hpp"
#include"resource/components/physics_component.hpp"
#include"resource/components/transform_component.hpp"

void Arcadia::ImguiWindowViewport::OnEvent(Arcadia::EventBase& event)
{
    Arcadia::EventDispatcher{ event }
        .Dispatch<Arcadia::Event::InputCursorMove>(ARCADIA_BIND_MEMBER_FN(_OnInputCursorMove))
        .Dispatch<Arcadia::Event::OpenImguiWindow>(ARCADIA_BIND_MEMBER_FN(_OnOpenImguiWindow))
        .Dispatch<Arcadia::Event::ProjectBuilt>(ARCADIA_BIND_MEMBER_FN(_OnProjectBuilt))
        .Dispatch<Arcadia::Event::ProjectUnbuilt>(ARCADIA_BIND_MEMBER_FN(_OnProjectUnbuilt))
        .Dispatch<Arcadia::Event::SceneActivated>(ARCADIA_BIND_MEMBER_FN(_OnSceneActivated))
        .Dispatch<Arcadia::Event::SceneDeactivated>(ARCADIA_BIND_MEMBER_FN(_OnSceneDeactivated))
        .Dispatch<Arcadia::Event::SelectEntity>(ARCADIA_BIND_MEMBER_FN(_OnSelectEntity))
        .Dispatch<Arcadia::Event::RenameEntity>(ARCADIA_BIND_MEMBER_FN(_OnRenameEntity))
        .Dispatch<Arcadia::Event::DeleteEntity>(ARCADIA_BIND_MEMBER_FN(_OnDeleteEntity))
        .Dispatch<Arcadia::Event::RendererBuilt>(ARCADIA_BIND_MEMBER_FN(_OnRendererBuilt))
        .Dispatch<Arcadia::Event::RendererUnbuilt>(ARCADIA_BIND_MEMBER_FN(_OnRendererUnbuilt))
        .Dispatch<Arcadia::Event::PhysicsSimulatorBuilt>(ARCADIA_BIND_MEMBER_FN(_OnPhysicsSimualtorBuilt))
        .Dispatch<Arcadia::Event::PhysicsSimulatorUnbuilt>(ARCADIA_BIND_MEMBER_FN(_OnPhysicsSimulatorUnbuilt))
        .Result();
}

void Arcadia::ImguiWindowViewport::OnUpdate()
{
    if(!_Open)
    {
        return;
    }

    auto scene = _Scene.lock();
    auto physics_simulator = _PhysicsSimulator.lock();
    auto renderer = _Renderer.lock();
    auto project = _Project.lock();

    const auto& app_context = Arcadia::AppContext::Instance();

    auto imgui_title = _Title + GetIdStr();

    ImGui::SetNextWindowSize(glm::vec2{ 1024,768 }, ImGuiCond_Once);
    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_title.c_str(), &_Open, window_flags))
    {
        if(!scene)
        {
            ImGui::Text("No scene to render here");
        }
        else if(!renderer)
        {
            ImGui::Text("No renderer to use here");
        }
        else
        {
            ARCADIA_ASSERT(physics_simulator);
            ARCADIA_ASSERT(renderer);

            physics_simulator->Prepare();
            renderer->Prepare();

            auto [viewport_camera_comp, viewport_transform_comp] = scene->Get<Arcadia::CameraComponent, Arcadia::TransformComponent>(ViewportCameraEntityName);
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

            const auto image_cursor_pos = ImGui::GetCursorPos();
            ImGui::Image(renderer->GetRenderResultId(0), viewport_camera_comp.ViewportSize, { 0,1 }, { 1,0 });

            if(!_InViewportFreeCam && ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Right))
            {
                Arcadia::EventQueue::Instance().Signal<Arcadia::Event::WindowSetInputModeCursor>(Arcadia::WindowInputModeCursor::Disabled);
                _InViewportFreeCam = true;
            }
            if(_InViewportFreeCam && !ImGui::IsMouseDown(ImGuiMouseButton_Right))
            {
                Arcadia::EventQueue::Instance().Signal<Arcadia::Event::WindowSetInputModeCursor>(Arcadia::WindowInputModeCursor::Normal);
                _InViewportFreeCam = false;
            }

            // Gizmo
            if(!_SelectedEntityName.empty() && scene->AllOf<Arcadia::TransformComponent>(_SelectedEntityName))
            {
                ImGuizmo::SetDrawlist();

                ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, viewport_camera_comp.ViewportSize.x, viewport_camera_comp.ViewportSize.y);
                auto view_mat = glm::lookAt(viewport_transform_comp.Position, viewport_transform_comp.Position + viewport_transform_comp.Direction, viewport_camera_comp.Up);
                auto proj_mat = glm::perspective(viewport_camera_comp.Fov, viewport_camera_comp.ViewportSize.x * 1.f / viewport_camera_comp.ViewportSize.y, viewport_camera_comp.NearPlane, viewport_camera_comp.FarPlane);

                auto& transform_comp = scene->Get<Arcadia::TransformComponent>(_SelectedEntityName);
                auto transform_mat =
                    // Translate
                    glm::translate(
                        // Move pivot back from origin
                        glm::translate(
                            // Rotate
                            glm::mat4_cast(transform_comp.Rotation)
                            // Scale about origin (same as pivot)
                            * glm::scale(
                                // Move pivot to origin
                                glm::translate(
                                    Arcadia::Mat4::Identity(),
                                    -transform_comp.Pivot
                                ),
                                transform_comp.Scale
                            ),
                            transform_comp.Pivot
                        ),
                        transform_comp.Position
                    );
                ImGuizmo::Manipulate(
                    glm::value_ptr(view_mat),
                    glm::value_ptr(proj_mat),
                    static_cast<ImGuizmo::OPERATION>(_GizmoType),
                    static_cast<ImGuizmo::MODE>(_GizmoMode),
                    glm::value_ptr(transform_mat)
                );

                if(ImGuizmo::IsUsing())
                {
                    _GizmoEdited = true;

                    glm::vec3
                        scale{},
                        translation{},
                        skew{};
                    glm::vec4 perspective{};
                    glm::quat rotation{};

                    glm::decompose(transform_mat, scale, rotation, translation, skew, perspective);

                    if(
                        transform_comp.Position != translation
                        || transform_comp.Rotation != rotation
                        || transform_comp.Scale != scale
                        )
                    {
                        _GizmoEdited = true;
                    }

                    transform_comp.Position = translation;
                    transform_comp.Rotation = rotation;
                    transform_comp.Scale = scale;
                }

                if(!ImGuizmo::IsUsingAny() && _GizmoEdited)
                {
                    _GizmoEdited = false;

                    std::string description{};
                    switch(_GizmoType)
                    {
                        case Arcadia::ImguiWindowViewport::GizmoType::Translation:
                        {
                            description = "Translation";
                            break;
                        }
                        case Arcadia::ImguiWindowViewport::GizmoType::Rotation:
                        {
                            description = "Rotation";
                            break;
                        }
                        case Arcadia::ImguiWindowViewport::GizmoType::Scale:
                        {
                            description = "Scale";
                            break;
                        }
                        default:
                            break;
                    }
                    if(!description.empty())
                    {
                        Arcadia::MementoList::Instance()
                            .Snapshot<Arcadia::TransformComponent>(
                                std::format("{} - {}", "Transform"s, description),
                                [_scene = scene, _entity_name = _SelectedEntityName]() -> Arcadia::TransformComponent&
                        {
                            return _scene->Get<Arcadia::TransformComponent>(_entity_name);
                        }
                        );
                    }
                }

                if(!_InViewportFreeCam)
                {
                    if(ImGui::IsKeyDown(ImGuiKey_Q))
                    {
                        _GizmoType = GizmoType::None;
                    }
                    else if(ImGui::IsKeyDown(ImGuiKey_W))
                    {
                        _GizmoType = GizmoType::Translation;
                    }
                    else if(ImGui::IsKeyDown(ImGuiKey_E))
                    {
                        _GizmoType = GizmoType::Rotation;
                    }
                    else if(ImGui::IsKeyDown(ImGuiKey_R))
                    {
                        _GizmoType = GizmoType::Scale;
                    }
                }
            }

            // Viewport camera control
            if(_InViewportFreeCam)
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
                    viewport_transform_comp.Position += glm::cross(Arcadia::CameraComponent::Up, viewport_transform_comp.Direction) * viewport_camera_comp.Speed;
                }
                else if(ImGui::IsKeyDown(ImGuiKey_D))
                {
                    viewport_transform_comp.Position -= glm::cross(Arcadia::CameraComponent::Up, viewport_transform_comp.Direction) * viewport_camera_comp.Speed;
                }
                else if(ImGui::IsKeyDown(ImGuiKey_E))
                {
                    viewport_transform_comp.Position += glm::cross(viewport_transform_comp.Direction, glm::cross(Arcadia::CameraComponent::Up, viewport_transform_comp.Direction)) * viewport_camera_comp.Speed;
                }
                else if(ImGui::IsKeyDown(ImGuiKey_Q))
                {
                    viewport_transform_comp.Position -= glm::cross(viewport_transform_comp.Direction, glm::cross(Arcadia::CameraComponent::Up, viewport_transform_comp.Direction)) * viewport_camera_comp.Speed;
                }

                // Rotate view
                auto offset = _CursorMove * .005f;
                auto x_angle_offset = -offset.x;
                viewport_transform_comp.Direction = glm::angleAxis(x_angle_offset, viewport_camera_comp.Up) * viewport_transform_comp.Direction;
                auto pitch_angle = glm::half_pi<float>() - glm::angle(viewport_transform_comp.Direction, viewport_camera_comp.Up);
                auto y_angle_offset = glm::clamp(-offset.y + pitch_angle, -glm::half_pi<float>() + viewport_camera_comp.UpEpsilon, glm::half_pi<float>() - viewport_camera_comp.UpEpsilon) - pitch_angle;
                viewport_transform_comp.Direction = glm::angleAxis(y_angle_offset, glm::cross(viewport_transform_comp.Direction, viewport_camera_comp.Up)) * viewport_transform_comp.Direction;

                _CursorMove = Arcadia::Vec2::Zero();
            }

            // Display viewport viewport_camera_comp info
            ImGui::SetCursorPos(image_cursor_pos);
            ImGui::Text(std::format("Camera - Pos: {} - Direction: {}", viewport_transform_comp.Position, viewport_transform_comp.Direction).c_str());
            float fps = 1.f / std::chrono::duration_cast<std::chrono::duration<float>>(app_context.DeltaTime).count();
            ImGui::Text(std::format("FPS: {:.2f}", fps).c_str());
        }
    }
    ImGui::End();
}

void Arcadia::ImguiWindowViewport::_OnInputCursorMove(Arcadia::Event::InputCursorMove& e)
{
    const auto& [wnd_ptr, cursor_move] = e.data_tuple;
    _CursorMove = cursor_move;
}

void Arcadia::ImguiWindowViewport::_OnOpenImguiWindow(Arcadia::Event::OpenImguiWindow& e)
{
    const auto& [id_str] = e.data_tuple;
    if(id_str == GetIdStr())
    {
        _Open = true;
    }
}

void Arcadia::ImguiWindowViewport::_OnProjectBuilt(Arcadia::Event::ProjectBuilt& e)
{
    const auto& [project] = e.data_tuple;
    _Project = project;
}

void Arcadia::ImguiWindowViewport::_OnProjectUnbuilt(Arcadia::Event::ProjectUnbuilt& e)
{
    _Project.reset();
}

void Arcadia::ImguiWindowViewport::_OnSceneActivated(Arcadia::Event::SceneActivated& e)
{
    const auto& [scene] = e.data_tuple;
    if(!scene->Contains(ViewportCameraEntityName))
    {
        auto& entity_info = scene->Create(ViewportCameraEntityName, "camera");
        entity_info.Internal = true;
        scene->Emplace<Arcadia::CameraComponent>(ViewportCameraEntityName);
        auto& transform_comp = scene->Emplace<Arcadia::TransformComponent>(ViewportCameraEntityName);
        transform_comp.Position = glm::vec3{ 1.f,1.f,1.f };
        transform_comp.Direction = -transform_comp.Position;
    }
    _Scene = scene;
}

void Arcadia::ImguiWindowViewport::_OnSceneDeactivated(Arcadia::Event::SceneDeactivated& e)
{
    _Scene.reset();
}

void Arcadia::ImguiWindowViewport::_OnSelectEntity(Arcadia::Event::SelectEntity& e)
{
    const auto& [entity_name] = e.data_tuple;
    _SelectedEntityName = entity_name;
}

void Arcadia::ImguiWindowViewport::_OnRenameEntity(Arcadia::Event::RenameEntity& e)
{
    const auto& [old_name, new_name] = e.data_tuple;
    if(old_name == _SelectedEntityName)
    {
        _SelectedEntityName = new_name;
    }
}

void Arcadia::ImguiWindowViewport::_OnDeleteEntity(Arcadia::Event::DeleteEntity& e)
{
    const auto& [entity] = e.data_tuple;
    if(_SelectedEntityName == entity)
    {
        _SelectedEntityName.clear();
    }
}

void Arcadia::ImguiWindowViewport::_OnRendererBuilt(Arcadia::Event::RendererBuilt& e)
{
    const auto& [renderer] = e.data_tuple;
    _Renderer = renderer;
}

void Arcadia::ImguiWindowViewport::_OnRendererUnbuilt(Arcadia::Event::RendererUnbuilt& e)
{
    _Renderer.reset();
}

void Arcadia::ImguiWindowViewport::_OnPhysicsSimualtorBuilt(Arcadia::Event::PhysicsSimulatorBuilt& e)
{
    const auto& [physics_simulator] = e.data_tuple;
    _PhysicsSimulator = physics_simulator;
}

void Arcadia::ImguiWindowViewport::_OnPhysicsSimulatorUnbuilt(Arcadia::Event::PhysicsSimulatorUnbuilt& e)
{
    _PhysicsSimulator.reset();
}
