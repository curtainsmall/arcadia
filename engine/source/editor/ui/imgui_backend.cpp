#include "imgui_backend.hpp"

#include <format>
#include <string>

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "app/runtime_config.hpp"
#include "core/match.hpp"
#include "platform/graphic_api.hpp"

namespace
{
    auto OpenglVersionString(const Arcadia::Version& version) -> std::string
    {
        if(version >= Arcadia::Version{ 3, 3, 0 })
        {
            return std::format("#version {0}{1}0", version.Major, version.Minor);
        }
        if(version >= Arcadia::Version{ 3, 0, 0 })
        {
            return std::format("#version 1{}0", version.Minor + 3);
        }
        return std::format("#version 1{}0", version.Minor + 1);
    }

    void OnWindowSetFocused(Arcadia::Engine& engine, Arcadia::Events::WindowSetFocused& e)
    {
        ImGui_ImplGlfw_WindowFocusCallback(engine.window->GetGlfwWindow(), e.Focused);
    }

    void OnInputCursorEnter(Arcadia::Engine& engine, Arcadia::Events::InputCursorEnter& e)
    {
        ImGui_ImplGlfw_CursorEnterCallback(engine.window->GetGlfwWindow(), e.Entered);
    }

    void OnInputCursorPosition(Arcadia::Engine& engine, Arcadia::Events::InputCursorPosition& e)
    {
        ImGui_ImplGlfw_CursorPosCallback(engine.window->GetGlfwWindow(), e.CursorPosition.x, e.CursorPosition.y);
    }

    void OnInputMouseButton(Arcadia::Engine& engine, Arcadia::Events::InputMouseButton& e)
    {
        ImGui_ImplGlfw_MouseButtonCallback(
            engine.window->GetGlfwWindow(),
            static_cast<int>(e.MouseButton),
            static_cast<int>(e.Action),
            static_cast<int>(e.Modifier)
        );
    }

    void OnInputScroll(Arcadia::Engine& engine, Arcadia::Events::InputScroll& e)
    {
        ImGui_ImplGlfw_ScrollCallback(engine.window->GetGlfwWindow(), e.ScrollOffsetX, e.ScrollOffsetY);
    }

    void OnInputKey(Arcadia::Engine& engine, Arcadia::Events::InputKey& e)
    {
        ImGui_ImplGlfw_KeyCallback(
            engine.window->GetGlfwWindow(),
            static_cast<int>(e.KeyCode),
            e.KeyScancode,
            static_cast<int>(e.Action),
            static_cast<int>(e.Modifier)
        );
    }

    void OnInputChar(Arcadia::Engine& engine, Arcadia::Events::InputChar& e)
    {
        ImGui_ImplGlfw_CharCallback(engine.window->GetGlfwWindow(), e.UnicodeCodepoint);
    }
}

void Arcadia::ImguiBackend::Initialize(Engine& engine, WindowService& window)
{
    const GraphicApi::Type graphic_api = GraphicApi::FromConfig(engine.config);

    MatchVariant<void>(
        graphic_api,
        [&](const GraphicApi::Opengl& opengl) -> void
        {
            ImGui_ImplGlfw_InitForOpenGL(window.GetGlfwWindow(), false);
            ImGui_ImplOpenGL3_Init(OpenglVersionString(opengl.Version).c_str());
        },
        [](auto&&) -> void
        {
        }
    );
}

void Arcadia::ImguiBackend::NewFrame(Engine& engine, WindowService& window)
{
    const GraphicApi::Type graphic_api = GraphicApi::FromConfig(engine.config);

    MatchVariant<void>(
        graphic_api,
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

void Arcadia::ImguiBackend::RenderDrawData(Engine& engine, WindowService& window)
{
    const GraphicApi::Type graphic_api = GraphicApi::FromConfig(engine.config);

    MatchVariant<void>(
        graphic_api,
        [](const GraphicApi::Opengl&) -> void
        {
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        },
        [](auto&&) -> void
        {
        }
    );
}

void Arcadia::ImguiBackend::Shutdown(Engine& engine, WindowService& window)
{
    const GraphicApi::Type graphic_api = GraphicApi::FromConfig(engine.config);

    MatchVariant<void>(
        graphic_api,
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

void Arcadia::ImguiBackend::OnEvent(Engine& engine, EventBase& e)
{
    if(EventDispatcher{ e }
       .Dispatch<Events::WindowSetFocused>([&engine](Events::WindowSetFocused& event) -> void { OnWindowSetFocused(engine, event); })
       .Dispatch<Events::InputCursorEnter>([&engine](Events::InputCursorEnter& event) -> void { OnInputCursorEnter(engine, event); })
       .Dispatch<Events::InputCursorPosition>([&engine](Events::InputCursorPosition& event) -> void { OnInputCursorPosition(engine, event); })
       .Dispatch<Events::InputMouseButton>([&engine](Events::InputMouseButton& event) -> void { OnInputMouseButton(engine, event); })
       .Dispatch<Events::InputScroll>([&engine](Events::InputScroll& event) -> void { OnInputScroll(engine, event); })
       .Dispatch<Events::InputKey>([&engine](Events::InputKey& event) -> void { OnInputKey(engine, event); })
       .Dispatch<Events::InputChar>([&engine](Events::InputChar& event) -> void { OnInputChar(engine, event); })
       // Monitor events are managed by the window service itself for now
       .IsDispatched())
    {
        ImGuiIO& io = ImGui::GetIO();
        if(io.WantCaptureMouse || io.WantCaptureKeyboard)
        {
            e.MarkHandled();
        }
    }
}
