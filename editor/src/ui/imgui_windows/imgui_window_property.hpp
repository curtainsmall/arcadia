#pragma once

#include<functional>
#include<memory>
#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"function/physics/physics_simulator.hpp"
#include"function/ui/imgui_window.hpp"
#include"platform/jolt/jolt_header.hpp"
#include"resource/component/camera_component/camera_component.hpp"
#include"resource/component/light_component/light_component.hpp"
#include"resource/component/model_component/model_component.hpp"
#include"resource/component/physics_component/physics_component.hpp"
#include"resource/component/skybox_component/skybox_component.hpp"
#include"resource/scene/scene.hpp"

#include"project/project_events.hpp"
#include"ui/ui_events.hpp"

namespace Arcadia
{
    struct ARCADIA_API ImguiWindowPropertyCameraComponent
    {
    public:
        using self_type = ImguiWindowPropertyCameraComponent;
    public:
        auto operator()(Arcadia::CameraComponent& camera_comp)->std::string;
    };

    struct ARCADIA_API ImguiWindowPropertyLightComponent
    {
    public:
        using self_type = ImguiWindowPropertyLightComponent;
    public:
        auto operator()(Arcadia::LightComponent& light_comp)->std::string;
    };

    struct ARCADIA_API ImguiWindowPropertyModelComponent
    {
    public:
        using self_type = ImguiWindowPropertyModelComponent;
    public:
        auto operator()(Arcadia::ModelComponent& model_comp)->std::string;
    };

    struct ARCADIA_API ImguiWindowPopupPhysicsComponentCreateBody
    {
    public:
        using self_type = ImguiWindowPopupPhysicsComponentCreateBody;
    public:
        void operator()(Arcadia::PhysicsComponent& physics_comp);
    public:
        bool Open{ false };
    private:
        Arcadia::JphBodyInfoInitial _TempJphBodyInfoInitial{};
    };

    struct ARCADIA_API ImguiWindowPropertyPhysicsComponent
    {
    public:
        using self_type = ImguiWindowPropertyPhysicsComponent;
    public:
        auto operator()(Arcadia::PhysicsComponent& physics_comp)->std::string;
    private:
        Arcadia::ImguiWindowPopupPhysicsComponentCreateBody _imgui_window_popup_physics_component_create_body{};
    };

    struct ARCADIA_API ImguiWindowProperty: Arcadia::iImguiWindow
    {
    public:
        using self_type = ImguiWindowProperty;
    public:
        ARCADIA_IMGUI_WINDOW_ID_STR_GETTERS("###property");

        inline ImguiWindowProperty(
            bool open,
            const std::string& title
        ):
            Arcadia::iImguiWindow(open, title)
        {}
        virtual ~ImguiWindowProperty() = default;

        virtual void OnEvent(Arcadia::EventBase& event) override;
        virtual void OnUpdate() override;
    private:
        template<Arcadia::cComponent Component>
        auto _contains_component(const entt::entity entity) -> bool
        {
            ARCADIA_ASSERT(!_wpScene.expired());

            return _wpScene.lock()->AllOf<Component>(entity);
        }
        template<Arcadia::cComponent Component>
        auto _get_component(const entt::entity entity) -> Component&
        {
            ARCADIA_ASSERT(!_wpScene.expired());
            ARCADIA_ASSERT(_contains_component<Component>(entity));

            return _wpScene.lock()->Get<Component>(entity);
        }

        void _OnOpenImguiWindow(Arcadia::Event::OpenImguiWindow& e);
        void _OnSceneActivated(Arcadia::Event::SceneActivated& e);
        void _OnSceneDeactivated(Arcadia::Event::SceneDeactivated& e);
        void _on_select_entity(Arcadia::Event::SelectEntity& e);
        void _on_delete_entity(Arcadia::Event::DeleteEntity& e);
        void _OnPhysicsSimualtorBuilt(Arcadia::Event::PhysicsSimulatorBuilt& e);
        void _OnPhysicsSimulatorUnbuilt(Arcadia::Event::PhysicsSimulatorUnbuilt& e);

    private:

        std::weak_ptr<Arcadia::Scene> _wpScene{};
        entt::entity _SelectedEntity{ entt::null };

        std::weak_ptr<Arcadia::PhysicsSimulator> _wpPhysicsSimulator{};

        Arcadia::ImguiWindowPropertyCameraComponent _imgui_window_property_camera_component{};
        Arcadia::ImguiWindowPropertyLightComponent _imgui_window_property_light_component{};
        Arcadia::ImguiWindowPropertyModelComponent _imgui_window_property_model_component{};
        Arcadia::ImguiWindowPropertyPhysicsComponent _imgui_window_property_physics_component{};
    };
}
