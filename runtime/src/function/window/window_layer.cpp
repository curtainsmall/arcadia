#include "pch.hpp"
#include "window_layer.hpp"

#include"core/app/app_config.hpp"
#include"function/input/input_events.hpp"

Arcadia::WindowLayer::WindowLayer(
    glm::ivec2 size,
    std::string title,
    int multisample_count
):
    Arcadia::iLayer(std::format("window_{}", title)),
    _Title(title),
    _MultisampleCount(multisample_count)
{
    const auto& app_config = Arcadia::AppConfig::Instance();

    Arcadia::Match<void>(
        app_config.GraphicApi,
        [&](const Arcadia::GraphicApi::Opengl& gl) -> void
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
        throw Arcadia::GlfwError{ std::format("Failed to create GLFW window, because {}",desr) };
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

Arcadia::WindowLayer::~WindowLayer()
{
    if(_GlfwWindow)
    {
        glfwDestroyWindow(_GlfwWindow);
    }
}

void Arcadia::WindowLayer::OnEvent(Arcadia::EventBase& event)
{
    Arcadia::EventDispatcher{ event }
        .Dispatch<Arcadia::Event::WindowSetInputModeCursor>(ARCADIA_BIND_MEMBER_FN(_OnWindowSetInputModeCursor))
        .Result();
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

auto Arcadia::WindowLayer::GetSizeState() const -> Arcadia::WindowSizeState
{
    if(glfwGetWindowAttrib(_GlfwWindow, GLFW_MAXIMIZED))
    {
        return Arcadia::WindowSizeState::Maxmized;
    }
    else if(glfwGetWindowAttrib(_GlfwWindow, GLFW_ICONIFIED))
    {
        return Arcadia::WindowSizeState::Minimized;
    }
    else
    {
        return Arcadia::WindowSizeState::Restored;
    }
}

auto Arcadia::WindowLayer::GetSize() const -> glm::ivec2
{
    glm::ivec2 vec{};
    glfwGetWindowSize(_GlfwWindow, &vec.x, &vec.y);
    return vec;
}

auto Arcadia::WindowLayer::GetPos() const -> glm::ivec2
{
    glm::ivec2 vec{};
    glfwGetWindowPos(_GlfwWindow, &vec.x, &vec.y);
    return vec;
}

void Arcadia::WindowLayer::_OnWindowSetInputModeCursor(Arcadia::Event::WindowSetInputModeCursor& e)
{
    auto& [value] = e.data_tuple;
    int val = Arcadia::Match<int>(
        value,
        Arcadia::WindowInputModeCursor::Normal,
        GLFW_CURSOR_NORMAL,
        Arcadia::WindowInputModeCursor::Hidden,
        GLFW_CURSOR_HIDDEN,
        Arcadia::WindowInputModeCursor::Disabled,
        GLFW_CURSOR_DISABLED,
        Arcadia::WindowInputModeCursor::Captured,
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
        Arcadia::EventQueue::Instance()
            .Signal<Arcadia::Event::InputKey>(
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
        auto& event_queue = Arcadia::EventQueue::Instance();

        event_queue.Signal<Arcadia::Event::InputCursorPos>(
            wnd_ptr,
            cursor_pos
        );

        auto& last_pos = wnd_ptr->_LastCursorPos;
        auto offset = cursor_pos - last_pos;
        if(Arcadia::IsInRange(offset.x, _LegalCursorMoveRange.x, _LegalCursorMoveRange.y)
           && Arcadia::IsInRange(offset.y, _LegalCursorMoveRange.x, _LegalCursorMoveRange.y))
        {
            event_queue.Signal<Arcadia::Event::InputCursorMove>(
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
        Arcadia::EventQueue::Instance()
            .Signal<Arcadia::Event::InputScroll>(
                _GetWindowPtrFromGlfwUserPtr(glfw_wnd_ptr),
                glm::vec2{ xoffset,yoffset }
        );
    }
    );
    glfwSetMouseButtonCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, int button, int action, int mods) -> void
    {
        Arcadia::EventQueue::Instance()
            .Signal<Arcadia::Event::InputMouseButton>(
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
        Arcadia::EventQueue::Instance()
            .Signal<Arcadia::Event::WindowSize>(
                _GetWindowPtrFromGlfwUserPtr(glfw_wnd_ptr),
                glm::ivec2{ width,height }
        );
    }
    );
    glfwSetWindowPosCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, int xpos, int ypos) -> void
    {
        Arcadia::EventQueue::Instance()
            .Signal<Arcadia::Event::WindowPos>(
                _GetWindowPtrFromGlfwUserPtr(glfw_wnd_ptr),
                glm::ivec2{ xpos,ypos }
        );
    }
    );
    glfwSetWindowIconifyCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, int iconified) -> void
    {
        auto& event_queue = Arcadia::EventQueue::Instance();
        if(iconified)
        {
            event_queue.Signal<Arcadia::Event::WindowSizeState>(
                _GetWindowPtrFromGlfwUserPtr(glfw_wnd_ptr),
                Arcadia::WindowSizeState::Minimized
            );
        }
        else
        {
            event_queue.Signal<Arcadia::Event::WindowSizeState>(
                _GetWindowPtrFromGlfwUserPtr(glfw_wnd_ptr),
                Arcadia::WindowSizeState::Restored
            );
        }
    }
    );
    glfwSetWindowMaximizeCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, int maxmized) -> void
    {
        auto& event_queue = Arcadia::EventQueue::Instance();
        if(maxmized)
        {
            event_queue.Signal<Arcadia::Event::WindowSizeState>(
                _GetWindowPtrFromGlfwUserPtr(glfw_wnd_ptr),
                Arcadia::WindowSizeState::Maxmized
            );
        }
        else
        {
            event_queue.Signal<Arcadia::Event::WindowSizeState>(
                _GetWindowPtrFromGlfwUserPtr(glfw_wnd_ptr),
                Arcadia::WindowSizeState::Restored
            );
        }
    }
    );
    glfwSetWindowFocusCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, int focused) -> void
    {
        Arcadia::EventQueue::Instance()
            .Signal<Arcadia::Event::WindowFocus>(
                _GetWindowPtrFromGlfwUserPtr(glfw_wnd_ptr),
                focused
            );
    }
    );
    glfwSetCursorEnterCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, int entered) -> void
    {
        Arcadia::EventQueue::Instance()
            .Signal<Arcadia::Event::InputCursorEnter>(
                _GetWindowPtrFromGlfwUserPtr(glfw_wnd_ptr),
                entered
            );
    }
    );
    glfwSetCharCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr, unsigned int code_point) -> void
    {
        Arcadia::EventQueue::Instance()
            .Signal<Arcadia::Event::InputChar>(
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
        Arcadia::EventQueue::Instance()
            .Signal<Arcadia::Event::MonitorConnection>(
                glfw_monitor_ptr,
                connection
            );
    }
    );
    glfwSetWindowCloseCallback(
        _GlfwWindow,
        [](GLFWwindow* glfw_wnd_ptr) -> void
    {
        Arcadia::EventQueue::Instance()
            .Signal<Arcadia::Event::WindowShouldClose>(
                _GetWindowPtrFromGlfwUserPtr(glfw_wnd_ptr)
            );
    }
    );
}

