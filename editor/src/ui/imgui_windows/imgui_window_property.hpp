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

namespace Arcadia
{
    struct ImguiWindowPropertyCameraComponent
    {
    public:
        using self_type = ImguiWindowPropertyCameraComponent;
    public:
        auto operator()(Arcadia::CameraComponent& camera_comp)->std::string;
    };

    struct ImguiWindowPropertyLightComponent
    {
    public:
        using self_type = ImguiWindowPropertyLightComponent;
    public:
        auto operator()(Arcadia::LightComponent& light_comp)->std::string;
    };

    struct ImguiWindowPropertyModelComponent
    {
    public:
        using self_type = ImguiWindowPropertyModelComponent;
    public:
        auto operator()(Arcadia::ModelComponent& model_comp)->std::string;
    };

    struct ImguiWindowPopupPhysicsComponentCreateBody
    {
    public:
        using self_type = ImguiWindowPopupPhysicsComponentCreateBody;
    public:
        void operator()(Arcadia::PhysicsComponent& physics_comp);
    public:
        bool Open{ false };
    private:
        Arcadia::JphBodyInfo _TempJphBodyInfo{};
    };

    struct ImguiWindowPropertyPhysicsComponent
    {
    public:
        using self_type = ImguiWindowPropertyPhysicsComponent;
    public:
        auto operator()(Arcadia::PhysicsComponent& physics_comp)->std::string;
    private:
        Arcadia::ImguiWindowPopupPhysicsComponentCreateBody _imgui_window_popup_physics_component_create_body{};
    };

    struct ImguiWindowPropertyTransformComponent
    {
    public:
        using self_type = ImguiWindowPropertyTransformComponent;
    public:
        auto operator()(Arcadia::TransformComponent& transform_comp)->std::string;
    };

    struct ImguiWindowProperty: Arcadia::iImguiWindow
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
        auto _ContainsComponent(const std::string& name) -> bool
        {
            auto scene = _Scene.lock();
            ARCADIA_ASSERT(scene);

            return scene->AllOf<Component>(name);
        }
        template<Arcadia::cComponent Component>
        auto _GetComponent(const std::string& name) -> Component&
        {
            auto scene = _Scene.lock();
            ARCADIA_ASSERT(scene);
            ARCADIA_ASSERT(_ContainsComponent<Component>(name));

            return scene->Get<Component>(name);
        }

        void _OnOpenImguiWindow(Arcadia::Event::OpenImguiWindow& e);
        void _OnSceneActivated(Arcadia::Event::SceneActivated& e);
        void _OnSceneDeactivated(Arcadia::Event::SceneDeactivated& e);
        void _OnSelectEntity(Arcadia::Event::SelectEntity& e);
        void _OnRenameEntity(Arcadia::Event::RenameEntity& e);
        void _OnDeleteEntity(Arcadia::Event::DeleteEntity& e);

    private:

        std::weak_ptr<Arcadia::Scene> _Scene{};
        std::string _SelectedEntityName{};

        Arcadia::ImguiWindowPropertyCameraComponent _ImguiWindowPropertyCameraComponent{};
        Arcadia::ImguiWindowPropertyLightComponent _ImguiWindowPropertyLightComponent{};
        Arcadia::ImguiWindowPropertyModelComponent _ImguiWindowPropertyModelComponent{};
        Arcadia::ImguiWindowPropertyPhysicsComponent _ImguiWindowPropertyPhysicsComponent{};
        Arcadia::ImguiWindowPropertyTransformComponent _ImguiWindowPropertyTransformComponent{};
    };
}
