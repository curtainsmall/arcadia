#include "window_layer.hpp"

#include "core/runtime_config.hpp"
#include "core/function.hpp"
#include "core/match.hpp"

Arcadia::WindowLayer::WindowLayer(
    glm::i32vec2 size,
    std::string_view title,
    std::int32_t multisample_count
):
    LayerInterface(std::format("window_{}", title)),
    _Title(title),
    _MultisampleCount(multisample_count)
{
    const RuntimeConfig& app_config = RuntimeConfig::Instance();

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

auto Arcadia::WindowLayer::GetGlfwWindow() const -> GLFWwindow*
{
    return _GlfwWindow;
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

auto Arcadia::WindowLayer::GetTitle() const -> std::string_view
{
    return _Title;
}

auto Arcadia::WindowLayer::GetSizeState() const -> GlfwWindowSizeState
{
    if(glfwGetWindowAttrib(_GlfwWindow, GLFW_MAXIMIZED))
    {
        return GlfwWindowSizeState::Maxmized;
    }
    else if(glfwGetWindowAttrib(_GlfwWindow, GLFW_ICONIFIED))
    {
        return GlfwWindowSizeState::Minimized;
    }
    else
    {
        return GlfwWindowSizeState::Restored;
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

auto Arcadia::WindowLayer::_GetWindowPointerFromGlfwUserPointer(GLFWwindow* glfw_window) -> SelfType*
{
    return static_cast<SelfType*>(glfwGetWindowUserPointer(glfw_window));
}

void Arcadia::WindowLayer::_OnWindowSetCursorInputMode(Events::WindowSetCursorInputMode& e)
{
    std::int32_t val = Match<std::int32_t>(
        e.Mode,
        GlfwWindowCursorInputMode::Normal,
        GLFW_CURSOR_NORMAL,
        GlfwWindowCursorInputMode::Hidden,
        GLFW_CURSOR_HIDDEN,
        GlfwWindowCursorInputMode::Disabled,
        GLFW_CURSOR_DISABLED,
        GlfwWindowCursorInputMode::Captured,
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
                    GlfwInputKey(key),
                    scancode,
                    GlfwInputAction(action),
                    GlfwInputModifier(mods)
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
                cursor_pos
            );

            glm::vec2& last_pos = wnd_ptr->_LastCursorPosition;
            glm::vec2 offset = cursor_pos - last_pos;
            if(IsInRange(offset.x, _LegalCursorMoveRange.x, _LegalCursorMoveRange.y)
               && IsInRange(offset.y, _LegalCursorMoveRange.x, _LegalCursorMoveRange.y))
            {
                event_queue.Signal<Events::InputCursorMove>(
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
                    GlfwInputMouseButton(button),
                    GlfwInputAction(action),
                    GlfwInputModifier(mods)
                );
        }
    );
    glfwSetWindowSizeCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, int width, int height) -> void
        {
            EventQueue::Instance()
                .Signal<Events::WindowSetSize>(
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
                    GlfwWindowSizeState::Minimized
                );
            }
            else
            {
                event_queue.Signal<Events::WindowSizeStateChanged>(
                    GlfwWindowSizeState::Restored
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
                    GlfwWindowSizeState::Maxmized
                );
            }
            else
            {
                event_queue.Signal<Events::WindowSizeStateChanged>(
                    GlfwWindowSizeState::Restored
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
                .Signal<Events::WindowShouldClose>();
        }
    );
}

void Arcadia::WindowLayer::_SwapBuffers()
{
    const RuntimeConfig& app_config = RuntimeConfig::Instance();

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
    glfwSetWindowShouldClose(_GlfwWindow, GLFW_FALSE);
}

auto Arcadia::WindowLayer::GetMultisampleCount() const -> std::int32_t
{
    return _MultisampleCount;
}

auto Arcadia::WindowLayer::GetCursorInputMode() const -> GlfwWindowCursorInputMode
{
    switch(glfwGetInputMode(_GlfwWindow, GLFW_CURSOR))
    {
        case GLFW_CURSOR_NORMAL:
        {
            return GlfwWindowCursorInputMode::Normal;
        }
        case GLFW_CURSOR_HIDDEN:
        {
            return GlfwWindowCursorInputMode::Hidden;
        }
        case GLFW_CURSOR_DISABLED:
        {
            return GlfwWindowCursorInputMode::Disabled;
        }
        case GLFW_CURSOR_CAPTURED:
        {
            return GlfwWindowCursorInputMode::Captured;
        }
    }
}