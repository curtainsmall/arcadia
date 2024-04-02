#include "pch.hpp"
#include "physics_component.hpp"

#include<string>

#include"core/math.hpp"

arcadia::physics_component::physics_component(const nlohmann::json& json):
    body_shape_color(arcadia::vec3::from_json(json.at("body_shape_color")))
{
    const auto& json_body_info_initial = json.at("jph_body_info_initial");
    if(!json_body_info_initial.is_null())
    {
        const auto& json_shape_info = json_body_info_initial.at("jph_shape_info");
        const std::string& json_shape_info_type_str = json_shape_info.at("type");
        const auto& json_shape_info_info = json_shape_info.at("info");
        auto shape_info = arcadia::match<arcadia::jph_shape_info_type>(
            json_shape_info_type_str,
            "box_shape"s,
            [&]() -> arcadia::jph_shape_info_type
        {
            return arcadia::jph_box_shape_info{
                arcadia::vec3::from_json(json_shape_info_info.at("half_extent")),
                json_shape_info_info.at("convex_radius")
            };
        },
            "capsule_shape"s,
            [&]() -> arcadia::jph_shape_info_type
        {
            return arcadia::jph_capsule_shape_info{
                json_shape_info_info.at("radius"),
                json_shape_info_info.at("half_height_of_cylinder")
            };
        },
            "cylinder"s,
            [&]() -> arcadia::jph_shape_info_type
        {
            return arcadia::jph_cylinder_shape_info{
                json_shape_info_info.at("half_height"),
                json_shape_info_info.at("radius"),
                json_shape_info_info.at("convex_radius")
            };
        },
            "sphere"s,
            [&]() -> arcadia::jph_shape_info_type
        {
            return arcadia::jph_sphere_shape_info{
                json_shape_info_info.at("radius")
            };
        }
        );

        build_identifiable_jph_body_info_initial(
            arcadia::vec3::from_json(json_body_info_initial.at("position")),
            arcadia::quat::from_json(json_body_info_initial.at("rotation")),
            JPH::EMotionType{ json_body_info_initial.at("jph_motion_type") },
            JPH::ObjectLayer{ json_body_info_initial.at("jph_object_layer") },
            shape_info
        );

    }
}

auto arcadia::physics_component::to_json() const -> nlohmann::json
{
    nlohmann::json json_body_info_initial{};
    if(has_body_info())
    {
        const auto& [uuid, body_info] = get_identifiable_jph_body_info_initial();
        auto json_shape_info = arcadia::match<nlohmann::json>(
            body_info.jph_shape_info,
            [&](const arcadia::jph_box_shape_info& info)
        {
            return nlohmann::json{
                {"type","box_shape"},
                {"info", {
                        {"half_extent",arcadia::vec3::to_json(info.half_extent)},
                        {"convex_radius",info.convex_radius}
                    }
                }
            };
        },
            [&](const arcadia::jph_capsule_shape_info& info)
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
            [&](const arcadia::jph_cylinder_shape_info& info)
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
            [&](const arcadia::jph_sphere_shape_info& info)
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
            {"position",arcadia::vec3::to_json(body_info.position)},
            {"rotation",arcadia::quat::to_json(body_info.rotation)},
            {"jph_motion_type", arcadia::to_underlying(body_info.jph_motion_type)},
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
        {"body_shape_color",arcadia::vec3::to_json(body_shape_color)}
    };
}

auto arcadia::physics_component::snapshot() const -> memento_data_type
{
    memento_data_type memento{};

    memento.body_shape_color = body_shape_color;

    return memento;
}

void arcadia::physics_component::restore(const memento_data_type& memento)
{
    body_shape_color = memento.body_shape_color;
}

auto arcadia::physics_component::has_body_info() const -> bool
{
    return _identifiable_jph_body_info_initial_uptr.get();
}

auto arcadia::physics_component::get_identifiable_jph_body_info_initial() const -> const identifiable_jph_body_info_initial_type&
{
    ARCADIA_ASSERT(has_body_info());
    return *_identifiable_jph_body_info_initial_uptr;
}

auto arcadia::physics_component::get_jph_body_info_ongoing() const -> const arcadia::jph_body_info_ongoing&
{
    ARCADIA_ASSERT(has_body_info());
    return *_jph_body_info_ongoing_uptr;
}

auto arcadia::physics_component::get_jph_body_info_ongoing() -> arcadia::jph_body_info_ongoing&
{
    ARCADIA_ASSERT(has_body_info());
    return *_jph_body_info_ongoing_uptr;
}

void arcadia::physics_component::build_identifiable_jph_body_info_initial(
    const glm::vec3& position,
    const glm::quat& rotation,
    JPH::EMotionType jph_motion_type,
    JPH::ObjectLayer jph_object_layer,
    const arcadia::jph_shape_info_type& jph_shape_info
)
{
    _identifiable_jph_body_info_initial_uptr = std::make_unique<identifiable_jph_body_info_initial_type>(
        position,
        rotation,
        jph_motion_type,
        jph_object_layer,
        jph_shape_info
    );

    _jph_body_info_ongoing_uptr = std::make_unique<arcadia::jph_body_info_ongoing>(
        false,
        position,
        rotation,
        arcadia::vec3::zero(),
        arcadia::vec3::zero()
    );
}

void arcadia::physics_component::build_identifiable_jph_body_info_initial(const arcadia::jph_body_info_initial& jph_body_info_initial)
{
    _identifiable_jph_body_info_initial_uptr = std::make_unique<identifiable_jph_body_info_initial_type>(
        jph_body_info_initial
    );

    _jph_body_info_ongoing_uptr = std::make_unique<arcadia::jph_body_info_ongoing>(
        false,
        jph_body_info_initial.position,
        jph_body_info_initial.rotation,
        arcadia::vec3::zero(),
        arcadia::vec3::zero()
    );
}
