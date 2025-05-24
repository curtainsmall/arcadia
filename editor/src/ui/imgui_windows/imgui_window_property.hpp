#pragma once

#include <functional>
#include <memory>
#include <string>

#include "core/assert.hpp"
#include "core/concept.hpp"
#include "core/event.hpp"
#include "core/pfd.hpp"
#include "function/physics/physics_simulator.hpp"
#include "platform/jolt.hpp"
#include "resource/components/camera_component.hpp"
#include "resource/components/light_component.hpp"
#include "resource/components/model_component.hpp"
#include "resource/components/physics_component.hpp"
#include "resource/components/transform_component.hpp"
#include "resource/scene_events.hpp"
#include "resource/scene_layer.hpp"

#include "editor/editor_layer.hpp"
#include "project/project_events.hpp"
#include "ui/imgui.hpp"
#include "ui/imgui_window.hpp"
#include "ui/ui_events.hpp"

namespace Arcadia
{
    struct ImguiWindowPropertyFunctor_CameraComponent
    {
    public:
        using SelfType = ImguiWindowPropertyFunctor_CameraComponent;
    public:
        void operator()(CameraComponent& camera_comp);

        void Refresh(const CameraComponent& comp);

    private:
        float _OriginNearPlane{};
        float _OriginFarPlane{};
        float _OriginFovY{};
        float _OriginFovYMin{};
        float _OriginFovYMax{};
        float _OriginSpeed{};
        glm::i32vec2 _OriginViewportSize{};
        bool _OriginUpAxisFixed{};
        float _OriginUpAxisAngleEpsilon{};
    };

    struct ImguiWindowPropertyFunctor_LightComponent
    {
    public:
        using SelfType = ImguiWindowPropertyFunctor_LightComponent;
    public:
        void operator()(LightComponent& light_comp);

        void Refresh(const LightComponent& comp);
    private:
        template<Concepts::VariantContainsType<LightType> Light>
        auto _ChangeLightTypeSelectable(LightComponent& light_comp, const std::string& light_name) -> bool
        {
            if(ImGui::Selectable(light_name.c_str()))
            {
                pfd::button res = pfd::message{
                        "Arcadia - Changing Light Type",
                        std::format("Do you want to change light type to {}? All properties for current light will be lost",light_name),
                        pfd::choice::yes_no,
                        pfd::icon::info
                }.result();
                if(res == pfd::button::yes)
                {
                    light_comp.SetLight<Light>();
                    Refresh(light_comp);
                    return true;
                }
            }
            return false;
        }

    private:
        glm::vec3 _OriginAttenuationCoefficients{};
        glm::vec2 _OriginCutoffAngle{};
        glm::vec2 _OriginSize{};
        glm::vec3 _OriginColor{};
        glm::vec3 _OriginAmbientStrength{};
        glm::vec3 _OriginDiffuseStrength{};
        glm::vec3 _OriginSpecularStrength{};
    };

    struct ImguiWindowPropertyFunctor_ModelComponent
    {
    public:
        using SelfType = ImguiWindowPropertyFunctor_ModelComponent;
    public:
        void operator()(ModelComponent& model_comp);
    };

    struct ImguiWindowPopupFunctor_PhysicsComponentCreateBody
    {
    public:
        using SelfType = ImguiWindowPopupFunctor_PhysicsComponentCreateBody;
    public:
        void operator()(PhysicsComponent& physics_comp);
    public:
        bool Opened{ false };
    private:
        JPH::EMotionType _TempJphMotionType{ JPH::EMotionType::Static };
        JPH::ObjectLayer _TempJphObjectLayer{ JphObjectLayers::NonMoving };
        JphShapeInfo     _TempJphShapeInfo{ JphNoShapeInfo{} };
    };

    struct ImguiWindowPropertyFunctor_PhysicsComponent
    {
    public:
        using SelfType = ImguiWindowPropertyFunctor_PhysicsComponent;
    public:
        void operator()(PhysicsComponent& physics_comp);
    private:
        ImguiWindowPopupFunctor_PhysicsComponentCreateBody _ImguiWindowPopupPhysicsComponentCreateBody{};
    };

    struct ImguiWindowPropertyFunctor_TransformComponent
    {
    public:
        using SelfType = ImguiWindowPropertyFunctor_TransformComponent;
    public:
        void Refresh(const TransformComponent& comp);

        void operator()(TransformComponent& transform_comp);

    private:
        glm::vec3 _OriginPosition{};
        glm::vec3 _OriginRotationEularAngle{};
        glm::vec3 _OriginDirection{};
        glm::vec3 _OriginScale{};
        glm::vec3 _OriginPivot{};
    };

    struct ImguiWindowProperty: public ImguiWindowInterface
    {
    public:
        using SelfType = ImguiWindowProperty;
    public:
        ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###property");

        ImguiWindowProperty(
            bool open,
            const std::string& title
        );
        virtual ~ImguiWindowProperty() override = default;

        virtual void OnEvent(EventBase& e) override;
        virtual void OnUpdate() override;
    private:
        template<Concepts::Component Component>
        auto _ContainsComponent(EntityId entity_id) const -> bool
        {
            std::shared_ptr<SceneLayer> scene_layer_sptr = LayerStack::Instance().GetLayerShared<SceneLayer>();
            ACDA_ASSERT(scene_layer_sptr->HasActiveScene());

            return scene_layer_sptr->ActiveScene_ContainsAllComponents<Component>(entity_id);
        }
        template<Concepts::Component Component>
        auto _GetComponent(EntityId entity_id) const -> Component&
        {
            std::shared_ptr<SceneLayer> scene_layer_sptr = LayerStack::Instance().GetLayerShared<SceneLayer>();
            ACDA_ASSERT(scene_layer_sptr->HasActiveScene());
            ACDA_ASSERT(_ContainsComponent<Component>(entity_id));

            return scene_layer_sptr->ActiveScene_GetComponent<Component>(entity_id);
        }

        void _OnOpenImguiWindow(Events::OpenImguiWindow& e);
        void _OnSceneDeactivated(Events::SceneDeactivated& e);
        void _OnSelectEntity(Events::SelectEntity& e);
        void _OnDeleteEntity(Events::DeleteEntity& e);

        template<Concepts::Component Component>
        void _DisplayProperty(const std::string& tab_name, const std::function<void(Component&)>& display_fn) const
        {
            if(_ContainsComponent<Component>(_SelectedEntityId) && ImGui::TreeNodeEx(tab_name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding))
            {
                display_fn(_GetComponent<Component>(_SelectedEntityId));
                ImGui::TreePop();
            }
        }

    private:
        EntityId _SelectedEntityId{};

        ImguiWindowPropertyFunctor_CameraComponent _ImguiWindowPropertyFunctor_CameraComponent{};
        ImguiWindowPropertyFunctor_LightComponent _ImguiWindowPropertyFunctor_LightComponent{};
        ImguiWindowPropertyFunctor_ModelComponent _ImguiWindowPropertyFunctor_ModelComponent{};
        ImguiWindowPropertyFunctor_PhysicsComponent _ImguiWindowPropertyFunctor_PhysicsComponent{};
        ImguiWindowPropertyFunctor_TransformComponent _ImguiWindowPropertyFunctor_TransformComponent{};
    };
}