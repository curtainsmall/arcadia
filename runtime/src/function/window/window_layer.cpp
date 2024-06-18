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
    _Title(title),
    _MultisampleCount(multisample_count)
{
    const auto& app_config = AppConfig::Instance();

    Match<void>(
        app_config.GraphicApi,
        [&](const GraphicApi::Opengl& gl) -> void
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl.version.Major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl.version.Minor);
        glfwWindowHint(GLFW_SAMPLES, _MultisampleCount);
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
    _GlfwWindow = glfwCreateWindow(size.x, size.y, _Title.c_str(), nullptr, nullptr);
    if(!_GlfwWindow)
    {
        const char* desr{ nullptr };
        auto err_code = glfwGetError(&desr);
        throw GlfwError{ std::format("Failed to create GLFW window, because {}",desr) };
    }
    glfwMakeContextCurrent(_GlfwWindow);

    double x = _LastCursorPos.x,
        y = _LastCursorPos.y;
    glfwGetCursorPos(_GlfwWindow, &x, &y);
    _LastCursorPos.x = static_cast<float>(x);
    _LastCursorPos.y = static_cast<float>(y);
    auto& min = app_config.WindowMinSize;
    auto& max = app_config.WindowMaxSize;
    glfwSetWindowSizeLimits(
        _GlfwWindow,
        min.x < 0 ? GLFW_DONT_CARE : min.x,
        min.y < 0 ? GLFW_DONT_CARE : min.y,
        max.x < 0 ? GLFW_DONT_CARE : max.x,
        max.y < 0 ? GLFW_DONT_CARE : max.y
    );
    if(app_config.WindowMaxmized)
    {
        glfwMaximizeWindow(_GlfwWindow);
    }
    else
    {
        auto& size = app_config.WindowSize;
        glfwSetWindowSize(_GlfwWindow, size.x, size.y);
    }

    glfwSetWindowUserPointer(_GlfwWindow, this);
    _SetupCallbacks();

    glfwShowWindow(_GlfwWindow);
}

WindowLayer::~WindowLayer()
{
    if(_GlfwWindow)
    {
        glfwDestroyWindow(_GlfwWindow);
    }
}

void WindowLayer::OnEvent(EventBase& event)
{
    EventDispatcher{ event }
        .Dispatch<Event::WindowSetInputModeCursor>(ARCADIA_BIND_MEMBER_FN(_OnWindowSetInputModeCursor))
        .Result();
}

void WindowLayer::OnUpdate()
{
    _SwapBuffers();

    glfwPollEvents();
}


auto WindowLayer::GetTitle() const -> const std::string&
{
    return _Title;
}

auto WindowLayer::GetSizeState() const -> WindowSizeState
{
    if(glfwGetWindowAttrib(_GlfwWindow, GLFW_MAXIMIZED))
    {
        return WindowSizeState::Maxmized;
    }
    else if(glfwGetWindowAttrib(_GlfwWindow, GLFW_ICONIFIED))
    {
        return WindowSizeState::Minimized;
    }
    else
    {
        return WindowSizeState::Restored;
    }
}

auto WindowLayer::GetSize() const -> glm::ivec2
{
    glm::ivec2 vec{};
    glfwGetWindowSize(_GlfwWindow, &vec.x, &vec.y);
    return vec;
}

auto WindowLayer::GetPos() const -> glm::ivec2
{
    glm::ivec2 vec{};
    glfwGetWindowPos(_GlfwWindow, &vec.x, &vec.y);
    return vec;
}

void WindowLayer::_OnWindowSetInputModeCursor(Event::WindowSetInputModeCursor& e)
{
    auto& [value] = e.data_tuple;
    int val = Match<int>(
        value,
        WindowInputModeCursor::Normal,
        GLFW_CURSOR_NORMAL,
        WindowInputModeCursor::Hidden,
        GLFW_CURSOR_HIDDEN,
        WindowInputModeCursor::Disabled,
        GLFW_CURSOR_DISABLED,
        WindowInputModeCursor::Captured,
        GLFW_CURSOR_CAPTURED
    );
    glfwSetInputMode(_GlfwWindow, GLFW_CURSOR, val);
}

