#include "backend.hpp"

#include"imgui/backends/imgui_impl_glfw.h"
#include"imgui/backends/imgui_impl_opengl3.h"

#include"core/util/conditional.hpp"

ARCADIA_API void arcadia::imgui_backend::initialize(const arcadia::window_layer& window)
{
    arcadia::match<void>(
        window.get_graphic_api(),
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
        ImGui_ImplGlfw_InitForOpenGL(window.get_glfw_window_ptr(), ARCADIA_IMGUI_USE_INTERNAL_CALLBACKS);
        ImGui_ImplOpenGL3_Init(glsl_version.c_str());
    },
        [](auto&&) -> void
    {
    }
    );
}

ARCADIA_API void arcadia::imgui_backend::new_frame(const arcadia::window_layer& window)
{
    arcadia::match<void>(
        window.get_graphic_api(),
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
    arcadia::match<void>(
        window.get_graphic_api(),
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
    arcadia::match<void>(
        window.get_graphic_api(),
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

ARCADIA_API auto arcadia::imgui_backend::imgui_on_event(const arcadia::event& event) -> bool
{

#if !ARCADIA_IMGUI_USE_INTERNAL_CALLBACKS
    ARCADIA_DISPATCH_EVENT(arcadia::window_focus, event, arcadia::imgui_backend::imgui_on_window_focus);
    ARCADIA_DISPATCH_EVENT(arcadia::input_cursor_enter, event, arcadia::imgui_backend::imgui_on_cursor_enter);
    ARCADIA_DISPATCH_EVENT(arcadia::input_cursor_pos, event, arcadia::imgui_backend::imgui_on_cursor_pos);
    ARCADIA_DISPATCH_EVENT(arcadia::input_mouse_button, event, arcadia::imgui_backend::imgui_on_mouse_button);
    ARCADIA_DISPATCH_EVENT(arcadia::input_scroll, event, arcadia::imgui_backend::imgui_on_scroll);
    ARCADIA_DISPATCH_EVENT(arcadia::input_key, event, arcadia::imgui_backend::imgui_on_key);
    ARCADIA_DISPATCH_EVENT(arcadia::input_char, event, arcadia::imgui_backend::imgui_on_char);
    //ARCADIA_DISPATCH_EVENT(arcadia::monitor_connection, event, arcadia::imgui_backend::imgui_on_monitor); // We will manage monitors ourselves for now
#endif

    return false;
}

ARCADIA_API auto arcadia::imgui_backend::imgui_on_window_focus(const arcadia::window_focus& window_focus) -> bool
{
    const auto& [wnd_ptr, focused] = window_focus.data_tuple;
    ImGui_ImplGlfw_WindowFocusCallback(wnd_ptr->get_glfw_window_ptr(), focused);
    return false;
}

ARCADIA_API auto arcadia::imgui_backend::imgui_on_cursor_enter(const arcadia::input_cursor_enter& input_cursor_enter) -> bool
{
    const auto& [wnd_ptr, entered] = input_cursor_enter.data_tuple;
    ImGui_ImplGlfw_CursorEnterCallback(wnd_ptr->get_glfw_window_ptr(), entered);
    return false;
}

ARCADIA_API auto arcadia::imgui_backend::imgui_on_cursor_pos(const arcadia::input_cursor_pos& input_cursor_pos) -> bool
{
    const auto& [wnd_ptr, pos] = input_cursor_pos.data_tuple;
    ImGui_ImplGlfw_CursorPosCallback(wnd_ptr->get_glfw_window_ptr(), pos.x, pos.y);
    return false;
}

ARCADIA_API auto arcadia::imgui_backend::imgui_on_mouse_button(const arcadia::input_mouse_button& input_mouse_button) -> bool
{
    const auto& [wnd_ptr, button, action, mods] = input_mouse_button.data_tuple;
    ImGui_ImplGlfw_MouseButtonCallback(wnd_ptr->get_glfw_window_ptr(), button, action, mods);
    return false;
}

ARCADIA_API auto arcadia::imgui_backend::imgui_on_scroll(const arcadia::input_scroll& input_scroll) -> bool
{
    const auto& [wnd_ptr, offset] = input_scroll.data_tuple;
    ImGui_ImplGlfw_ScrollCallback(wnd_ptr->get_glfw_window_ptr(), offset.x, offset.y);
    return false;
}

ARCADIA_API auto arcadia::imgui_backend::imgui_on_key(const arcadia::input_key& input_key) -> bool
{
    const auto& [wnd_ptr, key, scancode, action, mods] = input_key.data_tuple;
    ImGui_ImplGlfw_KeyCallback(wnd_ptr->get_glfw_window_ptr(), key, scancode, action, mods);
    return false;
}

ARCADIA_API auto arcadia::imgui_backend::imgui_on_char(const arcadia::input_char& input_char) -> bool
{
    const auto& [wnd_ptr, code] = input_char.data_tuple;
    ImGui_ImplGlfw_CharCallback(wnd_ptr->get_glfw_window_ptr(), code);
    return false;
}

ARCADIA_API auto arcadia::imgui_backend::imgui_on_monitor(const arcadia::monitor_connection& monitor_connection) -> bool
{
    const auto& [glfw_monitor_ptr, connection] = monitor_connection.data_tuple;
    ImGui_ImplGlfw_MonitorCallback(glfw_monitor_ptr, connection);
    return false;
}

