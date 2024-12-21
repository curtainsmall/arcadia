#include "imgui_window_property.hpp"

#include<algorithm>
#include<string>

#include"core/enum.hpp"
#include"core/file/pfd_header.hpp"
#include"core/function.hpp"
#include"core/match.hpp"
#include"core/memento/memento.hpp"
#include"resource/fonts/icon_header.hpp"
#include"ui/imgui_header.hpp"
#include"ui/imgui_wrapper.hpp"

auto Arcadia::ImguiWindowPropertyCameraComponent::operator()(CameraComponent& camera_comp) -> std::string
{
    const float speed = 1.f;
    const float min = .0;
    const float max = .0f;
    const char* format = "%.3f";
    const auto flags =
        ImGuiSliderFlags_AlwaysClamp;

    std::string description{};
    ImGui::BeginGroup();

    ImGui::DragFloat("Near Plane", &camera_comp.NearPlane, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        description = "Near Plane";
    }

    ImGui::DragFloat("Far Plane", &camera_comp.FarPlane, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        description = "Far Plane";
    }

    auto fovy = glm::degrees(camera_comp.FovY);
    ImGui::DragFloat("FOV-Y", &fovy, speed, camera_comp.FovYMin, camera_comp.FovYMax, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        description = "FOV";
    }
    camera_comp.FovY = glm::radians(fovy);

    auto fovy_min = glm::degrees(camera_comp.FovYMin);
    ImGui::DragFloat("FOV-Y Min", &fovy_min, speed, 0.0f, 180.0f, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        description = "FOV Min";
    }
    camera_comp.FovYMin = glm::radians(fovy_min);

    auto fovy_max = glm::degrees(camera_comp.FovYMax);
    ImGui::DragFloat("FOV-Y Max", &fovy_max, speed, 0.0f, 180.0f, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        description = "FOV Max";
    }
    camera_comp.FovYMax = glm::radians(fovy_max);

    ImGui::DragFloat("Speed", &camera_comp.Speed, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        description = "Speed";
    }

    ImGui::DragInt2("Viewport Size", glm::value_ptr(camera_comp.ViewportSize), speed, 1.0f, INT_MAX, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        description = "Viewport Size";
    }

    ImGui::Checkbox("Fixed Up", &camera_comp.FixedUp);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        description = "Fixed Up";
    }

    auto up_epsilon = glm::degrees(camera_comp.UpEpsilon);
    ImGui::DragFloat("Up Epsilon", &up_epsilon, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        description = "Up Epsilon";
    }
    camera_comp.UpEpsilon = glm::radians(up_epsilon);

    ImGui::EndGroup();

    return description;
}