void WindowLayer::_SetupCallbacks()
{
    // Set callbacks
    glfwSetKeyCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, int key, int scancode, int action, int mods) -> void
    {
        EventQueue::Instance()
            .Signal<Event::InputKey>(
                _GetWindowPtrFromGlfwUserPtr(glfw_wnd_ptr),
                key,
                scancode,
                action,
                mods
            );
    }
    );
    glfwSetCursorPosCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, double xpos, double ypos) -> void
    {
        glm::vec2 cursor_pos{ xpos,ypos };
        auto wnd_ptr = _GetWindowPtrFromGlfwUserPtr(glfw_wnd_ptr);
        auto& event_queue = EventQueue::Instance();

        event_queue.Signal<Event::InputCursorPos>(
            wnd_ptr,
            cursor_pos
        );

        auto& last_pos = wnd_ptr->_LastCursorPos;
        auto offset = cursor_pos - last_pos;
        if(IsInRange(offset.x, _LegalCursorMoveRange.x, _LegalCursorMoveRange.y)
           && IsInRange(offset.y, _LegalCursorMoveRange.x, _LegalCursorMoveRange.y))
        {
            event_queue.Signal<Event::InputCursorMove>(
                wnd_ptr,
                offset
            );
        }
        last_pos = cursor_pos;
    }
    );
    glfwSetScrollCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, double xoffset, double yoffset) -> void
    {
        EventQueue::Instance()
            .Signal<Event::InputScroll>(
                _GetWindowPtrFromGlfwUserPtr(glfw_wnd_ptr),
                glm::vec2{ xoffset,yoffset }
        );
    }
    );
    glfwSetMouseButtonCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, int button, int action, int mods) -> void
    {
        EventQueue::Instance()
            .Signal<Event::InputMouseButton>(
                _GetWindowPtrFromGlfwUserPtr(glfw_wnd_ptr),
                button,
                action,
                mods
            );
    }
    );
    glfwSetWindowSizeCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, int width, int height) -> void
    {
        EventQueue::Instance()
            .Signal<Event::WindowSize>(
                _GetWindowPtrFromGlfwUserPtr(glfw_wnd_ptr),
                glm::ivec2{ width,height }
        );
    }
    );
    glfwSetWindowPosCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, int xpos, int ypos) -> void
    {
        EventQueue::Instance()
            .Signal<Event::WindowPos>(
                _GetWindowPtrFromGlfwUserPtr(glfw_wnd_ptr),
                glm::ivec2{ xpos,ypos }
        );
    }
    );
    glfwSetWindowIconifyCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, int iconified) -> void
    {
        auto& event_queue = EventQueue::Instance();
        if(iconified)
        {
            event_queue.Signal<Event::WindowSizeState>(
                _GetWindowPtrFromGlfwUserPtr(glfw_wnd_ptr),
                WindowSizeState::Minimized
            );
        }
        else
        {
            event_queue.Signal<Event::WindowSizeState>(
                _GetWindowPtrFromGlfwUserPtr(glfw_wnd_ptr),
                WindowSizeState::Restored
            );
        }
    }
    );
    glfwSetWindowMaximizeCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, int maxmized) -> void
    {
        auto& event_queue = EventQueue::Instance();
        if(maxmized)
        {
            event_queue.Signal<Event::WindowSizeState>(
                _GetWindowPtrFromGlfwUserPtr(glfw_wnd_ptr),
                WindowSizeState::Maxmized
            );
        }
        else
        {
            event_queue.Signal<Event::WindowSizeState>(
                _GetWindowPtrFromGlfwUserPtr(glfw_wnd_ptr),
                WindowSizeState::Restored
            );
        }
    }
    );
    glfwSetWindowFocusCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, int focused) -> void
    {
        EventQueue::Instance()
            .Signal<Event::WindowFocus>(
                _GetWindowPtrFromGlfwUserPtr(glfw_wnd_ptr),
                focused
            );
    }
    );
    glfwSetCursorEnterCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, int entered) -> void
    {
        EventQueue::Instance()
            .Signal<Event::InputCursorEnter>(
                _GetWindowPtrFromGlfwUserPtr(glfw_wnd_ptr),
                entered
            );
    }
    );
    glfwSetCharCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, unsigned int code_point) -> void
    {
        EventQueue::Instance()
            .Signal<Event::InputChar>(
                _GetWindowPtrFromGlfwUserPtr(glfw_wnd_ptr),
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
        EventQueue::Instance()
            .Signal<Event::MonitorConnection>(
                glfw_monitor_ptr,
                connection
            );
    }
    );
    glfwSetWindowCloseCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr) -> void
    {
        EventQueue::Instance()
            .Signal<Event::WindowShouldClose>(
                _GetWindowPtrFromGlfwUserPtr(glfw_wnd_ptr)
            );
    }
    );
}

void WindowLayer::_SwapBuffers()
{
    const auto& app_config = AppConfig::Instance();

    Match<void>(
        app_config.GraphicApi,
        [&](const GraphicApi::Opengl&) -> void
    {
        glfwSwapBuffers(_GlfwWindow);
    },
        [](auto&&) -> void
    {
    }
    );
}

void WindowLayer::_OnWindowCloseCanceled(Event::WindowCloseCanceled& e)
{
    const auto& [p_wnd] = e.data_tuple;
    if(p_wnd == this)
    {
        glfwSetWindowShouldClose(_GlfwWindow, GLFW_FALSE);
    }
}

auto WindowLayer::GetMultisampleCount() const -> int
{
    return _MultisampleCount;
}

auto WindowLayer::GetInputModeCursor() const -> WindowInputModeCursor
{
    switch(glfwGetInputMode(_GlfwWindow, GLFW_CURSOR))
    {
        case GLFW_CURSOR_NORMAL:
        {
            return WindowInputModeCursor::Normal;
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
            ARCADIA_ASSERT(false);
    }
}
