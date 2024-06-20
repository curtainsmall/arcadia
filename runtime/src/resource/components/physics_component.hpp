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
    glm::vec3 half_extent{ 1.f,1.f,1.f };
    float convex_radius{ JPH::cDefaultConvexRadius };
};

struct JphCapsuleShapeInfo
{
public:
    float radius{ 1.f };
    float half_height_of_cylinder{ 1.f };
};

struct JphCylinderShapeInfo
{
public:
    float half_height{ 1.f };
    float radius{ 1.f };
    float convex_radius{ JPH::cDefaultConvexRadius };
};

struct JphSphereShapeInfo
{
public:
    float radius{ 1.f };
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

    JPH::EMotionType jph_motion_type{ JPH::EMotionType::Static };
    JPH::ObjectLayer jph_object_layer{ jph_object_layers::non_moving };
    JphShapeInfo jph_shape_info{ JphBoxShapeInfo{} };
};

struct JphBodyState
{
public:
    using self_type = JphBodyState;
public:
    bool active{ false };
    glm::vec3 linear_velocity{ vec3::zero() };
    glm::vec3 angular_velocity{ vec3::zero() };

};

struct PhysicsComponentMementoData: MementoDataBase
{
public:
    auto operator==(const PhysicsComponentMementoData&) const -> bool = default;
public:
    glm::vec3 body_shape_color{};
};

struct PhysicsComponent:
    iComponent,
    iMementoOriginator
{
public:
    using identifiable_jph_body_info_type = BasicIdentifiable<JphBodyInfo>;
    using self_type = PhysicsComponent;
public:
    ACDA_COMPONENT_TYPE_STR_GETTERS("physics");

    PhysicsComponent() = default;
    PhysicsComponent(const nlohmann::json& json);
    ~PhysicsComponent() = default;
    [[nodiscard]]
    auto to_json() const->nlohmann::json;

    [[nodiscard]]
    auto has_body_info() const -> bool;

    [[nodiscard]]
    auto get_identifiable_jph_body_info() const -> const identifiable_jph_body_info_type&;

    void build_identifiable_jph_body_info(
        JPH::EMotionType jph_motion_type,
        JPH::ObjectLayer jph_object_layer,
        const JphShapeInfo& jph_shape_info
    );

    void build_identifiable_jph_body_info(
        const JphBodyInfo& jph_body_info_initial
    );

    void destroy_jph_body_info();

protected:
    [[nodiscard]]
    virtual auto on_snapshot() const->std::shared_ptr<MementoDataBase> override;
    virtual void on_restore(const std::shared_ptr<MementoDataBase>& memento_data) override;

public:
    glm::vec3 body_shape_color{ .2f,.2f,.2f };

    JphBodyState jph_body_state{};
private:
    std::unique_ptr<identifiable_jph_body_info_type> _identifiable_jph_body_info{};
};
