#include "imgui_window_property.hpp"

#include<algorithm>
#include<string>

#include"core/file/pfd_header.hpp"
#include"core/memento/memento.hpp"
#include"function/ui/imgui_header.hpp"
#include"function/ui/imgui_wrapper.hpp"

auto arcadia::imgui_window_property_camera_component::operator()(arcadia::camera_component& camera_comp) -> std::string
{
    const float speed = 1.f;
    const float min = .0;
    const float max = .0f;
    const char* format = "%.3f";
    const auto flags =
        ImGuiSliderFlags_AlwaysClamp;

    std::string description{};
    ImGui::BeginGroup();

    ImGui::NewLine();
    if(arcadia::imgui_wrapper::drag_vec3(
        "Position",
        camera_comp.position,
        speed,
        min,
        max,
        format,
        flags
    ))
    {
        description = "Position";
    }

    ImGui::NewLine();
    if(arcadia::imgui_wrapper::drag_vec3(
        "Target",
        camera_comp.target,
        speed,
        min,
        max, format,
        flags
    ))
    {
        description = "Target";
    }

    ImGui::NewLine();
    if(arcadia::imgui_wrapper::drag_vec3(
        "Up",
        camera_comp.up,
        speed,
        min,
        max,
        format,
        flags
    ))
    {
        description = "Up";
    }

    ImGui::NewLine();
    if(arcadia::imgui_wrapper::drag_float(
        "Near Plane",
        camera_comp.near_plane,
        speed,
        min,
        max,
        format,
        flags
    ))
    {
        description = "Near Plane";
    }

    ImGui::NewLine();
    if(arcadia::imgui_wrapper::drag_float(
        "Far Plane",
        camera_comp.far_plane,
        speed,
        min,
        max,
        format,
        flags
    ))
    {
        description = "Far Plane";
    }

    ImGui::NewLine();
    if(arcadia::imgui_wrapper::drag_float(
        "FOV",
        camera_comp.fov,
        speed,
        camera_comp.fov_min,
        camera_comp.fov_max,
        format,
        flags
    ))
    {
        description = "FOV";
    }

    ImGui::NewLine();
    if(arcadia::imgui_wrapper::drag_float(
        "FOV Min",
        camera_comp.fov_min,
        speed,
        .0f,
        180.f,
        format,
        flags
    ))
    {
        description = "FOV Min";
    }

    ImGui::NewLine();
    if(arcadia::imgui_wrapper::drag_float(
        "FOV Max",
        camera_comp.fov_max,
        speed,
        .0f,
        180.f,
        format,
        flags
    ))
    {
        description = "FOV Max";
    }

    ImGui::NewLine();
    if(arcadia::imgui_wrapper::drag_float(
        "Speed",
        camera_comp.speed,
        speed,
        min,
        max,
        format,
        flags
    ))
    {
        description = "Speed";
    }

    ImGui::NewLine();
    if(arcadia::imgui_wrapper::drag_ivec2(
        "Viewport Size",
        camera_comp.viewport_size,
        speed,
        1.f,
        std::numeric_limits<float>::max(),
        format,
        flags
    ))
    {
        description = "Viewport Size";
    }

    ImGui::NewLine();
    if(arcadia::imgui_wrapper::checkbox(
        "Fixed Up",
        camera_comp.fixed_up
    ))
    {
        description = "Fixed Up";
    }

    ImGui::NewLine();
    if(arcadia::imgui_wrapper::drag_float(
        "Up Epsilon",
        camera_comp.up_epsilon,
        speed,
        min,
        max,
        format,
        flags
    ))
    {
        description = "Up Epsilon";
    }

    ImGui::EndGroup();

    return description;
}

