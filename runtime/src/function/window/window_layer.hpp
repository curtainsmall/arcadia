#pragma once

#include "core/event.hpp"
#include "core/layer.hpp"
#include "core/math.hpp"
#include "function/window/window_events.hpp"
#include "platform/api_def.hpp"
#include "platform/glfw.hpp"
#include "platform/graphic_api.hpp"

namespace Arcadia
{
    class WindowLayer: public LayerInterface
    {
    public:
        using SelfType = WindowLayer;
    public:
        WindowLayer(
            glm::i32vec2 size ={ 800,600 },
            std::string title = "Untitled",
            std::int32_t multisample_count = 0
        );
        virtual ~WindowLayer();

        [[nodiscard]]
        auto GetGlfwWindow() const->GLFWwindow*;

        virtual void OnEvent(EventBase& event) override;
        virtual void OnUpdate() override;

        [[nodiscard]]
        auto GetTitle() const->const std::string&;

        [[nodiscard]]
        auto GetSizeState() const->WindowSizeState;

        [[nodiscard]]
        auto GetSize() const->glm::i32vec2;

        [[nodiscard]]
        auto GetPosition() const->glm::i32vec2;

        [[nodiscard]]
        auto GetMultisampleCount() const->std::int32_t;

        [[nodiscard]]
        auto GetCursorInputMode() const->WindowCursorInputMode;

    private:
        static auto _GetWindowPointerFromGlfwUserPointer(GLFWwindow* glfw_window) -> SelfType*;

        void _OnWindowSetCursorInputMode(Events::WindowSetCursorInputMode& e);

        void _SetupCallbacks();

        void _SwapBuffers();

        void _OnWindowCloseCanceled(Events::WindowCloseCanceled& e);

    private:
        static inline GlfwContext _GlfwContext{};

        // Cursor move offset that is out of this range will be silently ignored
        static constexpr glm::vec2 _LegalCursorMoveRange{ -20.f,20.f };

        std::string _Title;
        std::int32_t _SwapInterval{ 0 };

        GLFWwindow* _GlfwWindow{ nullptr };
        glm::vec2 _LastCursorPosition{ .0f };
        const std::int32_t _MultisampleCount;
    };
}