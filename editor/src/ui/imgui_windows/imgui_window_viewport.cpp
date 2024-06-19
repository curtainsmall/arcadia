#include "imgui_window_viewport.hpp"

#include"core/app/app_context.hpp"
#include"function/window/window_events.hpp"
#include"resource/components/camera_component.hpp"
#include"resource/components/model_component.hpp"
#include"resource/components/physics_component.hpp"
#include"resource/components/transform_component.hpp"

void ImguiWindowViewport::on_event(EventBase& e)
{
    EventDispatcher{ e }
        .dispatch<event::InputCursorMove>(ACDA_BIND_MEMBER_FN(_on_input_cursor_move))
        .dispatch<event::OpenImguiWindow>(ACDA_BIND_MEMBER_FN(_on_open_imgui_window))
        .dispatch<event::ProjectBuilt>(ACDA_BIND_MEMBER_FN(_on_project_built))
        .dispatch<event::ProjectUnbuilt>(ACDA_BIND_MEMBER_FN(_on_project_unbuilt))
        .dispatch<event::SceneActivated>(ACDA_BIND_MEMBER_FN(_on_scene_activated))
        .dispatch<event::SceneDeactivated>(ACDA_BIND_MEMBER_FN(_on_scene_deactivated))
        .dispatch<event::SelectEntity>(ACDA_BIND_MEMBER_FN(_on_select_entity))
        .dispatch<event::RenameEntity>(ACDA_BIND_MEMBER_FN(_on_rename_entity))
        .dispatch<event::DeleteEntity>(ACDA_BIND_MEMBER_FN(_on_delete_entity))
        .dispatch<event::RendererBuilt>(ACDA_BIND_MEMBER_FN(_on_renderer_built))
        .dispatch<event::RendererUnbuilt>(ACDA_BIND_MEMBER_FN(_on_renderer_unbuilt))
        .dispatch<event::PhysicsSimulatorBuilt>(ACDA_BIND_MEMBER_FN(_on_physics_simulator_built))
        .dispatch<event::PhysicsSimulatorUnbuilt>(ACDA_BIND_MEMBER_FN(_on_physics_simulator_unbuilt))
        .dispatch<event::ShowGizmo>(ACDA_BIND_MEMBER_FN(_on_show_gizmo))
        .result();
}

