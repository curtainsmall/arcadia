#include "imgui_window_property.hpp"

#include<algorithm>
#include<string>

#include"core/file/pfd_header.hpp"
#include"core/memento/memento.hpp"
#include"ui/imgui_header.hpp"
#include"ui/imgui_wrapper.hpp"

auto ImguiWindowPropertyCameraComponent::operator()(CameraComponent& camera_comp) -> std::string
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
    if(imgui_wrapper::drag_float(
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
    if(imgui_wrapper::drag_float(
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
    auto fovy = glm::degrees(camera_comp.fovy);
    if(imgui_wrapper::drag_float(
        "FOV",
        fovy,
        speed,
        camera_comp.fovy_min,
        camera_comp.fovy_max,
        format,
        flags
    ))
    {
        description = "FOV";
    }
    camera_comp.fovy = glm::radians(fovy);

    ImGui::NewLine();
    auto fovy_min = glm::degrees(camera_comp.fovy_min);
    if(imgui_wrapper::drag_float(
        "FOV Min",
        fovy_min,
        speed,
        .0f,
        180.f,
        format,
        flags
    ))
    {
        description = "FOV Min";
    }
    camera_comp.fovy_min = glm::radians(fovy_min);

    ImGui::NewLine();
    auto fovy_max = glm::degrees(camera_comp.fovy_max);
    if(imgui_wrapper::drag_float(
        "FOV Max",
        fovy_max,
        speed,
        .0f,
        180.f,
        format,
        flags
    ))
    {
        description = "FOV Max";
    }
    camera_comp.fovy_max = glm::radians(fovy_max);

    ImGui::NewLine();
    if(imgui_wrapper::drag_float(
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
    if(imgui_wrapper::drag_ivec2(
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
    if(imgui_wrapper::checkbox(
        "Fixed Up",
        camera_comp.fixed_up
    ))
    {
        description = "Fixed Up";
    }

    ImGui::NewLine();
    auto up_epsilon = glm::degrees(camera_comp.up_epsilon);
    if(imgui_wrapper::drag_float(
        "Up Epsilon",
        up_epsilon,
        speed,
        min,
        max,
        format,
        flags
    ))
    {
        description = "Up Epsilon";
    }
    camera_comp.up_epsilon = glm::radians(up_epsilon);

    ImGui::EndGroup();

    return description;
}

auto ImguiWindowPropertyLightComponent::operator()(LightComponent& light_comp) -> std::string
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
    if(match<bool>(
        light_comp.light,
        [&](NullLight&)
    {
        ImGui::Text("Light Type");
        ImGui::SameLine();
        if(ImGui::BeginCombo("##light_type", "(No light)"))
        {
            if(ImGui::Selectable("Spot Light"))
            {
                light_comp.light = SpotLight{};
                ImGui::EndCombo();
                return true;
            }
            if(ImGui::Selectable("Direct Light"))
            {
                light_comp.light = DirectLight{};
                ImGui::EndCombo();
                return true;
            }
            if(ImGui::Selectable("Area Light"))
            {
                light_comp.light = AreaLight{};
                ImGui::EndCombo();
                return true;
            }
            if(ImGui::Selectable("Point Light"))
            {
                light_comp.light = PointLight{};
                ImGui::EndCombo();
                return true;
            }
            ImGui::EndCombo();
        }
        return false;
    },
        [&](SpotLight& light)
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
                    light_comp.light = DirectLight{};
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
                    light_comp.light = AreaLight{};
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
                    light_comp.light = PointLight{};
                    ImGui::EndCombo();
                    return true;
                }
            }
            ImGui::EndCombo();
        }

        ImGui::NewLine();
        edited |= imgui_wrapper::drag_float(
            "Attenuation Contant",
            light.attenuation_coefs.x,
            speed,
            min,
            max,
            format,
            flags
        );
        edited |= imgui_wrapper::drag_float(
            "             Linear",
            light.attenuation_coefs.y,
            speed,
            min,
            max,
            format,
            flags
        );
        edited |= imgui_wrapper::drag_float(
            "          Quadratic",
            light.attenuation_coefs.z,
            speed,
            min,
            max,
            format,
            flags
        );

        ImGui::NewLine();

        const float cutoff_angle_drag_speend = .1f;
        const float cutoff_angle_min = 0.f;
        const float cutoff_angle_max = 180.f;
        edited |= imgui_wrapper::drag_float(
            "Inner Cutoff Angle",
            light.cutoff_angles.x,
            cutoff_angle_drag_speend,
            cutoff_angle_min,
            cutoff_angle_max,
            format,
            flags
        );
        edited |= imgui_wrapper::drag_float(
            "Outer Cutoff Angle",
            light.cutoff_angles.y,
            cutoff_angle_drag_speend,
            cutoff_angle_min,
            cutoff_angle_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= imgui_wrapper::color_edit_vec3(
            "Color",
            light.color
        );

        ImGui::NewLine();
        edited |= imgui_wrapper::drag_vec3_color(
            "Ambient Strength",
            light.ambient_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= imgui_wrapper::drag_vec3_color(
            "Diffuse Strength",
            light.diffuse_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= imgui_wrapper::drag_vec3_color(
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
        [&](DirectLight& light)
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
                    light_comp.light = SpotLight{};
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
                    light_comp.light = AreaLight{};
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
                    light_comp.light = PointLight{};
                    ImGui::EndCombo();
                    return true;
                }
            }
            ImGui::EndCombo();
        }

        ImGui::NewLine();
        edited |= imgui_wrapper::color_edit_vec3(
            "Color",
            light.color
        );

        ImGui::NewLine();
        edited |= imgui_wrapper::drag_vec3_color(
            "Ambient Strengt",
            light.ambient_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= imgui_wrapper::drag_vec3_color(
            "Diffuse Strength",
            light.diffuse_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= imgui_wrapper::drag_vec3_color(
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
        [&](AreaLight& light)
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
                    light_comp.light = SpotLight{};
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
                    light_comp.light = DirectLight{};
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
                    light_comp.light = PointLight{};
                    ImGui::EndCombo();
                    return true;
                }
            }
            ImGui::EndCombo();
        }

        ImGui::NewLine();
        edited |= imgui_wrapper::drag_float(
            "Width",
            light.size.x,
            speed,
            min,
            max,
            format,
            flags
        );
        edited |= imgui_wrapper::drag_float(
            "Height",
            light.size.y,
            speed,
            min,
            max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= imgui_wrapper::color_edit_vec3(
            "Color",
            light.color
        );

        ImGui::NewLine();
        edited |= imgui_wrapper::drag_vec3_color(
            "Ambient Strength",
            light.ambient_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= imgui_wrapper::drag_vec3_color(
            "Diffuse Strength",
            light.diffuse_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= imgui_wrapper::drag_vec3_color(
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
        [&](PointLight& light)
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
                    light_comp.light = SpotLight{};
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
                    light_comp.light = DirectLight{};
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
                    light_comp.light = AreaLight{};
                    ImGui::EndCombo();
                    return true;
                }
            }
            ImGui::EndCombo();
        }

        ImGui::NewLine();
        edited |= imgui_wrapper::drag_float(
            "Attenuation Contant",
            light.attenuation_coefs.x,
            speed,
            min,
            max,
            format,
            flags
        );
        edited |= imgui_wrapper::drag_float(
            "             Linear",
            light.attenuation_coefs.x,
            speed,
            min,
            max,
            format,
            flags
        );
        edited |= imgui_wrapper::drag_float(
            "          Quadratic",
            light.attenuation_coefs.x,
            speed,
            min,
            max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= imgui_wrapper::color_edit_vec3(
            "Color",
            light.color
        );

        ImGui::NewLine();
        edited |= imgui_wrapper::drag_vec3_color(
            "Ambient Strength",
            light.ambient_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= imgui_wrapper::drag_vec3_color(
            "Diffuse Strength",
            light.diffuse_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= imgui_wrapper::drag_vec3_color(
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

auto ImguiWindowPropertyModelComponent::operator()(ModelComponent& model_comp) -> std::string
{
    std::string description{};
    ImGui::BeginGroup();

    ImGui::SeparatorText("Filepath");
    auto filepath_str = model_comp.filepath().empty()
        ? "(No filepath)"s
        : model_comp.filepath().generic_string();
    ImGui::TextWrapped(filepath_str.c_str());
    if(ImGui::Button("..."))
    {
        auto res = pfd::open_file{
            "Import Model"
        }.result();
        model_comp.import(res.size() ? res.at(0) : ""s);
    }

    ImGui::EndGroup();

    return description;
}

void ImguiWindowPopupPhysicsComponentCreateBody::operator()(PhysicsComponent& physics_comp)
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

        // Motion type
        ImGui::NewLine();
        auto jph_motion_type_preview = match<std::string>(
            _temp_jph_body_info.jph_motion_type,
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
                _temp_jph_body_info.jph_motion_type = JPH::EMotionType::Static;
                _temp_jph_body_info.jph_object_layer = jph_object_layers::non_moving;
            }
            if(ImGui::Selectable("Dynamic"))
            {
                _temp_jph_body_info.jph_motion_type = JPH::EMotionType::Dynamic;
                _temp_jph_body_info.jph_object_layer = jph_object_layers::moving;
            }
            if(ImGui::Selectable("Kinematic"))
            {
                _temp_jph_body_info.jph_motion_type = JPH::EMotionType::Kinematic;
                _temp_jph_body_info.jph_object_layer = jph_object_layers::moving;
            }
            ImGui::EndCombo();
        }

        //Shape
        _temp_jph_body_info.jph_shape_info = match<JphShapeInfo>(
            _temp_jph_body_info.jph_shape_info,
            [&](JphBoxShapeInfo& info) -> JphShapeInfo
        {
            ImGui::Text("  Shape Type");
            ImGui::SameLine();
            if(ImGui::BeginCombo("##shape_type", "Box Shape"))
            {
                ImGui::Selectable("Box Shape");
                if(ImGui::Selectable("Capsule Shape"))
                {
                    ImGui::EndCombo();
                    return JphCapsuleShapeInfo{};
                }
                if(ImGui::Selectable("Cylinder Shape"))
                {
                    ImGui::EndCombo();
                    return JphCylinderShapeInfo{};
                }
                if(ImGui::Selectable("Sphere Shape"))
                {
                    ImGui::EndCombo();
                    return JphSphereShapeInfo{};
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

            return _temp_jph_body_info.jph_shape_info;
        },
            [&](JphCapsuleShapeInfo& info) -> JphShapeInfo
        {
            ImGui::Text("  Shape Type");
            ImGui::SameLine();
            if(ImGui::BeginCombo("##shape_type", "Capsule Shape"))
            {
                if(ImGui::Selectable("Box Shape"))
                {
                    ImGui::EndCombo();
                    return JphShapeInfo{};
                }
                ImGui::Selectable("Capsule Shape");
                if(ImGui::Selectable("Cylinder Shape"))
                {
                    ImGui::EndCombo();
                    return JphCylinderShapeInfo{};
                }
                if(ImGui::Selectable("Sphere Shape"))
                {
                    ImGui::EndCombo();
                    return JphSphereShapeInfo{};
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

            return _temp_jph_body_info.jph_shape_info;
        },
            [&](JphCylinderShapeInfo& info) -> JphShapeInfo
        {
            ImGui::Text("  Shape Type");
            ImGui::SameLine();
            if(ImGui::BeginCombo("##shape_type", "Cylinder Shape"))
            {
                if(ImGui::Selectable("Box Shape"))
                {
                    ImGui::EndCombo();
                    return JphShapeInfo{};
                }
                if(ImGui::Selectable("Capsule Shape"))
                {
                    ImGui::EndCombo();
                    return JphCapsuleShapeInfo{};
                }
                ImGui::Selectable("Cylinder Shape");
                if(ImGui::Selectable("Sphere Shape"))
                {
                    ImGui::EndCombo();
                    return JphSphereShapeInfo{};
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

            return _temp_jph_body_info.jph_shape_info;
        },
            [&](JphSphereShapeInfo& info) -> JphShapeInfo
        {
            ImGui::Text("  Shape Type");
            ImGui::SameLine();
            if(ImGui::BeginCombo("##shape_type", "Sphere Shape"))
            {
                if(ImGui::Selectable("Box Shape"))
                {
                    ImGui::EndCombo();
                    return JphShapeInfo{};
                }
                if(ImGui::Selectable("Capsule Shape"))
                {
                    ImGui::EndCombo();
                    return JphCapsuleShapeInfo{};
                }
                if(ImGui::Selectable("Cylinder Shape"))
                {
                    ImGui::EndCombo();
                    return JphCylinderShapeInfo{};
                }
                ImGui::Selectable("Sphere Shape");
                ImGui::EndCombo();
            }
            ImGui::NewLine();
            ImGui::SeparatorText("Sphere Shape");

            auto radius_min = .0f;
            auto radius_max = (std::numeric_limits<float>::max)();
            ImGui::Text("   Radius"); ImGui::SameLine(); ImGui::DragFloat("##radius", &info.radius, speed, radius_min, radius_max, format, slider_flags);

            return _temp_jph_body_info.jph_shape_info;
        }
        );

        ImGui::NewLine();
        auto confirmed = ImGui::Button("Confirm");
        if(confirmed)
        {
            physics_comp.build_identifiable_jph_body_info(
                _temp_jph_body_info
            );

        }
        ImGui::SameLine();
        if(confirmed || ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            open = false;
            _temp_jph_body_info = JphBodyInfo{};
        }

        ImGui::EndPopup();
    }
}

auto ImguiWindowPropertyPhysicsComponent::operator()(PhysicsComponent& physics_comp) -> std::string
{
    _imgui_window_popup_physics_component_create_body(physics_comp);

    std::string description{};
    ImGui::BeginGroup();

    if(physics_comp.has_body_info())
    {
        const auto& [uuid, jph_body_info_initial] = physics_comp.get_identifiable_jph_body_info();

        ImGui::SeparatorText("Initial");

        ImGui::NewLine();
        ImGui::Text(std::format(
            "Motion Type: {}",
            match<std::string>(
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

        const auto& jph_body_state = physics_comp.jph_body_state;
        ImGui::Text(std::format("Active: {}", jph_body_state.active).c_str());

        ImGui::NewLine();
        imgui_wrapper::text_vec3("Linear Velocity", jph_body_state.linear_velocity);

        ImGui::NewLine();
        imgui_wrapper::text_vec3("Angular Velocity", jph_body_state.angular_velocity);

        match<void>(
            jph_body_info_initial.jph_shape_info,
            [&](const JphBoxShapeInfo& info)
        {
            ImGui::SeparatorText("Body Shape - Box");
            ImGui::Text(std::format("Half Extent: {}", info.half_extent).c_str());
            ImGui::Text(std::format("Convex Radius: {:.2f}", info.convex_radius).c_str());
        },
            [&](const JphCapsuleShapeInfo& info)
        {
            ImGui::SeparatorText("Body Shape - Capsule");
            ImGui::Text(std::format("Radius: {:.2f}", info.radius).c_str());
            ImGui::Text(std::format("Half Height of Cylinder: {:.2f}", info.half_height_of_cylinder).c_str());
        },
            [&](const JphCylinderShapeInfo& info)
        {
            ImGui::SeparatorText("Body Shape - Cylinder");
            ImGui::Text(std::format("Half Height: {:.2f}", info.half_height).c_str());
            ImGui::Text(std::format("Radius: {:.2f}", info.radius).c_str());
            ImGui::Text(std::format("Convex Radius: {:.2f}", info.convex_radius).c_str());
        },
            [&](const JphSphereShapeInfo& info)
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

    if(physics_comp.has_body_info())
    {
        if(imgui_wrapper::color_edit_vec3(
            "Body Shape Color",
            physics_comp.body_shape_color
        ))
        {
            description = "Body Shape Color";
        }

        if(ImGui::Button("Recreate Body"))
        {
            _imgui_window_popup_physics_component_create_body.open = true;
        }
        ImGui::SameLine();
        if(ImGui::Button("Destroy Body"))
        {
            physics_comp.destroy_jph_body_info();
        }
    }
    else
    {
        if(ImGui::Button("Create Body"))
        {
            _imgui_window_popup_physics_component_create_body.open = true;
        }
    }

    ImGui::EndGroup();

    return description;
}

auto ImguiWindowPropertyTransformComponent::operator()(TransformComponent& transform_comp) -> std::string
{
    std::string description{};
    ImGui::BeginGroup();

    const float speed = 1.f;
    const float min = .0f;
    const float max = .0f;
    const char* format = "%.3f";
    const auto flags =
        ImGuiSliderFlags_AlwaysClamp;

    ImGui::SeparatorText("Transform");
    auto position_delta = transform_comp.position; // Previous position
    ImGui::NewLine();
    if(imgui_wrapper::drag_vec3(
        "Position",
        transform_comp.position,
        speed,
        min,
        max,
        format,
        flags
    ))
    {
        description = "Position";
    }
    position_delta = transform_comp.position - position_delta; // current - previous

    if(transform_comp.Flags & transform_component_flags::UseRotation)
    {
        ImGui::NewLine();
        float rotation_drag_speed{ .05f };
        if(imgui_wrapper::drag_quat_normalized(
            "Rotation",
            transform_comp.rotation,
            rotation_drag_speed,
            format,
            flags
        ))
        {
            description = "Rotation";
        }
    }

    if(transform_comp.Flags & transform_component_flags::UseDirection)
    {
        ImGui::NewLine();
        float direction_drag_speed{ .05f };
        if(imgui_wrapper::drag_vec3_normalized(
            "Direction",
            transform_comp.direction,
            direction_drag_speed,
            format,
            flags
        ))
        {
            description = "Direction";
        }
    }

    ImGui::NewLine();
    if(imgui_wrapper::drag_vec3(
        "Scale",
        transform_comp.scale,
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
    if(position_delta != vec3::zero())
    {
        transform_comp.pivot += position_delta; // Make pivot move with translation
    }
    if(imgui_wrapper::drag_vec3(
        "Pivot",
        transform_comp.pivot,
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

void ImguiWindowProperty::on_event(EventBase& e)
{
    EventDispatcher{ e }
        .dispatch<event::OpenImguiWindow>(ACDA_BIND_MEMBER_FN(_on_open_imgui_window))
        .dispatch<event::SceneActivated>(ACDA_BIND_MEMBER_FN(_on_scene_activated))
        .dispatch<event::SceneDeactivated>(ACDA_BIND_MEMBER_FN(_on_scene_deactivated))
        .dispatch<event::SelectEntity>(ACDA_BIND_MEMBER_FN(_on_select_entity))
        .dispatch<event::RenameEntity>(ACDA_BIND_MEMBER_FN(_on_rename_entity))
        .dispatch<event::DeleteEntity>(ACDA_BIND_MEMBER_FN(_on_delete_entity))
        .result();
}

#define ACDA_IMGUI_WINDOW_PROPERTY_HELPER(component_type, tab_name, property_display_fn) \
if(_contains_component<component_type>(_selected_entity_name) && ImGui::TreeNodeEx(tab_name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding))\
{\
    auto description = property_display_fn(_get_component<component_type>(_selected_entity_name));\
    if(!description.empty())\
    {\
        memento_list\
            .snapshot<component_type>(\
                std::format("{} - {}", tab_name, description),\
                [_scene = scene, _entity_name = _selected_entity_name]() -> component_type&\
            {\
                return _scene->get<component_type>(_entity_name);\
            }\
        );\
    }\
    ImGui::TreePop();\
}

void ImguiWindowProperty::on_update()
{
    if(!_open)
    {
        return;
    }

    auto scene = _scene.lock();

    if(scene)
    {
        int i = 0;
    }

    auto imgui_title = scene && !_selected_entity_name.empty()
        ? _title + " - " + _selected_entity_name + get_id_str()
        : _title + get_id_str();

    ImGui::SetNextWindowSize(glm::vec2{ 1024,768 }, ImGuiCond_Once);
    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_title.c_str(), &_open, window_flags))
    {
        if(!scene)
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
            if(!_selected_entity_name.empty())
            {
                ImGui::PushItemWidth(200.f);

                auto& memento_list = MementoList::instance();

                ACDA_IMGUI_WINDOW_PROPERTY_HELPER(CameraComponent, "Camera"s, _imgui_window_property_camera_component);
                ACDA_IMGUI_WINDOW_PROPERTY_HELPER(LightComponent, "Light"s, _imgui_window_property_light_component);
                ACDA_IMGUI_WINDOW_PROPERTY_HELPER(ModelComponent, "Model"s, _imgui_window_property_model_component);
                ACDA_IMGUI_WINDOW_PROPERTY_HELPER(PhysicsComponent, "Physics"s, _imgui_window_property_physics_component);
                ACDA_IMGUI_WINDOW_PROPERTY_HELPER(TransformComponent, "Transform"s, _imgui_window_property_transform_component);


                ImGui::PopItemWidth();

            }

        }
    }
    ImGui::End();
}

void ImguiWindowProperty::_on_open_imgui_window(event::OpenImguiWindow& e)
{
    const auto& [id_str] = e.data_tuple;
    if(id_str == get_id_str())
    {
        _open = true;
    }
}

void ImguiWindowProperty::_on_scene_activated(event::SceneActivated& e)
{
    const auto& [scene] = e.data_tuple;
    _scene = scene;
}

void ImguiWindowProperty::_on_scene_deactivated(event::SceneDeactivated& e)
{
    _scene.reset();
    _selected_entity_name.clear();
}

void ImguiWindowProperty::_on_select_entity(event::SelectEntity& e)
{
    const auto& [entity_name] = e.data_tuple;
    _selected_entity_name = entity_name;
}

void ImguiWindowProperty::_on_rename_entity(event::RenameEntity& e)
{
    const auto& [old_name, new_name] = e.data_tuple;
    if(old_name == _selected_entity_name)
    {
        _selected_entity_name = new_name;
    }
}

void ImguiWindowProperty::_on_delete_entity(event::DeleteEntity& e)
{
    const auto& [entity] = e.data_tuple;
    if(_selected_entity_name == entity)
    {
        _selected_entity_name.clear();
    }
}


