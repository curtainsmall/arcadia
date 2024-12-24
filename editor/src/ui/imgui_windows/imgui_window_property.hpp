#pragma once

#include<functional>
#include<memory>
#include<string>

#include"core/assert.hpp"
#include"core/event/event.hpp"
#include"function/physics/physics_simulator.hpp"
#include"platform/api_def.hpp"
#include"platform/jolt/jolt_header.hpp"
#include"resource/components/camera_component.hpp"
#include"resource/components/light_component.hpp"
#include"resource/components/model_component.hpp"
#include"resource/components/physics_component.hpp"
#include"resource/components/skybox_component.hpp"
#include"resource/components/transform_component.hpp"
#include"resource/scene.hpp"
#include"ui/imgui_header.hpp"
#include"ui/imgui_window.hpp"

#include"project/project_events.hpp"
#include"ui/ui_events.hpp"

namespace Arcadia
{
    class ImguiWindowPropertyCameraComponent
    {
    public:
        using SelfType = ImguiWindowPropertyCameraComponent;
    public:
        auto operator()(CameraComponent& camera_comp)->std::string;
    };

    class ImguiWindowPropertyLightComponent
    {
    public:
        using SelfType = ImguiWindowPropertyLightComponent;
    public:
        auto operator()(LightComponent& light_comp)->std::string;
    };

    class ImguiWindowPropertyModelComponent
    {
    public:
        using SelfType = ImguiWindowPropertyModelComponent;
    public:
        auto operator()(ModelComponent& model_comp)->std::string;
    };

    class ImguiWindowPopupPhysicsComponentCreateBody
    {
    public:
        using SelfType = ImguiWindowPopupPhysicsComponentCreateBody;
    public:
        void operator()(PhysicsComponent& physics_comp);
    public:
        bool Opened{ false };
    private:
        JphBodyInfo _TempJphBodyInfo{};
    };

    class ImguiWindowPropertyPhysicsComponent
    {
    public:
        using SelfType = ImguiWindowPropertyPhysicsComponent;
    public:
        auto operator()(PhysicsComponent& physics_comp)->std::string;
    private:
        ImguiWindowPopupPhysicsComponentCreateBody _ImguiWindowPopupPhysicsComponentCreateBody{};
    };

    class ImguiWindowPropertyTransformComponent
    {
    public:
        using SelfType = ImguiWindowPropertyTransformComponent;
    public:
        auto operator()(TransformComponent& transform_comp)->std::string;
    };

    class ImguiWindowProperty: public iImguiWindow
    {
    public:
        using SelfType = ImguiWindowProperty;
    public:
        ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###property");

        inline ImguiWindowProperty(
            bool open,
            const std::string& title
        ) :
            iImguiWindow(open, title)
        {}
        virtual ~ImguiWindowProperty() = default;

        virtual void OnEvent(EventBase& e) override;
        virtual void OnUpdate() override;
    private:
        template<cComponent Component>
        auto _ContainsComponent(const std::string& name) -> bool
        {
            std::shared_ptr<Scene> scene_sptr = _SceneWeakPtr.lock();
            ACDA_ASSERT(scene_sptr);

            return scene_sptr->ContainsAllComponents<Component>(name);
        }
        template<cComponent Component>
        auto _GetComponent(const std::string& name) -> Component&
        {
            std::shared_ptr<Scene> scene_sptr = _SceneWeakPtr.lock();
            ACDA_ASSERT(scene_sptr);
            ACDA_ASSERT(_ContainsComponent<Component>(name));

            return scene_sptr->GetComponent<Component>(name);
        }

        void _OnOpenImguiWindow(Events::OpenImguiWindow& e);
        void _OnSceneActivated(Events::SceneActivated& e);
        void _OnSceneDeactivated(Events::SceneDeactivated& e);
        void _OnSelectEntity(Events::SelectEntity& e);
        void _OnRenameEntity(Events::RenameEntity& e);
        void _OnDeleteEntity(Events::DeleteEntity& e);

    private:

        std::weak_ptr<Scene> _SceneWeakPtr{};
        std::string _SelectedEntityName{};

        ImguiWindowPropertyCameraComponent _ImguiWindowPropertyCameraComponent{};
        ImguiWindowPropertyLightComponent _ImguiWindowPropertyLightComponent{};
        ImguiWindowPropertyModelComponent _ImguiWindowPropertyModelComponent{};
        ImguiWindowPropertyPhysicsComponent _ImguiWindowPropertyPhysicsComponent{};
        ImguiWindowPropertyTransformComponent _ImguiWindowPropertyTransformComponent{};
    };
}