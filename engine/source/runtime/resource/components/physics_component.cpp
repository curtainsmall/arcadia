#include "physics_component.hpp"

#include <string>

#include "core/assert.hpp"
#include "core/match.hpp"
#include "core/math.hpp"

Arcadia::PhysicsComponent::PhysicsComponent(const nlohmann::json& json):
    _BodyShapeColor(Glm::Vec3_FromJson(json.at("body_shape_color")))
{
    const nlohmann::json& json_shape_info = json.at("jph_shape_info");
    const std::string& json_shape_info_type_string = json_shape_info.at("type");

    // If there is no shape, the physics component is not valid.
    // so we do not need to read other fields.
    if(json_shape_info_type_string == "none")
    {
        SetValid(false);
        return;
    }

    const nlohmann::json& json_shape_info_info = json_shape_info.at("info");
    JphShapeInfo shape_info = Match<JphShapeInfo>(
        json_shape_info_type_string,
        "box_shape",
        [&]() -> JphShapeInfo
        {
            return JphBoxShapeInfo(
                Glm::Vec3_FromJson(json_shape_info_info.at("half_extent")),
                json_shape_info_info.at("convex_radius")
            );
        },
        "capsule_shape",
        [&]() -> JphShapeInfo
        {
            return JphCapsuleShapeInfo(
                json_shape_info_info.at("radius"),
                json_shape_info_info.at("half_height_of_cylinder")
            );
        },
        "cylinder",
        [&]() -> JphShapeInfo
        {
            return JphCylinderShapeInfo(
                json_shape_info_info.at("half_height"),
                json_shape_info_info.at("radius"),
                json_shape_info_info.at("convex_radius")
            );
        },
        "sphere",
        [&]() -> JphShapeInfo
        {
            return JphSphereShapeInfo(
                json_shape_info_info.at("radius")
            );
        },
        "none",
        [&]() -> JphShapeInfo
        {
            ACDA_UNREACHABLE("No shape case should have early returned");
            return JphNoShapeInfo{};
        }
    );
    SetJphShapeInfo(shape_info);
    SetJphMotionType(json.at("jph_motion_type"));
    SetJphObjectLayer(json.at("jph_object_layer"));
    SetValid(true);
}

auto Arcadia::PhysicsComponent::ToJson() const -> nlohmann::json
{
    nlohmann::json json_shape_info = MatchVariant<nlohmann::json>(
        GetJphShapeInfo(),
        [&](const JphNoShapeInfo&)
        {
            return nlohmann::json{
                {"type","none"}
            };
        },
        [&](const JphBoxShapeInfo& info)
        {
            return nlohmann::json{
                {"type","box_shape"},
                {"info", {
                        {"half_extent",Glm::Vec3_ToJson(info.HalfExtent)},
                        {"convex_radius",info.ConvexRadius}
                    }
                }
            };
        },
        [&](const JphCapsuleShapeInfo& info)
        {
            return nlohmann::json{
                {"type","capsule_shape"},
                {"info", {
                        {"radius",info.Radius},
                        {"half_height_of_cylinder",info.HalfHeightOfCylinder}
                    }
                }
            };
        },
        [&](const JphCylinderShapeInfo& info)
        {
            return nlohmann::json{
                {"type","cylinder"},
                {"info", {
                        {"half_height",info.HalfHeight},
                        {"radius",info.Radius},
                        {"convex_radius",info.ConvexRadius}
                    }
                }
            };
        },
        [&](const JphSphereShapeInfo& info)
        {
            return nlohmann::json{
                {"type","sphere"},
                {"info", {
                        {"radius",info.Radius}
                    }
                }
            };
        }
    );
    return nlohmann::json{
        {"body_shape_color",Glm::Vec3_ToJson(GetBodyShapeColor())},
            {"jph_motion_type", ToUnderlying(GetJphMotionType())},
            {"jph_object_layer",GetJphObjectLayer()},
            {"jph_shape_info",json_shape_info}
    };
}

auto Arcadia::PhysicsComponent::IsValid() const -> bool
{
    return _Validity;
}

void Arcadia::PhysicsComponent::SetValid(bool validity)
{
    _Validity = validity;
}

auto Arcadia::PhysicsComponent::GetBodyShapeColor() const -> const glm::vec3&
{
    return _BodyShapeColor;
}

void Arcadia::PhysicsComponent::SetBodyShapeColor(const glm::vec3& color)
{
    _BodyShapeColor = color;
}

auto Arcadia::PhysicsComponent::IsActive() const -> bool
{
    return _Active;
}

void Arcadia::PhysicsComponent::SetActive(bool active)
{
    _Active = active;
}

auto Arcadia::PhysicsComponent::GetLinearVelocity() const -> const glm::vec3&
{
    return _LinearVelocity;
}

void Arcadia::PhysicsComponent::SetLinearVelocity(const glm::vec3& linear_velocity)
{
    _LinearVelocity = linear_velocity;
}

auto Arcadia::PhysicsComponent::GetAngularVelocity() const -> const glm::vec3&
{
    return _AngularVelocity;
}

void Arcadia::PhysicsComponent::SetAngularVelocity(const glm::vec3& angular_velocity)
{
    _AngularVelocity = angular_velocity;
}

auto Arcadia::PhysicsComponent::GetJphMotionType() const -> JPH::EMotionType
{
    return _JphMotionType;
}

void Arcadia::PhysicsComponent::SetJphMotionType(JPH::EMotionType jph_motion_type)
{
    _JphMotionType = jph_motion_type;
}

auto Arcadia::PhysicsComponent::GetJphObjectLayer() const -> JPH::ObjectLayer
{
    return _JphObjectLayer;
}

void Arcadia::PhysicsComponent::SetJphObjectLayer(JPH::ObjectLayer jph_object_layer)
{
    _JphObjectLayer = jph_object_layer;
}

auto Arcadia::PhysicsComponent::GetJphShapeInfo() const -> const JphShapeInfo&
{
    return _JphShapeInfo;
}

void Arcadia::PhysicsComponent::SetJphShapeInfo(const JphShapeInfo& jph_shape_info)
{
    _JphShapeInfo = jph_shape_info;
}

auto Arcadia::PhysicsComponent::OnSnapshot() const -> std::unique_ptr<MementoType>
{
    std::unique_ptr<MementoType> memento_uptr = std::make_unique<MementoType>();
    memento_uptr->Active = IsActive();
    memento_uptr->JphMotionType = GetJphMotionType();
    memento_uptr->JphObjectLayer = GetJphObjectLayer();
    memento_uptr->JphShapeInfo = GetJphShapeInfo();
    return memento_uptr;
}

void Arcadia::PhysicsComponent::OnRestore(const std::unique_ptr<MementoType>& memento_uptr)
{
    SetActive(memento_uptr->Active);
    SetJphMotionType(memento_uptr->JphMotionType);
    SetJphObjectLayer(memento_uptr->JphObjectLayer);
    SetJphShapeInfo(memento_uptr->JphShapeInfo);
}