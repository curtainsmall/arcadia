#include"pch.hpp"

#include "imgui_backend.hpp"

#include"imgui/backends/imgui_impl_glfw.h"
#include"imgui/backends/imgui_impl_opengl3.h"

#include"core/app/app_config.hpp"

ACDA_API void ImguiBackend::initialize(const WindowLayer& window)
{
    const auto& app_config = AppConfig::instance();

    match<void>(
        app_config.graphic_api,
        [&](const graphic_api::Opengl& Opengl) -> void
    {
        std::string glsl_version{};
        if(Opengl.version >= Version{ 3,3,0 })
        {
            glsl_version = std::format("#version {0}{1}0", Opengl.version.major, Opengl.version.minor);
        }
        else
        {
            if(Opengl.version >= Version{ 3,0,0 })
            {
                glsl_version = std::format("#version 1{}0", Opengl.version.minor + 3);
            }
            else
            {
                glsl_version = std::format("#version 1{}0", Opengl.version.minor + 1);
            }
        }
        ImGui_ImplGlfw_InitForOpenGL(window.glfw_window(), false);
        ImGui_ImplOpenGL3_Init(glsl_version.c_str());
    },
        [](auto&&) -> void
    {
    }
    );
}

ACDA_API void ImguiBackend::new_frame(const WindowLayer& window)
{
    const auto& app_config = AppConfig::instance();

    match<void>(
        app_config.graphic_api,
        [](const graphic_api::Opengl&) -> void
    {
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
    },
        [](auto&&) -> void
    {
    }
    );
}

ACDA_API void ImguiBackend::render_draw_data(const WindowLayer& window)
{
    const auto& app_config = AppConfig::instance();

    match<void>(
        app_config.graphic_api,
        [](const graphic_api::Opengl&) -> void
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    },
        [](auto&&) -> void
    {
    }
    );
}

ACDA_API void ImguiBackend::shutdown(const WindowLayer& window)
{
    const auto& app_config = AppConfig::instance();

    match<void>(
        app_config.graphic_api,
        [](const graphic_api::Opengl&) -> void
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
    },
        [](auto&&) -> void
    {
    }
    );
}

ACDA_API void ImguiBackend::imgui_on_event(EventBase& e)
{
    if(EventDispatcher{ e }
       .dispatch<event::WindowFocus>(ImguiBackend::imgui_on_window_focus)
       .dispatch<event::InputCursorEnter>(ImguiBackend::imgui_on_cursor_enter)
       .dispatch<event::InputCursorPos>(ImguiBackend::imgui_on_cursor_pos)
       .dispatch<event::InputMouseButton>(ImguiBackend::imgui_on_mouse_button)
       .dispatch<event::InputScroll>(ImguiBackend::imgui_on_scroll)
       .dispatch<event::InputKey>(ImguiBackend::imgui_on_key)
       .dispatch<event::InputChar>(ImguiBackend::imgui_on_char)
       //.dispatch<event::MonitorConnection>(ImguiBackend::ImguiOnMonitor); // We will manage monitors ourselves for now
       .result())
    {
        auto& io = ImGui::GetIO();
        if(io.WantCaptureMouse || io.WantCaptureKeyboard)
        {
            e.handled = true;
        }
        else
        {
            e.handled = false;
        }
    }
}

ACDA_API void ImguiBackend::imgui_on_window_focus(event::WindowFocus& WindowFocus)
{
    const auto& [wnd_ptr, focused] = WindowFocus.data_tuple;
    ImGui_ImplGlfw_WindowFocusCallback(wnd_ptr->glfw_window(), focused);
}

ACDA_API void ImguiBackend::imgui_on_cursor_enter(event::InputCursorEnter& input_cursor_enter)
{
    const auto& [wnd_ptr, entered] = input_cursor_enter.data_tuple;
    ImGui_ImplGlfw_CursorEnterCallback(wnd_ptr->glfw_window(), entered);
}

ACDA_API void ImguiBackend::imgui_on_cursor_pos(event::InputCursorPos& input_cursor_pos)
{
    const auto& [wnd_ptr, pos] = input_cursor_pos.data_tuple;
    ImGui_ImplGlfw_CursorPosCallback(wnd_ptr->glfw_window(), pos.x, pos.y);
}

ACDA_API void ImguiBackend::imgui_on_mouse_button(event::InputMouseButton& input_mouse_button)
{
    const auto& [wnd_ptr, button, action, mods] = input_mouse_button.data_tuple;
    ImGui_ImplGlfw_MouseButtonCallback(wnd_ptr->glfw_window(), button, action, mods);
}

ACDA_API void ImguiBackend::imgui_on_scroll(event::InputScroll& input_scroll)
{
    const auto& [wnd_ptr, Offset] = input_scroll.data_tuple;
    ImGui_ImplGlfw_ScrollCallback(wnd_ptr->glfw_window(), Offset.x, Offset.y);
}

ACDA_API void ImguiBackend::imgui_on_key(event::InputKey& input_key)
{
    const auto& [wnd_ptr, key, scancode, action, mods] = input_key.data_tuple;
    ImGui_ImplGlfw_KeyCallback(wnd_ptr->glfw_window(), key, scancode, action, mods);
}

ACDA_API void ImguiBackend::imgui_on_char(event::InputChar& input_char)
{
    const auto& [wnd_ptr, code] = input_char.data_tuple;
    ImGui_ImplGlfw_CharCallback(wnd_ptr->glfw_window(), code);
}

ACDA_API void ImguiBackend::imgui_on_monitor(event::MonitorConnection& monitor_connection)
{
    const auto& [glfw_monitor_ptr, connection] = monitor_connection.data_tuple;
    ImGui_ImplGlfw_MonitorCallback(glfw_monitor_ptr, connection);
}