void ImguiWindowViewport::on_update()
{
    if(!_open)
    {
        return;
    }

    auto scene = _scene.lock();
    auto physics_simulator = _physics_simulator.lock();
    auto renderer = _renderer.lock();
    auto project = _project.lock();

    const auto& app_context = AppContext::instance();

    auto imgui_title = _title + get_id_str();

    ImGui::SetNextWindowSize(glm::vec2{ 1024,768 }, ImGuiCond_Once);
    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_title.c_str(), &_open, window_flags))
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
            ACDA_ASSERT(physics_simulator);
            ACDA_ASSERT(renderer);

            physics_simulator->prepare();
            renderer->prepare();

            auto [viewport_camera_comp, viewport_transform_comp] = scene->get<CameraComponent, TransformComponent>(viewport_camera_entity_name);
            viewport_camera_comp.viewport_size = ImGui::GetContentRegionAvail();
            for(const auto& [name, entity_info] : *scene)
            {
                physics_simulator->submit(*scene, name);
            }
            physics_simulator->finalize();
            physics_simulator->update();
            for(const auto& [name, entity_info] : *scene)
            {
                physics_simulator->query(*scene, name);

                // We submit entity to renderer after query
                if(entity_info.display)
                {
                    renderer->submit(*scene, name);
                }
            }

            renderer->finalize();
            renderer->draw();

            glm::vec2 image_cursor_pos = ImGui::GetCursorPos();
            ImGui::Image(renderer->render_result_id(0), viewport_camera_comp.viewport_size, { 0,1 }, { 1,0 });

            if(!_in_viewport_free_cam && ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Right))
            {
                EventQueue::instance().signal<event::WindowSetInputModeCursor>(WindowInputModeCursor::Disabled);
                _in_viewport_free_cam = true;
            }
            if(_in_viewport_free_cam && !ImGui::IsMouseDown(ImGuiMouseButton_Right))
            {
                EventQueue::instance().signal<event::WindowSetInputModeCursor>(WindowInputModeCursor::normal);
                _in_viewport_free_cam = false;
            }

            // Viewport camera control
            if(_in_viewport_free_cam)
            {
                // Scroll to zoom (move viewport_camera_comp forwards or backwards along direction)
                if(ImGui::IsKeyDown(ImGuiKey_W))
                {
                    viewport_transform_comp.position += viewport_transform_comp.direction * viewport_camera_comp.speed;
                }
                else if(ImGui::IsKeyDown(ImGuiKey_S))
                {
                    viewport_transform_comp.position -= viewport_transform_comp.direction * viewport_camera_comp.speed;
                }
                else if(ImGui::IsKeyDown(ImGuiKey_A))
                {
                    viewport_transform_comp.position += glm::cross(CameraComponent::up, viewport_transform_comp.direction) * viewport_camera_comp.speed;
                }
                else if(ImGui::IsKeyDown(ImGuiKey_D))
                {
                    viewport_transform_comp.position -= glm::cross(CameraComponent::up, viewport_transform_comp.direction) * viewport_camera_comp.speed;
                }
                else if(ImGui::IsKeyDown(ImGuiKey_E))
                {
                    viewport_transform_comp.position += glm::cross(viewport_transform_comp.direction, glm::cross(CameraComponent::up, viewport_transform_comp.direction)) * viewport_camera_comp.speed;
                }
                else if(ImGui::IsKeyDown(ImGuiKey_Q))
                {
                    viewport_transform_comp.position -= glm::cross(viewport_transform_comp.direction, glm::cross(CameraComponent::up, viewport_transform_comp.direction)) * viewport_camera_comp.speed;
                }

                // Rotate view
                auto offset = _cursor_move * .005f;
                auto x_angle_offset = -offset.x;
                viewport_transform_comp.direction = glm::normalize(glm::angleAxis(x_angle_offset, viewport_camera_comp.up) * viewport_transform_comp.direction);
                auto pitch_angle = glm::half_pi<float>() - glm::angle(viewport_transform_comp.direction, viewport_camera_comp.up);
                auto y_angle_offset = glm::clamp(-offset.y + pitch_angle, -glm::half_pi<float>() + viewport_camera_comp.up_epsilon, glm::half_pi<float>() - viewport_camera_comp.up_epsilon) - pitch_angle;
                viewport_transform_comp.direction = glm::normalize(glm::angleAxis(y_angle_offset, glm::cross(viewport_transform_comp.direction, viewport_camera_comp.up)) * viewport_transform_comp.direction);

                _cursor_move = vec2::zero();
            }

            // Display viewport viewport_camera_comp info
            ImGui::SetCursorPos(image_cursor_pos);
            ImGui::Text(std::format("Camera - Pos: {} - Direction: {}", viewport_transform_comp.position, viewport_transform_comp.direction).c_str());
            ImGui::SameLine(ImGui::GetWindowWidth() - 300.f);
            const auto gizmo_options_cursor_pos = ImGui::GetCursorPos();
            ImGui::Dummy({ 0,0 });
            float fps = 1.f / std::chrono::duration_cast<std::chrono::duration<float>>(app_context.delta_time).count();
            ImGui::Text(std::format("FPS: {:.2f}", fps).c_str());

            ImGui::SetCursorPos(gizmo_options_cursor_pos);

            if(_show_gizmo)
            {

                // Gizmo option
                auto selected_color = IM_COL32(50, 50, 120, 255);
                auto hovered_color = selected_color;
                auto active_color = selected_color;

                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hovered_color);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, active_color);

                if(_gizmo_option == GizmoOption::None)
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, selected_color);
                }
                ImGui::Button("None");
                if(_gizmo_option == GizmoOption::None)
                {
                    ImGui::PopStyleColor();
                }
                if(!_in_viewport_free_cam && (ImGui::IsItemClicked() || ImGui::IsKeyDown(ImGuiKey_Q)))
                {
                    _gizmo_option = GizmoOption::None;
                }
                ImGui::SetItemTooltip("Turn off gizmo (Q)");

                ImGui::SameLine();
                if(_gizmo_option == GizmoOption::Translation)
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, selected_color);
                }
                ImGui::Button("Translation");
                if(_gizmo_option == GizmoOption::Translation)
                {
                    ImGui::PopStyleColor();
                }
                if(!_in_viewport_free_cam && (ImGui::IsItemClicked() || ImGui::IsKeyDown(ImGuiKey_W)))
                {
                    _gizmo_option = GizmoOption::Translation;
                }
                ImGui::SetItemTooltip("Set gizmo to translation mode (W)");

                ImGui::SameLine();
                if(_gizmo_option == GizmoOption::Rotation)
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, selected_color);
                }
                ImGui::Button("Rotation");
                if(_gizmo_option == GizmoOption::Rotation)
                {
                    ImGui::PopStyleColor();
                }
                if(!_in_viewport_free_cam && (ImGui::IsItemClicked() || ImGui::IsKeyDown(ImGuiKey_E)))
                {
                    _gizmo_option = GizmoOption::Rotation;
                }
                ImGui::SetItemTooltip("Set gizmo to rotation mode (E)");

                ImGui::SameLine();
                if(_gizmo_option == GizmoOption::Scale)
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, selected_color);
                }
                ImGui::Button("Scale");
                if(_gizmo_option == GizmoOption::Scale)
                {
                    ImGui::PopStyleColor();
                }
                if(!_in_viewport_free_cam && (ImGui::IsItemClicked() || ImGui::IsKeyDown(ImGuiKey_R)))
                {
                    _gizmo_option = GizmoOption::Scale;
                }
                ImGui::SetItemTooltip("Set gizmo to scale mode (R)");

                ImGui::PopStyleColor(2);

                // Gizmo
                if(!_selected_entity_name.empty() && scene->all_of<TransformComponent>(_selected_entity_name))
                {
                    ImGuizmo::SetDrawlist();

                    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, viewport_camera_comp.viewport_size.x, viewport_camera_comp.viewport_size.y);
                    glm::mat4 view_mat = viewport_camera_comp.generate_view_mat4(viewport_transform_comp.position, viewport_transform_comp.direction);
                    glm::mat4 proj_mat = viewport_camera_comp.generate_proj_mat4();

                    auto& transform_comp = scene->get<TransformComponent>(_selected_entity_name);
                    glm::mat4 transform_mat = transform_comp.generate_transform_matrix();
                    ImGuizmo::Manipulate(
                        glm::value_ptr(view_mat),
                        glm::value_ptr(proj_mat),
                        static_cast<ImGuizmo::OPERATION>(_gizmo_option),
                        static_cast<ImGuizmo::MODE>(_gizmo_mode),
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

                        if(transform_comp.position != translation
                           || transform_comp.rotation != rotation
                           || transform_comp.scale != scale)
                        {
                            _gizmo_edited = true;
                        }

                        if(transform_comp.position != translation)
                        {
                            //transform_comp.pivot += translation - transform_comp.position;
                        }

                        transform_comp.position = translation;
                        transform_comp.rotation = rotation;
                        transform_comp.scale = scale;
                    }

                    // On release gizmo
                    if(!ImGuizmo::IsUsingAny() && _gizmo_edited)
                    {
                        _gizmo_edited = false;

                        std::string description{};
                        switch(_gizmo_option)
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
                            MementoList::instance()
                                .snapshot<TransformComponent>(
                                    std::format("{} - {}", "Transform"s, description),
                                    [_scene = scene, _entity_name = _selected_entity_name]() -> TransformComponent&
                            {
                                return _scene->get<TransformComponent>(_entity_name);
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

void ImguiWindowViewport::_on_input_cursor_move(event::InputCursorMove& e)
{
    const auto& [wnd_ptr, cursor_move] = e.data_tuple;
    _cursor_move = cursor_move;
}

void ImguiWindowViewport::_on_open_imgui_window(event::OpenImguiWindow& e)
{
    const auto& [id_str] = e.data_tuple;
    if(id_str == get_id_str())
    {
        _open = true;
    }
}

void ImguiWindowViewport::_on_project_built(event::ProjectBuilt& e)
{
    const auto& [project] = e.data_tuple;
    _project = project;
}

void ImguiWindowViewport::_on_project_unbuilt(event::ProjectUnbuilt& e)
{
    _project.reset();
}

void ImguiWindowViewport::_on_scene_activated(event::SceneActivated& e)
{
    const auto& [scene] = e.data_tuple;
    if(!scene->contains(viewport_camera_entity_name))
    {
        auto& entity_info = scene->create(viewport_camera_entity_name, "camera");
        entity_info.internal = true;
        scene->emplace<CameraComponent>(viewport_camera_entity_name);
        auto& transform_comp = scene->emplace<TransformComponent>(viewport_camera_entity_name);
        transform_comp.position = glm::vec3{ 1.f,1.f,1.f };
        transform_comp.direction = -transform_comp.position;
    }
    _scene = scene;
}

void ImguiWindowViewport::_on_scene_deactivated(event::SceneDeactivated& e)
{
    _scene.reset();
}

void ImguiWindowViewport::_on_select_entity(event::SelectEntity& e)
{
    const auto& [entity_name] = e.data_tuple;
    _selected_entity_name = entity_name;
}

void ImguiWindowViewport::_on_rename_entity(event::RenameEntity& e)
{
    const auto& [old_name, new_name] = e.data_tuple;
    if(old_name == _selected_entity_name)
    {
        _selected_entity_name = new_name;
    }
}

void ImguiWindowViewport::_on_delete_entity(event::DeleteEntity& e)
{
    const auto& [entity] = e.data_tuple;
    if(_selected_entity_name == entity)
    {
        _selected_entity_name.clear();
    }
}

void ImguiWindowViewport::_on_renderer_built(event::RendererBuilt& e)
{
    const auto& [renderer] = e.data_tuple;
    _renderer = renderer;
}

void ImguiWindowViewport::_on_renderer_unbuilt(event::RendererUnbuilt& e)
{
    _renderer.reset();
}

void ImguiWindowViewport::_on_physics_simulator_built(event::PhysicsSimulatorBuilt& e)
{
    const auto& [physics_simulator] = e.data_tuple;
    _physics_simulator = physics_simulator;
}

void ImguiWindowViewport::_on_physics_simulator_unbuilt(event::PhysicsSimulatorUnbuilt& e)
{
    _physics_simulator.reset();
}

void ImguiWindowViewport::_on_show_gizmo(event::ShowGizmo& e)
{
    const auto& [show_gizmo] = e.data_tuple;
    _show_gizmo = show_gizmo;
}