auto Arcadia::ImguiWindowPropertyLightComponent::operator()(LightComponent& light_comp) -> std::string
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
    if(MatchVariant<bool>(
        light_comp.Light,
        [&](NullLight&)
    {
        if(ImGui::BeginCombo("Light Typee", "(No light)"))
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
        bool edited = false;

        if(ImGui::BeginCombo("Light Type", "Spot Light"))
        {
            if(ImGui::Selectable("Direct Light"))
            {
                auto res = pfd::message{
                    "Arcadia - Changing Light Type",
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
                    "Arcadia - Changing Light Type",
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
                    "Arcadia - Changing Light Type",
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

        ImGui::DragFloat3("Attenuation", glm::value_ptr(light.AttenuationCoefficients), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();
        ImGui::SameLine();
        ImguiWrappers::HelpMark(ICON_FA_QUESTION, "In order of constant, linear and quadratic terms");

        const float cutoff_angle_drag_speend = .1f;
        const float cutoff_angle_min = 0.f;
        const float cutoff_angle_max = 180.f;
        ImGui::DragFloat2("Cutoff Angle", glm::value_ptr(light.CutoffAngles), cutoff_angle_drag_speend, cutoff_angle_min, cutoff_angle_max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();
        ImGui::SameLine();
        ImguiWrappers::HelpMark(ICON_FA_QUESTION, "Inner and outter");

        ImGui::ColorEdit3("Color", glm::value_ptr(light.Color));
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        ImGui::NewLine();

        ImGui::DragFloat3("Ambient Strenght", glm::value_ptr(light.AmbientStrength), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        ImGui::DragFloat3("Diffuse Strenght", glm::value_ptr(light.AmbientStrength), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        ImGui::DragFloat3("Specular Strenght", glm::value_ptr(light.AmbientStrength), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        return edited;
    },
        [&](DirectLight& light)
    {
        bool edited = false;

        if(ImGui::BeginCombo("Light Type", "Direct Light"))
        {
            if(ImGui::Selectable("Spot Light"))
            {
                auto res = pfd::message{
                    "Arcadia - Changing Light Type",
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
                    "Arcadia - Changing Light Type",
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
                    "Arcadia - Changing Light Type",
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

        ImGui::ColorEdit3("Color", glm::value_ptr(light.Color));
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        ImGui::NewLine();

        ImGui::DragFloat3("Ambient Strenght", glm::value_ptr(light.AmbientStrength), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        ImGui::DragFloat3("Diffuse Strenght", glm::value_ptr(light.AmbientStrength), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        ImGui::DragFloat3("Specular Strenght", glm::value_ptr(light.AmbientStrength), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        return edited;
    },
        [&](AreaLight& light)
    {
        bool edited = false;

        if(ImGui::BeginCombo("Light Type", "Area Light"))
        {
            if(ImGui::Selectable("Spot Light"))
            {
                auto res = pfd::message{
                    "Arcadia - Changing Light Type",
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
                    "Arcadia - Changing Light Type",
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
                    "Arcadia - Changing Light Type",
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

        ImGui::DragFloat2("Size", glm::value_ptr(light.Size), speed, min, max, format, flags);

        ImGui::ColorEdit3("Color", glm::value_ptr(light.Color));
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        ImGui::NewLine();

        ImGui::DragFloat3("Ambient Strenght", glm::value_ptr(light.AmbientStrength), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        ImGui::DragFloat3("Diffuse Strenght", glm::value_ptr(light.AmbientStrength), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        ImGui::DragFloat3("Specular Strenght", glm::value_ptr(light.AmbientStrength), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        return edited;
    },
        [&](PointLight& light)
    {
        bool edited = false;

        if(ImGui::BeginCombo("Light Type", "Point Light"))
        {
            if(ImGui::Selectable("Spot Light"))
            {
                auto res = pfd::message{
                    "Arcadia - Changing Light Type",
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
                    "Arcadia - Changing Light Type",
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
                    "Arcadia - Changing Light Type",
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

        ImGui::DragFloat3("Attenuation", glm::value_ptr(light.AttenuationCoefficients), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();
        ImGui::SameLine();
        ImguiWrappers::HelpMark(ICON_FA_QUESTION, "In order of constant, linear and quadratic terms");

        ImGui::ColorEdit3("Color", glm::value_ptr(light.Color));
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        ImGui::NewLine();

        ImGui::DragFloat3("Ambient Strenght", glm::value_ptr(light.AmbientStrength), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        ImGui::DragFloat3("Diffuse Strenght", glm::value_ptr(light.AmbientStrength), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        ImGui::DragFloat3("Specular Strenght", glm::value_ptr(light.AmbientStrength), speed, min, max, format, flags);
        edited |= ImGui::IsItemDeactivatedAfterEdit();

        return edited;
    }
    )){
        description = "Light";
    }

    ImGui::EndGroup();

    return description;
}

auto Arcadia::ImguiWindowPropertyModelComponent::operator()(ModelComponent& model_comp) -> std::string
{
    std::string description{};
    ImGui::BeginGroup();

    ImGui::SeparatorText("Filepath");
    std::string filepath_string = model_comp.GetFilepath().empty()
        ? "(No filepath)"
        : model_comp.GetFilepath().generic_string();
    ImGui::TextWrapped(filepath_string.c_str());
    if(ImGui::Button("..."))
    {
        auto res = pfd::open_file{
            "Import Model"
        }.result();
        model_comp.Import(res.size() ? res.at(0) : "");
    }

    ImGui::EndGroup();

    return description;
}

void Arcadia::ImguiWindowPopupPhysicsComponentCreateBody::operator()(PhysicsComponent& physics_comp)
{
    if(!Opened)
    {
        return;
    }

    std::string imgui_window_title("Physics Component - Create Body");

    auto popup_flags =
        ImGuiPopupFlags_NoOpenOverExistingPopup;
    ImGui::OpenPopup(imgui_window_title.c_str(), popup_flags);

    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::BeginPopupModal(imgui_window_title.c_str(), &Opened, window_flags))
    {
        float speed = .05f;
        float min = .0f;
        float max = .0f;
        const char* format = "%.3f";
        auto slider_flags =
            ImGuiSliderFlags_AlwaysClamp;

        // Motion type
        auto jph_motion_type_preview = Match<std::string>(
            _TempJphBodyInfo.JphMotionType,
            JPH::EMotionType::Static,
            "Static",
            JPH::EMotionType::Dynamic,
            "Dynamic",
            JPH::EMotionType::Kinematic,
            "Kinematic"
        );
        if(ImGui::BeginCombo("Motion Type", jph_motion_type_preview.c_str()))
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
        _TempJphBodyInfo.JphShapeInfo = MatchVariant<JphShapeInfo>(
            _TempJphBodyInfo.JphShapeInfo,
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
            auto half_extent_min = std::max({ .01f,info.ConvexRadius });
            auto half_extent_max = FLT_MAX;
            ImGui::DragFloat3("Half Extent", glm::value_ptr(info.HalfExtent), speed, half_extent_min, half_extent_max, format, slider_flags);

            // Convex radius
            auto convex_radius_min = .0f;
            auto convex_radius_max = std::min({ info.HalfExtent.x,info.HalfExtent.y,info.HalfExtent.z });
            ImGui::DragFloat("Convex Radius", &info.ConvexRadius, speed, convex_radius_min, convex_radius_max, format, slider_flags);

            return _TempJphBodyInfo.JphShapeInfo;
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
            ImGui::DragFloat("Radius", &info.Radius, speed, radius_min, radius_max, format, slider_flags);

            auto half_height_of_cylinder_min = .0f;
            auto half_height_of_cylinder_max = FLT_MAX;
            ImGui::DragFloat("Half Height if Cylinder", &info.HalfHeightOfCylinder, speed, half_height_of_cylinder_min, half_height_of_cylinder_max, format, slider_flags);

            return _TempJphBodyInfo.JphShapeInfo;
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
            ImGui::DragFloat("Half Height", &info.HalfHeight, speed, half_height_min, half_height_max, format, slider_flags);

            auto radius_min = .0f;
            auto radius_max = FLT_MAX;
            ImGui::DragFloat("Radius", &info.Radius, speed, radius_min, radius_max, format, slider_flags);

            auto convex_radius_min = .0f;
            auto convex_radius_max = FLT_MAX;
            ImGui::DragFloat("Convex Radius", &info.ConvexRadius, speed, convex_radius_min, convex_radius_max, format, slider_flags);

            return _TempJphBodyInfo.JphShapeInfo;
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
            ImGui::DragFloat("Radius", &info.Radius, speed, radius_min, radius_max, format, slider_flags);

            return _TempJphBodyInfo.JphShapeInfo;
        }
        );

        ImGui::NewLine();
        auto confirmed = ImGui::Button("Confirm");
        if(confirmed)
        {
            physics_comp.BuildIndentifiableJphBodyInfo(
                _TempJphBodyInfo
            );
        }
        ImGui::SameLine();
        if(confirmed || ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            Opened = false;
            _TempJphBodyInfo = JphBodyInfo{};
        }

        ImGui::EndPopup();
    }
}

auto Arcadia::ImguiWindowPropertyPhysicsComponent::operator()(PhysicsComponent& physics_comp) -> std::string
{
    _ImguiWindowPopupPhysicsComponentCreateBody(physics_comp);

    std::string description{};
    ImGui::BeginGroup();

    if(physics_comp.HasBodyInfo())
    {
        const auto& [uuid, jph_body_info_initial] = physics_comp.GetIdentifiableJphBodyInfo();

        if(ImGui::TreeNodeEx("Initial", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding))
        {
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
            ImGui::TreePop();
        }

        if(ImGui::TreeNodeEx("Current", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding))
        {
            const auto& jph_body_state = physics_comp.JphBodyState;
            ImGui::Text(std::format("Active: {}", jph_body_state.Active).c_str());
            ImGui::Text(std::format("Linear Velocity - {}", jph_body_state.LinearVelocity).c_str());
            ImGui::Text(std::format("Angular Velocity - {}", jph_body_state.AngularVelocity).c_str());
            ImGui::TreePop();
        }

        bool tree_open = MatchVariant<bool>(
            jph_body_info_initial.JphShapeInfo,
            [&](const JphBoxShapeInfo& info)
        {
            bool tree_open = ImGui::TreeNodeEx("Body Shape - Box", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding);
            if(tree_open)
            {
                ImGui::Text(std::format("Half Extent: {}", info.HalfExtent).c_str());
                ImGui::Text(std::format("Convex Radius: {:.2f}", info.ConvexRadius).c_str());
            }
            return tree_open;
        },
            [&](const JphCapsuleShapeInfo& info)
        {
            bool tree_open = ImGui::TreeNodeEx("Body Shape - Capsule", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding);
            if(tree_open)
            {
                ImGui::Text(std::format("Radius: {:.2f}", info.Radius).c_str());
                ImGui::Text(std::format("Half Height of Cylinder: {:.2f}", info.HalfHeightOfCylinder).c_str());
            }
            return tree_open;
        },
            [&](const JphCylinderShapeInfo& info)
        {
            bool tree_open = ImGui::TreeNodeEx("Body Shape - Cylinder", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding);
            if(tree_open)
            {
                ImGui::Text(std::format("Half Height: {:.2f}", info.HalfHeight).c_str());
                ImGui::Text(std::format("Radius: {:.2f}", info.Radius).c_str());
                ImGui::Text(std::format("Convex Radius: {:.2f}", info.ConvexRadius).c_str());
            }
            return tree_open;
        },
            [&](const JphSphereShapeInfo& info)
        {
            bool tree_open = ImGui::TreeNodeEx("Body Shape - Sphere", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding);
            if(tree_open)
            {
                ImGui::Text(std::format("Radius: {:.2f}", info.Radius).c_str());
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

    if(physics_comp.HasBodyInfo())
    {
        ImGui::ColorEdit3("Body Shape Color", glm::value_ptr(physics_comp.BodyShapeColor));
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
            description = "Body Shape Color";
        }

        if(ImGui::Button("Recreate Body"))
        {
            _ImguiWindowPopupPhysicsComponentCreateBody.Opened = true;
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
            _ImguiWindowPopupPhysicsComponentCreateBody.Opened = true;
        }
    }

    ImGui::EndGroup();

    return description;
}

auto Arcadia::ImguiWindowPropertyTransformComponent::operator()(TransformComponent& transform_comp) -> std::string
{
    std::string description{};
    ImGui::BeginGroup();

    const float speed = 1.f;
    const float min = .0f;
    const float max = .0f;
    const char* format = "%.3f";
    const auto flags =
        ImGuiSliderFlags_AlwaysClamp;

    auto position_delta = transform_comp.Position; // Previous position
    ImGui::DragFloat3("Position", glm::value_ptr(transform_comp.Position), speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        description = "Position";
    }
    position_delta = transform_comp.Position - position_delta; // current - previous

    if(!!(transform_comp.Flags & TransformComponentFlags::UseRotation))
    {
        float rotation_drag_speed = .05f;
        glm::quat temp = transform_comp.Rotation;
        ImGui::DragFloat3("Rotation", glm::value_ptr(temp), rotation_drag_speed, min, max, format, flags);
        transform_comp.Rotation = GlmQuat::NormalizeFixedly(transform_comp.Rotation, temp);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
            description = "Rotation";
        }
    }
    if(!!(transform_comp.Flags & TransformComponentFlags::UseDirection))
    {
        float direction_drag_speed = .05f;
        glm::vec3 temp = transform_comp.Direction;
        ImGui::DragFloat3("Direction", glm::value_ptr(temp), direction_drag_speed, min, max, format, flags);
        transform_comp.Direction = GlmVec3::NormalizeFixedly(transform_comp.Direction, temp);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
            description = "Direction";
        }
    }

    ImGui::DragFloat3("Scale", glm::value_ptr(transform_comp.Scale), speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        description = "Scale";
    }

    if(position_delta != GlmVec3::CreateZero())
    {
        transform_comp.Pivot += position_delta; // Make pivot move with translation
    }
    ImGui::DragFloat3("Pivot", glm::value_ptr(transform_comp.Pivot), speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        description = "Pivot";
    }

    ImGui::EndGroup();

    return description;
}

void Arcadia::ImguiWindowProperty::OnEvent(EventBase& e)
{
    EventDispatcher{ e }
        .Dispatch<Events::OpenImguiWindow>(ACDA_BIND_MEMBER_FN(_OnOpenImguiWindow))
        .Dispatch<Events::SceneActivated>(ACDA_BIND_MEMBER_FN(_OnSceneActivated))
        .Dispatch<Events::SceneDeactivated>(ACDA_BIND_MEMBER_FN(_OnSceneDeactivated))
        .Dispatch<Events::SelectEntity>(ACDA_BIND_MEMBER_FN(_OnSelectEntity))
        .Dispatch<Events::RenameEntity>(ACDA_BIND_MEMBER_FN(_OnRenameEntity))
        .Dispatch<Events::DeleteEntity>(ACDA_BIND_MEMBER_FN(_OnDeleteEntity))
        .IsDispatched();
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
                [scene,this]() -> component_type&\
            {\
                return scene->GetComponent<component_type>(_SelectedEntityName);\
            }\
        );\
    }\
    ImGui::TreePop();\
}

void Arcadia::ImguiWindowProperty::OnUpdate()
{
    if(!_Opened)
    {
        return;
    }

    auto scene = _Scene.lock();

    auto imgui_title = scene && !_SelectedEntityName.empty()
        ? _Title + " - " + _SelectedEntityName + GetIdString()
        : _Title + GetIdString();

    ImGui::SetNextWindowSize(glm::vec2{ 1024,768 }, ImGuiCond_Once);
    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_title.c_str(), &_Opened, window_flags))
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
            if(!_SelectedEntityName.empty())
            {
                ImGui::PushItemWidth(200.f);

                auto& memento_list = MementoList::Instance();

                ACDA_IMGUI_WINDOW_PROPERTY_HELPER(CameraComponent, std::string("Camera"), _ImguiWindowPropertyCameraComponent);
                ACDA_IMGUI_WINDOW_PROPERTY_HELPER(LightComponent, std::string("Light"), _ImguiWindowPropertyLightComponent);
                ACDA_IMGUI_WINDOW_PROPERTY_HELPER(ModelComponent, std::string("Model"), _ImguiWindowPropertyModelComponent);
                ACDA_IMGUI_WINDOW_PROPERTY_HELPER(PhysicsComponent, std::string("Physics"), _ImguiWindowPropertyPhysicsComponent);
                ACDA_IMGUI_WINDOW_PROPERTY_HELPER(TransformComponent, std::string("Transform"), _ImguiWindowPropertyTransformComponent);

                ImGui::PopItemWidth();
            }
        }
    }
    ImGui::End();
}

void Arcadia::ImguiWindowProperty::_OnOpenImguiWindow(Events::OpenImguiWindow& e)
{
    if(e.IdString == GetIdString())
    {
        _Opened = true;
    }
}

void Arcadia::ImguiWindowProperty::_OnSceneActivated(Events::SceneActivated& e)
{
    _Scene = e.Scene;
}

void Arcadia::ImguiWindowProperty::_OnSceneDeactivated(Events::SceneDeactivated& e)
{
    _Scene.reset();
    _SelectedEntityName.clear();
}

void Arcadia::ImguiWindowProperty::_OnSelectEntity(Events::SelectEntity& e)
{
    _SelectedEntityName = e.EntityName;
}

void Arcadia::ImguiWindowProperty::_OnRenameEntity(Events::RenameEntity& e)
{
    if(e.OldName == _SelectedEntityName)
    {
        _SelectedEntityName = e.NewName;
    }
}

void Arcadia::ImguiWindowProperty::_OnDeleteEntity(Events::DeleteEntity& e)
{
    if(_SelectedEntityName == e.EntityName)
    {
        _SelectedEntityName.clear();
    }
}