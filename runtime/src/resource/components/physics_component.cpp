#include "pch.hpp"

#include "physics_component.hpp"

#include <string>

#include "core/assert.hpp"
#include "core/match.hpp"
#include "core/math.hpp"

Arcadia::PhysicsComponent::PhysicsComponent(const nlohmann::json& json):
    _BodyShapeColor(GlmVec3::FromJson(json.at("body_shape_color")))
{
    const nlohmann::json& json_body_info_initial = json.at("jph_body_info_initial");
    if(!json_body_info_initial.is_null())
    {
        const nlohmann::json& json_shape_info = json_body_info_initial.at("jph_shape_info");
        const std::string& json_shape_info_type_string = json_shape_info.at("type");
        const nlohmann::json& json_shape_info_info = json_shape_info.at("info");
        JphShapeInfo shape_info = Match<JphShapeInfo>(
            json_shape_info_type_string,
            "box_shape",
            [&]() -> JphShapeInfo
        {
            return JphBoxShapeInfo(
                GlmVec3::FromJson(json_shape_info_info.at("half_extent")),
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
        }
        );

        BuildIndentifiableJphBodyInfo(
            JPH::EMotionType(json_body_info_initial.at("jph_motion_type")),
            JPH::ObjectLayer(json_body_info_initial.at("jph_object_layer")),
            shape_info
        );
    }
}

auto Arcadia::PhysicsComponent::ToJson() const -> nlohmann::json
{
    nlohmann::json json_body_info_initial{};
    if(HasBodyInfo())
    {
        const auto& [uuid, body_info] = GetIdentifiableJphBodyInfo();
        nlohmann::json json_shape_info = MatchVariant<nlohmann::json>(
            body_info.JphShapeInfo,
            [&](const JphBoxShapeInfo& info)
        {
            return nlohmann::json{
                {"type","box_shape"},
                {"info", {
                        {"half_extent",GlmVec3::ToJson(info.HalfExtent)},
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
        json_body_info_initial = nlohmann::json{
            {"jph_motion_type", ToUnderlying(body_info.JphMotionType)},
            {"jph_object_layer",body_info.JphObjectLayer},
            {"jph_shape_info",json_shape_info}
        };
    }
    else
    {
        json_body_info_initial = nullptr;
    }

    return nlohmann::json{
        {"jph_body_info_initial",json_body_info_initial},
        {"body_shape_color",GlmVec3::ToJson(_BodyShapeColor)}
    };
}

auto Arcadia::PhysicsComponent::HasBodyInfo() const -> bool
{
    return !!_upIdentifiableJphBodyInfo;
}

auto Arcadia::PhysicsComponent::GetIdentifiableJphBodyInfo() const -> const IdentifiableJphBodyInfoType&
{
    ACDA_ASSERT(HasBodyInfo());
    return *_upIdentifiableJphBodyInfo;
}

void Arcadia::PhysicsComponent::BuildIndentifiableJphBodyInfo(
    JPH::EMotionType jph_motion_type,
    JPH::ObjectLayer jph_object_layer,
    const JphShapeInfo& jph_shape_info
)
{
    BuildIndentifiableJphBodyInfo({ jph_motion_type,jph_object_layer,jph_shape_info });
}

void Arcadia::PhysicsComponent::BuildIndentifiableJphBodyInfo(const JphBodyInfo& jph_body_info_initial)
{
    _upIdentifiableJphBodyInfo = std::make_unique<IdentifiableJphBodyInfoType>(
        jph_body_info_initial
    );
}

void Arcadia::PhysicsComponent::DestroyJphBodyInfo()
{
    _upIdentifiableJphBodyInfo.reset();
}

auto Arcadia::PhysicsComponent::GetBodyShapeColor() const -> const glm::vec3&
{
    return _BodyShapeColor;
}

void Arcadia::PhysicsComponent::SetBodyShapeColor(const glm::vec3& color)
{
    _BodyShapeColor = color;
}

auto Arcadia::PhysicsComponent::GetBodyState() const -> const JphBodyState&
{
    return _BodyState;
}

void Arcadia::PhysicsComponent::SetBodyState(const JphBodyState& state)
{
    _BodyState = state;
}