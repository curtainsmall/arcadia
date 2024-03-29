#include "imgui_window_property.hpp"

#include<algorithm>
#include<string>

#include"core/file/pfd_header.hpp"
#include"function/ui/imgui_header.hpp"
#include"function/ui/imgui_wrapper.hpp"
#include"resource/component/camera_component/camera_component.hpp"
#include"resource/component/light_component/light_component.hpp"
#include"resource/component/model_component/model_component.hpp"
#include"resource/component/physics_component/physics_component.hpp"
#include"resource/component/skybox_component/skybox_component.hpp"

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
        auto pos = arcadia::from_jph_vec3(_jph_position);
        arcadia::imgui_wrapper::drag_vec3("Position", pos, speed, min, max, format, slider_flags);
        _jph_position = arcadia::to_jph_vec3(pos);

        // Rotation
        ImGui::NewLine();
        auto rot = arcadia::from_jph_quat(_jph_rotation);
        float
            rot_speed = .05f,
            rot_min = -1.f,
            rot_max = 1.f;
        arcadia::imgui_wrapper::drag_quat_normalized("Rotation", rot, rot_speed, rot_min, rot_max, format, slider_flags);
        _jph_rotation = arcadia::to_jph_quat(rot);

        // Motion type
        ImGui::NewLine();
        auto jph_motion_type_preview = arcadia::match<std::string>(
            _jph_motion_type,
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
                _jph_motion_type = JPH::EMotionType::Static;
            }
            if(ImGui::Selectable("Dynamic"))
            {
                _jph_motion_type = JPH::EMotionType::Dynamic;
            }
            if(ImGui::Selectable("Kinematic"))
            {
                _jph_motion_type = JPH::EMotionType::Kinematic;
            }
            ImGui::EndCombo();
        }

        // Object layer
        auto jph_object_layer_prview = arcadia::match<std::string>(
            _jph_object_layer,
            arcadia::jph_object_layers::non_moving,
            "Non Moving",
            arcadia::jph_object_layers::moving,
            "Moving"
        );
        ImGui::Text("Object Layer");
        ImGui::SameLine();
        if(ImGui::BeginCombo("##object_layer", jph_object_layer_prview.c_str()))
        {
            if(ImGui::Selectable("Non Moving"))
            {
                _jph_object_layer = arcadia::jph_object_layers::non_moving;
            }
            if(ImGui::Selectable("Moving"))
            {
                _jph_object_layer = arcadia::jph_object_layers::moving;
            }
            ImGui::EndCombo();
        }

        //Shape
        _jph_shape_info = arcadia::match<arcadia::jph_shape_info_type>(
            _jph_shape_info,
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

            return _jph_shape_info;
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

            return _jph_shape_info;
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

            return _jph_shape_info;
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

            return _jph_shape_info;
        }
        );

        ImGui::NewLine();
        auto confirmed = ImGui::Button("Confirm");
        if(confirmed)
        {
            physics_comp.build_identifiable_jph_body_info(
                arcadia::from_jph_vec3(_jph_position),
                arcadia::from_jph_quat(_jph_rotation),
                _jph_motion_type,
                _jph_object_layer,
                std::move(_jph_shape_info)
            );
        }
        ImGui::SameLine();
        if(confirmed || ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            open = false;
            _jph_position = JPH::RVec3::sZero();
            _jph_rotation = JPH::Quat::sIdentity();
            _jph_motion_type = JPH::EMotionType::Static;
            _jph_object_layer = arcadia::jph_object_layers::non_moving;
            _jph_shape_info = arcadia::jph_box_shape_info{};
        }

        ImGui::EndPopup();
    }
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
            _display_components(scene_sptr);
        }
    }
    ImGui::End();
}

