#include "pch.hpp"

#include "window_layer.hpp"

#include"core/app/app_config.hpp"
#include"function/input/input_events.hpp"

WindowLayer::WindowLayer(
    glm::ivec2 size,
    std::string title,
    int multisample_count
):
    iLayer(std::format("window_{}", title)),
    _title(title),
    _multisample_count(multisample_count)
{
    const auto& app_config = AppConfig::instance();

    match<void>(
        app_config.graphic_api,
        [&](const graphic_api::Opengl& gl) -> void
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl.version.major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl.version.minor);
        glfwWindowHint(GLFW_SAMPLES, _multisample_count);
    #ifndef NDEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    #endif // NDEBUG

    },
        [&](auto&) -> void
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }
    );

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    _glfw_window = glfwCreateWindow(size.x, size.y, _title.c_str(), nullptr, nullptr);
    if(!_glfw_window)
    {
        const char* desr{ nullptr };
        auto err_code = glfwGetError(&desr);
        throw GlfwError{ std::format("Failed to create GLFW window, because {}",desr) };
    }
    glfwMakeContextCurrent(_glfw_window);

    double x = _last_cursor_pos.x,
        y = _last_cursor_pos.y;
    glfwGetCursorPos(_glfw_window, &x, &y);
    _last_cursor_pos.x = static_cast<float>(x);
    _last_cursor_pos.y = static_cast<float>(y);
    auto& min = app_config.window_size_min;
    auto& max = app_config.window_size_max;
    glfwSetWindowSizeLimits(
        _glfw_window,
        min.x < 0 ? GLFW_DONT_CARE : min.x,
        min.y < 0 ? GLFW_DONT_CARE : min.y,
        max.x < 0 ? GLFW_DONT_CARE : max.x,
        max.y < 0 ? GLFW_DONT_CARE : max.y
    );
    if(app_config.window_maxmized)
    {
        glfwMaximizeWindow(_glfw_window);
    }
    else
    {
        auto& size = app_config.window_size;
        glfwSetWindowSize(_glfw_window, size.x, size.y);
    }

    glfwSetWindowUserPointer(_glfw_window, this);
    _setup_callbacks();

    glfwShowWindow(_glfw_window);
}

WindowLayer::~WindowLayer()
{
    if(_glfw_window)
    {
        glfwDestroyWindow(_glfw_window);
    }
}

void WindowLayer::on_event(EventBase& event)
{
    EventDispatcher{ event }
        .dispatch<events::WindowSetInputModeCursor>(ACDA_BIND_MEMBER_FN(_on_window_set_input_mode_cursor))
        .result();
}

void WindowLayer::on_update()
{
    _swap_buffers();

    glfwPollEvents();
}


auto WindowLayer::title() const -> const std::string&
{
    return _title;
}

auto WindowLayer::size_state() const -> WindowSizeState
{
    if(glfwGetWindowAttrib(_glfw_window, GLFW_MAXIMIZED))
    {
        return WindowSizeState::Maxmized;
    }
    else if(glfwGetWindowAttrib(_glfw_window, GLFW_ICONIFIED))
    {
        return WindowSizeState::Minimized;
    }
    else
    {
        return WindowSizeState::Restored;
    }
}

auto WindowLayer::size() const -> glm::ivec2
{
    glm::ivec2 vec{};
    glfwGetWindowSize(_glfw_window, &vec.x, &vec.y);
    return vec;
}

auto WindowLayer::pos() const -> glm::ivec2
{
    glm::ivec2 vec{};
    glfwGetWindowPos(_glfw_window, &vec.x, &vec.y);
    return vec;
}

