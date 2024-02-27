#pragma once

#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"

namespace arcadia
{
    struct ARCADIA_API imgui_window_interface
    {
    public:
        inline imgui_window_interface(
            const std::string& title,
            bool open = false
        ):
            _title(title),
            _open(open)
        {}
        virtual ~imgui_window_interface() = default;

        virtual void on_event(const arcadia::event_base& event)
        {}
        virtual void on_update() = 0;

    protected:
        std::string _title;
        bool _open;
    };

    template<class ImGuiWindow>
    concept imgui_window_like = requires{
        std::derived_from<ImGuiWindow, arcadia::imgui_window_interface>;
    };


}
