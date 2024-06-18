#include"pch.hpp"

#include "imgui_backend.hpp"

#include"imgui/backends/imgui_impl_glfw.h"
#include"imgui/backends/imgui_impl_opengl3.h"

#include"core/app/app_config.hpp"

ACDA_API void ImguiBackend::Initialize(const WindowLayer& window)
{
    const auto& app_config = AppConfig::Instance();

    Match<void>(
        app_config.GraphicApi,
        [&](const GraphicApi::Opengl& Opengl) -> void
    {
        std::string glsl_version{};
        if(Opengl.version >= Version{ 3,3,0 })
        {
            glsl_version = std::format("#version {0}{1}0", Opengl.version.Major, Opengl.version.Minor);
        }
        else
        {
            if(Opengl.version >= Version{ 3,0,0 })
            {
                glsl_version = std::format("#version 1{}0", Opengl.version.Minor + 3);
            }
            else
            {
                glsl_version = std::format("#version 1{}0", Opengl.version.Minor + 1);
            }
        }
        ImGui_ImplGlfw_InitForOpenGL(window.GetGlfwWindowPtr(), false);
        ImGui_ImplOpenGL3_Init(glsl_version.c_str());
    },
        [](auto&&) -> void
    {
    }
    );
}

ACDA_API void ImguiBackend::NewFrame(const WindowLayer& window)
{
    const auto& app_config = AppConfig::Instance();

    Match<void>(
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

ACDA_API void ImguiBackend::RenderDrawData(const WindowLayer& window)
{
    const auto& app_config = AppConfig::Instance();

    Match<void>(
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

ACDA_API void ImguiBackend::Shutdown(const WindowLayer& window)
{
    const auto& app_config = AppConfig::Instance();

    Match<void>(
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

ACDA_API void ImguiBackend::ImguiOnEvent(EventBase& event)
{
    if(EventDispatcher{ event }
       .Dispatch<Event::WindowFocus>(ImguiBackend::ImguiOnWindowFocus)
       .Dispatch<Event::InputCursorEnter>(ImguiBackend::ImguiOnCursorEnter)
       .Dispatch<Event::InputCursorPos>(ImguiBackend::ImguiOnCursorPos)
       .Dispatch<Event::InputMouseButton>(ImguiBackend::ImguiOnMouseButton)
       .Dispatch<Event::InputScroll>(ImguiBackend::ImguiOnScroll)
       .Dispatch<Event::InputKey>(ImguiBackend::ImguiOnKey)
       .Dispatch<Event::InputChar>(ImguiBackend::ImguiOnChar)
       //.Dispatch<Event::MonitorConnection>(ImguiBackend::ImguiOnMonitor); // We will manage monitors ourselves for now
       .Result())
    {
        auto& io = ImGui::GetIO();
        if(io.WantCaptureMouse || io.WantCaptureKeyboard)
        {
            event.Handled = true;
        }
        else
        {
            event.Handled = false;
        }
    }
}

ACDA_API void ImguiBackend::ImguiOnWindowFocus(Event::WindowFocus& WindowFocus)
{
    const auto& [wnd_ptr, focused] = WindowFocus.data_tuple;
    ImGui_ImplGlfw_WindowFocusCallback(wnd_ptr->GetGlfwWindowPtr(), focused);
}

ACDA_API void ImguiBackend::ImguiOnCursorEnter(Event::InputCursorEnter& input_cursor_enter)
{
    const auto& [wnd_ptr, entered] = input_cursor_enter.data_tuple;
    ImGui_ImplGlfw_CursorEnterCallback(wnd_ptr->GetGlfwWindowPtr(), entered);
}

ACDA_API void ImguiBackend::ImguiOnCursorPos(Event::InputCursorPos& input_cursor_pos)
{
    const auto& [wnd_ptr, pos] = input_cursor_pos.data_tuple;
    ImGui_ImplGlfw_CursorPosCallback(wnd_ptr->GetGlfwWindowPtr(), pos.x, pos.y);
}

ACDA_API void ImguiBackend::ImguiOnMouseButton(Event::InputMouseButton& input_mouse_button)
{
    const auto& [wnd_ptr, button, action, mods] = input_mouse_button.data_tuple;
    ImGui_ImplGlfw_MouseButtonCallback(wnd_ptr->GetGlfwWindowPtr(), button, action, mods);
}

ACDA_API void ImguiBackend::ImguiOnScroll(Event::InputScroll& input_scroll)
{
    const auto& [wnd_ptr, Offset] = input_scroll.data_tuple;
    ImGui_ImplGlfw_ScrollCallback(wnd_ptr->GetGlfwWindowPtr(), Offset.x, Offset.y);
}

ACDA_API void ImguiBackend::ImguiOnKey(Event::InputKey& input_key)
{
    const auto& [wnd_ptr, key, scancode, action, mods] = input_key.data_tuple;
    ImGui_ImplGlfw_KeyCallback(wnd_ptr->GetGlfwWindowPtr(), key, scancode, action, mods);
}

ACDA_API void ImguiBackend::ImguiOnChar(Event::InputChar& input_char)
{
    const auto& [wnd_ptr, code] = input_char.data_tuple;
    ImGui_ImplGlfw_CharCallback(wnd_ptr->GetGlfwWindowPtr(), code);
}

ACDA_API void ImguiBackend::ImguiOnMonitor(Event::MonitorConnection& monitor_connection)
{
    const auto& [glfw_monitor_ptr, connection] = monitor_connection.data_tuple;
    ImGui_ImplGlfw_MonitorCallback(glfw_monitor_ptr, connection);
}