void arcadia::imgui_window_property::_display_components(const std::shared_ptr<arcadia::scene>& scene_sptr)
{
    ARCADIA_ASSERT(scene_sptr);

    auto tab_bar_flags =
        ImGuiTabBarFlags_NoCloseWithMiddleMouseButton
        | ImGuiTabBarFlags_TabListPopupButton
        | ImGuiTabBarFlags_AutoSelectNewTabs
        | ImGuiTabBarFlags_FittingPolicyScroll
        | ImGuiTabBarFlags_Reorderable;
    if(_selected_entity != entt::null && ImGui::BeginTabBar("##component_name", tab_bar_flags))
    {
        ImGui::PushItemWidth(200.f);

        if(_contains_component<arcadia::camera_component>(scene_sptr) && ImGui::BeginTabItem("Camera"))
        {
            _display_camera_component(scene_sptr);
            ImGui::EndTabItem();
        }
        if(_contains_component<arcadia::light_component>(scene_sptr) && ImGui::BeginTabItem("Light"))
        {
            _display_light_component(scene_sptr);
            ImGui::EndTabItem();
        }
        if(_contains_component<arcadia::model_component>(scene_sptr) && ImGui::BeginTabItem("Model"))
        {
            _display_model_component(scene_sptr);
            ImGui::EndTabItem();
        }
        if(_contains_component<arcadia::physics_component>(scene_sptr) && ImGui::BeginTabItem("Physics"))
        {
            _display_physics_component(scene_sptr);
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();

        ImGui::PopItemWidth();
    }
}

void arcadia::imgui_window_property::_display_camera_component(const std::shared_ptr<arcadia::scene>& scene_sptr)
{
    ARCADIA_ASSERT(scene_sptr);

    const float speed = 1.f;
    const float min = .0;
    const float max = .0f;
    const char* format = "%.3f";
    const auto flags =
        ImGuiSliderFlags_AlwaysClamp;
    auto& camera_comp = _get_component<arcadia::camera_component>(scene_sptr);
    ImGui::BeginGroup();

    ImGui::NewLine();
    arcadia::imgui_wrapper::drag_vec3(
        "Position",
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(camera_comp, get_position),
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(camera_comp, set_position),
        speed,
        min,
        max,
        format,
        flags
    );

    ImGui::NewLine();
    arcadia::imgui_wrapper::drag_vec3(
        "Target",
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(camera_comp, get_target),
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(camera_comp, set_target),
        speed,
        min,
        max, format,
        flags
    );

    ImGui::NewLine();
    arcadia::imgui_wrapper::drag_vec3(
        "Up",
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(camera_comp, get_up),
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(camera_comp, set_up),
        speed,
        min,
        max,
        format,
        flags
    );

    ImGui::NewLine();
    arcadia::imgui_wrapper::drag_float(
        "Near Plane",
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(camera_comp, get_near_plane),
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(camera_comp, set_near_plane),
        speed,
        min,
        max,
        format,
        flags
    );

    ImGui::NewLine();
    arcadia::imgui_wrapper::drag_float(
        "Far Plane",
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(camera_comp, get_far_plane),
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(camera_comp, set_far_plane),
        speed,
        min,
        max,
        format,
        flags
    );

    ImGui::NewLine();
    arcadia::imgui_wrapper::drag_float(
        "FOV",
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(camera_comp, get_fov),
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(camera_comp, set_fov),
        speed,
        camera_comp.get_fov_min(),
        camera_comp.get_fov_max(),
        format,
        flags
    );

    ImGui::NewLine();
    arcadia::imgui_wrapper::drag_float(
        "FOV Min",
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(camera_comp, get_fov_min),
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(camera_comp, set_fov_min),
        speed,
        .0f,
        180.f,
        format,
        flags
    );

    ImGui::NewLine();
    arcadia::imgui_wrapper::drag_float(
        "FOV Max",
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(camera_comp, get_fov_max),
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(camera_comp, set_fov_max),
        speed,
        .0f,
        180.f,
        format,
        flags
    );

    ImGui::NewLine();
    arcadia::imgui_wrapper::drag_float(
        "Speed",
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(camera_comp, get_speed),
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(camera_comp, set_speed),
        speed,
        min,
        max,
        format,
        flags
    );

    ImGui::NewLine();
    arcadia::imgui_wrapper::drag_vec2(
        "Viewport Size",
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(camera_comp, get_viewport_size),
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(camera_comp, set_viewport_size),
        speed,
        1.f,
        std::numeric_limits<float>::max(),
        format,
        flags
    );

    ImGui::NewLine();
    arcadia::imgui_wrapper::checkbox(
        "Fixed Up",
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(camera_comp, get_fixed_up),
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(camera_comp, set_fixed_up)
    );

    ImGui::NewLine();
    arcadia::imgui_wrapper::drag_float(
        "Up Epsilon",
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(camera_comp, get_up_epsilon),
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(camera_comp, set_up_epsilon),
        speed,
        min,
        max,
        format,
        flags
    );

    ImGui::EndGroup();
}

void arcadia::imgui_window_property::_display_light_component(const std::shared_ptr<arcadia::scene>& scene_sptr)
{
    ARCADIA_ASSERT(scene_sptr);

    auto& light_comp = _get_component<arcadia::light_component>(scene_sptr);
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
    arcadia::match<void>(
        light_comp.get_light(),
        [&](const arcadia::null_light& light)
    {
        ImGui::Text("Light Type");
        ImGui::SameLine();
        if(ImGui::BeginCombo("##light_type", "(No light)"))
        {
            if(ImGui::Selectable("Spot Light"))
            {
                light_comp.set_light(arcadia::spot_light{});
            }
            if(ImGui::Selectable("Direct Light"))
            {
                light_comp.set_light(arcadia::direct_light{});
            }
            if(ImGui::Selectable("Area Light"))
            {
                light_comp.set_light(arcadia::area_light{});
            }
            if(ImGui::Selectable("Point Light"))
            {
                light_comp.set_light(arcadia::point_light{});
            }
            ImGui::EndCombo();
        }
    },
        [&](const arcadia::spot_light& light)
    {
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
                    light_comp.set_light(arcadia::direct_light{});
                    ImGui::EndCombo();
                    return;
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
                    light_comp.set_light(arcadia::area_light{});
                    ImGui::EndCombo();
                    return;
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
                    light_comp.set_light(arcadia::point_light{});
                    ImGui::EndCombo();
                    return;
                }
            }
            ImGui::EndCombo();
        }

        auto temp_light = light;

        ImGui::NewLine();
        arcadia::imgui_wrapper::drag_vec3(
            "Position",
            temp_light.position,
            speed,
            min,
            max,
            format,
            flags
        );

        ImGui::NewLine();
        arcadia::imgui_wrapper::drag_vec3(
            "Direction",
            temp_light.direction,
            light_direction_drag_speed,
            light_direction_min,
            light_direction_max,
            format,
            flags
        );

        ImGui::NewLine();
        arcadia::imgui_wrapper::drag_float(
            "Attenuation Contant",
            temp_light.attenuation_coefs.x,
            speed,
            min,
            max,
            format,
            flags
        );
        arcadia::imgui_wrapper::drag_float(
            "             Linear",
            temp_light.attenuation_coefs.x,
            speed,
            min,
            max,
            format,
            flags
        );
        arcadia::imgui_wrapper::drag_float(
            "          Quadratic",
            temp_light.attenuation_coefs.x,
            speed,
            min,
            max,
            format,
            flags
        );

        ImGui::NewLine();

        glm::vec2 cutoff_angles_degree{
            glm::degrees(temp_light.cutoff_angles.x),
            glm::degrees(temp_light.cutoff_angles.y)
        };
        const float cutoff_angle_drag_speend = .1f;
        const float cutoff_angle_min = 0.f;
        const float cutoff_angle_max = 180.f;
        arcadia::imgui_wrapper::drag_float(
            "Inner Cutoff Angle",
            temp_light.attenuation_coefs.x,
            cutoff_angle_drag_speend,
            cutoff_angle_min,
            cutoff_angle_max,
            format,
            flags
        );
        arcadia::imgui_wrapper::drag_float(
            "Outer Cutoff Angle",
            temp_light.attenuation_coefs.x,
            cutoff_angle_drag_speend,
            cutoff_angle_min,
            cutoff_angle_max,
            format,
            flags
        );
        temp_light.cutoff_angles = glm::vec2{
            glm::radians(cutoff_angles_degree.x),
            glm::radians(cutoff_angles_degree.y)
        };

        ImGui::NewLine();
        arcadia::imgui_wrapper::color_edit3(
            "Color",
            temp_light.color
        );

        ImGui::NewLine();
        arcadia::imgui_wrapper::drag_vec3_color(
            "Ambient Strength",
            temp_light.ambient_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        arcadia::imgui_wrapper::drag_vec3_color(
            "Diffuse Strength",
            temp_light.diffuse_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        arcadia::imgui_wrapper::drag_vec3_color(
            "Specular Strength",
            temp_light.specular_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        light_comp.set_light(temp_light);
    },
        [&](const arcadia::direct_light& light)
    {
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
                    light_comp.set_light(arcadia::spot_light{});
                    ImGui::EndCombo();
                    return;
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
                    light_comp.set_light(arcadia::area_light{});
                    ImGui::EndCombo();
                    return;
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
                    light_comp.set_light(arcadia::point_light{});
                    ImGui::EndCombo();
                    return;
                }
            }
            ImGui::EndCombo();
        }

        auto temp_light = light;

        ImGui::NewLine();
        arcadia::imgui_wrapper::drag_vec3(
            "Direction",
            temp_light.direction,
            light_direction_drag_speed,
            light_direction_min,
            light_direction_max,
            format,
            flags
        );

        ImGui::NewLine();
        arcadia::imgui_wrapper::color_edit3(
            "Color",
            temp_light.color
        );

        ImGui::NewLine();
        arcadia::imgui_wrapper::drag_vec3_color(
            "Ambient Strengt",
            temp_light.ambient_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        arcadia::imgui_wrapper::drag_vec3_color(
            "Diffuse Strength",
            temp_light.diffuse_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        arcadia::imgui_wrapper::drag_vec3_color(
            "Specular Strength",
            temp_light.specular_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        light_comp.set_light(temp_light);
    },
        [&](const arcadia::area_light& light)
    {
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
                    light_comp.set_light(arcadia::spot_light{});
                    ImGui::EndCombo();
                    return;
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
                    light_comp.set_light(arcadia::direct_light{});
                    ImGui::EndCombo();
                    return;
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
                    light_comp.set_light(arcadia::point_light{});
                    ImGui::EndCombo();
                    return;
                }
            }
            ImGui::EndCombo();
        }

        auto temp_light = light;

        ImGui::NewLine();
        arcadia::imgui_wrapper::drag_vec3(
            "Position",
            temp_light.position,
            speed,
            min,
            max,
            format,
            flags
        );

        ImGui::NewLine();
        arcadia::imgui_wrapper::drag_vec3(
            "Direction",
            temp_light.direction,
            light_direction_drag_speed,
            light_direction_min,
            light_direction_max,
            format,
            flags
        );

        ImGui::NewLine();
        arcadia::imgui_wrapper::drag_float(
            "Width",
            temp_light.size.x,
            speed,
            min,
            max,
            format,
            flags
        );
        arcadia::imgui_wrapper::drag_float(
            "Height",
            temp_light.size.y,
            speed,
            min,
            max,
            format,
            flags
        );

        ImGui::NewLine();
        arcadia::imgui_wrapper::color_edit3(
            "Color",
            temp_light.color
        );

        ImGui::NewLine();
        arcadia::imgui_wrapper::drag_vec3_color(
            "Ambient Strength",
            temp_light.ambient_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        arcadia::imgui_wrapper::drag_vec3_color(
            "Diffuse Strength",
            temp_light.diffuse_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        arcadia::imgui_wrapper::drag_vec3_color(
            "Specular Strength",
            temp_light.specular_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        light_comp.set_light(temp_light);
    },
        [&](const arcadia::point_light& light)
    {
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
                    light_comp.set_light(arcadia::spot_light{});
                    ImGui::EndCombo();
                    return;
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
                    light_comp.set_light(arcadia::direct_light{});
                    ImGui::EndCombo();
                    return;
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
                    light_comp.set_light(arcadia::area_light{});
                    ImGui::EndCombo();
                    return;
                }
            }
            ImGui::EndCombo();
        }

        auto temp_light = light;

        ImGui::NewLine();
        arcadia::imgui_wrapper::drag_vec3(
            "Position",
            temp_light.position,
            speed,
            min,
            max,
            format,
            flags
        );

        ImGui::NewLine();
        arcadia::imgui_wrapper::drag_float(
            "Attenuation Contant",
            temp_light.attenuation_coefs.x,
            speed,
            min,
            max,
            format,
            flags
        );
        arcadia::imgui_wrapper::drag_float(
            "             Linear",
            temp_light.attenuation_coefs.x,
            speed,
            min,
            max,
            format,
            flags
        );
        arcadia::imgui_wrapper::drag_float(
            "          Quadratic",
            temp_light.attenuation_coefs.x,
            speed,
            min,
            max,
            format,
            flags
        );

        ImGui::NewLine();
        arcadia::imgui_wrapper::color_edit3(
            "Color",
            temp_light.color
        );

        ImGui::NewLine();
        arcadia::imgui_wrapper::drag_vec3_color(
            "Ambient Strength",
            temp_light.ambient_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        arcadia::imgui_wrapper::drag_vec3_color(
            "Diffuse Strength",
            temp_light.diffuse_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        ImGui::NewLine();
        arcadia::imgui_wrapper::drag_vec3_color(
            "Specular Strength",
            temp_light.specular_strength,
            light_strength_speed,
            light_strength_min,
            light_strength_max,
            format,
            flags
        );

        light_comp.set_light(temp_light);
    }
    );

    ImGui::EndGroup();
}