auto arcadia::imgui_window_property_light_component::operator()(arcadia::light_component& light_comp) -> std::string
{
    std::string description{};
    ImGui::BeginGroup();

    const float light_direction_drag_speed = .01f;
    const float light_direction_min = -1.f;
    const float light_direction_max = 1.f;
    const float light_color_drag_speed = .005f;
    const float light_color_min = .0f;
    const float light_color_max = 1.f;
    const float light_strength_speed = 1.f;
    const float light_strength_min = 0.f;
    const float light_strength_max = 100.f;

    const float speed = 1.f;
    const float min = .0f;
    const float max = .0f;
    const auto format = "%.3f";
    const auto flags =
        ImGuiSliderFlags_AlwaysClamp;
    if(arcadia::match<bool>(
        light_comp.light,
        [&](arcadia::null_light&)
    {
        ImGui::Text("Light Type");
        ImGui::SameLine();
        if(ImGui::BeginCombo("##light_type", "(No light)"))
        {
            if(ImGui::Selectable("Spot Light"))
            {
                light_comp.light = arcadia::spot_light{};
                ImGui::EndCombo();
                return true;
            }
            if(ImGui::Selectable("Direct Light"))
            {
                light_comp.light = arcadia::direct_light{};
                ImGui::EndCombo();
                return true;
            }
            if(ImGui::Selectable("Area Light"))
            {
                light_comp.light = arcadia::area_light{};
                ImGui::EndCombo();
                return true;
            }
            if(ImGui::Selectable("Point Light"))
            {
                light_comp.light = arcadia::point_light{};
                ImGui::EndCombo();
                return true;
            }
            ImGui::EndCombo();
        }
        return false;
    },
        [&](arcadia::spot_light& light)
    {
        bool edited{ false };

        ImGui::Text("         Light Type");
        ImGui::SameLine();
        if(ImGui::BeginCombo("##light_type", "Spot Light"))
        {
            if(ImGui::Selectable("Direct Light"))
            {
                auto res = pfd::message{
                    "Changing Light Type",
                    "Do you want to change light type to Direct Light? All properties for current light will be lost",
                    pfd::choice::yes_no,
                    pfd::icon::info
                }.result();
                if(res == pfd::button::yes)
                {
                    light_comp.light = arcadia::direct_light{};
                    ImGui::EndCombo();
                    return true;
                }
            }
            if(ImGui::Selectable("Area Light"))
            {
                auto res = pfd::message{
                    "Changing Light Type",
                    "Do you want to change light type to Area Light? All properties for current light will be lost",
                    pfd::choice::yes_no,
                    pfd::icon::info
                }.result();
                if(res == pfd::button::yes)
                {
                    light_comp.light = arcadia::area_light{};
                    ImGui::EndCombo();
                    return true;
                }
            }
            if(ImGui::Selectable("Point Light"))
            {
                auto res = pfd::message{
                    "Changing Light Type",
                    "Do you want to change light type to Point Light? All properties for current light will be lost",
                    pfd::choice::yes_no,
                    pfd::icon::info
                }.result();
                if(res == pfd::button::yes)
                {
                    light_comp.light = arcadia::point_light{};
                    ImGui::EndCombo();
                    return true;
                }
            }
            ImGui::EndCombo();
        }

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::drag_vec3(
            "Position",
            light.position,
            speed,
            min,
            max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::drag_vec3(
            "Direction",
            light.direction,
            light_direction_drag_speed,
            light_direction_min,
            light_direction_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::drag_float(
            "Attenuation Contant",
            light.attenuation_coefs.x,
            speed,
            min,
            max,
            format,
            flags
        );
        edited |= arcadia::imgui_wrapper::drag_float(
            "             Linear",
            light.attenuation_coefs.x,
            speed,
            min,
            max,
            format,
            flags
        );
        edited |= arcadia::imgui_wrapper::drag_float(
            "          Quadratic",
            light.attenuation_coefs.x,
            speed,
            min,
            max,
            format,
            flags
        );

        ImGui::NewLine();

        glm::vec2 cutoff_angles_degree{
            glm::degrees(light.cutoff_angles.x),
            glm::degrees(light.cutoff_angles.y)
        };
        const float cutoff_angle_drag_speend = .1f;
        const float cutoff_angle_min = 0.f;
        const float cutoff_angle_max = 180.f;
        edited |= arcadia::imgui_wrapper::drag_float(
            "Inner Cutoff Angle",
            light.attenuation_coefs.x,
            cutoff_angle_drag_speend,
            cutoff_angle_min,
            cutoff_angle_max,
            format,
            flags
        );
        edited |= arcadia::imgui_wrapper::drag_float(
            "Outer Cutoff Angle",
            light.attenuation_coefs.x,
            cutoff_angle_drag_speend,
            cutoff_angle_min,
            cutoff_angle_max,
            format,
            flags
        );
        light.cutoff_angles = glm::vec2{
            glm::radians(cutoff_angles_degree.x),
            glm::radians(cutoff_angles_degree.y)
        };

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::color_edit3(
            "Color",
            light.color
        );

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::drag_vec3_color(
            "Ambient Strength",
            light.ambient_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::drag_vec3_color(
            "Diffuse Strength",
            light.diffuse_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::drag_vec3_color(
            "Specular Strength",
            light.specular_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        return edited;
    },
        [&](arcadia::direct_light& light)
    {
        bool edited{ false };

        ImGui::Text("         Light Type");
        ImGui::SameLine();
        if(ImGui::BeginCombo("##light_type", "Direct Light"))
        {
            if(ImGui::Selectable("Spot Light"))
            {
                auto res = pfd::message{
                    "Changing Light Type",
                    "Do you want to change light type to Spot Light? All properties for current light will be lost",
                    pfd::choice::yes_no,
                    pfd::icon::info
                }.result();
                if(res == pfd::button::yes)
                {
                    light_comp.light = arcadia::spot_light{};
                    ImGui::EndCombo();
                    return true;
                }
            }
            if(ImGui::Selectable("Area Light"))
            {
                auto res = pfd::message{
                    "Changing Light Type",
                    "Do you want to change light type to Area Light? All properties for current light will be lost",
                    pfd::choice::yes_no,
                    pfd::icon::info
                }.result();
                if(res == pfd::button::yes)
                {
                    light_comp.light = arcadia::area_light{};
                    ImGui::EndCombo();
                    return true;
                }
            }
            if(ImGui::Selectable("Point Light"))
            {
                auto res = pfd::message{
                    "Changing Light Type",
                    "Do you want to change light type to Point Light? All properties for current light will be lost",
                    pfd::choice::yes_no,
                    pfd::icon::info
                }.result();
                if(res == pfd::button::yes)
                {
                    light_comp.light = arcadia::point_light{};
                    ImGui::EndCombo();
                    return true;
                }
            }
            ImGui::EndCombo();
        }

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::drag_vec3(
            "Direction",
            light.direction,
            light_direction_drag_speed,
            light_direction_min,
            light_direction_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::color_edit3(
            "Color",
            light.color
        );

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::drag_vec3_color(
            "Ambient Strengt",
            light.ambient_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::drag_vec3_color(
            "Diffuse Strength",
            light.diffuse_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::drag_vec3_color(
            "Specular Strength",
            light.specular_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );
        return edited;
    },
        [&](arcadia::area_light& light)
    {
        bool edited{ false };

        ImGui::Text("         Light Type");
        ImGui::SameLine();
        if(ImGui::BeginCombo("##light_type", "Area Light"))
        {
            if(ImGui::Selectable("Spot Light"))
            {
                auto res = pfd::message{
                    "Changing Light Type",
                    "Do you want to change light type to Spot Light? All properties for current light will be lost",
                    pfd::choice::yes_no,
                    pfd::icon::info
                }.result();
                if(res == pfd::button::yes)
                {
                    light_comp.light = arcadia::spot_light{};
                    ImGui::EndCombo();
                    return true;
                }
            }
            if(ImGui::Selectable("Direct Light"))
            {
                auto res = pfd::message{
                    "Changing Light Type",
                    "Do you want to change light type to Direct Light? All properties for current light will be lost",
                    pfd::choice::yes_no,
                    pfd::icon::info
                }.result();
                if(res == pfd::button::yes)
                {
                    light_comp.light = arcadia::direct_light{};
                    ImGui::EndCombo();
                    return true;
                }
            }
            if(ImGui::Selectable("Point Light"))
            {
                auto res = pfd::message{
                    "Changing Light Type",
                    "Do you want to change light type to Point Light? All properties for current light will be lost",
                    pfd::choice::yes_no,
                    pfd::icon::info
                }.result();
                if(res == pfd::button::yes)
                {
                    light_comp.light = arcadia::point_light{};
                    ImGui::EndCombo();
                    return true;
                }
            }
            ImGui::EndCombo();
        }

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::drag_vec3(
            "Position",
            light.position,
            speed,
            min,
            max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::drag_vec3(
            "Direction",
            light.direction,
            light_direction_drag_speed,
            light_direction_min,
            light_direction_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::drag_float(
            "Width",
            light.size.x,
            speed,
            min,
            max,
            format,
            flags
        );
        edited |= arcadia::imgui_wrapper::drag_float(
            "Height",
            light.size.y,
            speed,
            min,
            max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::color_edit3(
            "Color",
            light.color
        );

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::drag_vec3_color(
            "Ambient Strength",
            light.ambient_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::drag_vec3_color(
            "Diffuse Strength",
            light.diffuse_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::drag_vec3_color(
            "Specular Strength",
            light.specular_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );
        return edited;
    },
        [&](arcadia::point_light& light)
    {
        bool edited{ false };

        ImGui::Text("         Light Type");
        ImGui::SameLine();
        if(ImGui::BeginCombo("##light_type", "Point Light"))
        {
            if(ImGui::Selectable("Spot Light"))
            {
                auto res = pfd::message{
                    "Changing Light Type",
                    "Do you want to change light type to Spot Light? All properties for current light will be lost",
                    pfd::choice::yes_no,
                    pfd::icon::info
                }.result();
                if(res == pfd::button::yes)
                {
                    light_comp.light = arcadia::spot_light{};
                    ImGui::EndCombo();
                    return true;
                }
            }
            if(ImGui::Selectable("Direct Light"))
            {
                auto res = pfd::message{
                    "Changing Light Type",
                    "Do you want to change light type to Direct Light? All properties for current light will be lost",
                    pfd::choice::yes_no,
                    pfd::icon::info
                }.result();
                if(res == pfd::button::yes)
                {
                    light_comp.light = arcadia::direct_light{};
                    ImGui::EndCombo();
                    return true;
                }
            }
            if(ImGui::Selectable("Area Light"))
            {
                auto res = pfd::message{
                    "Changing Light Type",
                    "Do you want to change light type to Area Light? All properties for current light will be lost",
                    pfd::choice::yes_no,
                    pfd::icon::info
                }.result();
                if(res == pfd::button::yes)
                {
                    light_comp.light = arcadia::area_light{};
                    ImGui::EndCombo();
                    return true;
                }
            }
            ImGui::EndCombo();
        }

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::drag_vec3(
            "Position",
            light.position,
            speed,
            min,
            max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::drag_float(
            "Attenuation Contant",
            light.attenuation_coefs.x,
            speed,
            min,
            max,
            format,
            flags
        );
        edited |= arcadia::imgui_wrapper::drag_float(
            "             Linear",
            light.attenuation_coefs.x,
            speed,
            min,
            max,
            format,
            flags
        );
        edited |= arcadia::imgui_wrapper::drag_float(
            "          Quadratic",
            light.attenuation_coefs.x,
            speed,
            min,
            max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::color_edit3(
            "Color",
            light.color
        );

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::drag_vec3_color(
            "Ambient Strength",
            light.ambient_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::drag_vec3_color(
            "Diffuse Strength",
            light.diffuse_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= arcadia::imgui_wrapper::drag_vec3_color(
            "Specular Strength",
            light.specular_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        return edited;
    }
    )){
        description = "Light";
    }

    ImGui::EndGroup();

    return description;
}

auto arcadia::imgui_window_property_model_component::operator()(arcadia::model_component& model_comp) -> std::string
{
    std::string description{};
    ImGui::BeginGroup();
    const float speed = 1.f;
    const float min = .0f;
    const float max = .0f;
    const char* format = "%.3f";
    const auto flags =
        ImGuiSliderFlags_AlwaysClamp;

    ImGui::SeparatorText("Filepath");
    auto filepath_str = model_comp.get_filepath().empty()
        ? "(No filepath)"s
        : model_comp.get_filepath().generic_string();
    ImGui::TextWrapped(filepath_str.c_str());
    if(ImGui::Button("..."))
    {
        auto res = pfd::open_file{
            "Import Model"
        }.result();
        model_comp.import(res.size() ? res.at(0) : ""s);
    }

    ImGui::SeparatorText("Transform");
    ImGui::NewLine();
    if(arcadia::imgui_wrapper::drag_vec3(
        "Location",
        model_comp.location,
        speed,
        min,
        max,
        format,
        flags
    ))
    {
        description = "Location";
    }

    ImGui::NewLine();
    float rotation_drag_speed{ .05f };
    if(arcadia::imgui_wrapper::drag_quat_normalized(
        "Rotation",
        model_comp.rotation,
        rotation_drag_speed,
        min,
        max,
        format,
        flags
    ))
    {
        description = "Rotation";
    }

    ImGui::NewLine();
    if(arcadia::imgui_wrapper::drag_vec3(
        "Scale",
        model_comp.scale,
        speed,
        min,
        max,
        format,
        flags
    ))
    {
        description = "Scale";
    }

    ImGui::NewLine();
    if(arcadia::imgui_wrapper::drag_vec3(
        "Pivot",
        model_comp.pivot,
        speed,
        min,
        max,
        format,
        flags
    ))
    {
        description = "Pivot";
    }

    ImGui::EndGroup();

    return description;
}

void arcadia::imgui_window_popup_physics_component_create_body::operator()(arcadia::physics_component& physics_comp)
{
    if(!open)
    {
        return;
    }

    auto imgui_window_title = "Physics Component - Create Body"s;

    auto popup_flags =
        ImGuiPopupFlags_NoOpenOverExistingPopup;
    ImGui::OpenPopup(imgui_window_title.c_str(), popup_flags);

    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::BeginPopupModal(imgui_window_title.c_str(), &open, window_flags))
    {
        float speed = .05f;
        float min = .0f;
        float max = .0f;
        const char* format = "%.3f";
        auto slider_flags =
            ImGuiSliderFlags_AlwaysClamp;

        // Position
        auto pos = _temp_jph_body_info_initial.position;
        arcadia::imgui_wrapper::drag_vec3("Position", pos, speed, min, max, format, slider_flags);
        _temp_jph_body_info_initial.position = pos;

        // Rotation
        ImGui::NewLine();
        auto rot = _temp_jph_body_info_initial.rotation;
        float
            rot_speed = .05f,
            rot_min = -1.f,
            rot_max = 1.f;
        arcadia::imgui_wrapper::drag_quat_normalized("Rotation", rot, rot_speed, rot_min, rot_max, format, slider_flags);
        _temp_jph_body_info_initial.rotation = rot;

        // Motion type
        ImGui::NewLine();
        auto jph_motion_type_preview = arcadia::match<std::string>(
            _temp_jph_body_info_initial.jph_motion_type,
            JPH::EMotionType::Static,
            "Static"s,
            JPH::EMotionType::Dynamic,
            "Dynamic"s,
            JPH::EMotionType::Kinematic,
            "Kinematic"s
        );
        ImGui::Text(" Motion Type");
        ImGui::SameLine();
        if(ImGui::BeginCombo("##motion_type", jph_motion_type_preview.c_str()))
        {
            if(ImGui::Selectable("Static"))
            {
                _temp_jph_body_info_initial.jph_motion_type = JPH::EMotionType::Static;
            }
            if(ImGui::Selectable("Dynamic"))
            {
                _temp_jph_body_info_initial.jph_motion_type = JPH::EMotionType::Dynamic;
            }
            if(ImGui::Selectable("Kinematic"))
            {
                _temp_jph_body_info_initial.jph_motion_type = JPH::EMotionType::Kinematic;
            }
            ImGui::EndCombo();
        }

        // Object layer
        auto jph_object_layer_preview = arcadia::match<std::string>(
            _temp_jph_body_info_initial.jph_object_layer,
            arcadia::jph_object_layers::non_moving,
            "Non Moving",
            arcadia::jph_object_layers::moving,
            "Moving"
        );
        ImGui::Text("Object Layer");
        ImGui::SameLine();
        if(ImGui::BeginCombo("##object_layer", jph_object_layer_preview.c_str()))
        {
            if(ImGui::Selectable("Non Moving"))
            {
                _temp_jph_body_info_initial.jph_object_layer = arcadia::jph_object_layers::non_moving;
            }
            if(ImGui::Selectable("Moving"))
            {
                _temp_jph_body_info_initial.jph_object_layer = arcadia::jph_object_layers::moving;
            }
            ImGui::EndCombo();
        }

        //Shape
        _temp_jph_body_info_initial.jph_shape_info = arcadia::match<arcadia::jph_shape_info_type>(
            _temp_jph_body_info_initial.jph_shape_info,
            [&](arcadia::jph_box_shape_info& info) -> arcadia::jph_shape_info_type
        {
            ImGui::Text("  Shape Type");
            ImGui::SameLine();
            if(ImGui::BeginCombo("##shape_type", "Box Shape"))
            {
                ImGui::Selectable("Box Shape");
                if(ImGui::Selectable("Capsule Shape"))
                {
                    ImGui::EndCombo();
                    return arcadia::jph_capsule_shape_info{};
                }
                if(ImGui::Selectable("Cylinder Shape"))
                {
                    ImGui::EndCombo();
                    return arcadia::jph_cylinder_shape_info{};
                }
                if(ImGui::Selectable("Sphere Shape"))
                {
                    ImGui::EndCombo();
                    return arcadia::jph_sphere_shape_info{};
                }
                ImGui::EndCombo();
            }
            ImGui::NewLine();
            ImGui::SeparatorText("Box Shape");

            // Half extent
            float
                half_extent_x = info.half_extent.x,
                half_extent_y = info.half_extent.y,
                half_extent_z = info.half_extent.z;
            auto half_extent_min = std::max({ .01f,info.convex_radius });
            auto half_extent_max = (std::numeric_limits<float>::max)();
            ImGui::Text("Half Extent X"); ImGui::SameLine(); ImGui::DragFloat("##half_extent_x", &half_extent_x, speed, half_extent_min, half_extent_max, format, slider_flags);
            ImGui::Text("            Y"); ImGui::SameLine(); ImGui::DragFloat("##half_extent_y", &half_extent_y, speed, half_extent_min, half_extent_max, format, slider_flags);
            ImGui::Text("            Z"); ImGui::SameLine(); ImGui::DragFloat("##half_extent_z", &half_extent_z, speed, half_extent_min, half_extent_max, format, slider_flags);
            info.half_extent = glm::vec3{ half_extent_x,half_extent_y,half_extent_z };

            // Convex radius
            ImGui::NewLine();
            auto convex_radius_min = .0f;
            auto convex_radius_max = std::min({ half_extent_x, half_extent_y, half_extent_z });
            ImGui::Text("Convex Radius"); ImGui::SameLine(); ImGui::DragFloat("##convex_radius", &info.convex_radius, speed, convex_radius_min, convex_radius_max, format, slider_flags);

            return _temp_jph_body_info_initial.jph_shape_info;
        },
            [&](arcadia::jph_capsule_shape_info& info) -> arcadia::jph_shape_info_type
        {
            ImGui::Text("  Shape Type");
            ImGui::SameLine();
            if(ImGui::BeginCombo("##shape_type", "Capsule Shape"))
            {
                if(ImGui::Selectable("Box Shape"))
                {
                    ImGui::EndCombo();
                    return arcadia::jph_box_shape_info{};
                }
                ImGui::Selectable("Capsule Shape");
                if(ImGui::Selectable("Cylinder Shape"))
                {
                    ImGui::EndCombo();
                    return arcadia::jph_cylinder_shape_info{};
                }
                if(ImGui::Selectable("Sphere Shape"))
                {
                    ImGui::EndCombo();
                    return arcadia::jph_sphere_shape_info{};
                }
                ImGui::EndCombo();
            }
            ImGui::NewLine();
            ImGui::SeparatorText("Capsule Type");

            auto radius_min = .0f;
            auto radius_max = (std::numeric_limits<float>::max)();
            ImGui::Text("                 Radius"); ImGui::SameLine(); ImGui::DragFloat("##radius", &info.radius, speed, radius_min, radius_max, format, slider_flags);

            ImGui::NewLine();
            auto half_height_of_cylinder_min = .0f;
            auto half_height_of_cylinder_max = (std::numeric_limits<float>::max)();
            ImGui::Text("Half Height of Cylinder"); ImGui::SameLine(); ImGui::DragFloat("##half_height_of_cylinder", &info.half_height_of_cylinder, speed, half_height_of_cylinder_min, half_height_of_cylinder_max, format, slider_flags);

            return _temp_jph_body_info_initial.jph_shape_info;
        },
            [&](arcadia::jph_cylinder_shape_info& info) -> arcadia::jph_shape_info_type
        {
            ImGui::Text("  Shape Type");
            ImGui::SameLine();
            if(ImGui::BeginCombo("##shape_type", "Cylinder Shape"))
            {
                if(ImGui::Selectable("Box Shape"))
                {
                    ImGui::EndCombo();
                    return arcadia::jph_box_shape_info{};
                }
                if(ImGui::Selectable("Capsule Shape"))
                {
                    ImGui::EndCombo();
                    return arcadia::jph_capsule_shape_info{};
                }
                ImGui::Selectable("Cylinder Shape");
                if(ImGui::Selectable("Sphere Shape"))
                {
                    ImGui::EndCombo();
                    return arcadia::jph_sphere_shape_info{};
                }
                ImGui::EndCombo();
            }
            ImGui::NewLine();
            ImGui::SeparatorText("Cylinder Shape");

            auto half_height_min = .0f;
            auto half_height_max = (std::numeric_limits<float>::max)();
            ImGui::Text("  Half Height"); ImGui::SameLine(); ImGui::DragFloat("##half_height", &info.half_height, speed, half_height_min, half_height_max, format, slider_flags);

            ImGui::NewLine();
            auto radius_min = .0f;
            auto radius_max = (std::numeric_limits<float>::max)();
            ImGui::Text("       Radius"); ImGui::SameLine(); ImGui::DragFloat("##radius", &info.radius, speed, radius_min, radius_max, format, slider_flags);

            ImGui::NewLine();
            auto convex_radius_min = .0f;
            auto convex_radius_max = (std::numeric_limits<float>::max)();
            ImGui::Text("Convex Radius"); ImGui::SameLine(); ImGui::DragFloat("##convex_radius", &info.convex_radius, speed, convex_radius_min, convex_radius_max, format, slider_flags);

            return _temp_jph_body_info_initial.jph_shape_info;
        },
            [&](arcadia::jph_sphere_shape_info& info) -> arcadia::jph_shape_info_type
        {
            ImGui::Text("  Shape Type");
            ImGui::SameLine();
            if(ImGui::BeginCombo("##shape_type", "Sphere Shape"))
            {
                if(ImGui::Selectable("Box Shape"))
                {
                    ImGui::EndCombo();
                    return arcadia::jph_box_shape_info{};
                }
                if(ImGui::Selectable("Capsule Shape"))
                {
                    ImGui::EndCombo();
                    return arcadia::jph_capsule_shape_info{};
                }
                if(ImGui::Selectable("Cylinder Shape"))
                {
                    ImGui::EndCombo();
                    return arcadia::jph_cylinder_shape_info{};
                }
                ImGui::Selectable("Sphere Shape");
                ImGui::EndCombo();
            }
            ImGui::NewLine();
            ImGui::SeparatorText("Sphere Shape");

            auto radius_min = .0f;
            auto radius_max = (std::numeric_limits<float>::max)();
            ImGui::Text("   Radius"); ImGui::SameLine(); ImGui::DragFloat("##radius", &info.radius, speed, radius_min, radius_max, format, slider_flags);

            return _temp_jph_body_info_initial.jph_shape_info;
        }
        );

        ImGui::NewLine();
        auto confirmed = ImGui::Button("Confirm");
        if(confirmed)
        {
            physics_comp.build_identifiable_jph_body_info_initial(
                _temp_jph_body_info_initial
            );

        }
        ImGui::SameLine();
        if(confirmed || ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            open = false;
            _temp_jph_body_info_initial = arcadia::jph_body_info_initial{};
        }

        ImGui::EndPopup();
    }
}

auto arcadia::imgui_window_property_physics_component::operator()(arcadia::physics_component& physics_comp) -> std::string
{
    _imgui_window_popup_physics_component_create_body(physics_comp);

    std::string description{};
    ImGui::BeginGroup();

    if(ImGui::BeginTabBar("##physics_comp"))
    {
        if(ImGui::BeginTabItem("State"))
        {
            if(physics_comp.has_body_info())
            {
                const auto& [uuid, jph_body_info_initial] = physics_comp.get_identifiable_jph_body_info_initial();

                ImGui::SeparatorText("Initial");

                arcadia::imgui_wrapper::text_vec3("Position", jph_body_info_initial.position);

                ImGui::NewLine();
                arcadia::imgui_wrapper::text_quat("Rotation", jph_body_info_initial.rotation);

                ImGui::NewLine();
                ImGui::Text(std::format(
                    "Motion Type: {}",
                    arcadia::match<std::string>(
                        jph_body_info_initial.jph_motion_type,
                        JPH::EMotionType::Static,
                        [&]()
                {
                    return "Static";
                },
                        JPH::EMotionType::Dynamic,
                        [&]()
                {
                    return "Dynamic";
                },
                        JPH::EMotionType::Kinematic,
                        [&]()
                {
                    return "Kinematic";
                }
                    )
                ).c_str());
                ImGui::Text(std::format("Object Layer: {}", jph_body_info_initial.jph_object_layer).c_str());

                ImGui::SeparatorText("Ongoing");

                const auto& jph_body_info_ongoing = physics_comp.get_jph_body_info_ongoing();
                ImGui::Text(std::format("Active: {}", jph_body_info_ongoing.active).c_str());

                ImGui::NewLine();
                arcadia::imgui_wrapper::text_vec3("Position", jph_body_info_ongoing.position);

                ImGui::NewLine();
                arcadia::imgui_wrapper::text_quat("Rotation", jph_body_info_ongoing.rotation);

                ImGui::NewLine();
                arcadia::imgui_wrapper::text_vec3("Linear Velocity", jph_body_info_ongoing.linear_velocity);

                ImGui::NewLine();
                arcadia::imgui_wrapper::text_vec3("Angular Velocity", jph_body_info_ongoing.angular_velocity);

                arcadia::match<void>(
                    jph_body_info_initial.jph_shape_info,
                    [&](const arcadia::jph_box_shape_info& info)
                {
                    ImGui::SeparatorText("Body Shape - Box");
                    ImGui::Text(std::format("Half Extent: {}", info.half_extent).c_str());
                    ImGui::Text(std::format("Convex Radius: {:.2f}", info.convex_radius).c_str());
                },
                    [&](const arcadia::jph_capsule_shape_info& info)
                {
                    ImGui::SeparatorText("Body Shape - Capsule");
                    ImGui::Text(std::format("Radius: {:.2f}", info.radius).c_str());
                    ImGui::Text(std::format("Half Height of Cylinder: {:.2f}", info.half_height_of_cylinder).c_str());
                },
                    [&](const arcadia::jph_cylinder_shape_info& info)
                {
                    ImGui::SeparatorText("Body Shape - Cylinder");
                    ImGui::Text(std::format("Half Height: {:.2f}", info.half_height).c_str());
                    ImGui::Text(std::format("Radius: {:.2f}", info.radius).c_str());
                    ImGui::Text(std::format("Convex Radius: {:.2f}", info.convex_radius).c_str());
                },
                    [&](const arcadia::jph_sphere_shape_info& info)
                {
                    ImGui::SeparatorText("Body Shape - Sphere");
                    ImGui::Text(std::format("Radius: {:.2f}", info.radius).c_str());
                }
                );
            }
            else
            {
                ImGui::Text("(No body state)");
            }
            ImGui::EndTabItem();
        }

        if(ImGui::BeginTabItem("Edit"))
        {
            if(physics_comp.has_body_info())
            {
                if(arcadia::imgui_wrapper::color_edit3(
                    "Body Shape Color",
                    physics_comp.body_shape_color
                ))
                {
                    description = "Body Shape Color";
                }

                ImGui::NewLine();
                if(ImGui::Button("Recreate Body"))
                {
                    _imgui_window_popup_physics_component_create_body.open = true;
                }
            }
            else
            {
                if(ImGui::Button("Create Body"))
                {
                    _imgui_window_popup_physics_component_create_body.open = true;
                }
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::EndGroup();

    return description;
}

void arcadia::imgui_window_property::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
        .dispatch<arcadia::event::open_imgui_window>(ARCADIA_BIND_MEMBER_FN(_on_open_imgui_window))
        .dispatch<arcadia::event::scene_activated>(ARCADIA_BIND_MEMBER_FN(_on_scene_activated))
        .dispatch<arcadia::event::scene_deactivated>(ARCADIA_BIND_MEMBER_FN(_on_scene_deactivated))
        .dispatch<arcadia::event::select_entity>(ARCADIA_BIND_MEMBER_FN(_on_select_entity))
        .dispatch<arcadia::event::delete_entity>(ARCADIA_BIND_MEMBER_FN(_on_delete_entity))
        .dispatch<arcadia::event::physics_simulator_built>(ARCADIA_BIND_MEMBER_FN(_on_physics_simulator_built))
        .dispatch<arcadia::event::physics_simulator_unbuilt>(ARCADIA_BIND_MEMBER_FN(_on_physics_simulator_unbuilt))
        .result();
}

void arcadia::imgui_window_property::on_update()
{
    if(!_open)
    {
        return;
    }

    auto has_scene = !_scene_wptr.expired();
    auto scene_sptr = _scene_wptr.lock();

    auto imgui_title = has_scene && _selected_entity != entt::null
        ? _title + " - " + scene_sptr->get_name_of_entity(_selected_entity) + get_id_str()
        : _title + get_id_str();

    ImGui::SetNextWindowSize(glm::vec2{ 1024,768 }, ImGuiCond_Once);
    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_title.c_str(), &_open, window_flags))
    {
        if(!has_scene)
        {
            ImGui::Text("(No scene selected)");
        }
        else
        {
            auto tab_bar_flags =
                ImGuiTabBarFlags_NoCloseWithMiddleMouseButton
                | ImGuiTabBarFlags_TabListPopupButton
                | ImGuiTabBarFlags_AutoSelectNewTabs
                | ImGuiTabBarFlags_FittingPolicyScroll
                | ImGuiTabBarFlags_Reorderable;
            if(_selected_entity != entt::null && ImGui::BeginTabBar("##component_name", tab_bar_flags))
            {
                ImGui::PushItemWidth(200.f);

                auto& memento_list = arcadia::memento_list::instance();
                if(_contains_component<arcadia::camera_component>() && ImGui::BeginTabItem("Camera"))
                {
                    auto description = _imgui_window_property_camera_component(_get_component<arcadia::camera_component>());
                    if(!description.empty())
                    {
                        memento_list
                            .snapshot<arcadia::camera_component_memento, arcadia::camera_component>(
                                std::format("Camera - {}", description),
                                [&]() -> arcadia::camera_component&
                        {
                            return _get_component<arcadia::camera_component>();
                        }
                        );
                    }
                    ImGui::EndTabItem();
                }
                if(_contains_component<arcadia::light_component>() && ImGui::BeginTabItem("Light"))
                {
                    auto description = _imgui_window_property_light_component(_get_component<arcadia::light_component>());
                    if(!description.empty())
                    {
                        memento_list
                            .snapshot<arcadia::light_component_memento, arcadia::light_component>(
                                std::format("Light - {}", description),
                                [&]() -> arcadia::light_component&
                        {
                            return _get_component<arcadia::light_component>();
                        }
                        );
                    }
                    ImGui::EndTabItem();
                }
                if(_contains_component<arcadia::model_component>() && ImGui::BeginTabItem("Model"))
                {
                    auto description = _imgui_window_property_model_component(_get_component<arcadia::model_component>());
                    if(!description.empty())
                    {
                        memento_list
                            .snapshot<arcadia::model_component_memento, arcadia::model_component>(
                                std::format("Model - {}", description),
                                [&]() -> arcadia::model_component&
                        {
                            return _get_component<arcadia::model_component>();
                        }
                        );
                    }
                    ImGui::EndTabItem();
                }
                if(_contains_component<arcadia::physics_component>() && ImGui::BeginTabItem("Physics"))
                {
                    auto description = _imgui_window_property_physics_component(_get_component<arcadia::physics_component>());
                    if(!description.empty())
                    {
                        memento_list
                            .snapshot<arcadia::physics_component_memento, arcadia::physics_component>(
                                std::format("Physics - {}", description),
                                [&]() -> arcadia::physics_component&
                        {
                            return _get_component<arcadia::physics_component>();
                        }
                        );
                    }
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();

                ImGui::PopItemWidth();

            }

        }
    }
    ImGui::End();
}

void arcadia::imgui_window_property::_on_open_imgui_window(arcadia::event::open_imgui_window& e)
{
    const auto& [id_str] = e.data_tuple;
    if(id_str == get_id_str())
    {
        _open = true;
    }
}

void arcadia::imgui_window_property::_on_scene_activated(arcadia::event::scene_activated& e)
{
    const auto& [scene_wptr] = e.data_tuple;
    _scene_wptr = scene_wptr;
}

void arcadia::imgui_window_property::_on_scene_deactivated(arcadia::event::scene_deactivated& e)
{
    _scene_wptr.reset();
    _selected_entity = entt::null;
}

void arcadia::imgui_window_property::_on_select_entity(arcadia::event::select_entity& e)
{
    const auto& [entity] = e.data_tuple;
    _selected_entity = entity;
}

void arcadia::imgui_window_property::_on_delete_entity(arcadia::event::delete_entity& e)
{
    const auto& [entity] = e.data_tuple;
    if(_selected_entity == entity)
    {
        _selected_entity = entt::null;
    }
}

void arcadia::imgui_window_property::_on_physics_simulator_built(arcadia::event::physics_simulator_built& e)
{
    const auto& [physics_simulator_wptr] = e.data_tuple;
    _physics_simulator_wptr = physics_simulator_wptr;
}

void arcadia::imgui_window_property::_on_physics_simulator_unbuilt(arcadia::event::physics_simulator_unbuilt& e)
{
    _physics_simulator_wptr.reset();
}


