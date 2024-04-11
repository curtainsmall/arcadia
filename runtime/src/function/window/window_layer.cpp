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
    _pGlfwWindow = glfwCreateWindow(size.x, size.y, _Title.c_str(), nullptr, nullptr);
    if(!_pGlfwWindow)
    {
        const char* desr{ nullptr };
        auto err_code = glfwGetError(&desr);
        throw Arcadia::GlfwError{ std::format("Failed to create GLFW window, because {}",desr) };
    }
    glfwMakeContextCurrent(_pGlfwWindow);

    double x = _LastCursorPos.x,
        y = _LastCursorPos.y;
    glfwGetCursorPos(_pGlfwWindow, &x, &y);
    _LastCursorPos.x = static_cast<float>(x);
    _LastCursorPos.y = static_cast<float>(y);
    auto& min = app_config.WindowMinSize;
    auto& max = app_config.WindowMaxSize;
    glfwSetWindowSizeLimits(
        _pGlfwWindow,
        min.x < 0 ? GLFW_DONT_CARE : min.x,
        min.y < 0 ? GLFW_DONT_CARE : min.y,
        max.x < 0 ? GLFW_DONT_CARE : max.x,
        max.y < 0 ? GLFW_DONT_CARE : max.y
    );
    if(app_config.WindowMaxmized)
    {
        SetSizeState(Arcadia::WindowSizeState::Maxmized);
    }
    else
    {
        SetPos(app_config.WindowPos);
    }

    glfwSetWindowUserPointer(_pGlfwWindow, this);
    _SetupCallbacks();

    SetVisible(true);
}

Arcadia::WindowLayer::~WindowLayer()
{
    if(_pGlfwWindow)
    {
        glfwDestroyWindow(_pGlfwWindow);
    }
}

void Arcadia::WindowLayer::OnEvent(Arcadia::EventBase& event)
{}

void Arcadia::WindowLayer::OnUpdate()
{
    _SwapBuffers();

    glfwPollEvents();
}


auto Arcadia::WindowLayer::GetTitle() const -> const std::string&
{
    return _Title;
}

auto Arcadia::WindowLayer::SetTitle(const std::string& title) -> self_type&
{
    _Title = title;
    glfwSetWindowTitle(_pGlfwWindow, _Title.c_str());
    return *this;
}

auto Arcadia::WindowLayer::GetSize() const -> glm::ivec2
{
    glm::ivec2 size{};
    glfwGetWindowSize(_pGlfwWindow, &size.x, &size.y);
    return size;
}

auto Arcadia::WindowLayer::SetSize(const glm::ivec2& size) -> self_type&
{
    glfwSetWindowSize(_pGlfwWindow, size.x, size.y);
    return *this;
}

auto Arcadia::WindowLayer::GetSizeState() const -> Arcadia::WindowSizeState
{
    if(glfwGetWindowAttrib(_pGlfwWindow, GLFW_MAXIMIZED))
    {
        return Arcadia::WindowSizeState::Maxmized;
    }
    else if(glfwGetWindowAttrib(_pGlfwWindow, GLFW_ICONIFIED))
    {
        return Arcadia::WindowSizeState::Minimized;
    }
    else
    {
        return Arcadia::WindowSizeState::Restored;
    }
}

auto Arcadia::WindowLayer::SetSizeState(Arcadia::WindowSizeState state) -> self_type&
{
    switch(state)
    {
        case Arcadia::WindowSizeState::Minimized:
            glfwIconifyWindow(_pGlfwWindow);
            break;
        case Arcadia::WindowSizeState::Restored:
            glfwRestoreWindow(_pGlfwWindow);
            break;
        case Arcadia::WindowSizeState::Maxmized:
            glfwMaximizeWindow(_pGlfwWindow);
            break;
    }
    return *this;
}

auto Arcadia::WindowLayer::GetPos() const -> glm::ivec2
{
    glm::ivec2 pos{};
    glfwGetWindowPos(_pGlfwWindow, &pos.x, &pos.y);
    return pos;
}

auto Arcadia::WindowLayer::SetPos(const glm::ivec2& pos) -> self_type&
{
    if(pos.x < 0 || pos.y < 0)
    {
        return *this;
    }

    glfwSetWindowPos(_pGlfwWindow, pos.x, pos.y);
    return *this;
}

