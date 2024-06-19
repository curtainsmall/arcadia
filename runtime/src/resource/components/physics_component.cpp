#include "pch.hpp"

#include "physics_component.hpp"

#include<string>

#include"core/math.hpp"

PhysicsComponent::PhysicsComponent(const nlohmann::json& json):
    body_shape_color(vec3::from_json(json.at("body_shape_color")))
{
    const auto& json_body_info_initial = json.at("jph_body_info_initial");
    if(!json_body_info_initial.is_null())
    {
        const auto& json_shape_info = json_body_info_initial.at("jph_shape_info");
        const std::string& json_shape_info_type_str = json_shape_info.at("type");
        const auto& json_shape_info_info = json_shape_info.at("info");
        auto shape_info = match<JphShapeInfo>(
            json_shape_info_type_str,
            "box_shape"s,
            [&]() -> JphShapeInfo
        {
            return JphBoxShapeInfo{
                vec3::from_json(json_shape_info_info.at("half_extent")),
                json_shape_info_info.at("convex_radius")
            };
        },
            "capsule_shape"s,
            [&]() -> JphShapeInfo
        {
            return JphCapsuleShapeInfo{
                json_shape_info_info.at("radius"),
                json_shape_info_info.at("half_height_of_cylinder")
            };
        },
            "cylinder"s,
            [&]() -> JphShapeInfo
        {
            return JphCylinderShapeInfo{
                json_shape_info_info.at("half_height"),
                json_shape_info_info.at("radius"),
                json_shape_info_info.at("convex_radius")
            };
        },
            "sphere"s,
            [&]() -> JphShapeInfo
        {
            return JphSphereShapeInfo{
                json_shape_info_info.at("radius")
            };
        }
        );

        build_identifiable_jph_body_info(
            JPH::EMotionType{ json_body_info_initial.at("jph_motion_type") },
            JPH::ObjectLayer{ json_body_info_initial.at("jph_object_layer") },
            shape_info
        );

    }
}

auto PhysicsComponent::to_json() const -> nlohmann::json
{
    nlohmann::json json_body_info_initial{};
    if(has_body_info())
    {
        const auto& [uuid, body_info] = get_identifiable_jph_body_info();
        auto json_shape_info = match<nlohmann::json>(
            body_info.jph_shape_info,
            [&](const JphBoxShapeInfo& info)
        {
            return nlohmann::json{
                {"type","box_shape"},
                {"info", {
                        {"half_extent",vec3::to_json(info.half_extent)},
                        {"convex_radius",info.convex_radius}
                    }
                }
            };
        },
            [&](const JphCapsuleShapeInfo& info)
        {
            return nlohmann::json{
                {"type","capsule_shape"},
                {"info", {
                        {"radius",info.radius},
                        {"half_height_of_cylinder",info.half_height_of_cylinder}
                    }
                }
            };
        },
            [&](const JphCylinderShapeInfo& info)
        {
            return nlohmann::json{
                {"type","cylinder"},
                {"info", {
                        {"half_height",info.half_height},
                        {"radius",info.radius},
                        {"convex_radius",info.convex_radius}
                    }
                }

            };
        },
            [&](const JphSphereShapeInfo& info)
        {
            return nlohmann::json{
                {"type","sphere"},
                {"info", {
                        {"radius",info.radius}
                    }
                }

            };
        }
        );
        json_body_info_initial = nlohmann::json{
            {"jph_motion_type", to_underlying(body_info.jph_motion_type)},
            {"jph_object_layer",body_info.jph_object_layer},
            {"jph_shape_info",json_shape_info}
        };
    }
    else
    {
        json_body_info_initial = nullptr;
    }

    return nlohmann::json{
        {"jph_body_info_initial",json_body_info_initial},
        {"body_shape_color",vec3::to_json(body_shape_color)}
    };
}

auto PhysicsComponent::on_snapshot() const -> std::shared_ptr<MementoDataBase>
{
    auto sp_memento = std::make_shared<PhysicsComponentMementoData>();

    sp_memento->body_shape_color = body_shape_color;

    return sp_memento;
}

void PhysicsComponent::on_restore(const std::shared_ptr<MementoDataBase>& sp_memento_data)
{
    auto& memento_data = sp_memento_data->as<PhysicsComponentMementoData>();

    body_shape_color = memento_data.body_shape_color;
}

auto PhysicsComponent::has_body_info() const -> bool
{
    return !!_identifiable_jph_body_info;
}

auto PhysicsComponent::get_identifiable_jph_body_info() const -> const identifiable_jph_body_info_type&
{
    ACDA_ASSERT(has_body_info());
    return *_identifiable_jph_body_info;
}

void PhysicsComponent::build_identifiable_jph_body_info(
    JPH::EMotionType jph_motion_type,
    JPH::ObjectLayer jph_object_layer,
    const JphShapeInfo& jph_shape_info
)
{
    build_identifiable_jph_body_info({ jph_motion_type,jph_object_layer,jph_shape_info });
}

void PhysicsComponent::build_identifiable_jph_body_info(const JphBodyInfo& jph_body_info_initial)
{
    _identifiable_jph_body_info = std::make_unique<identifiable_jph_body_info_type>(
        jph_body_info_initial
    );

}

void PhysicsComponent::destroy_jph_body_info()
{
    _identifiable_jph_body_info.reset();
}
