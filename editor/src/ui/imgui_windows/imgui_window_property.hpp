#pragma once

#include<memory>
#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"function/ui/imgui_window.hpp"
#include"resource/scene/scene.hpp"

#include"ui/ui_events.hpp"

namespace arcadia
{
    struct ARCADIA_API imgui_window_property: arcadia::imgui_window_interface
    {
    public:
        using self_type = imgui_window_property;
    public:
        ARCADIA_IMGUI_WINDOW_ID_STR("###property");

        using arcadia::imgui_window_interface::imgui_window_interface;
        virtual ~imgui_window_property() = default;

        virtual void on_event(arcadia::event_base& event) override;
        virtual void on_update() override;
    private:
        void _on_open_imgui_window(arcadia::event::open_imgui_window& e);

    private:
        std::weak_ptr<const arcadia::scene> _scene_wptr{};
        entt::entity _entity{ entt::null };
    };
}
