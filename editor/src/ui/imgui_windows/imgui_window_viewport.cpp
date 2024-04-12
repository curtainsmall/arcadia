#include "imgui_window_viewport.hpp"

#include"core/app/app_context.hpp"
#include"function/ui/imgui_header.hpp"
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
        if(!_spScene)
        {
            ImGui::Text("No scene to render here");
        }
        else if(!_spRenderer)
        {
            ImGui::Text("No renderer to use here");
        }
        else
        {
            //==== Physics Simulator ====// 
            if(_spPhysicsSimulator)
            {
                _spPhysicsSimulator->Prepare();

                auto physics_comp_view = _spScene->View<Arcadia::PhysicsComponent>();
                for(auto [entity, physics_comp] : physics_comp_view.each())
                {
                    _spPhysicsSimulator->Submit(physics_comp);
                }

                _spPhysicsSimulator->Finalize();

                _spPhysicsSimulator->Update();
                for(auto [entity, physics_comp] : physics_comp_view.each())
                {
                    _spPhysicsSimulator->Quary(physics_comp);
                }

            }

            //==== Renderer ====//
            auto& viewport_camera = _spProject->ViewportCamera;

            viewport_camera.ViewportSize = ImGui::GetContentRegionAvail();
            //viewport_camera.should_display_grid = true;
            _spRenderer->Prepare();

            // Cameras
            _spRenderer->Submit(viewport_camera);

            // Lights
            for(auto [entity, light_comp] : _spScene->View<Arcadia::LightComponent>().each())
            {
                if(_spScene->GetEntityInfo(entity).ShouldRenderInViewport)
                {
                    _spRenderer->Submit(light_comp);
                }
            }

            // Models
            for(auto [entity, model_comp] : _spScene->View<Arcadia::ModelComponent>().each())
            {
                if(_spScene->GetEntityInfo(entity).ShouldRenderInViewport)
                {
                    _spRenderer->Submit(model_comp);
                }
            }

            // Physcis simulator 
            if(_spPhysicsSimulator)
            {
                for(auto [entity, physics_comp] : _spScene->View<Arcadia::PhysicsComponent>().each())
                {
                    if(_spScene->GetEntityInfo(entity).ShouldRenderInViewport)
                    {
                        _spRenderer->Submit(physics_comp);
                    }
                }
            }

            _spRenderer->Finalize();
            _spRenderer->Draw();

            auto image_cursor_pos = ImGui::GetCursorPos();
            ImGui::Image(_spRenderer->GetRenderResultId(0), viewport_camera.ViewportSize, { 0,1 }, { 1,0 });

            if(ImGui::IsItemHovered())
            {
                auto& io = ImGui::GetIO();

                // Scroll to zoom (move viewport_camera forwards or backwards along direction)
                auto mouse_wheel_offset = io.MouseWheel;
                viewport_camera.Move(viewport_camera.GetForwardDir() * mouse_wheel_offset);

                if(ImGui::IsMouseDown(ImGuiMouseButton_Middle))
                {
                    if(ImGui::IsKeyDown(ImGuiKey_LeftShift))
                    {
                        viewport_camera.DragViewMove(_CursorMove * .05f);
                    }
                    else
                    {
                        viewport_camera.DragViewRotate(_CursorMove * .005f);
                    }
                }

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
    const auto& [project_wptr] = e.data_tuple;
    _spProject = project_wptr;
}

void Arcadia::ImguiWindowViewport::_OnProjectUnbuilt(Arcadia::Event::ProjectUnbuilt& e)
{
    _spProject.reset();
}

void Arcadia::ImguiWindowViewport::_OnSceneActivated(Arcadia::Event::SceneActivated& e)
{
    const auto& [scene_wptr] = e.data_tuple;
    _spScene = scene_wptr;
}

void Arcadia::ImguiWindowViewport::_OnSceneDeactivated(Arcadia::Event::SceneDeactivated& e)
{
    _spScene.reset();
}

void Arcadia::ImguiWindowViewport::_OnRendererBuilt(Arcadia::Event::RendererBuilt& e)
{
    const auto& [renderer_wptr] = e.data_tuple;
    _spRenderer = renderer_wptr;
}

void Arcadia::ImguiWindowViewport::_OnRendererUnbuilt(Arcadia::Event::RendererUnbuilt& e)
{
    _spRenderer.reset();
}

void Arcadia::ImguiWindowViewport::_OnPhysicsSimualtorBuilt(Arcadia::Event::PhysicsSimulatorBuilt& e)
{
    const auto& [physics_simulator_wptr] = e.data_tuple;
    _spPhysicsSimulator = physics_simulator_wptr;
}

void Arcadia::ImguiWindowViewport::_OnPhysicsSimulatorUnbuilt(Arcadia::Event::PhysicsSimulatorUnbuilt& e)
{
    _spPhysicsSimulator.reset();
}
