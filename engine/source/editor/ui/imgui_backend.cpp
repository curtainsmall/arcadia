#include "pch.hpp"

#include "imgui_backend.hpp"

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "core/app/app_config.hpp"
#include "core/match.hpp"

void Arcadia::ImguiBackend::Initialize(const std::shared_ptr<WindowLayer>& window_sptr)
{
    const AppConfig& app_config = AppConfig::Instance();

    MatchVariant<void>(
        app_config.GraphicApi,
        [&](const GraphicApi::Opengl& opengl) -> void
        {
            std::string glsl_version{};
            if(opengl.Version >= Version{ 3, 3, 0 })
            {
                glsl_version = std::format("#version {0}{1}0", opengl.Version.Major, opengl.Version.Minor);
            }
            else
            {
                if(opengl.Version >= Version{ 3, 0, 0 })
                {
                    glsl_version = std::format("#version 1{}0", opengl.Version.Minor + 3);
                }
                else
                {
                    glsl_version = std::format("#version 1{}0", opengl.Version.Minor + 1);
                }
            }
            ImGui_ImplGlfw_InitForOpenGL(window_sptr->GetGlfwWindow(), false);
            ImGui_ImplOpenGL3_Init(glsl_version.c_str());
        },
        [](auto&&) -> void
        {
        }
    );
}

void Arcadia::ImguiBackend::NewFrame(const std::shared_ptr<WindowLayer>& window_sptr)
{
    const AppConfig& app_config = AppConfig::Instance();

    MatchVariant<void>(
        app_config.GraphicApi,
        [](const GraphicApi::Opengl&) -> void
        {
            ImGui_ImplGlfw_NewFrame();
            ImGui_ImplOpenGL3_NewFrame();
        },
        [](auto&&) -> void
        {
        }
    );
}

void Arcadia::ImguiBackend::RenderDrawData(const std::shared_ptr<WindowLayer>& window_sptr)
{
    const AppConfig& app_config = AppConfig::Instance();

    MatchVariant<void>(
        app_config.GraphicApi,
        [](const GraphicApi::Opengl&) -> void
        {
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        },
        [](auto&&) -> void
        {
        }
    );
}

void Arcadia::ImguiBackend::Shutdown(const std::shared_ptr<WindowLayer>& window_sptr)
{
    const AppConfig& app_config = AppConfig::Instance();

    MatchVariant<void>(
        app_config.GraphicApi,
        [](const GraphicApi::Opengl&) -> void
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
        },
        [](auto&&) -> void
        {
        }
    );
}

void Arcadia::ImguiBackend::OnEvent(EventBase& e)
{
    if(EventDispatcher{ e }
       .Dispatch<Events::WindowSetFocused>(ImguiBackend::OnWindowSetFocused)
       .Dispatch<Events::InputCursorEnter>(ImguiBackend::OnInputCursorEnter)
       .Dispatch<Events::InputCursorPosition>(ImguiBackend::OnInputCursorPosition)
       .Dispatch<Events::InputMouseButton>(ImguiBackend::OnInputMouseButton)
       .Dispatch<Events::InputScroll>(ImguiBackend::OnInputScroll)
       .Dispatch<Events::InputKey>(ImguiBackend::OnInputKey)
       .Dispatch<Events::InputChar>(ImguiBackend::OnInputChar)
       //.dispatch<Events::MonitorConnect>(ImguiBackend::OnMonitorConnect) // We will manage monitors ourselves for now
       .IsDispatched())
    {
        ImGuiIO& io = ImGui::GetIO();
        if(io.WantCaptureMouse || io.WantCaptureKeyboard)
        {
            e.MarkHandled();
        }
    }
}

void Arcadia::ImguiBackend::OnWindowSetFocused(Events::WindowSetFocused& e)
{
    std::shared_ptr<WindowLayer> window_layer_sptr = LayerStack::Instance().GetLayerShared<WindowLayer>();
    ImGui_ImplGlfw_WindowFocusCallback(window_layer_sptr->GetGlfwWindow(), e.Focused);
}

void Arcadia::ImguiBackend::OnInputCursorEnter(Events::InputCursorEnter& e)
{
    std::shared_ptr<WindowLayer> window_layer_sptr = LayerStack::Instance().GetLayerShared<WindowLayer>();
    ImGui_ImplGlfw_CursorEnterCallback(window_layer_sptr->GetGlfwWindow(), e.Entered);
}

void Arcadia::ImguiBackend::OnInputCursorPosition(Events::InputCursorPosition& e)
{
    std::shared_ptr<WindowLayer> window_layer_sptr = LayerStack::Instance().GetLayerShared<WindowLayer>();
    ImGui_ImplGlfw_CursorPosCallback(window_layer_sptr->GetGlfwWindow(), e.CursorPosition.x, e.CursorPosition.y);
}

void Arcadia::ImguiBackend::OnInputMouseButton(Events::InputMouseButton& e)
{
    std::shared_ptr<WindowLayer> window_layer_sptr = LayerStack::Instance().GetLayerShared<WindowLayer>();
    ImGui_ImplGlfw_MouseButtonCallback(window_layer_sptr->GetGlfwWindow(), e.Code, e.Action, e.Modifier);
}

void Arcadia::ImguiBackend::OnInputScroll(Events::InputScroll& e)
{
    std::shared_ptr<WindowLayer> window_layer_sptr = LayerStack::Instance().GetLayerShared<WindowLayer>();
    ImGui_ImplGlfw_ScrollCallback(window_layer_sptr->GetGlfwWindow(), e.ScrollOffsetHorizontal, e.ScrollOffsetVertical);
}

void Arcadia::ImguiBackend::OnInputKey(Events::InputKey& e)
{
    std::shared_ptr<WindowLayer> window_layer_sptr = LayerStack::Instance().GetLayerShared<WindowLayer>();
    ImGui_ImplGlfw_KeyCallback(window_layer_sptr->GetGlfwWindow(), e.KeyCode, e.KeyScancode, e.Action, e.Modifier);
}

void Arcadia::ImguiBackend::OnInputChar(Events::InputChar& e)
{
    std::shared_ptr<WindowLayer> window_layer_sptr = LayerStack::Instance().GetLayerShared<WindowLayer>();
    ImGui_ImplGlfw_CharCallback(window_layer_sptr->GetGlfwWindow(), e.UnicodeCodepoint);
}

void Arcadia::ImguiBackend::OnMonitorSetConnected(Events::MonitorSetConnected& e)
{
    ImGui_ImplGlfw_MonitorCallback(e.GlfwMonitor, e.Connected);
}