void Arcadia::WindowLayer::_SwapBuffers()
{
    const auto& app_config = Arcadia::AppConfig::Instance();

    Arcadia::Match<void>(
        app_config.GraphicApi,
        [&](const Arcadia::GraphicApi::Opengl&) -> void
    {
        glfwSwapBuffers(_GlfwWindow);
    },
        [](auto&&) -> void
    {
    }
    );
}

void Arcadia::WindowLayer::_OnWindowCloseCanceled(Arcadia::Event::WindowCloseCanceled& e)
{
    const auto& [p_wnd] = e.data_tuple;
    if(p_wnd == this)
    {
        glfwSetWindowShouldClose(_GlfwWindow, GLFW_FALSE);
    }
}

auto Arcadia::WindowLayer::GetMultisampleCount() const -> int
{
    return _MultisampleCount;
}

auto Arcadia::WindowLayer::GetInputModeCursor() const -> Arcadia::WindowInputModeCursor
{
    switch(glfwGetInputMode(_GlfwWindow, GLFW_CURSOR))
    {
        case GLFW_CURSOR_NORMAL:
        {
            return Arcadia::WindowInputModeCursor::Normal;
        }
        case GLFW_CURSOR_HIDDEN:
        {
            return Arcadia::WindowInputModeCursor::Hidden;
        }
        case GLFW_CURSOR_DISABLED:
        {
            return Arcadia::WindowInputModeCursor::Disabled;
        }
        case GLFW_CURSOR_CAPTURED:
        {
            return Arcadia::WindowInputModeCursor::Captured;
        }
        default:
            ARCADIA_ASSERT(false);
    }
}
