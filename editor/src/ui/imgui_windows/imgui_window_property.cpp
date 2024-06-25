#include "imgui_window_property.hpp"

#include<algorithm>
#include<string>

#include"core/file/pfd_header.hpp"
#include"core/memento/memento.hpp"
#include"resource/fonts/icon_header.hpp"
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

    ImGui::DragFloat("Near Plane", &camera_comp.near_plane, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        description = "Near Plane";
    }

    ImGui::DragFloat("Far Plane", &camera_comp.far_plane, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        description = "Far Plane";
    }

    auto fovy = glm::degrees(camera_comp.fovy);
    ImGui::DragFloat("FOV-Y", &fovy, speed, camera_comp.fovy_min, camera_comp.fovy_max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        description = "FOV";
    }
    camera_comp.fovy = glm::radians(fovy);

    auto fovy_min = glm::degrees(camera_comp.fovy_min);
    ImGui::DragFloat("FOV-Y Min", &fovy_min, speed, 0.0f, 180.0f, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        description = "FOV Min";
    }
    camera_comp.fovy_min = glm::radians(fovy_min);

    auto fovy_max = glm::degrees(camera_comp.fovy_max);
    ImGui::DragFloat("FOV-Y Max", &fovy_max, speed, 0.0f, 180.0f, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        description = "FOV Max";
    }
    camera_comp.fovy_max = glm::radians(fovy_max);

    ImGui::DragFloat("Speed", &camera_comp.speed, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        description = "Speed";
    }

    ImGui::DragInt2("Viewport Size", glm::value_ptr(camera_comp.viewport_size), speed, 1.0f, FLT_MAX, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        description = "Viewport Size";
    }

    ImGui::Checkbox("Fixed Up", &camera_comp.fixed_up);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        description = "Fixed Up";
    }

    auto up_epsilon = glm::degrees(camera_comp.up_epsilon);
    ImGui::DragFloat("Up Epsilon", &up_epsilon, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
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
        if(ImGui::BeginCombo("Light Typee", "(No light)"))
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

        if(ImGui::BeginCombo("Light Type", "Spot Light"))
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

        ImGui::DragFloat3("Attenuation", glm::value_ptr(light.attenuation_coefs), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();
        ImGui::SameLine();
        imgui_wrappers::help_mark(ICON_FA_QUESTION, "In order of constant, linear and quadratic terms");

        const float cutoff_angle_drag_speend = .1f;
        const float cutoff_angle_min = 0.f;
        const float cutoff_angle_max = 180.f;
        ImGui::DragFloat2("Cutoff Angle", glm::value_ptr(light.cutoff_angles), cutoff_angle_drag_speend, cutoff_angle_min, cutoff_angle_max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();
        ImGui::SameLine();
        imgui_wrappers::help_mark(ICON_FA_QUESTION, "Inner and outter");

        ImGui::ColorEdit3("Color", glm::value_ptr(light.color));
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        ImGui::NewLine();

        ImGui::DragFloat3("Ambient Strenght", glm::value_ptr(light.ambient_strength), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        ImGui::DragFloat3("Diffuse Strenght", glm::value_ptr(light.ambient_strength), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        ImGui::DragFloat3("Specular Strenght", glm::value_ptr(light.ambient_strength), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        return edited;
    },
        [&](DirectLight& light)
    {
        bool edited{ false };

        if(ImGui::BeginCombo("Light Type", "Direct Light"))
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

        ImGui::ColorEdit3("Color", glm::value_ptr(light.color));
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        ImGui::NewLine();

        ImGui::DragFloat3("Ambient Strenght", glm::value_ptr(light.ambient_strength), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        ImGui::DragFloat3("Diffuse Strenght", glm::value_ptr(light.ambient_strength), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        ImGui::DragFloat3("Specular Strenght", glm::value_ptr(light.ambient_strength), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        return edited;
    },
        [&](AreaLight& light)
    {
        bool edited{ false };

        if(ImGui::BeginCombo("Light Type", "Area Light"))
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

        ImGui::DragFloat2("Size", glm::value_ptr(light.size), speed, min, max, format, flags);

        ImGui::ColorEdit3("Color", glm::value_ptr(light.color));
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        ImGui::NewLine();

        ImGui::DragFloat3("Ambient Strenght", glm::value_ptr(light.ambient_strength), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        ImGui::DragFloat3("Diffuse Strenght", glm::value_ptr(light.ambient_strength), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        ImGui::DragFloat3("Specular Strenght", glm::value_ptr(light.ambient_strength), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        return edited;
    },
        [&](PointLight& light)
    {
        bool edited{ false };

        if(ImGui::BeginCombo("Light Type", "Point Light"))
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

        ImGui::DragFloat3("Attenuation", glm::value_ptr(light.attenuation_coefs), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();
        ImGui::SameLine();
        imgui_wrappers::help_mark(ICON_FA_QUESTION, "In order of constant, linear and quadratic terms");

        ImGui::ColorEdit3("Color", glm::value_ptr(light.color));
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        ImGui::NewLine();

        ImGui::DragFloat3("Ambient Strenght", glm::value_ptr(light.ambient_strength), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        ImGui::DragFloat3("Diffuse Strenght", glm::value_ptr(light.ambient_strength), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        ImGui::DragFloat3("Specular Strenght", glm::value_ptr(light.ambient_strength), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();

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
        auto jph_motion_type_preview = match<std::string>(
            _temp_jph_body_info.jph_motion_type,
            JPH::EMotionType::Static,
            "Static"s,
            JPH::EMotionType::Dynamic,
            "Dynamic"s,
            JPH::EMotionType::Kinematic,
            "Kinematic"s
        );
        if(ImGui::BeginCombo("Motion Type", jph_motion_type_preview.c_str()))
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
            if(ImGui::BeginCombo("Shape Type", "Box Shape"))
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
            ImGui::SeparatorText("Box Shape");

            // Half extent
            auto half_extent_min = std::max({ .01f,info.convex_radius });
            auto half_extent_max = FLT_MAX;
            ImGui::DragFloat3("Half Extent", glm::value_ptr(info.half_extent), speed, half_extent_min, half_extent_max, format, slider_flags);

            // Convex radius
            auto convex_radius_min = .0f;
            auto convex_radius_max = std::min({ info.half_extent.x,info.half_extent.y,info.half_extent.z });
            ImGui::Text("Convex Radius"); ImGui::SameLine(); ImGui::DragFloat("##convex_radius", &info.convex_radius, speed, convex_radius_min, convex_radius_max, format, slider_flags);

            return _temp_jph_body_info.jph_shape_info;
        },
            [&](JphCapsuleShapeInfo& info) -> JphShapeInfo
        {
            if(ImGui::BeginCombo("Shape Type", "Capsule Shape"))
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
            ImGui::SeparatorText("Capsule Type");

            auto radius_min = .0f;
            auto radius_max = FLT_MAX;
            ImGui::DragFloat("Radius", &info.radius, speed, radius_min, radius_max, format, slider_flags);

            auto half_height_of_cylinder_min = .0f;
            auto half_height_of_cylinder_max = FLT_MAX;
            ImGui::DragFloat("Half Height if Cylinder", &info.half_height_of_cylinder, speed, half_height_of_cylinder_min, half_height_of_cylinder_max, format, slider_flags);

            return _temp_jph_body_info.jph_shape_info;
        },
            [&](JphCylinderShapeInfo& info) -> JphShapeInfo
        {
            if(ImGui::BeginCombo("Shape Type", "Cylinder Shape"))
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
            ImGui::SeparatorText("Cylinder Shape");

            auto half_height_min = .0f;
            auto half_height_max = FLT_MAX;
            ImGui::DragFloat("Half Height", &info.half_height, speed, half_height_min, half_height_max, format, slider_flags);

            auto radius_min = .0f;
            auto radius_max = FLT_MAX;
            ImGui::DragFloat("Radius", &info.radius, speed, radius_min, radius_max, format, slider_flags);

            auto convex_radius_min = .0f;
            auto convex_radius_max = FLT_MAX;
            ImGui::DragFloat("Convex Radius", &info.convex_radius, speed, convex_radius_min, convex_radius_max, format, slider_flags);

            return _temp_jph_body_info.jph_shape_info;
        },
            [&](JphSphereShapeInfo& info) -> JphShapeInfo
        {
            if(ImGui::BeginCombo("Shape Type", "Sphere Shape"))
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
            ImGui::SeparatorText("Sphere Shape");

            auto radius_min = .0f;
            auto radius_max = FLT_MAX;
            ImGui::DragFloat("Radius", &info.radius, speed, radius_min, radius_max, format, slider_flags);

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

        if(ImGui::TreeNodeEx("Initial", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding))
        {

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
            ImGui::TreePop();
        }

        if(ImGui::TreeNodeEx("Current", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding))
        {
            const auto& jph_body_state = physics_comp.jph_body_state;
            ImGui::Text(std::format("Active: {}", jph_body_state.active).c_str());
            ImGui::Text(std::format("Linear Velocity - {}", jph_body_state.linear_velocity).c_str());
            ImGui::Text(std::format("Angular Velocity - {}", jph_body_state.angular_velocity).c_str());
            ImGui::TreePop();
        }

        bool tree_open = match<bool>(
            jph_body_info_initial.jph_shape_info,
            [&](const JphBoxShapeInfo& info)
        {
            bool tree_open = ImGui::TreeNodeEx("Body Shape - Box", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding);
            if(tree_open)
            {
                ImGui::Text(std::format("Half Extent: {}", info.half_extent).c_str());
                ImGui::Text(std::format("Convex Radius: {:.2f}", info.convex_radius).c_str());
            }
            return tree_open;
        },
            [&](const JphCapsuleShapeInfo& info)
        {
            bool tree_open = ImGui::TreeNodeEx("Body Shape - Capsule", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding);
            if(tree_open)
            {
                ImGui::Text(std::format("Radius: {:.2f}", info.radius).c_str());
                ImGui::Text(std::format("Half Height of Cylinder: {:.2f}", info.half_height_of_cylinder).c_str());
            }
            return tree_open;
        },
            [&](const JphCylinderShapeInfo& info)
        {
            bool tree_open = ImGui::TreeNodeEx("Body Shape - Cylinder", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding);
            if(tree_open)
            {
                ImGui::Text(std::format("Half Height: {:.2f}", info.half_height).c_str());
                ImGui::Text(std::format("Radius: {:.2f}", info.radius).c_str());
                ImGui::Text(std::format("Convex Radius: {:.2f}", info.convex_radius).c_str());
            }
            return tree_open;
        },
            [&](const JphSphereShapeInfo& info)
        {
            bool tree_open = ImGui::TreeNodeEx("Body Shape - Sphere", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding);
            if(tree_open)
            {
                ImGui::Text(std::format("Radius: {:.2f}", info.radius).c_str());
            }
            return tree_open;
        }
        );
        if(tree_open)
        {
            ImGui::TreePop();
        }
    }
    else
    {
        ImGui::Text("(No body state)");
    }

    if(physics_comp.has_body_info())
    {
        ImGui::ColorEdit3("Body Shape Color", glm::value_ptr(physics_comp.body_shape_color));
        if(ImGui::IsItemDeactivatedAfterEdit())
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

    auto position_delta = transform_comp.position; // Previous position
    ImGui::DragFloat3("Position", glm::value_ptr(transform_comp.position), speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        description = "Position";
    }
    position_delta = transform_comp.position - position_delta; // current - previous

    if(transform_comp.Flags & transform_component_flags::UseRotation)
    {
        float rotation_drag_speed{ .05f };
        glm::quat temp = transform_comp.rotation;
        ImGui::DragFloat3("Direction", glm::value_ptr(temp), rotation_drag_speed, min, max, format, flags);
        transform_comp.rotation = quat::fixed_normalized(transform_comp.rotation, temp);
        if(ImGui::IsItemDeactivated())
        {
            description = "Rotation";
        }
    }
    if(transform_comp.Flags & transform_component_flags::UseDirection)
    {
        float direction_drag_speed{ .05f };
        glm::vec3 temp = transform_comp.direction;
        ImGui::DragFloat3("Direction", glm::value_ptr(temp), direction_drag_speed, min, max, format, flags);
        transform_comp.direction = vec3::fixed_normalized(transform_comp.direction, temp);
        if(ImGui::IsItemDeactivated())
        {
            description = "Direction";
        }
    }

    ImGui::DragFloat3("Scale", glm::value_ptr(transform_comp.scale), speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        description = "Scale";
    }

    if(position_delta != vec3::zero())
    {
        transform_comp.pivot += position_delta; // Make pivot move with translation
    }
    ImGui::DragFloat3("Pivot", glm::value_ptr(transform_comp.pivot), speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        description = "Pivot";
    }

    ImGui::EndGroup();

    return description;
}

void ImguiWindowProperty::on_event(EventBase& e)
{
    EventDispatcher{ e }
        .dispatch<events::OpenImguiWindow>(ACDA_BIND_MEMBER_FN(_on_open_imgui_window))
        .dispatch<events::SceneActivated>(ACDA_BIND_MEMBER_FN(_on_scene_activated))
        .dispatch<events::SceneDeactivated>(ACDA_BIND_MEMBER_FN(_on_scene_deactivated))
        .dispatch<events::SelectEntity>(ACDA_BIND_MEMBER_FN(_on_select_entity))
        .dispatch<events::RenameEntity>(ACDA_BIND_MEMBER_FN(_on_rename_entity))
        .dispatch<events::DeleteEntity>(ACDA_BIND_MEMBER_FN(_on_delete_entity))
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
            ImGui::Text("(No scene)");
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

void ImguiWindowProperty::_on_open_imgui_window(events::OpenImguiWindow& e)
{
    const auto& [id_str] = e.data_tuple;
    if(id_str == get_id_str())
    {
        _open = true;
    }
}

void ImguiWindowProperty::_on_scene_activated(events::SceneActivated& e)
{
    const auto& [scene] = e.data_tuple;
    _scene = scene;
}

void ImguiWindowProperty::_on_scene_deactivated(events::SceneDeactivated& e)
{
    _scene.reset();
    _selected_entity_name.clear();
}

void ImguiWindowProperty::_on_select_entity(events::SelectEntity& e)
{
    const auto& [entity_name] = e.data_tuple;
    _selected_entity_name = entity_name;
}

void ImguiWindowProperty::_on_rename_entity(events::RenameEntity& e)
{
    const auto& [old_name, new_name] = e.data_tuple;
    if(old_name == _selected_entity_name)
    {
        _selected_entity_name = new_name;
    }
}

void ImguiWindowProperty::_on_delete_entity(events::DeleteEntity& e)
{
    const auto& [entity] = e.data_tuple;
    if(_selected_entity_name == entity)
    {
        _selected_entity_name.clear();
    }
}


