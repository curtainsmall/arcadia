#pragma once

#include<memory>
#include<variant>

#include"core/base.hpp"
#include"core/math.hpp"
#include"core/memento/memento.hpp"
#include"core/nlohmann_json_header.hpp"
#include"core/uuid.hpp"
#include"platform/jolt/jolt_header.hpp"
#include"resource/components/component_interface.hpp"

struct JphBoxShapeInfo
{
public:
    glm::vec3 HalfExtent{ 1.f,1.f,1.f };
    float ConvexRadius{ JPH::cDefaultConvexRadius };
};
struct JphCapsuleShapeInfo
{
public:
    float Radius{ 1.f };
    float HalfHeightOfCylinder{ 1.f };
};
struct JphCylinderShapeInfo
{
public:
    float HalfHeight{ 1.f };
    float Radius{ 1.f };
    float ConvexRadius{ JPH::cDefaultConvexRadius };
};
struct JphSphereShapeInfo
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

struct JphBodyInfo
{
public:
    using self_type = JphBodyInfo;
public:
    // Transform information comes from transform component

    JPH::EMotionType JphMotionType{ JPH::EMotionType::Static };
    JPH::ObjectLayer JphObjectLayer{ JphObjectLayers::NonMoving };
    JphShapeInfo JphShapeInfo{ JphBoxShapeInfo{} };
};

struct JphBodyState
{
public:
    using self_type = JphBodyState;
public:
    bool Active{ false };
    glm::vec3 LinearVelocity{ Vec3::Zero() };
    glm::vec3 AngularVelocity{ Vec3::Zero() };

};

struct PhysicsComponentMementoData: MementoDataBase
{
public:
    auto operator==(const PhysicsComponentMementoData&) const -> bool = default;
public:
    glm::vec3 BodyShapeColor{};
};

struct PhysicsComponent:
    iComponent,
    iMementoOriginator
{
public:
    using identifiable_jph_body_info_type = BasicIdentifiable<JphBodyInfo>;
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
    auto GetIdentifiableJphBodyInfo() const -> const identifiable_jph_body_info_type&;

    void BuildIdentifiableJphBodyInfo(
        JPH::EMotionType jph_motion_type,
        JPH::ObjectLayer jph_object_layer,
        const JphShapeInfo& jph_shape_info
    );

    void BuildIdentifiableJphBodyInfo(
        const JphBodyInfo& jph_body_info_initial
    );

    void DestroyJphBodyInfo();

protected:
    [[nodiscard]]
    virtual auto OnSnapshot() const->std::shared_ptr<MementoDataBase> override;
    virtual void OnRestore(const std::shared_ptr<MementoDataBase>& memento_data) override;

public:
    glm::vec3 BodyShapeColor{ .2f,.2f,.2f };

    JphBodyState JphBodyState{};
private:
    std::unique_ptr<identifiable_jph_body_info_type> _IdentifiableJphBodyInfo{};
};
