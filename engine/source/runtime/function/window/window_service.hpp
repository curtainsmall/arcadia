#pragma once

#include "core/event.hpp"
#include "app/engine.hpp"
#include "app/services.hpp"
#include "core/math.hpp"
#include "function/window/window_events.hpp"
#include "platform/defines.hpp"
#include "platform/glfw.hpp"
#include "platform/graphic_api.hpp"

namespace Arcadia
{
    struct ACDA_API WindowService: public Platform
    {
    public:
        using SelfType = WindowService;
    public:
        WindowService(
            Engine& engine,
            glm::i32vec2 size = { 800,600 },
            std::string_view title = "Untitled",
            std::int32_t multisample_count = 0
        );
        virtual ~WindowService() override;

        void PollEvents() override;
        void SwapBuffers() override;

        [[nodiscard]]
        auto GetGlfwWindow() const -> GLFWwindow*;

        void OnEvent(EventBase& event);

        [[nodiscard]]
        auto GetTitle() const -> std::string_view;

        [[nodiscard]]
        auto GetSizeState() const -> GlfwWindowSizeState;

        [[nodiscard]]
        auto GetSize() const -> glm::i32vec2;

        [[nodiscard]]
        auto GetPosition() const -> glm::i32vec2;

        [[nodiscard]]
        auto GetMultisampleCount() const -> std::int32_t;

        [[nodiscard]]
        auto GetCursorInputMode() const -> GlfwWindowCursorInputMode;

    private:
        static auto _GetWindowPointerFromGlfwUserPointer(GLFWwindow* glfw_window) -> SelfType*;

        void _OnWindowSetCursorInputMode(Events::WindowSetCursorInputMode& e);

        void _SetupCallbacks();

        void _OnWindowCloseCanceled(Events::WindowCloseCanceled& e);

    private:
        Engine& _Engine;

        Foundation::Subscription _EventSubscription{};

        static inline GlfwContext _GlfwContext{};

        // GLFW monitor callbacks carry no user data; the service keeps the one
        // engine it belongs to.
        static inline Engine* _pEngine{ nullptr };

        // Cursor move offset that is out of this range will be silently ignored
        static constexpr glm::vec2 _LegalCursorMoveRange{ -20.f,20.f };

        std::string _Title;
        std::int32_t _SwapInterval{ 0 };

        GLFWwindow* _GlfwWindow{ nullptr };
        glm::vec2 _LastCursorPosition{ .0f };
        const std::int32_t _MultisampleCount;
    };
}
