#include "pch.hpp"
#include "physics_component.hpp"

#include<string>

#include"core/math.hpp"

arcadia::physics_component::physics_component(const nlohmann::json& json)
{
    const auto& json_body_info = json.at("jph_body_info");
    if(!json_body_info.is_null())
    {
        const auto& json_shape_info = json_body_info.at("jph_shape_info");
        const std::string& json_shape_info_type_str = json_shape_info.at("type");
        const auto& json_shape_info_info = json_shape_info.at("info");
        auto shape_info = arcadia::match<jph_shape_info_type>(
            json_shape_info_type_str,
            "box"s,
            [&]() -> jph_shape_info_type
        {
            return jph_box_shape_info{
                arcadia::to_jph_vec3(arcadia::vec3::from_json(json_shape_info_info.at("half_extent"))),
                json_shape_info_info.at("convex_radius")
            };
        },
            "capsule"s,
            [&]() -> jph_shape_info_type
        {
            return jph_capsule_shape_info{
                json_shape_info_info.at("radius"),
                json_shape_info_info.at("half_height_of_cylinder")
            };
        },
            "cylinder"s,
            [&]() -> jph_shape_info_type
        {
            return jph_cylinder_shape_info{
                json_shape_info_info.at("half_height"),
                json_shape_info_info.at("radius"),
                json_shape_info_info.at("convex_radius")
            };
        },
            "sphere"s,
            [&]() -> jph_shape_info_type
        {
            return jph_sphere_shape_info{
                json_shape_info_info.at("radius")
            };
        }
        );
        _identifiable_jph_body_info_uptr = std::make_unique<identifiable_jph_body_info>(
            arcadia::to_jph_vec3(arcadia::vec3::from_json(json_body_info.at("jph_position"))),
            arcadia::to_jph_quat(arcadia::quat::from_json(json_body_info.at("jph_rotation"))),
            JPH::EMotionType{ json_body_info.at("jph_motion_type") },
            JPH::ObjectLayer{ json_body_info.at("jph_object_layer") },
            shape_info
        );
    }
}

auto arcadia::physics_component::to_json() const -> nlohmann::json
{
    if(has_identifiable_jph_body_info())
    {
        const auto& [uuid, body_info] = get_identifiable_jph_body_info();
        auto json_shape_info = arcadia::match<nlohmann::json>(
            body_info.jph_shape_info,
            [&](const jph_box_shape_info& info)
        {
            return nlohmann::json{
                {"type","box"},
                {"info", {
                        {"half_extent",arcadia::vec3::to_json(arcadia::from_jph_vec3(info.half_extent))},
                        {"convex_radius",info.convex_radius}
                    }
                }
            };
        },
            [&](const jph_capsule_shape_info& info)
        {
            return nlohmann::json{
                {"type","capsule"},
                {"info", {
                        {"radius",info.radius},
                        {"half_height_of_cylinder",info.half_height_of_cylinder}
                    }
                }
            };
        },
            [&](const jph_cylinder_shape_info& info)
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
            [&](const jph_sphere_shape_info& info)
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
        return nlohmann::json{
            {"jph_body_info",{
                    {"jph_position",arcadia::vec3::to_json(arcadia::from_jph_vec3(body_info.jph_position))},
                    {"jph_rotation",arcadia::quat::to_json(arcadia::from_jph_quat(body_info.jph_rotation))},
                    {"jph_motion_type", arcadia::to_underlying(body_info.jph_motion_type)},
                    {"jph_object_layer",body_info.jph_object_layer},
                    {"jph_shape_info",json_shape_info}
                }
            }
        };
    }
    else
    {
        return nlohmann::json{
            {"jph_body_info", nullptr}
        };
    }
}
