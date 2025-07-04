#pragma once

#include <memory>
#include <variant>

#include "core/identifiable.hpp"
#include "core/math.hpp"
#include "core/memento.hpp"
#include "core/nlohmann_json.hpp"
#include "platform/defines.hpp"
#include "platform/jolt.hpp"
#include "resource/components/component_interface.hpp"

namespace Arcadia
{
    struct ACDA_API JphNoShapeInfo
    {
    };

    struct ACDA_API JphBoxShapeInfo
    {
    public:
        glm::vec3 HalfExtent{ 1.f,1.f,1.f };
        float ConvexRadius{ JPH::cDefaultConvexRadius };
    };

    struct ACDA_API JphCapsuleShapeInfo
    {
    public:
        float Radius{ 1.f };
        float HalfHeightOfCylinder{ 1.f };
    };

    struct ACDA_API JphCylinderShapeInfo
    {
    public:
        float HalfHeight{ 1.f };
        float Radius{ 1.f };
        float ConvexRadius{ JPH::cDefaultConvexRadius };
    };

    struct ACDA_API JphSphereShapeInfo
    {
    public:
        float Radius{ 1.f };
    };

    using JphShapeInfo = std::variant<
        JphNoShapeInfo,
        JphBoxShapeInfo,
        JphCapsuleShapeInfo,
        JphCylinderShapeInfo,
        JphSphereShapeInfo
    >;

    struct PhysicsComponent_Memento
    {
    public:
        bool Active{ false };
        JPH::EMotionType JphMotionType{ JPH::EMotionType::Static };
        JPH::ObjectLayer JphObjectLayer{ JphObjectLayers::NonMoving };
        JphShapeInfo JphShapeInfo{ JphNoShapeInfo{} };
    };

    struct ACDA_API PhysicsComponent:
        public ComponentInterface,
        public Mementoable<PhysicsComponent_Memento>
    {
    public:
        using MementoType = PhysicsComponent_Memento;
        using SelfType = PhysicsComponent;
    public:
        ACDA_COMPONENT_TYPE_STR_GETTERS("physics");

        PhysicsComponent() = default;
        PhysicsComponent(const nlohmann::json& json);
        ~PhysicsComponent() = default;
        [[nodiscard]]
        auto ToJson() const -> nlohmann::json;

        [[nodiscard]]
        auto IsInUse() const -> bool;
        void SetInUse(bool in_use);

        [[nodiscard]]
        auto GetBodyShapeColor() const -> const glm::vec3&;
        void SetBodyShapeColor(const glm::vec3& color);

        [[nodiscard]]
        auto IsActive() const -> bool;
        void SetActive(bool active);

        [[nodiscard]]
        auto GetLinearVelocity() const -> const glm::vec3&;
        void SetLinearVelocity(const glm::vec3& linear_velocity);

        [[nodiscard]]
        auto GetAngularVelocity() const -> const glm::vec3&;
        void SetAngularVelocity(const glm::vec3& angular_velocity);

        [[nodiscard]]
        auto GetJphMotionType() const -> JPH::EMotionType;
        void SetJphMotionType(JPH::EMotionType jph_motion_type);

        [[nodiscard]]
        auto GetJphObjectLayer() const -> JPH::ObjectLayer;
        void SetJphObjectLayer(JPH::ObjectLayer jph_object_layer);

        [[nodiscard]]
        auto GetJphShapeInfo() const -> const JphShapeInfo&;
        void SetJphShapeInfo(const JphShapeInfo& jph_shape_info);

    protected:
        [[nodiscard]]
        virtual auto OnSnapshot() const -> std::unique_ptr<MementoType> override;
        virtual void OnRestore(const std::unique_ptr<MementoType>& memento_data_uptr) override;

    private:
        bool _InUse{ false };

        bool _Active{ false };
        glm::vec3 _BodyShapeColor{ .2f,.2f,.2f };
        glm::vec3 _LinearVelocity{ Glm::Vec3_CreateZero() };
        glm::vec3 _AngularVelocity{ Glm::Vec3_CreateZero() };
        JPH::EMotionType _JphMotionType{ JPH::EMotionType::Static };
        JPH::ObjectLayer _JphObjectLayer{ JphObjectLayers::NonMoving };
        JphShapeInfo _JphShapeInfo{ JphNoShapeInfo{} };
    };
}