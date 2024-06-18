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
    if(ImguiWrapper::DragFloat(
        "Near Plane",
        camera_comp.NearPlane,
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
    if(ImguiWrapper::DragFloat(
        "Far Plane",
        camera_comp.FarPlane,
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
    auto fovy = glm::degrees(camera_comp.Fovy);
    if(ImguiWrapper::DragFloat(
        "FOV",
        fovy,
        speed,
        camera_comp.FovyMin,
        camera_comp.FovyMax,
        format,
        flags
    ))
    {
        description = "FOV";
    }
    camera_comp.Fovy = glm::radians(fovy);

    ImGui::NewLine();
    auto fovy_min = glm::degrees(camera_comp.FovyMin);
    if(ImguiWrapper::DragFloat(
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
    camera_comp.FovyMin = glm::radians(fovy_min);

    ImGui::NewLine();
    auto fovy_max = glm::degrees(camera_comp.FovyMax);
    if(ImguiWrapper::DragFloat(
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
    camera_comp.FovyMax = glm::radians(fovy_max);

    ImGui::NewLine();
    if(ImguiWrapper::DragFloat(
        "Speed",
        camera_comp.Speed,
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
    if(ImguiWrapper::DragIVec2(
        "Viewport Size",
        camera_comp.ViewportSize,
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
    if(ImguiWrapper::Checkbox(
        "Fixed Up",
        camera_comp.FixedUp
    ))
    {
        description = "Fixed Up";
    }

    ImGui::NewLine();
    auto up_epsilon = glm::degrees(camera_comp.UpEpsilon);
    if(ImguiWrapper::DragFloat(
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
    camera_comp.UpEpsilon = glm::radians(up_epsilon);

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
    if(Match<bool>(
        light_comp.Light,
        [&](NullLight&)
    {
        ImGui::Text("Light Type");
        ImGui::SameLine();
        if(ImGui::BeginCombo("##light_type", "(No light)"))
        {
            if(ImGui::Selectable("Spot Light"))
            {
                light_comp.Light = SpotLight{};
                ImGui::EndCombo();
                return true;
            }
            if(ImGui::Selectable("Direct Light"))
            {
                light_comp.Light = DirectLight{};
                ImGui::EndCombo();
                return true;
            }
            if(ImGui::Selectable("Area Light"))
            {
                light_comp.Light = AreaLight{};
                ImGui::EndCombo();
                return true;
            }
            if(ImGui::Selectable("Point Light"))
            {
                light_comp.Light = PointLight{};
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
                    light_comp.Light = DirectLight{};
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
                    light_comp.Light = AreaLight{};
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
                    light_comp.Light = PointLight{};
                    ImGui::EndCombo();
                    return true;
                }
            }
            ImGui::EndCombo();
        }

        ImGui::NewLine();
        edited |= ImguiWrapper::DragFloat(
            "Attenuation Contant",
            light.AttenuationCoefs.x,
            speed,
            min,
            max,
            format,
            flags
        );
        edited |= ImguiWrapper::DragFloat(
            "             Linear",
            light.AttenuationCoefs.y,
            speed,
            min,
            max,
            format,
            flags
        );
        edited |= ImguiWrapper::DragFloat(
            "          Quadratic",
            light.AttenuationCoefs.z,
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
        edited |= ImguiWrapper::DragFloat(
            "Inner Cutoff Angle",
            light.CutoffAngles.x,
            cutoff_angle_drag_speend,
            cutoff_angle_min,
            cutoff_angle_max,
            format,
            flags
        );
        edited |= ImguiWrapper::DragFloat(
            "Outer Cutoff Angle",
            light.CutoffAngles.y,
            cutoff_angle_drag_speend,
            cutoff_angle_min,
            cutoff_angle_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= ImguiWrapper::ColorEditVec3(
            "Color",
            light.Color
        );

        ImGui::NewLine();
        edited |= ImguiWrapper::DragVec3Color(
            "Ambient Strength",
            light.AmbientStrength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= ImguiWrapper::DragVec3Color(
            "Diffuse Strength",
            light.DiffuseStrength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= ImguiWrapper::DragVec3Color(
            "Specular Strength",
            light.SpecularStrength,
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
                    light_comp.Light = SpotLight{};
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
                    light_comp.Light = AreaLight{};
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
                    light_comp.Light = PointLight{};
                    ImGui::EndCombo();
                    return true;
                }
            }
            ImGui::EndCombo();
        }

        ImGui::NewLine();
        edited |= ImguiWrapper::ColorEditVec3(
            "Color",
            light.Color
        );

        ImGui::NewLine();
        edited |= ImguiWrapper::DragVec3Color(
            "Ambient Strengt",
            light.AmbientStrength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= ImguiWrapper::DragVec3Color(
            "Diffuse Strength",
            light.DiffuseStrength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= ImguiWrapper::DragVec3Color(
            "Specular Strength",
            light.SpecularStrength,
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
                    light_comp.Light = SpotLight{};
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
                    light_comp.Light = DirectLight{};
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
                    light_comp.Light = PointLight{};
                    ImGui::EndCombo();
                    return true;
                }
            }
            ImGui::EndCombo();
        }

        ImGui::NewLine();
        edited |= ImguiWrapper::DragFloat(
            "Width",
            light.Size.x,
            speed,
            min,
            max,
            format,
            flags
        );
        edited |= ImguiWrapper::DragFloat(
            "Height",
            light.Size.y,
            speed,
            min,
            max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= ImguiWrapper::ColorEditVec3(
            "Color",
            light.Color
        );

        ImGui::NewLine();
        edited |= ImguiWrapper::DragVec3Color(
            "Ambient Strength",
            light.AmbientStrength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= ImguiWrapper::DragVec3Color(
            "Diffuse Strength",
            light.DiffuseStrength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= ImguiWrapper::DragVec3Color(
            "Specular Strength",
            light.SpecularStrength,
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
                    light_comp.Light = SpotLight{};
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
                    light_comp.Light = DirectLight{};
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
                    light_comp.Light = AreaLight{};
                    ImGui::EndCombo();
                    return true;
                }
            }
            ImGui::EndCombo();
        }

        ImGui::NewLine();
        edited |= ImguiWrapper::DragFloat(
            "Attenuation Contant",
            light.AttenuationCoefs.x,
            speed,
            min,
            max,
            format,
            flags
        );
        edited |= ImguiWrapper::DragFloat(
            "             Linear",
            light.AttenuationCoefs.x,
            speed,
            min,
            max,
            format,
            flags
        );
        edited |= ImguiWrapper::DragFloat(
            "          Quadratic",
            light.AttenuationCoefs.x,
            speed,
            min,
            max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= ImguiWrapper::ColorEditVec3(
            "Color",
            light.Color
        );

        ImGui::NewLine();
        edited |= ImguiWrapper::DragVec3Color(
            "Ambient Strength",
            light.AmbientStrength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= ImguiWrapper::DragVec3Color(
            "Diffuse Strength",
            light.DiffuseStrength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        edited |= ImguiWrapper::DragVec3Color(
            "Specular Strength",
            light.SpecularStrength,
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
    auto filepath_str = model_comp.GetFilepath().empty()
        ? "(No filepath)"s
        : model_comp.GetFilepath().generic_string();
    ImGui::TextWrapped(filepath_str.c_str());
    if(ImGui::Button("..."))
    {
        auto res = pfd::open_file{
            "Import Model"
        }.result();
        model_comp.Import(res.size() ? res.at(0) : ""s);
    }

    ImGui::EndGroup();

    return description;
}

void ImguiWindowPopupPhysicsComponentCreateBody::operator()(PhysicsComponent& physics_comp)
{
    if(!Open)
    {
        return;
    }

    auto imgui_window_title = "Physics Component - Create Body"s;

    auto popup_flags =
        ImGuiPopupFlags_NoOpenOverExistingPopup;
    ImGui::OpenPopup(imgui_window_title.c_str(), popup_flags);

    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::BeginPopupModal(imgui_window_title.c_str(), &Open, window_flags))
    {
        float speed = .05f;
        float min = .0f;
        float max = .0f;
        const char* format = "%.3f";
        auto slider_flags =
            ImGuiSliderFlags_AlwaysClamp;

        // Motion type
        ImGui::NewLine();
        auto jph_motion_type_preview = Match<std::string>(
            _TempJphBodyInfo.JphMotionType,
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
                _TempJphBodyInfo.JphMotionType = JPH::EMotionType::Static;
                _TempJphBodyInfo.JphObjectLayer = JphObjectLayers::NonMoving;
            }
            if(ImGui::Selectable("Dynamic"))
            {
                _TempJphBodyInfo.JphMotionType = JPH::EMotionType::Dynamic;
                _TempJphBodyInfo.JphObjectLayer = JphObjectLayers::Moving;
            }
            if(ImGui::Selectable("Kinematic"))
            {
                _TempJphBodyInfo.JphMotionType = JPH::EMotionType::Kinematic;
                _TempJphBodyInfo.JphObjectLayer = JphObjectLayers::Moving;
            }
            ImGui::EndCombo();
        }

        //Shape
        _TempJphBodyInfo.JphShapeInfo = Match<JphShapeInfo>(
            _TempJphBodyInfo.JphShapeInfo,
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
                half_extent_x = info.HalfExtent.x,
                half_extent_y = info.HalfExtent.y,
                half_extent_z = info.HalfExtent.z;
            auto half_extent_min = std::max({ .01f,info.ConvexRadius });
            auto half_extent_max = (std::numeric_limits<float>::max)();
            ImGui::Text("Half Extent X"); ImGui::SameLine(); ImGui::DragFloat("##half_extent_x", &half_extent_x, speed, half_extent_min, half_extent_max, format, slider_flags);
            ImGui::Text("            Y"); ImGui::SameLine(); ImGui::DragFloat("##half_extent_y", &half_extent_y, speed, half_extent_min, half_extent_max, format, slider_flags);
            ImGui::Text("            Z"); ImGui::SameLine(); ImGui::DragFloat("##half_extent_z", &half_extent_z, speed, half_extent_min, half_extent_max, format, slider_flags);
            info.HalfExtent = glm::vec3{ half_extent_x,half_extent_y,half_extent_z };

            // Convex radius
            ImGui::NewLine();
            auto convex_radius_min = .0f;
            auto convex_radius_max = std::min({ half_extent_x, half_extent_y, half_extent_z });
            ImGui::Text("Convex Radius"); ImGui::SameLine(); ImGui::DragFloat("##convex_radius", &info.ConvexRadius, speed, convex_radius_min, convex_radius_max, format, slider_flags);

            return _TempJphBodyInfo.JphShapeInfo;
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
            ImGui::Text("                 Radius"); ImGui::SameLine(); ImGui::DragFloat("##radius", &info.Radius, speed, radius_min, radius_max, format, slider_flags);

            ImGui::NewLine();
            auto half_height_of_cylinder_min = .0f;
            auto half_height_of_cylinder_max = (std::numeric_limits<float>::max)();
            ImGui::Text("Half Height of Cylinder"); ImGui::SameLine(); ImGui::DragFloat("##half_height_of_cylinder", &info.HalfHeightOfCylinder, speed, half_height_of_cylinder_min, half_height_of_cylinder_max, format, slider_flags);

            return _TempJphBodyInfo.JphShapeInfo;
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
            ImGui::Text("  Half Height"); ImGui::SameLine(); ImGui::DragFloat("##half_height", &info.HalfHeight, speed, half_height_min, half_height_max, format, slider_flags);

            ImGui::NewLine();
            auto radius_min = .0f;
            auto radius_max = (std::numeric_limits<float>::max)();
            ImGui::Text("       Radius"); ImGui::SameLine(); ImGui::DragFloat("##radius", &info.Radius, speed, radius_min, radius_max, format, slider_flags);

            ImGui::NewLine();
            auto convex_radius_min = .0f;
            auto convex_radius_max = (std::numeric_limits<float>::max)();
            ImGui::Text("Convex Radius"); ImGui::SameLine(); ImGui::DragFloat("##convex_radius", &info.ConvexRadius, speed, convex_radius_min, convex_radius_max, format, slider_flags);

            return _TempJphBodyInfo.JphShapeInfo;
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
            ImGui::Text("   Radius"); ImGui::SameLine(); ImGui::DragFloat("##radius", &info.Radius, speed, radius_min, radius_max, format, slider_flags);

            return _TempJphBodyInfo.JphShapeInfo;
        }
        );

        ImGui::NewLine();
        auto confirmed = ImGui::Button("Confirm");
        if(confirmed)
        {
            physics_comp.BuildIdentifiableJphBodyInfo(
                _TempJphBodyInfo
            );

        }
        ImGui::SameLine();
        if(confirmed || ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            Open = false;
            _TempJphBodyInfo = JphBodyInfo{};
        }

        ImGui::EndPopup();
    }
}

auto ImguiWindowPropertyPhysicsComponent::operator()(PhysicsComponent& physics_comp) -> std::string
{
    _imgui_window_popup_physics_component_create_body(physics_comp);

    std::string description{};
    ImGui::BeginGroup();

    if(physics_comp.HasBodyInfo())
    {
        const auto& [uuid, jph_body_info_initial] = physics_comp.GetIdentifiableJphBodyInfo();

        ImGui::SeparatorText("Initial");

        ImGui::NewLine();
        ImGui::Text(std::format(
            "Motion Type: {}",
            Match<std::string>(
                jph_body_info_initial.JphMotionType,
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
        ImGui::Text(std::format("Object Layer: {}", jph_body_info_initial.JphObjectLayer).c_str());

        ImGui::SeparatorText("Ongoing");

        const auto& jph_body_state = physics_comp.JphBodyState;
        ImGui::Text(std::format("Active: {}", jph_body_state.Active).c_str());

        ImGui::NewLine();
        ImguiWrapper::TextVec3("Linear Velocity", jph_body_state.LinearVelocity);

        ImGui::NewLine();
        ImguiWrapper::TextVec3("Angular Velocity", jph_body_state.AngularVelocity);

        Match<void>(
            jph_body_info_initial.JphShapeInfo,
            [&](const JphBoxShapeInfo& info)
        {
            ImGui::SeparatorText("Body Shape - Box");
            ImGui::Text(std::format("Half Extent: {}", info.HalfExtent).c_str());
            ImGui::Text(std::format("Convex Radius: {:.2f}", info.ConvexRadius).c_str());
        },
            [&](const JphCapsuleShapeInfo& info)
        {
            ImGui::SeparatorText("Body Shape - Capsule");
            ImGui::Text(std::format("Radius: {:.2f}", info.Radius).c_str());
            ImGui::Text(std::format("Half Height of Cylinder: {:.2f}", info.HalfHeightOfCylinder).c_str());
        },
            [&](const JphCylinderShapeInfo& info)
        {
            ImGui::SeparatorText("Body Shape - Cylinder");
            ImGui::Text(std::format("Half Height: {:.2f}", info.HalfHeight).c_str());
            ImGui::Text(std::format("Radius: {:.2f}", info.Radius).c_str());
            ImGui::Text(std::format("Convex Radius: {:.2f}", info.ConvexRadius).c_str());
        },
            [&](const JphSphereShapeInfo& info)
        {
            ImGui::SeparatorText("Body Shape - Sphere");
            ImGui::Text(std::format("Radius: {:.2f}", info.Radius).c_str());
        }
        );
    }
    else
    {
        ImGui::Text("(No body state)");
    }

    if(physics_comp.HasBodyInfo())
    {
        if(ImguiWrapper::ColorEditVec3(
            "Body Shape Color",
            physics_comp.BodyShapeColor
        ))
        {
            description = "Body Shape Color";
        }

        if(ImGui::Button("Recreate Body"))
        {
            _imgui_window_popup_physics_component_create_body.Open = true;
        }
        ImGui::SameLine();
        if(ImGui::Button("Destroy Body"))
        {
            physics_comp.DestroyJphBodyInfo();
        }
    }
    else
    {
        if(ImGui::Button("Create Body"))
        {
            _imgui_window_popup_physics_component_create_body.Open = true;
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
    auto position_delta = transform_comp.Position; // Previous position
    ImGui::NewLine();
    if(ImguiWrapper::DragVec3(
        "Position",
        transform_comp.Position,
        speed,
        min,
        max,
        format,
        flags
    ))
    {
        description = "Position";
    }
    position_delta = transform_comp.Position - position_delta; // current - previous

    if(transform_comp.Flags & TransformComponentFlags::UseRotation)
    {
        ImGui::NewLine();
        float rotation_drag_speed{ .05f };
        if(ImguiWrapper::DragQuatNormalized(
            "Rotation",
            transform_comp.Rotation,
            rotation_drag_speed,
            format,
            flags
        ))
        {
            description = "Rotation";
        }
    }

    if(transform_comp.Flags & TransformComponentFlags::UseDirection)
    {
        ImGui::NewLine();
        float direction_drag_speed{ .05f };
        if(ImguiWrapper::DragVec3Normalized(
            "Direction",
            transform_comp.Direction,
            direction_drag_speed,
            format,
            flags
        ))
        {
            description = "Direction";
        }
    }

    ImGui::NewLine();
    if(ImguiWrapper::DragVec3(
        "Scale",
        transform_comp.Scale,
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
    if(position_delta != Vec3::Zero())
    {
        transform_comp.Pivot += position_delta; // Make pivot move with translation
    }
    if(ImguiWrapper::DragVec3(
        "Pivot",
        transform_comp.Pivot,
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

void ImguiWindowProperty::OnEvent(EventBase& event)
{
    EventDispatcher{ event }
        .Dispatch<Event::OpenImguiWindow>(ACDA_BIND_MEMBER_FN(_OnOpenImguiWindow))
        .Dispatch<Event::SceneActivated>(ACDA_BIND_MEMBER_FN(_OnSceneActivated))
        .Dispatch<Event::SceneDeactivated>(ACDA_BIND_MEMBER_FN(_OnSceneDeactivated))
        .Dispatch<Event::SelectEntity>(ACDA_BIND_MEMBER_FN(_OnSelectEntity))
        .Dispatch<Event::RenameEntity>(ACDA_BIND_MEMBER_FN(_OnRenameEntity))
        .Dispatch<Event::DeleteEntity>(ACDA_BIND_MEMBER_FN(_OnDeleteEntity))
        .Result();
}

#define ACDA_IMGUI_WINDOW_PROPERTY_HELPER(component_type, tab_name, property_display_fn) \
if(_ContainsComponent<component_type>(_SelectedEntityName) && ImGui::TreeNodeEx(tab_name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding))\
{\
    auto description = property_display_fn(_GetComponent<component_type>(_SelectedEntityName));\
    if(!description.empty())\
    {\
        memento_list\
            .Snapshot<component_type>(\
                std::format("{} - {}", tab_name, description),\
                [_scene = scene, _entity_name = _SelectedEntityName]() -> component_type&\
            {\
                return _scene->Get<component_type>(_entity_name);\
            }\
        );\
    }\
    ImGui::TreePop();\
}

void ImguiWindowProperty::OnUpdate()
{
    if(!_Open)
    {
        return;
    }

    auto scene = _Scene.lock();

    if(scene)
    {
        int i = 0;
    }

    auto imgui_title = scene && !_SelectedEntityName.empty()
        ? _Title + " - " + _SelectedEntityName + GetIdStr()
        : _Title + GetIdStr();

    ImGui::SetNextWindowSize(glm::vec2{ 1024,768 }, ImGuiCond_Once);
    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_title.c_str(), &_Open, window_flags))
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
            if(!_SelectedEntityName.empty())
            {
                ImGui::PushItemWidth(200.f);

                auto& memento_list = MementoList::Instance();

                ACDA_IMGUI_WINDOW_PROPERTY_HELPER(CameraComponent, "Camera"s, _ImguiWindowPropertyCameraComponent);
                ACDA_IMGUI_WINDOW_PROPERTY_HELPER(LightComponent, "Light"s, _ImguiWindowPropertyLightComponent);
                ACDA_IMGUI_WINDOW_PROPERTY_HELPER(ModelComponent, "Model"s, _ImguiWindowPropertyModelComponent);
                ACDA_IMGUI_WINDOW_PROPERTY_HELPER(PhysicsComponent, "Physics"s, _ImguiWindowPropertyPhysicsComponent);
                ACDA_IMGUI_WINDOW_PROPERTY_HELPER(TransformComponent, "Transform"s, _ImguiWindowPropertyTransformComponent);


                ImGui::PopItemWidth();

            }

        }
    }
    ImGui::End();
}

void ImguiWindowProperty::_OnOpenImguiWindow(Event::OpenImguiWindow& e)
{
    const auto& [id_str] = e.data_tuple;
    if(id_str == GetIdStr())
    {
        _Open = true;
    }
}

void ImguiWindowProperty::_OnSceneActivated(Event::SceneActivated& e)
{
    const auto& [scene] = e.data_tuple;
    _Scene = scene;
}

void ImguiWindowProperty::_OnSceneDeactivated(Event::SceneDeactivated& e)
{
    _Scene.reset();
    _SelectedEntityName.clear();
}

void ImguiWindowProperty::_OnSelectEntity(Event::SelectEntity& e)
{
    const auto& [entity_name] = e.data_tuple;
    _SelectedEntityName = entity_name;
}

void ImguiWindowProperty::_OnRenameEntity(Event::RenameEntity& e)
{
    const auto& [old_name, new_name] = e.data_tuple;
    if(old_name == _SelectedEntityName)
    {
        _SelectedEntityName = new_name;
    }
}

void ImguiWindowProperty::_OnDeleteEntity(Event::DeleteEntity& e)
{
    const auto& [entity] = e.data_tuple;
    if(_SelectedEntityName == entity)
    {
        _SelectedEntityName.clear();
    }
}


