#include"pch.hpp"

#include "imgui_backend.hpp"

#include"imgui/backends/imgui_impl_glfw.h"
#include"imgui/backends/imgui_impl_opengl3.h"

#include"core/app/app_config.hpp"

ARCADIA_API void Arcadia::ImguiBackend::Initialize(const Arcadia::WindowLayer& window)
{
    const auto& app_config = Arcadia::AppConfig::Instance();

    Arcadia::Match<void>(
        app_config.GraphicApi,
        [&](const Arcadia::GraphicApi::Opengl& Opengl) -> void
    {
        std::string glsl_version{};
        if(Opengl.version >= Arcadia::Version{ 3,3,0 })
        {
            glsl_version = std::format("#version {0}{1}0", Opengl.version.Major, Opengl.version.Minor);
        }
        else
        {
            if(Opengl.version >= Arcadia::Version{ 3,0,0 })
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

ARCADIA_API void Arcadia::ImguiBackend::NewFrame(const Arcadia::WindowLayer& window)
{
    const auto& app_config = Arcadia::AppConfig::Instance();

    Arcadia::Match<void>(
        app_config.GraphicApi,
        [](const Arcadia::GraphicApi::Opengl&) -> void
    {
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
    },
        [](auto&&) -> void
    {
    }
    );
}

ARCADIA_API void Arcadia::ImguiBackend::RenderDrawData(const Arcadia::WindowLayer& window)
{
    const auto& app_config = Arcadia::AppConfig::Instance();

    Arcadia::Match<void>(
        app_config.GraphicApi,
        [](const Arcadia::GraphicApi::Opengl&) -> void
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    },
        [](auto&&) -> void
    {
    }
    );
}

ARCADIA_API void Arcadia::ImguiBackend::Shutdown(const Arcadia::WindowLayer& window)
{
    const auto& app_config = Arcadia::AppConfig::Instance();

    Arcadia::Match<void>(
        app_config.GraphicApi,
        [](const Arcadia::GraphicApi::Opengl&) -> void
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
    },
        [](auto&&) -> void
    {
    }
    );
}

ARCADIA_API void Arcadia::ImguiBackend::ImguiOnEvent(Arcadia::EventBase& event)
{
    if(Arcadia::EventDispatcher{ event }
       .Dispatch<Arcadia::Event::WindowFocus>(Arcadia::ImguiBackend::ImguiOnWindowFocus)
       .Dispatch<Arcadia::Event::InputCursorEnter>(Arcadia::ImguiBackend::ImguiOnCursorEnter)
       .Dispatch<Arcadia::Event::InputCursorPos>(Arcadia::ImguiBackend::ImguiOnCursorPos)
       .Dispatch<Arcadia::Event::InputMouseButton>(Arcadia::ImguiBackend::ImguiOnMouseButton)
       .Dispatch<Arcadia::Event::InputScroll>(Arcadia::ImguiBackend::ImguiOnScroll)
       .Dispatch<Arcadia::Event::InputKey>(Arcadia::ImguiBackend::ImguiOnKey)
       .Dispatch<Arcadia::Event::InputChar>(Arcadia::ImguiBackend::ImguiOnChar)
       //.Dispatch<Arcadia::Event::MonitorConnection>(Arcadia::ImguiBackend::ImguiOnMonitor); // We will manage monitors ourselves for now
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

ARCADIA_API void Arcadia::ImguiBackend::ImguiOnWindowFocus(Arcadia::Event::WindowFocus& WindowFocus)
{
    const auto& [wnd_ptr, focused] = WindowFocus.data_tuple;
    ImGui_ImplGlfw_WindowFocusCallback(wnd_ptr->GetGlfwWindowPtr(), focused);
}

ARCADIA_API void Arcadia::ImguiBackend::ImguiOnCursorEnter(Arcadia::Event::InputCursorEnter& input_cursor_enter)
{
    const auto& [wnd_ptr, entered] = input_cursor_enter.data_tuple;
    ImGui_ImplGlfw_CursorEnterCallback(wnd_ptr->GetGlfwWindowPtr(), entered);
}

ARCADIA_API void Arcadia::ImguiBackend::ImguiOnCursorPos(Arcadia::Event::InputCursorPos& input_cursor_pos)
{
    const auto& [wnd_ptr, pos] = input_cursor_pos.data_tuple;
    ImGui_ImplGlfw_CursorPosCallback(wnd_ptr->GetGlfwWindowPtr(), pos.x, pos.y);
}

ARCADIA_API void Arcadia::ImguiBackend::ImguiOnMouseButton(Arcadia::Event::InputMouseButton& input_mouse_button)
{
    const auto& [wnd_ptr, button, action, mods] = input_mouse_button.data_tuple;
    ImGui_ImplGlfw_MouseButtonCallback(wnd_ptr->GetGlfwWindowPtr(), button, action, mods);
}

ARCADIA_API void Arcadia::ImguiBackend::ImguiOnScroll(Arcadia::Event::InputScroll& input_scroll)
{
    const auto& [wnd_ptr, Offset] = input_scroll.data_tuple;
    ImGui_ImplGlfw_ScrollCallback(wnd_ptr->GetGlfwWindowPtr(), Offset.x, Offset.y);
}

ARCADIA_API void Arcadia::ImguiBackend::ImguiOnKey(Arcadia::Event::InputKey& input_key)
{
    const auto& [wnd_ptr, key, scancode, action, mods] = input_key.data_tuple;
    ImGui_ImplGlfw_KeyCallback(wnd_ptr->GetGlfwWindowPtr(), key, scancode, action, mods);
}

ARCADIA_API void Arcadia::ImguiBackend::ImguiOnChar(Arcadia::Event::InputChar& input_char)
{
    const auto& [wnd_ptr, code] = input_char.data_tuple;
    ImGui_ImplGlfw_CharCallback(wnd_ptr->GetGlfwWindowPtr(), code);
}

ARCADIA_API void Arcadia::ImguiBackend::ImguiOnMonitor(Arcadia::Event::MonitorConnection& monitor_connection)
{
    const auto& [glfw_monitor_ptr, connection] = monitor_connection.data_tuple;
    ImGui_ImplGlfw_MonitorCallback(glfw_monitor_ptr, connection);
}

