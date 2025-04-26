#include "pch.hpp"

#include "window_layer.hpp"

#include"core/app/app_config.hpp"
#include"core/function.hpp"
#include"core/match.hpp"
#include"function/input/input_events.hpp"

Arcadia::WindowLayer::WindowLayer(
    glm::i32vec2 size,
    std::string title,
    std::int32_t multisample_count
):
    LayerInterface(std::format("window_{}", title)),
    _Title(title),
    _MultisampleCount(multisample_count)
{
    const AppConfig& app_config = AppConfig::Instance();

    MatchVariant<void>(
        app_config.GraphicApi,
        [&](const GraphicApi::Opengl& gl) -> void
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl.Version.Major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl.Version.Minor);
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
        const char* desr = nullptr;
        int err_code = glfwGetError(&desr);
        throw Exceptions::GlfwError(std::format("Failed to create GLFW window, because {}", desr));
    }
    glfwMakeContextCurrent(_GlfwWindow);

    double x = _LastCursorPosition.x;
    double y = _LastCursorPosition.y;
    glfwGetCursorPos(_GlfwWindow, &x, &y);
    _LastCursorPosition.x = static_cast<float>(x);
    _LastCursorPosition.y = static_cast<float>(y);
    const glm::i32vec2& min = app_config.WindowSizeMin;
    const glm::i32vec2& max = app_config.WindowSizeMax;
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
        const glm::i32vec2& size = app_config.WindowSize;
        glfwSetWindowSize(_GlfwWindow, size.x, size.y);
    }

    glfwSetWindowUserPointer(_GlfwWindow, this);
    _SetupCallbacks();

    glfwShowWindow(_GlfwWindow);
}

Arcadia::WindowLayer::~WindowLayer()
{
    if(_GlfwWindow)
    {
        glfwDestroyWindow(_GlfwWindow);
    }
}

void Arcadia::WindowLayer::OnEvent(EventBase& event)
{
    EventDispatcher{ event }
        .Dispatch<Events::WindowSetCursorInputMode>(ACDA_BIND_MEMBER_FN(_OnWindowSetCursorInputMode))
        .IsDispatched();
}

void Arcadia::WindowLayer::OnUpdate()
{
    _SwapBuffers();

    glfwPollEvents();
}

auto Arcadia::WindowLayer::GetTitle() const -> const std::string&
{
    return _Title;
}

auto Arcadia::WindowLayer::GetSizeState() const -> WindowSizeState
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

auto Arcadia::WindowLayer::GetSize() const -> glm::i32vec2
{
    glm::i32vec2 vec{};
    glfwGetWindowSize(_GlfwWindow, &vec.x, &vec.y);
    return vec;
}

auto Arcadia::WindowLayer::GetPosition() const -> glm::i32vec2
{
    glm::i32vec2 vec{};
    glfwGetWindowPos(_GlfwWindow, &vec.x, &vec.y);
    return vec;
}

void Arcadia::WindowLayer::_OnWindowSetCursorInputMode(Events::WindowSetCursorInputMode& e)
{
    std::int32_t val = Match<std::int32_t>(
        e.Mode,
        WindowCursorInputMode::Normal,
        GLFW_CURSOR_NORMAL,
        WindowCursorInputMode::Hidden,
        GLFW_CURSOR_HIDDEN,
        WindowCursorInputMode::Disabled,
        GLFW_CURSOR_DISABLED,
        WindowCursorInputMode::Captured,
        GLFW_CURSOR_CAPTURED
    );
    glfwSetInputMode(_GlfwWindow, GLFW_CURSOR, val);
}

