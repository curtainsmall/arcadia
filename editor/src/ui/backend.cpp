#include "backend.hpp"

#include"imgui/backends/imgui_impl_glfw.h"
#include"imgui/backends/imgui_impl_opengl3.h"

#include"core/app/app_config.hpp"
#include"core/util/conditional.hpp"

ARCADIA_API void arcadia::imgui_backend::initialize(const arcadia::window_layer& window)
{
    const auto& app_config = arcadia::app_config::instance();

    arcadia::match<void>(
        app_config.graphic_api,
        [&](const arcadia::graphic_api::opengl& opengl) -> void
    {
        std::string glsl_version{};
        if(opengl.version >= arcadia::version{ 3,3,0 })
        {
            glsl_version = std::format("#version {0}{1}0", opengl.version.major, opengl.version.minor);
        }
        else
        {
            if(opengl.version >= arcadia::version{ 3,0,0 })
            {
                glsl_version = std::format("#version 1{}0", opengl.version.minor + 3);
            }
            else
            {
                glsl_version = std::format("#version 1{}0", opengl.version.minor + 1);
            }
        }
        ImGui_ImplGlfw_InitForOpenGL(window.get_glfw_window_ptr(), false);
        ImGui_ImplOpenGL3_Init(glsl_version.c_str());
    },
        [](auto&&) -> void
    {
    }
    );
}

ARCADIA_API void arcadia::imgui_backend::new_frame(const arcadia::window_layer& window)
{
    const auto& app_config = arcadia::app_config::instance();

    arcadia::match<void>(
        app_config.graphic_api,
        [](const arcadia::graphic_api::opengl&) -> void
    {
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
    },
        [](auto&&) -> void
    {
    }
    );
}

ARCADIA_API void arcadia::imgui_backend::render_draw_data(const arcadia::window_layer& window)
{
    const auto& app_config = arcadia::app_config::instance();

    arcadia::match<void>(
        app_config.graphic_api,
        [](const arcadia::graphic_api::opengl&) -> void
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    },
        [](auto&&) -> void
    {
    }
    );
}

ARCADIA_API void arcadia::imgui_backend::shutdown(const arcadia::window_layer& window)
{
    const auto& app_config = arcadia::app_config::instance();

    arcadia::match<void>(
        app_config.graphic_api,
        [](const arcadia::graphic_api::opengl&) -> void
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
    },
        [](auto&&) -> void
    {
    }
    );
}

ARCADIA_API void arcadia::imgui_backend::imgui_on_event(arcadia::event_base& event)
{

    if(
        arcadia::event_dispatcher{ event }
        .bind_handler<arcadia::event::window_focus>(arcadia::imgui_backend::imgui_on_window_focus)
        .bind_handler<arcadia::event::input_cursor_enter>(arcadia::imgui_backend::imgui_on_cursor_enter)
        .bind_handler<arcadia::event::input_cursor_pos>(arcadia::imgui_backend::imgui_on_cursor_pos)
        .bind_handler<arcadia::event::input_mouse_button>(arcadia::imgui_backend::imgui_on_mouse_button)
        .bind_handler<arcadia::event::input_scroll>(arcadia::imgui_backend::imgui_on_scroll)
        .bind_handler<arcadia::event::input_key>(arcadia::imgui_backend::imgui_on_key)
        .bind_handler<arcadia::event::input_char>(arcadia::imgui_backend::imgui_on_char)
        //.bind_handler<arcadia::event::monitor_connection>(arcadia::imgui_backend::imgui_on_monitor); // We will manage monitors ourselves for now
        .dispatch()
        )
    {
        auto& io = ImGui::GetIO();
        if(io.WantCaptureMouse || io.WantCaptureKeyboard)
        {
            event.handled = true;
        }
        else
        {
            event.handled = false;
        }
    }
}

ARCADIA_API void arcadia::imgui_backend::imgui_on_window_focus(arcadia::event::window_focus& window_focus)
{
    const auto& [wnd_ptr, focused] = window_focus.data_tuple;
    ImGui_ImplGlfw_WindowFocusCallback(wnd_ptr->get_glfw_window_ptr(), focused);
}

ARCADIA_API void arcadia::imgui_backend::imgui_on_cursor_enter(arcadia::event::input_cursor_enter& input_cursor_enter)
{
    const auto& [wnd_ptr, entered] = input_cursor_enter.data_tuple;
    ImGui_ImplGlfw_CursorEnterCallback(wnd_ptr->get_glfw_window_ptr(), entered);
}

ARCADIA_API void arcadia::imgui_backend::imgui_on_cursor_pos(arcadia::event::input_cursor_pos& input_cursor_pos)
{
    const auto& [wnd_ptr, pos] = input_cursor_pos.data_tuple;
    ImGui_ImplGlfw_CursorPosCallback(wnd_ptr->get_glfw_window_ptr(), pos.x, pos.y);
}

ARCADIA_API void arcadia::imgui_backend::imgui_on_mouse_button(arcadia::event::input_mouse_button& input_mouse_button)
{
    const auto& [wnd_ptr, button, action, mods] = input_mouse_button.data_tuple;
    ImGui_ImplGlfw_MouseButtonCallback(wnd_ptr->get_glfw_window_ptr(), button, action, mods);
}

ARCADIA_API void arcadia::imgui_backend::imgui_on_scroll(arcadia::event::input_scroll& input_scroll)
{
    const auto& [wnd_ptr, offset] = input_scroll.data_tuple;
    ImGui_ImplGlfw_ScrollCallback(wnd_ptr->get_glfw_window_ptr(), offset.x, offset.y);
}

ARCADIA_API void arcadia::imgui_backend::imgui_on_key(arcadia::event::input_key& input_key)
{
    const auto& [wnd_ptr, key, scancode, action, mods] = input_key.data_tuple;
    ImGui_ImplGlfw_KeyCallback(wnd_ptr->get_glfw_window_ptr(), key, scancode, action, mods);
}

ARCADIA_API void arcadia::imgui_backend::imgui_on_char(arcadia::event::input_char& input_char)
{
    const auto& [wnd_ptr, code] = input_char.data_tuple;
    ImGui_ImplGlfw_CharCallback(wnd_ptr->get_glfw_window_ptr(), code);
}

ARCADIA_API void arcadia::imgui_backend::imgui_on_monitor(arcadia::event::monitor_connection& monitor_connection)
{
    const auto& [glfw_monitor_ptr, connection] = monitor_connection.data_tuple;
    ImGui_ImplGlfw_MonitorCallback(glfw_monitor_ptr, connection);
}

