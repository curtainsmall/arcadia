#pragma once

#include<string>

#include"core/base.hpp"
#include"project/project_events.hpp"
#include"resource/scene/scene.hpp"

#include"ui/imgui_windows/imgui_window.hpp"
#include"ui/ui_events.hpp"

namespace arcadia
{
    struct ARCADIA_API imgui_window_outliner: arcadia::imgui_window_interface
    {
    public:
        using self_type = imgui_window_outliner;
    public:
        [[nodiscard]]
        static constexpr auto get_title() -> std::string
        {
            return "Outliner";
        }

        using arcadia::imgui_window_interface::imgui_window_interface;
        virtual ~imgui_window_outliner() = default;

        virtual void on_event(arcadia::event_base& event);
        virtual void on_update();


    private:
        void _on_open_window(arcadia::event::open_imgui_window& e);
        void _on_scene_built(arcadia::event::scene_built& e);
        void _on_scene_built(arcadia::event::scene_unbuilt& e);

    private:
        const arcadia::scene* _scene_cptr{};
    };
}
