#pragma once

#include<functional>
#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"

#define ARCADIA_IMGUI_WINDOW_ID_STR_GETTERS(IdStr) \
[[nodiscard]]\
static constexpr auto get_id_str_static() -> std::string\
{\
    return IdStr;\
}\
[[nodiscard]]\
virtual inline auto get_id_str() const -> std::string override\
{\
    return get_id_str_static();\
}

namespace arcadia
{
    struct ARCADIA_API imgui_window_interface
    {
    public:
        using self_type = imgui_window_interface;
    public:
        inline imgui_window_interface(
            bool open = false,
            const std::string& title={}
        ):
            _open(open),
            _title(title)
        {}
        virtual ~imgui_window_interface() = default;

        [[nodiscard]]
        auto is_open() const -> bool
        {
            return _open;
        }

        [[nodiscard]]
        auto get_title() const -> const std::string&
        {
            return _title;
        }

        virtual void on_event(arcadia::event_base& event)
        {}
        virtual void on_update() = 0;
        virtual auto get_id_str() const->std::string = 0;

    protected:
        bool _open;
        std::string _title{};
    };

    template<class ImGuiWindow>
    concept imgui_window_like = requires{
        std::derived_from<ImGuiWindow, arcadia::imgui_window_interface>;
        {
            ImGuiWindow::get_id_str_static()
        } -> std::same_as<std::string>;
    };

}
