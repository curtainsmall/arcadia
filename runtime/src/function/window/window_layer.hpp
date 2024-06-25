#pragma once

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"core/layer/layer.hpp"
#include"core/math.hpp"
#include"function/window/window_events.hpp"
#include"platform/glfw/glfw_header.hpp"
#include"platform/graphic_api/graphic_api.hpp"

struct WindowLayer: iLayer
{
public:
    using self_type = WindowLayer;
public:
    WindowLayer(
        glm::ivec2 size ={ 800,600 },
        std::string title = "Untitled",
        int multisample_count = 0
    );
    virtual ~WindowLayer();

    [[nodiscard]]
    auto glfw_window() const -> GLFWwindow*
    {
        return _glfw_window;
    }

    virtual void on_event(EventBase& event) override;
    virtual void on_update() override;

    [[nodiscard]]
    auto title() const->const std::string&;

    [[nodiscard]]
    auto size_state() const->WindowSizeState;

    [[nodiscard]]
    auto size() const->glm::ivec2;

    [[nodiscard]]
    auto pos() const->glm::ivec2;

    [[nodiscard]]
    auto multisample_count() const -> int;

    [[nodiscard]]
    auto input_mode_cursor() const->WindowInputModeCursor;

private:
    static auto _get_window_ptr_from_glfw_user_ptr(GLFWwindow* glfw_window) -> self_type*
    {
        return static_cast<self_type*>(glfwGetWindowUserPointer(glfw_window));
    }

    void _on_window_set_input_mode_cursor(events::WindowSetInputModeCursor& e);

    void _setup_callbacks();

    /// @brief Call [glfwSwapBuffers](https://www.glfw.org/docs/3.3/group__window.html#ga15a5a1ee5b3c2ca6b15ca209a12efd14)
    void _swap_buffers();

    void _on_window_close_canceled(events::WindowCloseCanceled& e);

private:
    static inline GlfwContext _glfw_context{};
    /// @brief Cursor move offset that is out of this range will be silently ignored
    static constexpr glm::vec2 _legal_cursor_move_range{ -20.f,20.f };

    std::string _title;
    int _swap_interval{ 0 };

    GLFWwindow* _glfw_window{ nullptr };
    glm::vec2 _last_cursor_pos{ .0f };
    const int _multisample_count;
};
