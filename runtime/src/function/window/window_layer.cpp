#include "pch.hpp"
#include "window_layer.hpp"

#include"core/app/app_config.hpp"
#include"function/input/input_events.hpp"

arcadia::window_layer::window_layer(
    glm::ivec2 size,
    std::string title,
    int multisample_count
):
    arcadia::layer_interface(std::format("window_{}", title)),
    _title(title),
    _multisample_count(multisample_count)
{
    const auto& app_config = arcadia::app_config::instance();

    arcadia::match<void>(
        app_config.graphic_api,
        [&](const arcadia::graphic_api::opengl& gl) -> void
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
    _glfw_window_ptr = glfwCreateWindow(size.x, size.y, _title.c_str(), nullptr, nullptr);
    if(!_glfw_window_ptr)
    {
        const char* desr{ nullptr };
        auto err_code = glfwGetError(&desr);
        throw arcadia::glfw_error{ std::format("Failed to create GLFW window, because {}",desr) };
    }
    glfwMakeContextCurrent(_glfw_window_ptr);

    glfwGetCursorPos(_glfw_window_ptr, &_last_cursor_pos.x, &_last_cursor_pos.y);
    auto& min = app_config.window_min_size;
    auto& max = app_config.window_max_size;
    glfwSetWindowSizeLimits(
        _glfw_window_ptr,
        min.x < 0 ? GLFW_DONT_CARE : min.x,
        min.y < 0 ? GLFW_DONT_CARE : min.y,
        max.x < 0 ? GLFW_DONT_CARE : max.x,
        max.y < 0 ? GLFW_DONT_CARE : max.y
    );
    if(app_config.window_maxmized)
    {
        set_size_state(arcadia::window_size_state::maxmized);
    }
    else
    {
        set_pos(app_config.window_pos);
    }

    glfwSetWindowUserPointer(_glfw_window_ptr, this);
    _setup_callbacks();

    set_visible(true);
}

arcadia::window_layer::~window_layer()
{
    if(_glfw_window_ptr)
    {
        glfwDestroyWindow(_glfw_window_ptr);
    }
}

void arcadia::window_layer::on_event(arcadia::event_base& event)
{}

void arcadia::window_layer::on_update()
{
    _swap_buffers();

    glfwPollEvents();
}


auto arcadia::window_layer::get_title() const -> const std::string&
{
    return _title;
}

auto arcadia::window_layer::set_title(const std::string& title) -> self_type&
{
    _title = title;
    glfwSetWindowTitle(_glfw_window_ptr, _title.c_str());
    return *this;
}

auto arcadia::window_layer::get_size() const -> glm::ivec2
{
    glm::ivec2 size{};
    glfwGetWindowSize(_glfw_window_ptr, &size.x, &size.y);
    return size;
}

auto arcadia::window_layer::set_size(const glm::ivec2& size) -> self_type&
{
    glfwSetWindowSize(_glfw_window_ptr, size.x, size.y);
    return *this;
}

auto arcadia::window_layer::get_size_state() const -> arcadia::window_size_state
{
    if(glfwGetWindowAttrib(_glfw_window_ptr, GLFW_MAXIMIZED))
    {
        return arcadia::window_size_state::maxmized;
    }
    else if(glfwGetWindowAttrib(_glfw_window_ptr, GLFW_ICONIFIED))
    {
        return arcadia::window_size_state::minimized;
    }
    else
    {
        return arcadia::window_size_state::restored;
    }
}

auto arcadia::window_layer::set_size_state(arcadia::window_size_state state) -> self_type&
{
    switch(state)
    {
        case arcadia::window_size_state::minimized:
            glfwIconifyWindow(_glfw_window_ptr);
            break;
        case arcadia::window_size_state::restored:
            glfwRestoreWindow(_glfw_window_ptr);
            break;
        case arcadia::window_size_state::maxmized:
            glfwMaximizeWindow(_glfw_window_ptr);
            break;
    }
    return *this;
}

auto arcadia::window_layer::get_pos() const -> glm::ivec2
{
    glm::ivec2 pos{};
    glfwGetWindowPos(_glfw_window_ptr, &pos.x, &pos.y);
    return pos;
}

auto arcadia::window_layer::set_pos(const glm::ivec2& pos) -> self_type&
{
    if(pos.x < 0 || pos.y < 0)
    {
        return *this;
    }

    glfwSetWindowPos(_glfw_window_ptr, pos.x, pos.y);
    return *this;
}

auto arcadia::window_layer::get_swap_interval() const -> int
{
    return _swap_interval;
}

auto arcadia::window_layer::set_swap_interval(int interval) -> self_type&
{
    _swap_interval = interval;
    glfwSwapInterval(interval);
    return *this;
}

auto arcadia::window_layer::get_visible() const -> bool
{
    return glfwGetWindowAttrib(_glfw_window_ptr, GLFW_VISIBLE);
}

auto arcadia::window_layer::set_visible(bool visible) -> self_type&
{
    if(visible)
    {
        glfwShowWindow(_glfw_window_ptr);
    }
    else
    {
        glfwHideWindow(_glfw_window_ptr);
    }
    return *this;
}

auto arcadia::window_layer::get_input_mode_cursor() const -> int
{
    return glfwGetInputMode(_glfw_window_ptr, GLFW_CURSOR);
}

auto arcadia::window_layer::set_input_mode_cursor(int value) -> self_type&
{
    glfwSetInputMode(_glfw_window_ptr, GLFW_CURSOR, value);
    return *this;
}

auto arcadia::window_layer::get_input_mode_sticky_keys() const -> int
{
    return glfwGetInputMode(_glfw_window_ptr, GLFW_STICKY_KEYS);
}

auto arcadia::window_layer::set_input_mode_sticky_keys(int value) -> self_type&
{
    glfwSetInputMode(_glfw_window_ptr, GLFW_STICKY_KEYS, value);
    return *this;
}

auto arcadia::window_layer::get_input_mode_sticky_mouse_buttons() const -> int
{
    return glfwGetInputMode(_glfw_window_ptr, GLFW_STICKY_MOUSE_BUTTONS);
}

auto arcadia::window_layer::set_input_mode_sticky_mouse_buttons(int value) -> self_type&
{
    glfwSetInputMode(_glfw_window_ptr, GLFW_STICKY_MOUSE_BUTTONS, value);
    return *this;
}

auto arcadia::window_layer::get_input_mode_lock_key_mods() const -> int
{
    return glfwGetInputMode(_glfw_window_ptr, GLFW_LOCK_KEY_MODS);
}

auto arcadia::window_layer::set_input_mode_lock_key_mods(int value) -> self_type&
{
    glfwSetInputMode(_glfw_window_ptr, GLFW_LOCK_KEY_MODS, value);
    return *this;
}

auto arcadia::window_layer::get_input_mode_raw_mouse_motion() const -> int
{
    return glfwGetInputMode(_glfw_window_ptr, GLFW_RAW_MOUSE_MOTION);
}

auto arcadia::window_layer::set_input_mode_raw_mouse_motion(int value) -> self_type&
{
    glfwSetInputMode(_glfw_window_ptr, GLFW_RAW_MOUSE_MOTION, value);
    return *this;
}

void arcadia::window_layer::_setup_callbacks()
{
    // Set callbacks
    glfwSetKeyCallback(
        _glfw_window_ptr,
        [](GLFWwindow* glfw_wnd_ptr, int key, int scancode, int action, int mods) -> void
    {
        arcadia::event_queue::instance()
            .signal<arcadia::event::input_key>(
                _get_window_ptr_from_glfw_user_pointer(glfw_wnd_ptr),
                key,
                scancode,
                action,
                mods
            );
    }
    );
    glfwSetCursorPosCallback(
        _glfw_window_ptr,
        [](GLFWwindow* glfw_wnd_ptr, double xpos, double ypos) -> void
    {
        glm::dvec2 cursor_pos{ xpos,ypos };
        auto wnd_ptr = _get_window_ptr_from_glfw_user_pointer(glfw_wnd_ptr);
        auto& event_queue = arcadia::event_queue::instance();

        event_queue.signal<arcadia::event::input_cursor_pos>(
            wnd_ptr,
            cursor_pos
        );
        auto& last_pos = wnd_ptr->_last_cursor_pos;
        if(cursor_pos != last_pos)
        {

            event_queue.signal<arcadia::event::input_cursor_move>(
                wnd_ptr,
                cursor_pos - last_pos
            );
            last_pos = cursor_pos;
        }
    }
    );
    glfwSetScrollCallback(
        _glfw_window_ptr,
        [](GLFWwindow* glfw_wnd_ptr, double xoffset, double yoffset) -> void
    {
        arcadia::event_queue::instance()
            .signal<arcadia::event::input_scroll>(
                _get_window_ptr_from_glfw_user_pointer(glfw_wnd_ptr),
                glm::dvec2{ xoffset,yoffset }
        );
    }
    );
    glfwSetMouseButtonCallback(
        _glfw_window_ptr,
        [](GLFWwindow* glfw_wnd_ptr, int button, int action, int mods) -> void
    {
        arcadia::event_queue::instance()
            .signal<arcadia::event::input_mouse_button>(
                _get_window_ptr_from_glfw_user_pointer(glfw_wnd_ptr),
                button,
                action,
                mods
            );
    }
    );
    glfwSetWindowSizeCallback(
        _glfw_window_ptr,
        [](GLFWwindow* glfw_wnd_ptr, int width, int height) -> void
    {
        arcadia::event_queue::instance()
            .signal<arcadia::event::window_size>(
                _get_window_ptr_from_glfw_user_pointer(glfw_wnd_ptr),
                glm::ivec2{ width,height }
        );
    }
    );
    glfwSetWindowPosCallback(
        _glfw_window_ptr,
        [](GLFWwindow* glfw_wnd_ptr, int xpos, int ypos) -> void
    {
        arcadia::event_queue::instance()
            .signal<arcadia::event::window_pos>(
                _get_window_ptr_from_glfw_user_pointer(glfw_wnd_ptr),
                glm::ivec2{ xpos,ypos }
        );
    }
    );
    glfwSetWindowIconifyCallback(
        _glfw_window_ptr,
        [](GLFWwindow* glfw_wnd_ptr, int iconified) -> void
    {
        auto& event_queue = arcadia::event_queue::instance();
        if(iconified)
        {
            event_queue.signal<arcadia::event::window_size_state>(
                _get_window_ptr_from_glfw_user_pointer(glfw_wnd_ptr),
                arcadia::window_size_state::minimized
            );
        }
        else
        {
            event_queue.signal<arcadia::event::window_size_state>(
                _get_window_ptr_from_glfw_user_pointer(glfw_wnd_ptr),
                arcadia::window_size_state::restored
            );
        }
    }
    );
    glfwSetWindowMaximizeCallback(
        _glfw_window_ptr,
        [](GLFWwindow* glfw_wnd_ptr, int maxmized) -> void
    {
        auto& event_queue = arcadia::event_queue::instance();
        if(maxmized)
        {
            event_queue.signal<arcadia::event::window_size_state>(
                _get_window_ptr_from_glfw_user_pointer(glfw_wnd_ptr),
                arcadia::window_size_state::maxmized
            );
        }
        else
        {
            event_queue.signal<arcadia::event::window_size_state>(
                _get_window_ptr_from_glfw_user_pointer(glfw_wnd_ptr),
                arcadia::window_size_state::restored
            );
        }
    }
    );
    glfwSetWindowFocusCallback(
        _glfw_window_ptr,
        [](GLFWwindow* glfw_wnd_ptr, int focused) -> void
    {
        arcadia::event_queue::instance()
            .signal<arcadia::event::window_focus>(
                _get_window_ptr_from_glfw_user_pointer(glfw_wnd_ptr),
                focused
            );
    }
    );
    glfwSetCursorEnterCallback(
        _glfw_window_ptr,
        [](GLFWwindow* glfw_wnd_ptr, int entered) -> void
    {
        arcadia::event_queue::instance()
            .signal<arcadia::event::input_cursor_enter>(
                _get_window_ptr_from_glfw_user_pointer(glfw_wnd_ptr),
                entered
            );
    }
    );
    glfwSetCharCallback(
        _glfw_window_ptr,
        [](GLFWwindow* glfw_wnd_ptr, unsigned int code_point) -> void
    {
        arcadia::event_queue::instance()
            .signal<arcadia::event::input_char>(
                _get_window_ptr_from_glfw_user_pointer(glfw_wnd_ptr),
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
        arcadia::event_queue::instance()
            .signal<arcadia::event::monitor_connection>(
                glfw_monitor_ptr,
                connection
            );
    }
    );
    glfwSetWindowCloseCallback(
        _glfw_window_ptr,
        [](GLFWwindow* glfw_wnd_ptr) -> void
    {
        arcadia::event_queue::instance()
            .signal<arcadia::event::window_should_close>(
                _get_window_ptr_from_glfw_user_pointer(glfw_wnd_ptr)
            );
    }
    );
}

void arcadia::window_layer::_swap_buffers()
{
    const auto& app_config = arcadia::app_config::instance();

    arcadia::match<void>(
        app_config.graphic_api,
        [&](const arcadia::graphic_api::opengl&) -> void
    {
        glfwSwapBuffers(_glfw_window_ptr);
    },
        [](auto&&) -> void
    {
    }
    );
}

void arcadia::window_layer::_on_window_close_canceled(arcadia::event::window_close_canceled& e)
{
    const auto& [wnd_ptr] = e.data_tuple;
    if(wnd_ptr == this)
    {
        glfwSetWindowShouldClose(_glfw_window_ptr, GLFW_FALSE);
    }
}

auto arcadia::window_layer::get_multisample_count() const -> int
{
    return _multisample_count;
}