void Arcadia::WindowLayer::_SetupCallbacks()
{
    // Set callbacks
    glfwSetKeyCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, int key, int scancode, int action, int mods) -> void
    {
        EventQueue::Instance()
            .Signal<Events::InputKey>(
                _GetWindowPointerFromGlfwUserPointer(glfw_wnd_ptr),
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
        glm::vec2 cursor_pos(xpos, ypos);
        WindowLayer* wnd_ptr = _GetWindowPointerFromGlfwUserPointer(glfw_wnd_ptr);
        EventQueue& event_queue = EventQueue::Instance();

        event_queue.Signal<Events::InputCursorPosition>(
            wnd_ptr,
            cursor_pos
        );

        glm::vec2& last_pos = wnd_ptr->_LastCursorPosition;
        glm::vec2 offset = cursor_pos - last_pos;
        if(IsInRange(offset.x, _LegalCursorMoveRange.x, _LegalCursorMoveRange.y)
           && IsInRange(offset.y, _LegalCursorMoveRange.x, _LegalCursorMoveRange.y))
        {
            event_queue.Signal<Events::InputCursorMove>(
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
            .Signal<Events::InputScroll>(
                _GetWindowPointerFromGlfwUserPointer(glfw_wnd_ptr),
                xoffset,
                yoffset
            );
    }
    );
    glfwSetMouseButtonCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, int button, int action, int mods) -> void
    {
        EventQueue::Instance()
            .Signal<Events::InputMouseButton>(
                _GetWindowPointerFromGlfwUserPointer(glfw_wnd_ptr),
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
            .Signal<Events::WindowSetSize>(
                _GetWindowPointerFromGlfwUserPointer(glfw_wnd_ptr),
                glm::i32vec2{ width,height }
            );
    }
    );
    glfwSetWindowPosCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, int xpos, int ypos) -> void
    {
        EventQueue::Instance()
            .Signal<Events::WindowSetPosition>(
                _GetWindowPointerFromGlfwUserPointer(glfw_wnd_ptr),
                glm::i32vec2{ xpos,ypos }
            );
    }
    );
    glfwSetWindowIconifyCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, int iconified) -> void
    {
        EventQueue& event_queue = EventQueue::Instance();
        if(iconified)
        {
            event_queue.Signal<Events::WindowSizeStateChanged>(
                _GetWindowPointerFromGlfwUserPointer(glfw_wnd_ptr),
                WindowSizeState::Minimized
            );
        }
        else
        {
            event_queue.Signal<Events::WindowSizeStateChanged>(
                _GetWindowPointerFromGlfwUserPointer(glfw_wnd_ptr),
                WindowSizeState::Restored
            );
        }
    }
    );
    glfwSetWindowMaximizeCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, int maxmized) -> void
    {
        EventQueue& event_queue = EventQueue::Instance();
        if(maxmized)
        {
            event_queue.Signal<Events::WindowSizeStateChanged>(
                _GetWindowPointerFromGlfwUserPointer(glfw_wnd_ptr),
                WindowSizeState::Maxmized
            );
        }
        else
        {
            event_queue.Signal<Events::WindowSizeStateChanged>(
                _GetWindowPointerFromGlfwUserPointer(glfw_wnd_ptr),
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
            .Signal<Events::WindowSetFocused>(
                _GetWindowPointerFromGlfwUserPointer(glfw_wnd_ptr),
                focused
            );
    }
    );
    glfwSetCursorEnterCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, int entered) -> void
    {
        EventQueue::Instance()
            .Signal<Events::InputCursorEnter>(
                _GetWindowPointerFromGlfwUserPointer(glfw_wnd_ptr),
                entered
            );
    }
    );
    glfwSetCharCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, unsigned int code_point) -> void
    {
        EventQueue::Instance()
            .Signal<Events::InputChar>(
                _GetWindowPointerFromGlfwUserPointer(glfw_wnd_ptr),
                code_point
            );
    }
    );
    glfwSetMonitorCallback(
        [](GLFWmonitor* glfw_monitor_ptr, int event) -> void
    {
        bool connection = false;
        if(event == GLFW_CONNECTED)
        {
            connection = true;
        }
        else if(event == GLFW_DISCONNECTED)
        {
            connection = false;
        }
        EventQueue::Instance()
            .Signal<Events::MonitorSetConnected>(
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
            .Signal<Events::WindowShouldClose>(
                _GetWindowPointerFromGlfwUserPointer(glfw_wnd_ptr)
            );
    }
    );
}

void Arcadia::WindowLayer::_SwapBuffers()
{
    const AppConfig& app_config = AppConfig::Instance();

    MatchVariant<void>(
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

void Arcadia::WindowLayer::_OnWindowCloseCanceled(Events::WindowCloseCanceled& e)
{
    if(e.pWindowLayer == this)
    {
        glfwSetWindowShouldClose(_GlfwWindow, GLFW_FALSE);
    }
}

auto Arcadia::WindowLayer::GetMultisampleCount() const -> std::int32_t
{
    return _MultisampleCount;
}

auto Arcadia::WindowLayer::GetCursorInputMode() const -> WindowCursorInputMode
{
    switch(glfwGetInputMode(_GlfwWindow, GLFW_CURSOR))
    {
        case GLFW_CURSOR_NORMAL:
        {
            return WindowCursorInputMode::Normal;
        }
        case GLFW_CURSOR_HIDDEN:
        {
            return WindowCursorInputMode::Hidden;
        }
        case GLFW_CURSOR_DISABLED:
        {
            return WindowCursorInputMode::Disabled;
        }
        case GLFW_CURSOR_CAPTURED:
        {
            return WindowCursorInputMode::Captured;
        }
    }
}