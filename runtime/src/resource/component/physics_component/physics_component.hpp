#pragma once

#include<memory>
#include<variant>

#include"core/base.hpp"
#include"core/math.hpp"
#include"core/memento/memento.hpp"
#include"core/nlohmann_json_header.hpp"
#include"core/uuid.hpp"
#include"platform/jolt/jolt_header.hpp"
#include"resource/component/component.hpp"

namespace Arcadia
{
    struct ARCADIA_API JphBoxShapeInfo
    {
    public:
        glm::vec3 HalfExtent{ 1.f,1.f,1.f };
        float ConvexRadius{ JPH::cDefaultConvexRadius };
    };
    struct ARCADIA_API JphCapsuleShapeInfo
    {
    public:
        float Radius{ 1.f };
        float HalfHeightOfCylinder{ 1.f };
    };
    struct ARCADIA_API JphCylinderShapeInfo
    {
    public:
        float HalfHeight{ 1.f };
        float Radius{ 1.f };
        float ConvexRadius{ JPH::cDefaultConvexRadius };
    };
    struct ARCADIA_API JphSphereShapeInfo
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

    struct ARCADIA_API JphBodyInfoInitial
    {
    public:
        using self_type = JphBodyInfoInitial;
    public:
        glm::vec3 Position{ Arcadia::Vec3::Zero() };
        glm::quat Rotation{ Arcadia::Quat::Identity() };
        JPH::EMotionType JphMotionType{ JPH::EMotionType::Static };
        JPH::ObjectLayer JphObjectLayer{ Arcadia::JphObjectLayers::NonMoving };
        JphShapeInfo JphShapeInfo{ Arcadia::JphBoxShapeInfo{} };
    };

    struct ARCADIA_API JphBodyInfoOngoing
    {
    public:
        using self_type = JphBodyInfoOngoing;
    public:
        bool Active{ false };
        glm::vec3 Position{ Arcadia::Vec3::Zero() };
        glm::quat Rotation{ Arcadia::Quat::Identity() };
        glm::vec3 LinearVelocity{ Arcadia::Vec3::Zero() };
        glm::vec3 AngularVelocity{ Arcadia::Vec3::Zero() };

    };

    struct ARCADIA_API PhysicsComponentMementoData: Arcadia::MementoDataBase
    {
    public:
        auto operator==(const PhysicsComponentMementoData&) const -> bool = default;
    public:
        glm::vec3 BodyShapeColor{};
    };

    struct ARCADIA_API PhysicsComponent:
        Arcadia::iComponent,
        Arcadia::iMementoOriginator
    {
    public:
        using identifiable_jph_body_info_initial_type = Arcadia::BasicIdentifiable<JphBodyInfoInitial>;
        using self_type = PhysicsComponent;
    public:
        ARCADIA_COMPONENT_TYPE_STR_GETTERS("physics");

        PhysicsComponent() = default;
        PhysicsComponent(const nlohmann::json& json);
        ~PhysicsComponent() = default;
        [[nodiscard]]
        auto ToJson() const->nlohmann::json;

        [[nodiscard]]
        auto HasBodyInfo() const -> bool;

        [[nodiscard]]
        auto GetIdentifiableJphBodyInfoInitial() const -> const identifiable_jph_body_info_initial_type&;

        [[nodiscard]]
        auto GetJphBodyInfoOngoing() const -> const Arcadia::JphBodyInfoOngoing&;
        [[nodiscard]]
        auto GetJphBodyInfoOngoing() -> Arcadia::JphBodyInfoOngoing&;

        void BuildIdentifiableJphBodyInfoInitial(
            const glm::vec3& position,
            const glm::quat& rotation,
            JPH::EMotionType jph_motion_type,
            JPH::ObjectLayer jph_object_layer,
            const JphShapeInfo& jph_shape_info
        );

        void BuildIdentifiableJphBodyInfoInitial(
            const Arcadia::JphBodyInfoInitial& jph_body_info_initial
        );

    protected:
        [[nodiscard]]
        virtual auto OnSnapshot() const->std::shared_ptr<MementoDataBase> override;
        virtual void OnRestore(const std::shared_ptr<MementoDataBase>& sp_memento_data) override;

    public:
        glm::vec3 BodyShapeColor{ .2f,.2f,.2f };
    private:
        std::unique_ptr<identifiable_jph_body_info_initial_type> _upIdentifiableJphBodyInfoInitial{};
        std::unique_ptr<Arcadia::JphBodyInfoOngoing> _upJphBodyInfoOngoing{};
    };
}