void arcadia::imgui_window_property::_display_model_component(const std::shared_ptr<arcadia::scene>& scene_sptr)
{
    ARCADIA_ASSERT(scene_sptr);

    const float speed = 1.f;
    const float min = .0f;
    const float max = .0f;
    const char* format = "%.3f";
    const auto flags =
        ImGuiSliderFlags_AlwaysClamp;

    auto& model_comp = _get_component<arcadia::model_component>(scene_sptr);
    ImGui::BeginGroup();

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
        std::filesystem::path filepath = res.size() ? res.at(0) : "";

        if(!filepath.empty())
        {
            model_comp.import(filepath);
        }
    }

    ImGui::SeparatorText("Transform");
    ImGui::NewLine();
    arcadia::imgui_wrapper::drag_vec3(
        "Location",
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(model_comp, get_location),
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(model_comp, set_location),
        speed,
        min,
        max,
        format,
        flags
    );

    ImGui::NewLine();
    float rotation_drag_speed{ .05f };
    arcadia::imgui_wrapper::drag_quat_normalized(
        "Rotation",
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(model_comp, get_rotation),
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(model_comp, set_rotation),
        rotation_drag_speed,
        min,
        max,
        format,
        flags
    );

    ImGui::NewLine();
    arcadia::imgui_wrapper::drag_vec3(
        "Scale",
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(model_comp, get_scale),
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(model_comp, set_scale),
        speed,
        min,
        max,
        format,
        flags
    );

    ImGui::NewLine();
    arcadia::imgui_wrapper::drag_vec3(
        "Pivot",
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(model_comp, get_pivot),
        ARCADIA_BIND_MEMBER_FN_ARBITRARY(model_comp, set_pivot),
        speed,
        min,
        max,
        format,
        flags
    );

    ImGui::EndGroup();

}

void arcadia::imgui_window_property::_display_physics_component(const std::shared_ptr<arcadia::scene>& scene_sptr)
{
    ARCADIA_ASSERT(scene_sptr);

    auto has_physics_simulator = !_physics_simulator_wptr.expired();
    auto physics_simulator_sptr = _physics_simulator_wptr.lock();

    auto& physics_comp = _get_component<arcadia::physics_component>(scene_sptr);

    _imgui_window_popup_physics_component_create_body(physics_comp);

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
                arcadia::imgui_wrapper::color_edit3(
                    "Body Shape Color",
                    ARCADIA_BIND_MEMBER_FN_ARBITRARY(physics_comp, get_body_shape_color),
                    ARCADIA_BIND_MEMBER_FN_ARBITRARY(physics_comp, set_body_shape_color)
                );

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
