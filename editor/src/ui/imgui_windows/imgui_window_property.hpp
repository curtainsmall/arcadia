#pragma once

#include<memory>
#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"function/physics/physics_simulator.hpp"
#include"function/ui/imgui_window.hpp"
#include"platform/jolt/jolt_header.hpp"
#include"resource/component/physics_component/physics_component.hpp"
#include"resource/scene/scene.hpp"

#include"project/project_events.hpp"
#include"ui/ui_events.hpp"

namespace arcadia
{
    struct ARCADIA_API imgui_window_popup_physics_component_create_body
    {
    public:
        using self_type = imgui_window_popup_physics_component_create_body;
    public:
        void operator()(arcadia::physics_component& physics_comp);
    public:
        bool open{ false };
    private:
        JPH::RVec3 _jph_position{ JPH::RVec3::sZero() };
        JPH::Quat _jph_rotation{ JPH::Quat::sIdentity() };
        JPH::EMotionType _jph_motion_type{ JPH::EMotionType::Static };
        JPH::ObjectLayer _jph_object_layer{ arcadia::jph_object_layers::non_moving };
        arcadia::physics_component::jph_shape_info_type _jph_shape_info{ arcadia::physics_component::jph_box_shape_info{} };
    };

    struct ARCADIA_API imgui_window_property: arcadia::imgui_window_interface
    {
    public:
        using self_type = imgui_window_property;
    public:
        ARCADIA_IMGUI_WINDOW_ID_STR_GETTERS("###property");

        inline imgui_window_property(
            bool open,
            const std::string& title
        ):
            imgui_window_interface(open, title)
        {}
        virtual ~imgui_window_property() = default;

        virtual void on_event(arcadia::event_base& event) override;
        virtual void on_update() override;
    private:
        template<arcadia::component_like Component>
        auto _contains_component(const std::shared_ptr<arcadia::scene>& scene_sptr) -> bool
        {
            ARCADIA_ASSERT(scene_sptr);
            return scene_sptr->all_of<Component>(_selected_entity);
        }
        template<arcadia::component_like Component>
        auto _get_component(const std::shared_ptr<arcadia::scene>& scene_sptr) -> Component&
        {
            ARCADIA_ASSERT(_contains_component<Component>(scene_sptr));
            return scene_sptr->get<Component>(_selected_entity);
        }

        void _display_components(const std::shared_ptr<arcadia::scene>& scene_sptr);
        void _display_camera_component(const std::shared_ptr<arcadia::scene>& scene_sptr);
        void _display_light_component(const std::shared_ptr<arcadia::scene>& scene_sptr);
        void _display_model_component(const std::shared_ptr<arcadia::scene>& scene_sptr);
        void _display_physics_component(const std::shared_ptr<arcadia::scene>& scene_sptr);

        void _on_open_imgui_window(arcadia::event::open_imgui_window& e);
        void _on_scene_activated(arcadia::event::scene_activated& e);
        void _on_scene_deactivated(arcadia::event::scene_deactivated& e);
        void _on_select_entity(arcadia::event::select_entity& e);
        void _on_delete_entity(arcadia::event::delete_entity& e);
        void _on_physics_simulator_built(arcadia::event::physics_simulator_built& e);
        void _on_physics_simulator_unbuilt(arcadia::event::physics_simulator_unbuilt& e);

    private:
        arcadia::imgui_window_popup_physics_component_create_body _imgui_window_popup_physics_component_create_body{};

        std::weak_ptr<arcadia::scene> _scene_wptr{};
        entt::entity _selected_entity{ entt::null };

        std::weak_ptr<arcadia::physics_simulator> _physics_simulator_wptr{};
    };
}