auto Arcadia::WindowLayer::GetSwapInterval() const -> int
{
    return _SwapInterval;
}

auto Arcadia::WindowLayer::SetSwapInterval(int interval) -> self_type&
{
    _SwapInterval = interval;
    glfwSwapInterval(interval);
    return *this;
}

auto Arcadia::WindowLayer::GetVisible() const -> bool
{
    return glfwGetWindowAttrib(_pGlfwWindow, GLFW_VISIBLE);
}

auto Arcadia::WindowLayer::SetVisible(bool visible) -> self_type&
{
    if(visible)
    {
        glfwShowWindow(_pGlfwWindow);
    }
    else
    {
        glfwHideWindow(_pGlfwWindow);
    }
    return *this;
}

auto Arcadia::WindowLayer::GetInputModeCursor() const -> int
{
    return glfwGetInputMode(_pGlfwWindow, GLFW_CURSOR);
}

auto Arcadia::WindowLayer::SetInputModeCursor(int Value) -> self_type&
{
    glfwSetInputMode(_pGlfwWindow, GLFW_CURSOR, Value);
    return *this;
}

auto Arcadia::WindowLayer::GetInputModeStickyKeys() const -> int
{
    return glfwGetInputMode(_pGlfwWindow, GLFW_STICKY_KEYS);
}

auto Arcadia::WindowLayer::SetInputModeStickyKeys(int Value) -> self_type&
{
    glfwSetInputMode(_pGlfwWindow, GLFW_STICKY_KEYS, Value);
    return *this;
}

auto Arcadia::WindowLayer::GetInputModeStickyMouseButtons() const -> int
{
    return glfwGetInputMode(_pGlfwWindow, GLFW_STICKY_MOUSE_BUTTONS);
}

auto Arcadia::WindowLayer::SetInputModeStickMouseButtons(int Value) -> self_type&
{
    glfwSetInputMode(_pGlfwWindow, GLFW_STICKY_MOUSE_BUTTONS, Value);
    return *this;
}

auto Arcadia::WindowLayer::GetInputModeLockKeyMods() const -> int
{
    return glfwGetInputMode(_pGlfwWindow, GLFW_LOCK_KEY_MODS);
}

auto Arcadia::WindowLayer::SetInputModeLockKeyMods(int Value) -> self_type&
{
    glfwSetInputMode(_pGlfwWindow, GLFW_LOCK_KEY_MODS, Value);
    return *this;
}

auto Arcadia::WindowLayer::GetInputModeRawMouseMotion() const -> int
{
    return glfwGetInputMode(_pGlfwWindow, GLFW_RAW_MOUSE_MOTION);
}

auto Arcadia::WindowLayer::SetInputModeRawMouseMotion(int Value) -> self_type&
{
    glfwSetInputMode(_pGlfwWindow, GLFW_RAW_MOUSE_MOTION, Value);
    return *this;
}

void Arcadia::WindowLayer::_SetupCallbacks()
{
    // Set callbacks
    glfwSetKeyCallback(
        _pGlfwWindow,
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
        _pGlfwWindow,
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
        if(cursor_pos != last_pos)
        {

            event_queue.Signal<Arcadia::Event::InputCursorMove>(
                wnd_ptr,
                cursor_pos - last_pos
            );
            last_pos = cursor_pos;
        }
    }
    );
    glfwSetScrollCallback(
        _pGlfwWindow,
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
        _pGlfwWindow,
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
        _pGlfwWindow,
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
        _pGlfwWindow,
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
        _pGlfwWindow,
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
        _pGlfwWindow,
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
        _pGlfwWindow,
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
        _pGlfwWindow,
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
        _pGlfwWindow,
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
        _pGlfwWindow,
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
        glfwSwapBuffers(_pGlfwWindow);
    },
        [](auto&&) -> void
    {
    }
    );
}

void Arcadia::WindowLayer::_OnWindowCloseCanceled(Arcadia::Event::WindowCloseCanceled& e)
{
    const auto& [wnd_ptr] = e.data_tuple;
    if(wnd_ptr == this)
    {
        glfwSetWindowShouldClose(_pGlfwWindow, GLFW_FALSE);
    }
}

auto Arcadia::WindowLayer::GetMultisampleCount() const -> int
{
    return _MultisampleCount;
}
