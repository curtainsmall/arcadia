#include "pch.hpp"

#include "imgui_backend.hpp"

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "core/app/app_config.hpp"
#include "core/match.hpp"

ACDA_API void Arcadia::ImguiBackend::Initialize(const WindowLayer& window)
{
    const AppConfig& app_config = AppConfig::Instance();

    MatchVariant<void>(
        app_config.GraphicApi,
        [&](const GraphicApi::Opengl& opengl) -> void
    {
        std::string glsl_version{};
        if(opengl.Version >= Version{ 3,3,0 })
        {
            glsl_version = std::format("#version {0}{1}0", opengl.Version.Major, opengl.Version.Minor);
        }
        else
        {
            if(opengl.Version >= Version{ 3,0,0 })
            {
                glsl_version = std::format("#version 1{}0", opengl.Version.Minor + 3);
            }
            else
            {
                glsl_version = std::format("#version 1{}0", opengl.Version.Minor + 1);
            }
        }
        ImGui_ImplGlfw_InitForOpenGL(window.GetGlfwWindow(), false);
        ImGui_ImplOpenGL3_Init(glsl_version.c_str());
    },
        [](auto&&) -> void
    {
    }
    );
}

ACDA_API void Arcadia::ImguiBackend::NewFrame(const WindowLayer& window)
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

ACDA_API void Arcadia::ImguiBackend::RenderDrawData(const WindowLayer& window)
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

ACDA_API void Arcadia::ImguiBackend::Shutdown(const WindowLayer& window)
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

ACDA_API void Arcadia::ImguiBackend::OnEvent(EventBase& e)
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

ACDA_API void Arcadia::ImguiBackend::OnWindowSetFocused(Events::WindowSetFocused& e)
{
    ImGui_ImplGlfw_WindowFocusCallback(e.pWindowLayer->GetGlfwWindow(), e.Focused);
}

ACDA_API void Arcadia::ImguiBackend::OnInputCursorEnter(Events::InputCursorEnter& e)
{
    ImGui_ImplGlfw_CursorEnterCallback(e.pWindowLayer->GetGlfwWindow(), e.Entered);
}

ACDA_API void Arcadia::ImguiBackend::OnInputCursorPosition(Events::InputCursorPosition& e)
{
    ImGui_ImplGlfw_CursorPosCallback(e.pWindowLayer->GetGlfwWindow(), e.CursorPosition.x, e.CursorPosition.y);
}

ACDA_API void Arcadia::ImguiBackend::OnInputMouseButton(Events::InputMouseButton& e)
{
    ImGui_ImplGlfw_MouseButtonCallback(e.pWindowLayer->GetGlfwWindow(), e.Code, e.Action, e.Modifier);
}

ACDA_API void Arcadia::ImguiBackend::OnInputScroll(Events::InputScroll& e)
{
    ImGui_ImplGlfw_ScrollCallback(e.pWindowLayer->GetGlfwWindow(), e.ScrollOffsetHorizontal, e.ScrollOffsetVertical);
}

ACDA_API void Arcadia::ImguiBackend::OnInputKey(Events::InputKey& e)
{
    ImGui_ImplGlfw_KeyCallback(e.pWindowLayer->GetGlfwWindow(), e.KeyCode, e.KeyScancode, e.Action, e.Modifier);
}

ACDA_API void Arcadia::ImguiBackend::OnInputChar(Events::InputChar& e)
{
    ImGui_ImplGlfw_CharCallback(e.pWindowLayer->GetGlfwWindow(), e.UnicodeCodepoint);
}

ACDA_API void Arcadia::ImguiBackend::OnMonitorSetConnected(Events::MonitorSetConnected& e)
{
    ImGui_ImplGlfw_MonitorCallback(e.GlfwMonitor, e.Connected);
}