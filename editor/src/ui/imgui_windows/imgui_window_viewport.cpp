#include "imgui_window_viewport.hpp"

#include"core/app/app_context.hpp"
#include"function/ui/imgui_header.hpp"
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

            auto [camera_comp, transform_comp] = scene->Get<Arcadia::CameraComponent, Arcadia::TransformComponent>(ViewportCameraEntityName);
            camera_comp.ViewportSize = ImGui::GetContentRegionAvail();
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
            ImGui::Image(renderer->GetRenderResultId(0), camera_comp.ViewportSize, { 0,1 }, { 1,0 });

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

            if(_InViewportFreeCam)
            {
                // Scroll to zoom (move camera_comp forwards or backwards along direction)
                if(ImGui::IsKeyDown(ImGuiKey_W))
                {
                    transform_comp.Position += transform_comp.Direction * camera_comp.Speed;
                }
                else if(ImGui::IsKeyDown(ImGuiKey_S))
                {
                    transform_comp.Position -= transform_comp.Direction * camera_comp.Speed;
                }
                else if(ImGui::IsKeyDown(ImGuiKey_A))
                {
                    transform_comp.Position += glm::cross(Arcadia::CameraComponent::Up, transform_comp.Direction) * camera_comp.Speed;
                }
                else if(ImGui::IsKeyDown(ImGuiKey_D))
                {
                    transform_comp.Position -= glm::cross(Arcadia::CameraComponent::Up, transform_comp.Direction) * camera_comp.Speed;
                }
                else if(ImGui::IsKeyDown(ImGuiKey_E))
                {
                    transform_comp.Position += glm::cross(transform_comp.Direction, glm::cross(Arcadia::CameraComponent::Up, transform_comp.Direction)) * camera_comp.Speed;
                }
                else if(ImGui::IsKeyDown(ImGuiKey_Q))
                {
                    transform_comp.Position -= glm::cross(transform_comp.Direction, glm::cross(Arcadia::CameraComponent::Up, transform_comp.Direction)) * camera_comp.Speed;
                }

                // Rotate view
                auto offset = _CursorMove * .005f;
                auto x_angle_offset = -offset.x;
                transform_comp.Direction = glm::angleAxis(x_angle_offset, camera_comp.Up) * transform_comp.Direction;
                auto pitch_angle = glm::half_pi<float>() - glm::angle(transform_comp.Direction, camera_comp.Up);
                auto y_angle_offset = glm::clamp(-offset.y + pitch_angle, -glm::half_pi<float>() + camera_comp.UpEpsilon, glm::half_pi<float>() - camera_comp.UpEpsilon) - pitch_angle;
                transform_comp.Direction = glm::angleAxis(y_angle_offset, glm::cross(transform_comp.Direction, camera_comp.Up)) * transform_comp.Direction;

                _CursorMove = Arcadia::Vec2::Zero();
            }

            // Display viewport camera_comp info
            ImGui::SetCursorPos(image_cursor_pos);
            ImGui::Text(std::format("Camera - Pos: {} - Direction: {}", transform_comp.Position, transform_comp.Direction).c_str());
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
