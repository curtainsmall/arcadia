#pragma once

#include <memory>
#include <variant>

#include "core/identifiable.hpp"
#include "core/math.hpp"
#include "core/nlohmann_json_header.hpp"
#include "platform/api_def.hpp"
#include "platform/jolt_header.hpp"
#include "resource/components/component_interface.hpp"

namespace Arcadia
{
    class JphBoxShapeInfo
    {
    public:
        glm::vec3 HalfExtent{ 1.f,1.f,1.f };
        float ConvexRadius{ JPH::cDefaultConvexRadius };
    };

    class JphCapsuleShapeInfo
    {
    public:
        float Radius{ 1.f };
        float HalfHeightOfCylinder{ 1.f };
    };

    class JphCylinderShapeInfo
    {
    public:
        float HalfHeight{ 1.f };
        float Radius{ 1.f };
        float ConvexRadius{ JPH::cDefaultConvexRadius };
    };

    class JphSphereShapeInfo
    {
    public:
        float Radius{ 1.f };
    };

    using JphShapeInfo = std::variant<
        JphBoxShapeInfo,
        JphCapsuleShapeInfo,
        JphCylinderShapeInfo,
        JphSphereShapeInfo
    >;

    class JphBodyInfo
    {
    public:
        using SelfType = JphBodyInfo;
    public:
        // Transform information comes from transform component

        JPH::EMotionType JphMotionType{ JPH::EMotionType::Static };
        JPH::ObjectLayer JphObjectLayer{ JphObjectLayers::NonMoving };
        JphShapeInfo JphShapeInfo{ JphBoxShapeInfo{} };
    };

    class JphBodyState
    {
    public:
        using SelfType = JphBodyState;
    public:
        bool Active{ false };
        glm::vec3 LinearVelocity{ GlmVec3::CreateZero() };
        glm::vec3 AngularVelocity{ GlmVec3::CreateZero() };
    };

    class PhysicsComponent:
        public ComponentInterface
    {
    public:
        using IdentifiableJphBodyInfoType = Identifiable<JphBodyInfo>;
        using SelfType = PhysicsComponent;
    public:
        ACDA_COMPONENT_TYPE_STR_GETTERS("physics");

        PhysicsComponent() = default;
        PhysicsComponent(const nlohmann::json& json);
        ~PhysicsComponent() = default;
        [[nodiscard]]
        auto ToJson() const->nlohmann::json;

        [[nodiscard]]
        auto HasBodyInfo() const -> bool;

        [[nodiscard]]
        auto GetIdentifiableJphBodyInfo() const -> const IdentifiableJphBodyInfoType&;

        void BuildIndentifiableJphBodyInfo(
            JPH::EMotionType jph_motion_type,
            JPH::ObjectLayer jph_object_layer,
            const JphShapeInfo& jph_shape_info
        );

        void BuildIndentifiableJphBodyInfo(
            const JphBodyInfo& jph_body_info_initial
        );

        void DestroyJphBodyInfo();

        [[nodiscard]]
        auto GetBodyShapeColor() const -> const glm::vec3&;
        void SetBodyShapeColor(const glm::vec3& color);

        [[nodiscard]]
        auto GetBodyState() const -> const JphBodyState&;
        void SetBodyState(const JphBodyState& state);

    private:
        glm::vec3 _BodyShapeColor{ .2f,.2f,.2f };
        JphBodyState _BodyState{};
        std::unique_ptr<IdentifiableJphBodyInfoType> _upIdentifiableJphBodyInfo{};
    };
}