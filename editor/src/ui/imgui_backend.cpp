#include"pch.hpp"

#include "imgui_backend.hpp"

#include"imgui/backends/imgui_impl_glfw.h"
#include"imgui/backends/imgui_impl_opengl3.h"

#include"core/app/app_config.hpp"
#include"core/match.hpp"

ACDA_API void Arcadia::ImguiBackend::Initialize(const WindowLayer& window)
{
    const auto& app_config = AppConfig::Instance();

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
    const auto& app_config = AppConfig::Instance();

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
    const auto& app_config = AppConfig::Instance();

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
    const auto& app_config = AppConfig::Instance();

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
        .Dispatch<Events::WindowFocused>(ImguiBackend::OnWindowFocus)
        .Dispatch<Events::InputCursorEnter>(ImguiBackend::OnInputCursorEnter)
        .Dispatch<Events::InputCursorPos>(ImguiBackend::OnInputCursorPos)
        .Dispatch<Events::InputMouseButton>(ImguiBackend::OnInputMouseButton)
        .Dispatch<Events::InputScroll>(ImguiBackend::OnInputScroll)
        .Dispatch<Events::InputKey>(ImguiBackend::OnInputKey)
        .Dispatch<Events::InputChar>(ImguiBackend::OnInputChar)
        //.dispatch<Events::MonitorConnect>(ImguiBackend::OnMonitorConnect) // We will manage monitors ourselves for now
        .IsDispatched())
    {
        auto& io = ImGui::GetIO();
        if(io.WantCaptureMouse || io.WantCaptureKeyboard)
        {
            e.Handled = true;
        }
        else
        {
            e.Handled = false;
        }
    }
}

ACDA_API void Arcadia::ImguiBackend::OnWindowFocus(Events::WindowFocused& window_focused)
{
    const auto& [wnd_ptr, focused] = window_focused.DataTuple;
    ImGui_ImplGlfw_WindowFocusCallback(wnd_ptr->GetGlfwWindow(), focused);
}

ACDA_API void Arcadia::ImguiBackend::OnInputCursorEnter(Events::InputCursorEnter& input_cursor_enter)
{
    const auto& [wnd_ptr, entered] = input_cursor_enter.DataTuple;
    ImGui_ImplGlfw_CursorEnterCallback(wnd_ptr->GetGlfwWindow(), entered);
}

ACDA_API void Arcadia::ImguiBackend::OnInputCursorPos(Events::InputCursorPos& input_cursor_pos)
{
    const auto& [wnd_ptr, pos] = input_cursor_pos.DataTuple;
    ImGui_ImplGlfw_CursorPosCallback(wnd_ptr->GetGlfwWindow(), pos.x, pos.y);
}

ACDA_API void Arcadia::ImguiBackend::OnInputMouseButton(Events::InputMouseButton& input_mouse_button)
{
    const auto& [wnd_ptr, button, action, mods] = input_mouse_button.DataTuple;
    ImGui_ImplGlfw_MouseButtonCallback(wnd_ptr->GetGlfwWindow(), button, action, mods);
}

ACDA_API void Arcadia::ImguiBackend::OnInputScroll(Events::InputScroll& input_scroll)
{
    const auto& [wnd_ptr, Offset] = input_scroll.DataTuple;
    ImGui_ImplGlfw_ScrollCallback(wnd_ptr->GetGlfwWindow(), Offset.x, Offset.y);
}

ACDA_API void Arcadia::ImguiBackend::OnInputKey(Events::InputKey& input_key)
{
    const auto& [wnd_ptr, key, scancode, action, mods] = input_key.DataTuple;
    ImGui_ImplGlfw_KeyCallback(wnd_ptr->GetGlfwWindow(), key, scancode, action, mods);
}

ACDA_API void Arcadia::ImguiBackend::OnInputChar(Events::InputChar& input_char)
{
    const auto& [wnd_ptr, code] = input_char.DataTuple;
    ImGui_ImplGlfw_CharCallback(wnd_ptr->GetGlfwWindow(), code);
}

ACDA_API void Arcadia::ImguiBackend::OnMonitorConnect(Events::MonitorConnect& monitor_connection)
{
    const auto& [glfw_monitor_ptr, connection] = monitor_connection.DataTuple;
    ImGui_ImplGlfw_MonitorCallback(glfw_monitor_ptr, connection);
}