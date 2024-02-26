#pragma once

#include<string>

#include"core/base.hpp"

namespace arcadia
{
    struct ARCADIA_API imgui_window_interface
    {
    public:
        imgui_window_interface(
            const std::string& title,
            bool open = false
        );
        virtual ~imgui_window_interface() = default;

        virtual void on_render() = 0;

    public:
        bool open;
        std::string title;
    };

    template<class ImGuiWindow>
    concept imgui_window_like = requires{
        std::derived_from<ImGuiWindow, arcadia::imgui_window_interface>;
    };
}
