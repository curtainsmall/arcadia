#pragma once

#include<functional>
#include<memory>
#include<string>

#include"core/assert.hpp"
#include"core/concept.hpp"
#include"core/event/event.hpp"
#include"core/file/pfd_header.hpp"
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
    class ImguiWindowPropertyFunctor_CameraComponent
    {
    public:
        using SelfType = ImguiWindowPropertyFunctor_CameraComponent;
    public:
        void operator()(CameraComponent& camera_comp);

        void Refresh(const CameraComponent& comp);

    private:
        float _TempNearPlane{};
        float _TempFarPlane{};
        float _TempFovY{};
        float _TempFovYMin{};
        float _TempFovYMax{};
        float _TempSpeed{};
        glm::i32vec2 _TempViewportSize{};
        bool _TempUpAxisFixed{};
        float _TempUpAxisAngleEpsilon{};
    };

    class ImguiWindowPropertyFunctor_LightComponent
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
        glm::vec3 _TempAttenuationCoefficients{};
        glm::vec2 _TempCutoffAngle{};
        glm::vec3 _TempColor{};
        glm::vec3 _TempAmbientStrength{};
        glm::vec3 _TempDiffuseStrength{};
        glm::vec3 _TempSpecularStrength{};
    };

    class ImguiWindowPropertyFunctor_ModelComponent
    {
    public:
        using SelfType = ImguiWindowPropertyFunctor_ModelComponent;
    public:
        void operator()(ModelComponent& model_comp);
    };

    class ImguiWindowPopupFunctor_PhysicsComponentCreateBody
    {
    public:
        using SelfType = ImguiWindowPopupFunctor_PhysicsComponentCreateBody;
    public:
        void operator()(PhysicsComponent& physics_comp);
    public:
        bool Opened{ false };
    private:
        JphBodyInfo _TempJphBodyInfo{};
    };

    class ImguiWindowPropertyFunctor_PhysicsComponent
    {
    public:
        using SelfType = ImguiWindowPropertyFunctor_PhysicsComponent;
    public:
        void operator()(PhysicsComponent& physics_comp);
    private:
        ImguiWindowPopupFunctor_PhysicsComponentCreateBody _ImguiWindowPopupPhysicsComponentCreateBody{};
    };

    class ImguiWindowPropertyFunctor_TransformComponent
    {
    public:
        using SelfType = ImguiWindowPropertyFunctor_TransformComponent;
    public:
        void Refresh(const TransformComponent& comp);

        void operator()(TransformComponent& transform_comp);

    private:
        glm::vec3 _TempPosition{};
        glm::vec3 _TempRotationEularAngle{};
        glm::vec3 _TempDirection{};
        glm::vec3 _TempScale{};
        glm::vec3 _TempPivot{};
    };

    class ImguiWindowProperty: public ImguiWindowInterface
    {
    public:
        using SelfType = ImguiWindowProperty;
    public:
        ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###property");

        inline ImguiWindowProperty(
            bool open,
            const std::string& title
        ):
            ImguiWindowInterface(open, title)
        {}
        virtual ~ImguiWindowProperty() = default;

        virtual void OnEvent(EventBase& e) override;
        virtual void OnUpdate() override;
    private:
        template<Concepts::Component Component>
        auto _ContainsComponent(EntityId entity_id) const -> bool
        {
            std::shared_ptr<Scene> scene_sptr = _SceneWeakPtr.lock();
            ACDA_ASSERT(scene_sptr);

            return scene_sptr->ContainsAllComponents<Component>(entity_id);
        }
        template<Concepts::Component Component>
        auto _GetComponent(EntityId entity_id) const -> Component&
        {
            std::shared_ptr<Scene> scene_sptr = _SceneWeakPtr.lock();
            ACDA_ASSERT(scene_sptr);
            ACDA_ASSERT(_ContainsComponent<Component>(entity_id));

            return scene_sptr->GetComponent<Component>(entity_id);
        }

        void _OnOpenImguiWindow(Events::OpenImguiWindow& e);
        void _OnSceneActivated(Events::SceneActivated& e);
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

        std::weak_ptr<Scene> _SceneWeakPtr{};
        EntityId _SelectedEntityId{};

        ImguiWindowPropertyFunctor_CameraComponent _ImguiWindowPropertyFunctor_CameraComponent{};
        ImguiWindowPropertyFunctor_LightComponent _ImguiWindowPropertyFunctor_LightComponent{};
        ImguiWindowPropertyFunctor_ModelComponent _ImguiWindowPropertyFunctor_ModelComponent{};
        ImguiWindowPropertyFunctor_PhysicsComponent _ImguiWindowPropertyFunctor_PhysicsComponent{};
        ImguiWindowPropertyFunctor_TransformComponent _ImguiWindowPropertyFunctor_TransformComponent{};
    };
}