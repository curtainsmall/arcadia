#include "imgui_window_viewport.hpp"

#include"core/app/app_context.hpp"
#include"function/ui/imgui_header.hpp"
#include"function/window/window_events.hpp"
#include"resource/component/camera_component/camera_component.hpp"
#include"resource/component/model_component/model_component.hpp"

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
    if(!_open)
    {
        return;
    }

    const auto& app_context = Arcadia::AppContext::Instance();

    auto imgui_title = _Title + GetIdStr();

    ImGui::SetNextWindowSize(glm::vec2{ 1024,768 }, ImGuiCond_Once);
    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_title.c_str(), &_Open, window_flags))
    {
        if(!_Scene)
        {
            ImGui::Text("No scene to render here");
        }
        else if(!_Renderer)
        {
            ImGui::Text("No renderer to use here");
        }
        else
        {
            //==== Physics Simulator ====// 
            if(_PhysicsSimulator)
            {
                _PhysicsSimulator->Prepare();

                auto physics_comp_view = _Scene->View<Arcadia::PhysicsComponent>();
                for(auto [entity, physics_comp] : physics_comp_view.each())
                {
                    _PhysicsSimulator->Submit(physics_comp);
                }

                _PhysicsSimulator->Finalize();

                _PhysicsSimulator->Update();
                for(auto [entity, physics_comp] : physics_comp_view.each())
                {
                    _PhysicsSimulator->Quary(physics_comp);
                }

            }

            //==== Renderer ====//
            auto& viewport_camera = _Project->ViewportCamera;

            viewport_camera.ViewportSize = ImGui::GetContentRegionAvail();
            //viewport_camera.should_display_grid = true;
            _Renderer->Prepare();

            // Cameras
            _Renderer->Submit(viewport_camera);

            // Lights
            for(auto [entity, light_comp] : _Scene->View<Arcadia::LightComponent>().each())
            {
                if(_Scene->GetEntityInfo(entity).ShouldRenderInViewport)
                {
                    _Renderer->Submit(light_comp);
                }
            }

            // Models
            for(auto [entity, model_comp] : _Scene->View<Arcadia::ModelComponent>().each())
            {
                if(_Scene->GetEntityInfo(entity).ShouldRenderInViewport)
                {
                    _Renderer->Submit(model_comp);
                }
            }

            // Physcis simulator 
            if(_PhysicsSimulator)
            {
                for(auto [entity, physics_comp] : _Scene->View<Arcadia::PhysicsComponent>().each())
                {
                    if(_Scene->GetEntityInfo(entity).ShouldRenderInViewport)
                    {
                        _Renderer->Submit(physics_comp);
                    }
                }
            }

            _Renderer->Finalize();
            _Renderer->Draw();

            auto image_cursor_pos = ImGui::GetCursorPos();
            ImGui::Image(_Renderer->GetRenderResultId(0), viewport_camera.ViewportSize, { 0,1 }, { 1,0 });

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
                auto& io = ImGui::GetIO();

                // Scroll to zoom (move viewport_camera forwards or backwards along direction)
                if(ImGui::IsKeyDown(ImGuiKey_W))
                {
                    viewport_camera.MoveForward();
                }
                else if(ImGui::IsKeyDown(ImGuiKey_S))
                {
                    viewport_camera.MoveBackward();
                }
                else if(ImGui::IsKeyDown(ImGuiKey_A))
                {
                    viewport_camera.MoveLeft();
                }
                else if(ImGui::IsKeyDown(ImGuiKey_D))
                {
                    viewport_camera.MoveRight();
                }
                else if(ImGui::IsKeyDown(ImGuiKey_E))
                {
                    viewport_camera.MoveUp();
                }
                else if(ImGui::IsKeyDown(ImGuiKey_Q))
                {
                    viewport_camera.MoveDown();
                }

                //Arcadia::Log::Debug(std::format("Cursor Move: {}", _CursorMove));
                viewport_camera.RotateView(_CursorMove * .005f);
                _CursorMove = Arcadia::Vec2::Zero();
            }

            // Display viewport viewport_camera info
            ImGui::SetCursorPos(image_cursor_pos);
            ImGui::Text(std::format("Camera - Pos: {} - Direction: {}", viewport_camera.Position, viewport_camera.GetForwardDir()).c_str());
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
