#include "pch.hpp"

#include "physics_component.hpp"

#include<string>

#include"core/math.hpp"

Arcadia::PhysicsComponent::PhysicsComponent(const nlohmann::json& json):
    BodyShapeColor(Arcadia::Vec3::FromJson(json.at("body_shape_color")))
{
    const auto& json_body_info_initial = json.at("jph_body_info_initial");
    if(!json_body_info_initial.is_null())
    {
        const auto& json_shape_info = json_body_info_initial.at("jph_shape_info");
        const std::string& json_shape_info_type_str = json_shape_info.at("type");
        const auto& json_shape_info_info = json_shape_info.at("info");
        auto shape_info = Arcadia::Match<Arcadia::JphShapeInfo>(
            json_shape_info_type_str,
            "box_shape"s,
            [&]() -> Arcadia::JphShapeInfo
        {
            return Arcadia::JphBoxShapeInfo{
                Arcadia::Vec3::FromJson(json_shape_info_info.at("half_extent")),
                json_shape_info_info.at("convex_radius")
            };
        },
            "capsule_shape"s,
            [&]() -> Arcadia::JphShapeInfo
        {
            return Arcadia::JphCapsuleShapeInfo{
                json_shape_info_info.at("radius"),
                json_shape_info_info.at("half_height_of_cylinder")
            };
        },
            "cylinder"s,
            [&]() -> Arcadia::JphShapeInfo
        {
            return Arcadia::JphCylinderShapeInfo{
                json_shape_info_info.at("half_height"),
                json_shape_info_info.at("radius"),
                json_shape_info_info.at("convex_radius")
            };
        },
            "sphere"s,
            [&]() -> Arcadia::JphShapeInfo
        {
            return Arcadia::JphSphereShapeInfo{
                json_shape_info_info.at("radius")
            };
        }
        );

        BuildIdentifiableJphBodyInfoInitial(
            Arcadia::Vec3::FromJson(json_body_info_initial.at("position")),
            Arcadia::Quat::FromJson(json_body_info_initial.at("rotation")),
            JPH::EMotionType{ json_body_info_initial.at("jph_motion_type") },
            JPH::ObjectLayer{ json_body_info_initial.at("jph_object_layer") },
            shape_info
        );

    }
}

auto Arcadia::PhysicsComponent::ToJson() const -> nlohmann::json
{
    nlohmann::json json_body_info_initial{};
    if(HasBodyInfo())
    {
        const auto& [Uuid, body_info] = GetIdentifiableJphBodyInfoInitial();
        auto json_shape_info = Arcadia::Match<nlohmann::json>(
            body_info.JphShapeInfo,
            [&](const Arcadia::JphBoxShapeInfo& info)
        {
            return nlohmann::json{
                {"type","box_shape"},
                {"info", {
                        {"half_extent",Arcadia::Vec3::ToJson(info.HalfExtent)},
                        {"convex_radius",info.ConvexRadius}
                    }
                }
            };
        },
            [&](const Arcadia::JphCapsuleShapeInfo& info)
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
            [&](const Arcadia::JphCylinderShapeInfo& info)
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
            [&](const Arcadia::JphSphereShapeInfo& info)
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
            {"position",Arcadia::Vec3::ToJson(body_info.Position)},
            {"rotation",Arcadia::Quat::ToJson(body_info.Rotation)},
            {"jph_motion_type", Arcadia::ToUnderlying(body_info.JphMotionType)},
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
        {"body_shape_color",Arcadia::Vec3::ToJson(BodyShapeColor)}
    };
}

auto Arcadia::PhysicsComponent::OnSnapshot() const -> std::shared_ptr<MementoDataBase>
{
    auto sp_memento = std::make_shared<Arcadia::PhysicsComponentMementoData>();

    sp_memento->BodyShapeColor = BodyShapeColor;

    return sp_memento;
}

void Arcadia::PhysicsComponent::OnRestore(const std::shared_ptr<MementoDataBase>& sp_memento_data)
{
    auto& memento_data = sp_memento_data->As<Arcadia::PhysicsComponentMementoData>();

    BodyShapeColor = memento_data.BodyShapeColor;
}

auto Arcadia::PhysicsComponent::HasBodyInfo() const -> bool
{
    return _upIdentifiableJphBodyInfoInitial.get();
}

auto Arcadia::PhysicsComponent::GetIdentifiableJphBodyInfoInitial() const -> const identifiable_jph_body_info_initial_type&
{
    ARCADIA_ASSERT(HasBodyInfo());
    return *_upIdentifiableJphBodyInfoInitial;
}

auto Arcadia::PhysicsComponent::GetJphBodyInfoOngoing() const -> const Arcadia::JphBodyInfoOngoing&
{
    ARCADIA_ASSERT(HasBodyInfo());
    return *_upJphBodyInfoOngoing;
}

auto Arcadia::PhysicsComponent::GetJphBodyInfoOngoing() -> Arcadia::JphBodyInfoOngoing&
{
    ARCADIA_ASSERT(HasBodyInfo());
    return *_upJphBodyInfoOngoing;
}

void Arcadia::PhysicsComponent::BuildIdentifiableJphBodyInfoInitial(
    const glm::vec3& position,
    const glm::quat& rotation,
    JPH::EMotionType jph_motion_type,
    JPH::ObjectLayer jph_object_layer,
    const Arcadia::JphShapeInfo& jph_shape_info
)
{
    _upIdentifiableJphBodyInfoInitial = std::make_unique<identifiable_jph_body_info_initial_type>(
        position,
        rotation,
        jph_motion_type,
        jph_object_layer,
        jph_shape_info
    );

    _upJphBodyInfoOngoing = std::make_unique<Arcadia::JphBodyInfoOngoing>(
        false,
        position,
        rotation,
        Arcadia::Vec3::Zero(),
        Arcadia::Vec3::Zero()
    );
}

void Arcadia::PhysicsComponent::BuildIdentifiableJphBodyInfoInitial(const Arcadia::JphBodyInfoInitial& jph_body_info_initial)
{
    _upIdentifiableJphBodyInfoInitial = std::make_unique<identifiable_jph_body_info_initial_type>(
        jph_body_info_initial
    );

    _upJphBodyInfoOngoing = std::make_unique<Arcadia::JphBodyInfoOngoing>(
        false,
        jph_body_info_initial.Position,
        jph_body_info_initial.Rotation,
        Arcadia::Vec3::Zero(),
        Arcadia::Vec3::Zero()
    );
}