void WindowLayer::_on_window_set_input_mode_cursor(events::WindowSetInputModeCursor& e)
{
    auto& [value] = e.data_tuple;
    int val = match<int>(
        value,
        WindowInputModeCursor::normal,
        GLFW_CURSOR_NORMAL,
        WindowInputModeCursor::Hidden,
        GLFW_CURSOR_HIDDEN,
        WindowInputModeCursor::Disabled,
        GLFW_CURSOR_DISABLED,
        WindowInputModeCursor::Captured,
        GLFW_CURSOR_CAPTURED
    );
    glfwSetInputMode(_glfw_window, GLFW_CURSOR, val);
}

void WindowLayer::_setup_callbacks()
{
    // Set callbacks
    glfwSetKeyCallback(
        _glfw_window,
        [](GLFWwindow* glfw_wnd_ptr, int key, int scancode, int action, int mods) -> void
    {
        EventQueue::instance()
            .signal<events::InputKey>(
                _get_window_ptr_from_glfw_user_ptr(glfw_wnd_ptr),
                key,
                scancode,
                action,
                mods
            );
    }
    );
    glfwSetCursorPosCallback(
        _glfw_window,
        [](GLFWwindow* glfw_wnd_ptr, double xpos, double ypos) -> void
    {
        glm::vec2 cursor_pos{ xpos,ypos };
        auto wnd_ptr = _get_window_ptr_from_glfw_user_ptr(glfw_wnd_ptr);
        auto& event_queue = EventQueue::instance();

        event_queue.signal<events::InputCursorPos>(
            wnd_ptr,
            cursor_pos
        );

        auto& last_pos = wnd_ptr->_last_cursor_pos;
        auto offset = cursor_pos - last_pos;
        if(is_in_range(offset.x, _legal_cursor_move_range.x, _legal_cursor_move_range.y)
           && is_in_range(offset.y, _legal_cursor_move_range.x, _legal_cursor_move_range.y))
        {
            event_queue.signal<events::InputCursorMove>(
                wnd_ptr,
                offset
            );
        }
        last_pos = cursor_pos;
    }
    );
    glfwSetScrollCallback(
        _glfw_window,
        [](GLFWwindow* glfw_wnd_ptr, double xoffset, double yoffset) -> void
    {
        EventQueue::instance()
            .signal<events::InputScroll>(
                _get_window_ptr_from_glfw_user_ptr(glfw_wnd_ptr),
                glm::vec2{ xoffset,yoffset }
        );
    }
    );
    glfwSetMouseButtonCallback(
        _glfw_window,
        [](GLFWwindow* glfw_wnd_ptr, int button, int action, int mods) -> void
    {
        EventQueue::instance()
            .signal<events::InputMouseButton>(
                _get_window_ptr_from_glfw_user_ptr(glfw_wnd_ptr),
                button,
                action,
                mods
            );
    }
    );
    glfwSetWindowSizeCallback(
        _glfw_window,
        [](GLFWwindow* glfw_wnd_ptr, int width, int height) -> void
    {
        EventQueue::instance()
            .signal<events::window_size>(
                _get_window_ptr_from_glfw_user_ptr(glfw_wnd_ptr),
                glm::ivec2{ width,height }
        );
    }
    );
    glfwSetWindowPosCallback(
        _glfw_window,
        [](GLFWwindow* glfw_wnd_ptr, int xpos, int ypos) -> void
    {
        EventQueue::instance()
            .signal<events::window_pos>(
                _get_window_ptr_from_glfw_user_ptr(glfw_wnd_ptr),
                glm::ivec2{ xpos,ypos }
        );
    }
    );
    glfwSetWindowIconifyCallback(
        _glfw_window,
        [](GLFWwindow* glfw_wnd_ptr, int iconified) -> void
    {
        auto& event_queue = EventQueue::instance();
        if(iconified)
        {
            event_queue.signal<events::WindowSizeState>(
                _get_window_ptr_from_glfw_user_ptr(glfw_wnd_ptr),
                WindowSizeState::Minimized
            );
        }
        else
        {
            event_queue.signal<events::WindowSizeState>(
                _get_window_ptr_from_glfw_user_ptr(glfw_wnd_ptr),
                WindowSizeState::Restored
            );
        }
    }
    );
    glfwSetWindowMaximizeCallback(
        _glfw_window,
        [](GLFWwindow* glfw_wnd_ptr, int maxmized) -> void
    {
        auto& event_queue = EventQueue::instance();
        if(maxmized)
        {
            event_queue.signal<events::WindowSizeState>(
                _get_window_ptr_from_glfw_user_ptr(glfw_wnd_ptr),
                WindowSizeState::Maxmized
            );
        }
        else
        {
            event_queue.signal<events::WindowSizeState>(
                _get_window_ptr_from_glfw_user_ptr(glfw_wnd_ptr),
                WindowSizeState::Restored
            );
        }
    }
    );
    glfwSetWindowFocusCallback(
        _glfw_window,
        [](GLFWwindow* glfw_wnd_ptr, int focused) -> void
    {
        EventQueue::instance()
            .signal<events::WindowFocus>(
                _get_window_ptr_from_glfw_user_ptr(glfw_wnd_ptr),
                focused
            );
    }
    );
    glfwSetCursorEnterCallback(
        _glfw_window,
        [](GLFWwindow* glfw_wnd_ptr, int entered) -> void
    {
        EventQueue::instance()
            .signal<events::InputCursorEnter>(
                _get_window_ptr_from_glfw_user_ptr(glfw_wnd_ptr),
                entered
            );
    }
    );
    glfwSetCharCallback(
        _glfw_window,
        [](GLFWwindow* glfw_wnd_ptr, unsigned int code_point) -> void
    {
        EventQueue::instance()
            .signal<events::InputChar>(
                _get_window_ptr_from_glfw_user_ptr(glfw_wnd_ptr),
                code_point
            );
    }
    );
    glfwSetMonitorCallback(
        [](GLFWmonitor* glfw_monitor_ptr, int event) -> void
    {
        bool connection{ false };
        if(event == GLFW_CONNECTED)
        {
            connection = true;
        }
        else if(event == GLFW_DISCONNECTED)
        {
            connection = false;
        }
        EventQueue::instance()
            .signal<events::MonitorConnection>(
                glfw_monitor_ptr,
                connection
            );
    }
    );
    glfwSetWindowCloseCallback(
        _glfw_window,
        [](GLFWwindow* glfw_wnd_ptr) -> void
    {
        EventQueue::instance()
            .signal<events::WindowShouldClose>(
                _get_window_ptr_from_glfw_user_ptr(glfw_wnd_ptr)
            );
    }
    );
}

