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
            return _pGlfwWindow;
        }

        virtual void OnEvent(Arcadia::EventBase& event) override;
        virtual void OnUpdate() override;

        [[nodiscard]]
        auto GetTitle() const->const std::string&;
        auto SetTitle(const std::string& title) -> self_type&;

        [[nodiscard]]
        auto GetSize() const->glm::ivec2;
        auto SetSize(const glm::ivec2& size) -> self_type&;

        [[nodiscard]]
        auto GetSizeState() const->Arcadia::WindowSizeState;
        auto SetSizeState(Arcadia::WindowSizeState state) -> self_type&;

        [[nodiscard]]
        auto GetPos() const->glm::ivec2;
        auto SetPos(const glm::ivec2& pos) -> self_type&;

        [[nodiscard]]
        auto GetSwapInterval() const -> int;
        auto SetSwapInterval(int interval) -> self_type&;

        [[nodiscard]]
        auto GetVisible() const -> bool;
        auto SetVisible(bool visible) -> self_type&;

        [[nodiscard]]
        auto GetInputModeCursor() const -> int;
        auto SetInputModeCursor(int Value) -> self_type&;

        [[nodiscard]]
        auto GetInputModeStickyKeys() const -> int;
        auto SetInputModeStickyKeys(int Value) -> self_type&;

        [[nodiscard]]
        auto GetInputModeStickyMouseButtons() const -> int;
        auto SetInputModeStickMouseButtons(int Value) -> self_type&;

        [[nodiscard]]
        auto GetInputModeLockKeyMods() const -> int;
        auto SetInputModeLockKeyMods(int Value) -> self_type&;

        [[nodiscard]]
        auto GetInputModeRawMouseMotion() const -> int;
        auto SetInputModeRawMouseMotion(int Value) -> self_type&;

        auto GetMultisampleCount() const -> int;

    private:
        static auto _GetWindowPtrFromGlfwUserPtr(GLFWwindow* glfw_window_ptr) -> self_type*
        {
            return static_cast<self_type*>(glfwGetWindowUserPointer(glfw_window_ptr));
        }

        void _SetupCallbacks();

        /// @brief Call [glfwSwapBuffers](https://www.glfw.org/docs/3.3/group__window.html#ga15a5a1ee5b3c2ca6b15ca209a12efd14)
        void _SwapBuffers();

        void _OnWindowCloseCanceled(Arcadia::Event::WindowCloseCanceled& e);

    private:
        static inline GlfwContext _GlfwContext{};

        std::string _Title;
        int _SwapInterval{ 0 };

        GLFWwindow* _pGlfwWindow{ nullptr };
        glm::vec2 _LastCursorPos{ .0f };
        const int _MultisampleCount;
    };


}
