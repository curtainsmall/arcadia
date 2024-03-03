#pragma once

#include<string>
#include<typeinfo>

#include"core/base.hpp"
#include"core/event/event.hpp"

#define ARCADIA_IMGUI_WINDOW(Name) \
struct ARCADIA_API Name: arcadia::imgui_window_interface<Name>

namespace arcadia
{

    struct ARCADIA_API imgui_window_interface
    {
    public:
        using self_type = imgui_window_interface;
    public:

        inline imgui_window_interface(
            bool open = false
        ):
            _open(open)
        {}
        virtual ~imgui_window_interface() = default;

        virtual void on_event(arcadia::event_base& event)
        {}
        virtual void on_update() = 0;
        virtual auto get_title() const->std::string = 0;

    protected:
        bool _open;
    };

    template<class ImGuiWindow>
    concept imgui_window_like = requires{
        std::derived_from<ImGuiWindow, arcadia::imgui_window_interface>;
    };

    template<class ImGuiWindow>
    struct ARCADIA_API basic_imgui_window: arcadia::imgui_window_interface
    {
    public:
        using imgui_window_type = ImGuiWindow;
        using self_type = basic_imgui_window<imgui_window_type>;
    public:
        static constexpr auto get_type_info() -> const std::type_info&
        {
            return typeid(imgui_window_type);
        }
    };


}