void WindowLayer::_swap_buffers()
{
    const auto& app_config = AppConfig::instance();

    match<void>(
        app_config.graphic_api,
        [&](const graphic_api::Opengl&) -> void
    {
        glfwSwapBuffers(_glfw_window);
    },
        [](auto&&) -> void
    {
    }
    );
}

void WindowLayer::_on_window_close_canceled(events::WindowCloseCanceled& e)
{
    const auto& [p_wnd] = e.data_tuple;
    if(p_wnd == this)
    {
        glfwSetWindowShouldClose(_glfw_window, GLFW_FALSE);
    }
}

auto WindowLayer::multisample_count() const -> int
{
    return _multisample_count;
}

auto WindowLayer::input_mode_cursor() const -> WindowInputModeCursor
{
    switch(glfwGetInputMode(_glfw_window, GLFW_CURSOR))
    {
        case GLFW_CURSOR_NORMAL:
        {
            return WindowInputModeCursor::normal;
        }
        case GLFW_CURSOR_HIDDEN:
        {
            return WindowInputModeCursor::Hidden;
        }
        case GLFW_CURSOR_DISABLED:
        {
            return WindowInputModeCursor::Disabled;
        }
        case GLFW_CURSOR_CAPTURED:
        {
            return WindowInputModeCursor::Captured;
        }
        default:
            ACDA_ASSERT(false);
    }
}
