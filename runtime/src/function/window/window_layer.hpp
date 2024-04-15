#pragma once

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"core/layer/layer.hpp"
#include"core/math.hpp"
#include"function/window/window_events.hpp"
#include"platform/glfw/glfw_header.hpp"
#include"platform/graphic_api/graphic_api.hpp"

namespace Arcadia
{
    struct ARCADIA_API WindowLayer: Arcadia::iLayer
    {
    public:
        using self_type = Arcadia::WindowLayer;
    public:
        WindowLayer(
            glm::ivec2 size ={ 800,600 },
            std::string title = "Untitled",
            int multisample_count = 0
        );
        virtual ~WindowLayer();

        [[nodiscard]]
        auto GetGlfwWindowPtr() const -> GLFWwindow*
        {
            return _GlfwWindow;
        }

        virtual void OnEvent(Arcadia::EventBase& event) override;
        virtual void OnUpdate() override;

        [[nodiscard]]
        auto GetTitle() const->const std::string&;

        [[nodiscard]]
        auto GetSizeState() const->Arcadia::WindowSizeState;

        [[nodiscard]]
        auto GetSize() const->glm::ivec2;

        [[nodiscard]]
        auto GetPos() const->glm::ivec2;

        [[nodiscard]]
        auto GetMultisampleCount() const -> int;

        [[nodiscard]]
        auto GetInputModeCursor() const->Arcadia::WindowInputModeCursor;

    private:
        static auto _GetWindowPtrFromGlfwUserPtr(GLFWwindow* glfw_window) -> self_type*
        {
            return static_cast<self_type*>(glfwGetWindowUserPointer(glfw_window));
        }

        void _OnWindowSetInputModeCursor(Arcadia::Event::WindowSetInputModeCursor& e);

        void _SetupCallbacks();

        /// @brief Call [glfwSwapBuffers](https://www.glfw.org/docs/3.3/group__window.html#ga15a5a1ee5b3c2ca6b15ca209a12efd14)
        void _SwapBuffers();

        void _OnWindowCloseCanceled(Arcadia::Event::WindowCloseCanceled& e);

    private:
        static inline GlfwContext _GlfwContext{};
        /// @brief Cursor move offset that is out of this range will be silently ignored
        static constexpr glm::vec2 _LegalCursorMoveRange{ -20.f,20.f };

        std::string _Title;
        int _SwapInterval{ 0 };

        GLFWwindow* _GlfwWindow{ nullptr };
        glm::vec2 _LastCursorPos{ .0f };
        const int _MultisampleCount;

    };


}
