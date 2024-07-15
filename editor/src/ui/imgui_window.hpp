#pragma once

#include<functional>
#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"

// ImGui window ID should follow: Title###id so that we can change the title for a curtain window
#define ACDA_IMGUI_WINDOW_ID_STR_GETTERS(id_str) \
[[nodiscard]]\
static constexpr auto get_id_str_static() -> std::string\
{\
    return id_str;\
}\
[[nodiscard]]\
virtual auto get_id_str() const -> std::string override\
{\
    return get_id_str_static();\
}

struct iImguiWindow
{
public:
    using self_type = iImguiWindow;
public:
    iImguiWindow(
        bool open = false,
        const std::string& title={}
    ):
        _open(open),
        _title(title)
    {}
    virtual ~iImguiWindow() = default;

    [[nodiscard]]
    auto open() const -> bool
    {
        return _open;
    }

    [[nodiscard]]
    auto get_title() const -> const std::string&
    {
        return _title;
    }

    virtual void on_event(EventBase& e)
    {}
    virtual void on_update() = 0;
    virtual auto get_id_str() const->std::string = 0;

protected:
    bool _open;
    std::string _title{};
};

template<class ImGuiWindow>
concept cImguiWindow = requires{
    std::derived_from<ImGuiWindow, iImguiWindow>;
    {
        ImGuiWindow::get_id_str_static()
    } -> std::same_as<std::string>;
};

