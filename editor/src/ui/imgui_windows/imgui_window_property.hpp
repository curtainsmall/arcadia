#pragma once

#include<memory>
#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"function/ui/imgui_window.hpp"
#include"resource/scene/scene.hpp"

#include"project/project_events.hpp"
#include"ui/ui_events.hpp"

namespace arcadia
{
    struct ARCADIA_API imgui_window_property: arcadia::imgui_window_interface
    {
    public:
        using self_type = imgui_window_property;
    public:
        ARCADIA_IMGUI_WINDOW_ID_STR_GETERS("###property");

        using arcadia::imgui_window_interface::imgui_window_interface;
        virtual ~imgui_window_property() = default;

        virtual void on_event(arcadia::event_base& event) override;
        virtual void on_update() override;
    private:
        template<arcadia::component_like Component>
        auto _contains_component() -> bool
        {
            ARCADIA_ASSERT(!_scene_wptr.expired());
            return _scene_wptr.lock()->contains_all_component_of<Component>(_selected_entity);
        }
        template<arcadia::component_like Component>
        auto _get_component() -> Component&
        {
            ARCADIA_ASSERT(_contains_component<Component>());
            return _scene_wptr.lock()->get_component<Component>(_selected_entity);
        }

        void _display_components();
        void _display_camera_component();
        void _display_model_component();

        void _on_open_imgui_window(arcadia::event::open_imgui_window& e);
        void _on_scene_activated(arcadia::event::scene_activated& e);
        void _on_scene_deactivated(arcadia::event::scene_deactivated& e);
        void _on_select_entity(arcadia::event::select_entity& e);
        void _on_delete_entity(arcadia::event::delete_entity& e);

    private:
        std::weak_ptr<arcadia::scene> _scene_wptr{};
        entt::entity _selected_entity{ entt::null };
    };
}
