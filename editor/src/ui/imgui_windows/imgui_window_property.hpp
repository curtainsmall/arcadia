#pragma once

#include<functional>
#include<memory>
#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"function/physics/physics_simulator.hpp"
#include"ui/imgui_header.hpp"
#include"ui/imgui_window.hpp"
#include"platform/jolt/jolt_header.hpp"
#include"resource/components/camera_component.hpp"
#include"resource/components/light_component.hpp"
#include"resource/components/model_component.hpp"
#include"resource/components/physics_component.hpp"
#include"resource/components/skybox_component.hpp"
#include"resource/components/transform_component.hpp"
#include"resource/scene.hpp"

#include"project/project_events.hpp"
#include"ui/ui_events.hpp"

struct ImguiWindowPropertyCameraComponent
{
public:
    using self_type = ImguiWindowPropertyCameraComponent;
public:
    auto operator()(CameraComponent& camera_comp)->std::string;
};

struct ImguiWindowPropertyLightComponent
{
public:
    using self_type = ImguiWindowPropertyLightComponent;
public:
    auto operator()(LightComponent& light_comp)->std::string;
};

struct ImguiWindowPropertyModelComponent
{
public:
    using self_type = ImguiWindowPropertyModelComponent;
public:
    auto operator()(ModelComponent& model_comp)->std::string;
};

struct ImguiWindowPopupPhysicsComponentCreateBody
{
public:
    using self_type = ImguiWindowPopupPhysicsComponentCreateBody;
public:
    void operator()(PhysicsComponent& physics_comp);
public:
    bool open{ false };
private:
    JphBodyInfo _temp_jph_body_info{};
};

struct ImguiWindowPropertyPhysicsComponent
{
public:
    using self_type = ImguiWindowPropertyPhysicsComponent;
public:
    auto operator()(PhysicsComponent& physics_comp)->std::string;
private:
    ImguiWindowPopupPhysicsComponentCreateBody _imgui_window_popup_physics_component_create_body{};
};

struct ImguiWindowPropertyTransformComponent
{
public:
    using self_type = ImguiWindowPropertyTransformComponent;
public:
    auto operator()(TransformComponent& transform_comp)->std::string;
};

struct ImguiWindowProperty: iImguiWindow
{
public:
    using self_type = ImguiWindowProperty;
public:
    ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###property");

    inline ImguiWindowProperty(
        bool open,
        const std::string& title
    ):
        iImguiWindow(open, title)
    {}
    virtual ~ImguiWindowProperty() = default;

    virtual void on_event(EventBase& e) override;
    virtual void on_update() override;
private:
    template<cComponent Component>
    auto _contains_component(const std::string& name) -> bool
    {
        auto scene = _scene.lock();
        ACDA_ASSERT(scene);

        return scene->all_of<Component>(name);
    }
    template<cComponent Component>
    auto _get_component(const std::string& name) -> Component&
    {
        auto scene = _scene.lock();
        ACDA_ASSERT(scene);
        ACDA_ASSERT(_contains_component<Component>(name));

        return scene->get<Component>(name);
    }

    void _on_open_imgui_window(event::OpenImguiWindow& e);
    void _on_scene_activated(event::SceneActivated& e);
    void _on_scene_deactivated(event::SceneDeactivated& e);
    void _on_select_entity(event::SelectEntity& e);
    void _on_rename_entity(event::RenameEntity& e);
    void _on_delete_entity(event::DeleteEntity& e);

private:

    std::weak_ptr<Scene> _scene{};
    std::string _selected_entity_name{};

    ImguiWindowPropertyCameraComponent _imgui_window_property_camera_component{};
    ImguiWindowPropertyLightComponent _imgui_window_property_light_component{};
    ImguiWindowPropertyModelComponent _imgui_window_property_model_component{};
    ImguiWindowPropertyPhysicsComponent _imgui_window_property_physics_component{};
    ImguiWindowPropertyTransformComponent _imgui_window_property_transform_component{};
};
