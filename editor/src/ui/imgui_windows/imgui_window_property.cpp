#include "imgui_window_property.hpp"

#include"core/file/pfd_header.hpp"
#include"function/ui/imgui_header.hpp"

void arcadia::imgui_window_property::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
        .dispatch<arcadia::event::open_imgui_window>(ARCADIA_BIND_MEMBER_FN(_on_open_imgui_window))
        .dispatch<arcadia::event::scene_activated>(ARCADIA_BIND_MEMBER_FN(_on_scene_activated))
        .dispatch<arcadia::event::scene_deactivated>(ARCADIA_BIND_MEMBER_FN(_on_scene_deactivated))
        .dispatch<arcadia::event::select_entity>(ARCADIA_BIND_MEMBER_FN(_on_select_entity))
        .dispatch<arcadia::event::delete_entity>(ARCADIA_BIND_MEMBER_FN(_on_delete_entity))
        .result();
}

void arcadia::imgui_window_property::on_update()
{
    if(!_open)
    {
        return;
    }

    auto has_scene = !_scene_wptr.expired();
    std::shared_ptr<const arcadia::scene> scene_sptr = _scene_wptr.lock();

    auto imgui_title = has_scene && _selected_entity != entt::null
        ? _title + " - " + scene_sptr->get_name_of_entity(_selected_entity) + get_id_str()
        : _title + get_id_str();

    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_title.c_str(), &_open, window_flags))
    {
        if(!has_scene)
        {
            ImGui::Text("No property to show here");
        }
        else
        {
            _display_components();
        }
    }
    ImGui::End();
}

void arcadia::imgui_window_property::_display_components()
{
    ARCADIA_ASSERT(!_scene_wptr.expired());

    _display_camera_component();
    _display_light_component();
    _display_model_component();
}

void arcadia::imgui_window_property::_display_camera_component()
{
    if(!_contains_component<arcadia::camera_component>())
    {
        return;
    }

    auto& camera_comp = _get_component<arcadia::camera_component>();
    ImGui::BeginGroup();

    ImGui::PushItemWidth(200.f);

    ImGui::SeparatorText("Camera");

    ImGui::NewLine();
    ImGui::Text("     Position X"); ImGui::SameLine(); ImGui::DragFloat("##pos_x", &camera_comp.pos.x);
    ImGui::Text("              Y"); ImGui::SameLine(); ImGui::DragFloat("##pos_y", &camera_comp.pos.y);
    ImGui::Text("              Z"); ImGui::SameLine(); ImGui::DragFloat("##pos_z", &camera_comp.pos.z);

    ImGui::NewLine();
    ImGui::Text("       Target X"); ImGui::SameLine(); ImGui::DragFloat("##target_x", &camera_comp.target.x);
    ImGui::Text("              Y"); ImGui::SameLine(); ImGui::DragFloat("##target_y", &camera_comp.target.y);
    ImGui::Text("              Z"); ImGui::SameLine(); ImGui::DragFloat("##target_z", &camera_comp.target.z);

    ImGui::NewLine();
    ImGui::Text("           Up X"); ImGui::SameLine(); ImGui::DragFloat("##up_x", &camera_comp.up.x);
    ImGui::Text("              Y"); ImGui::SameLine(); ImGui::DragFloat("##up_y", &camera_comp.up.y);
    ImGui::Text("              Z"); ImGui::SameLine(); ImGui::DragFloat("##up_z", &camera_comp.up.z);

    ImGui::NewLine();
    ImGui::Text("     Near Plane"); ImGui::SameLine(); ImGui::DragFloat("##near", &camera_comp.near_plane);

    ImGui::NewLine();
    ImGui::Text("      Far Plane"); ImGui::SameLine(); ImGui::DragFloat("##far", &camera_comp.far_plane);

    ImGui::NewLine();
    ImGui::Text("            FOV"); ImGui::SameLine(); ImGui::DragFloat("##fov", &camera_comp.fov);

    ImGui::NewLine();
    ImGui::Text("        FOV Min"); ImGui::SameLine(); ImGui::DragFloat("##fov_min", &camera_comp.fov_min);

    ImGui::NewLine();
    ImGui::Text("        FOV Max"); ImGui::SameLine(); ImGui::DragFloat("##fov_max", &camera_comp.fov_max);

    ImGui::NewLine();
    ImGui::Text("          Speed"); ImGui::SameLine(); ImGui::DragFloat("##speed", &camera_comp.speed);

    ImGui::NewLine();
    ImGui::Text("Viewport Size X"); ImGui::SameLine(); ImGui::DragInt("##viewport_size_x", &camera_comp.viewport_size.x);
    ImGui::Text("              Y"); ImGui::SameLine(); ImGui::DragInt("##viewport_size_y", &camera_comp.viewport_size.y);

    ImGui::NewLine();
    ImGui::Text("       Fixed Up"); ImGui::SameLine(); ImGui::Checkbox("##fixed_up", &camera_comp.fixed_up);

    ImGui::NewFrame();
    ImGui::Text("     Up Epsilon"); ImGui::SameLine(); ImGui::DragFloat("##up_epsilon", &camera_comp.up_epsilon);

    //ImGui::DragFloat2("Cursor Move Offset Range", &camera_comp.cursor_move_offset_range.x);

    ImGui::PopItemWidth();

    ImGui::EndGroup();
}

void arcadia::imgui_window_property::_display_light_component()
{
    if(!_contains_component<arcadia::light_component>())
    {
        return;
    }

    auto& light_comp = _get_component<arcadia::light_component>();
    ImGui::BeginGroup();

    ImGui::PushItemWidth(200.f);

    const float light_direction_drag_speed = .01f;
    const float light_direction_min = -1.f;
    const float light_direction_max = 1.f;
    const float light_color_drag_speed = .005f;
    const float light_color_min = .0f;
    const float light_color_max = 1.f;
    const float light_strength_speed = 1.f;
    const float light_strength_min = 0.f;
    const float light_strength_max = 100.f;
    arcadia::match<void>(
        light_comp.light,
        [&](arcadia::null_light& light)
    {
        if(ImGui::BeginCombo("Light Type", "(No light)"))
        {
            if(ImGui::Selectable("Spot Light"))
            {
                light_comp.light = arcadia::spot_light{};
            }
            if(ImGui::Selectable("Direct Light"))
            {
                light_comp.light = arcadia::direct_light{};
            }
            if(ImGui::Selectable("Area Light"))
            {
                light_comp.light = arcadia::area_light{};
            }
            if(ImGui::Selectable("Point Light"))
            {
                light_comp.light = arcadia::point_light{};
            }
            ImGui::EndCombo();
        }
    },
        [&](arcadia::spot_light& light)
    {
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
                    light_comp.light = arcadia::direct_light{};
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
                }
            }
            ImGui::EndCombo();
        }

        ImGui::SeparatorText("Spot Light");

        ImGui::NewLine();
        ImGui::Text("         Position X"); ImGui::SameLine(); ImGui::DragFloat("##pos_x", &light.position.x);
        ImGui::Text("                  Y"); ImGui::SameLine(); ImGui::DragFloat("##pos_y", &light.position.y);
        ImGui::Text("                  Z"); ImGui::SameLine(); ImGui::DragFloat("##pos_z", &light.position.z);

        ImGui::NewLine();
        ImGui::Text("        Direction X"); ImGui::SameLine(); ImGui::DragFloat("##dir_x", &light.direction.x, light_direction_drag_speed, light_direction_min, light_direction_max);
        ImGui::Text("                  Y"); ImGui::SameLine(); ImGui::DragFloat("##dir_y", &light.direction.y, light_direction_drag_speed, light_direction_min, light_direction_max);
        ImGui::Text("                  Z"); ImGui::SameLine(); ImGui::DragFloat("##dir_z", &light.direction.z, light_direction_drag_speed, light_direction_min, light_direction_max);

        ImGui::NewLine();
        ImGui::Text("Attenuation Contant"); ImGui::SameLine(); ImGui::DragFloat("##k0", &light.attenuation_coefs.x);
        ImGui::Text("             Linear"); ImGui::SameLine(); ImGui::DragFloat("##k1", &light.attenuation_coefs.y);
        ImGui::Text("          Quadratic"); ImGui::SameLine(); ImGui::DragFloat("##k2", &light.attenuation_coefs.z);

        ImGui::NewLine();

        glm::vec2 cutoff_angles_degree{
            glm::degrees(light.cutoff_angles.x),
            glm::degrees(light.cutoff_angles.y)
        };
        const float cutoff_angle_drag_speend = .1f;
        const float cutoff_angle_min = 0.f;
        const float cutoff_angle_max = 180.f;
        ImGui::Text(" Inner Cutoff Angle"); ImGui::SameLine(); ImGui::DragFloat("##inner", &cutoff_angles_degree.x, cutoff_angle_drag_speend, cutoff_angle_min, cutoff_angles_degree.y);
        ImGui::Text(" Outer Cutoff Angle"); ImGui::SameLine(); ImGui::DragFloat("##outer", &cutoff_angles_degree.y, cutoff_angle_drag_speend, cutoff_angles_degree.x, cutoff_angle_max);
        light.cutoff_angles = glm::vec2{
            glm::radians(cutoff_angles_degree.x),
            glm::radians(cutoff_angles_degree.y)
        };

        ImGui::NewLine();
        ImGui::Text("            Color R"); ImGui::SameLine(); ImGui::DragFloat("##color_r", &light.color.r, light_color_drag_speed, light_color_min, light_color_max);
        ImGui::Text("                  G"); ImGui::SameLine(); ImGui::DragFloat("##color_g", &light.color.g, light_color_drag_speed, light_color_min, light_color_max);
        ImGui::Text("                  B"); ImGui::SameLine(); ImGui::DragFloat("##color_b", &light.color.b, light_color_drag_speed, light_color_min, light_color_max);

        ImGui::NewLine();
        ImGui::Text(" Ambient Strength R"); ImGui::SameLine(); ImGui::DragFloat("##as_r", &light.ambient_strength.r, light_strength_speed, light_strength_min, light_strength_max);
        ImGui::Text("                  G"); ImGui::SameLine(); ImGui::DragFloat("##as_g", &light.ambient_strength.g, light_strength_speed, light_strength_min, light_strength_max);
        ImGui::Text("                  B"); ImGui::SameLine(); ImGui::DragFloat("##as_b", &light.ambient_strength.b, light_strength_speed, light_strength_min, light_strength_max);

        ImGui::NewLine();
        ImGui::Text(" Diffuse Strength R"); ImGui::SameLine(); ImGui::DragFloat("##ds_r", &light.diffuse_strength.r, light_strength_speed, light_strength_min, light_strength_max);
        ImGui::Text("                  G"); ImGui::SameLine(); ImGui::DragFloat("##ds_g", &light.diffuse_strength.g, light_strength_speed, light_strength_min, light_strength_max);
        ImGui::Text("                  B"); ImGui::SameLine(); ImGui::DragFloat("##ds_b", &light.diffuse_strength.b, light_strength_speed, light_strength_min, light_strength_max);

        ImGui::NewLine();
        ImGui::Text("Specular Strength R"); ImGui::SameLine(); ImGui::DragFloat("##ss_r", &light.specular_strength.r, light_strength_speed, light_strength_min, light_strength_max);
        ImGui::Text("                  G"); ImGui::SameLine(); ImGui::DragFloat("##ss_g", &light.specular_strength.g, light_strength_speed, light_strength_min, light_strength_max);
        ImGui::Text("                  B"); ImGui::SameLine(); ImGui::DragFloat("##ss_b", &light.specular_strength.b, light_strength_speed, light_strength_min, light_strength_max);
    },
        [&](arcadia::direct_light& light)
    {
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
                    light_comp.light = arcadia::spot_light{};
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
                }
            }
            ImGui::EndCombo();
        }

        ImGui::SeparatorText("Direct Light");

        ImGui::NewLine();
        ImGui::Text("        Direction X"); ImGui::SameLine(); ImGui::DragFloat("##dir_x", &light.direction.x, light_direction_drag_speed, light_direction_min, light_direction_max);
        ImGui::Text("                  Y"); ImGui::SameLine(); ImGui::DragFloat("##dir_y", &light.direction.y, light_direction_drag_speed, light_direction_min, light_direction_max);
        ImGui::Text("                  Z"); ImGui::SameLine(); ImGui::DragFloat("##dir_z", &light.direction.z, light_direction_drag_speed, light_direction_min, light_direction_max);

        ImGui::NewLine();
        ImGui::Text("            Color R"); ImGui::SameLine(); ImGui::DragFloat("##color_r", &light.color.r, light_color_drag_speed, light_color_min, light_color_max);
        ImGui::Text("                  G"); ImGui::SameLine(); ImGui::DragFloat("##color_g", &light.color.g, light_color_drag_speed, light_color_min, light_color_max);
        ImGui::Text("                  B"); ImGui::SameLine(); ImGui::DragFloat("##color_b", &light.color.b, light_color_drag_speed, light_color_min, light_color_max);

        ImGui::NewLine();
        ImGui::Text(" Ambient Strength R"); ImGui::SameLine(); ImGui::DragFloat("##as_r", &light.ambient_strength.r, light_strength_speed, light_strength_min, light_strength_max);
        ImGui::Text("                  G"); ImGui::SameLine(); ImGui::DragFloat("##as_g", &light.ambient_strength.g, light_strength_speed, light_strength_min, light_strength_max);
        ImGui::Text("                  B"); ImGui::SameLine(); ImGui::DragFloat("##as_b", &light.ambient_strength.b, light_strength_speed, light_strength_min, light_strength_max);

        ImGui::NewLine();
        ImGui::Text(" Diffuse Strength R"); ImGui::SameLine(); ImGui::DragFloat("##ds_r", &light.diffuse_strength.r, light_strength_speed, light_strength_min, light_strength_max);
        ImGui::Text("                  G"); ImGui::SameLine(); ImGui::DragFloat("##ds_g", &light.diffuse_strength.g, light_strength_speed, light_strength_min, light_strength_max);
        ImGui::Text("                  B"); ImGui::SameLine(); ImGui::DragFloat("##ds_b", &light.diffuse_strength.b, light_strength_speed, light_strength_min, light_strength_max);

        ImGui::NewLine();
        ImGui::Text("Specular Strength R"); ImGui::SameLine(); ImGui::DragFloat("##ss_r", &light.specular_strength.r, light_strength_speed, light_strength_min, light_strength_max);
        ImGui::Text("                  G"); ImGui::SameLine(); ImGui::DragFloat("##ss_g", &light.specular_strength.g, light_strength_speed, light_strength_min, light_strength_max);
        ImGui::Text("                  B"); ImGui::SameLine(); ImGui::DragFloat("##ss_b", &light.specular_strength.b, light_strength_speed, light_strength_min, light_strength_max);
    },
        [&](arcadia::area_light& light)
    {
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
                    light_comp.light = arcadia::spot_light{};
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
                }
            }
            ImGui::EndCombo();
        }

        ImGui::SeparatorText("Area Light");

        ImGui::NewLine();
        ImGui::Text("         Position X"); ImGui::SameLine(); ImGui::DragFloat("##pos_x", &light.position.x);
        ImGui::Text("                  Y"); ImGui::SameLine(); ImGui::DragFloat("##pos_y", &light.position.y);
        ImGui::Text("                  Z"); ImGui::SameLine(); ImGui::DragFloat("##pos_z", &light.position.z);

        ImGui::NewLine();
        ImGui::Text("        Direction X"); ImGui::SameLine(); ImGui::DragFloat("##dir_x", &light.direction.x, light_direction_drag_speed, light_direction_min, light_direction_max);
        ImGui::Text("                  Y"); ImGui::SameLine(); ImGui::DragFloat("##dir_y", &light.direction.y, light_direction_drag_speed, light_direction_min, light_direction_max);
        ImGui::Text("                  Z"); ImGui::SameLine(); ImGui::DragFloat("##dir_z", &light.direction.z, light_direction_drag_speed, light_direction_min, light_direction_max);

        ImGui::NewLine();
        ImGui::Text("         Size Width"); ImGui::SameLine(); ImGui::DragFloat("##width", &light.size.x);
        ImGui::Text("             Height"); ImGui::SameLine(); ImGui::DragFloat("##height", &light.size.y);

        ImGui::NewLine();
        ImGui::Text("            Color R"); ImGui::SameLine(); ImGui::DragFloat("##color_r", &light.color.r, light_color_drag_speed, light_color_min, light_color_max);
        ImGui::Text("                  G"); ImGui::SameLine(); ImGui::DragFloat("##color_g", &light.color.g, light_color_drag_speed, light_color_min, light_color_max);
        ImGui::Text("                  B"); ImGui::SameLine(); ImGui::DragFloat("##color_b", &light.color.b, light_color_drag_speed, light_color_min, light_color_max);

        ImGui::NewLine();
        ImGui::Text(" Ambient Strength R"); ImGui::SameLine(); ImGui::DragFloat("##as_r", &light.ambient_strength.r, light_strength_speed, light_strength_min, light_strength_max);
        ImGui::Text("                  G"); ImGui::SameLine(); ImGui::DragFloat("##as_g", &light.ambient_strength.g, light_strength_speed, light_strength_min, light_strength_max);
        ImGui::Text("                  B"); ImGui::SameLine(); ImGui::DragFloat("##as_b", &light.ambient_strength.b, light_strength_speed, light_strength_min, light_strength_max);

        ImGui::NewLine();
        ImGui::Text(" Diffuse Strength R"); ImGui::SameLine(); ImGui::DragFloat("##ds_r", &light.diffuse_strength.r, light_strength_speed, light_strength_min, light_strength_max);
        ImGui::Text("                  G"); ImGui::SameLine(); ImGui::DragFloat("##ds_g", &light.diffuse_strength.g, light_strength_speed, light_strength_min, light_strength_max);
        ImGui::Text("                  B"); ImGui::SameLine(); ImGui::DragFloat("##ds_b", &light.diffuse_strength.b, light_strength_speed, light_strength_min, light_strength_max);

        ImGui::NewLine();
        ImGui::Text("Specular Strength R"); ImGui::SameLine(); ImGui::DragFloat("##ss_r", &light.specular_strength.r, light_strength_speed, light_strength_min, light_strength_max);
        ImGui::Text("                  G"); ImGui::SameLine(); ImGui::DragFloat("##ss_g", &light.specular_strength.g, light_strength_speed, light_strength_min, light_strength_max);
        ImGui::Text("                  B"); ImGui::SameLine(); ImGui::DragFloat("##ss_b", &light.specular_strength.b, light_strength_speed, light_strength_min, light_strength_max);
    },
        [&](arcadia::point_light& light)
    {
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
                    light_comp.light = arcadia::spot_light{};
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
                }
            }
            ImGui::EndCombo();
        }

        ImGui::SeparatorText("Point Light");

        ImGui::NewLine();
        ImGui::Text("         Position X"); ImGui::SameLine(); ImGui::DragFloat("##pos_x", &light.position.x);
        ImGui::Text("                  Y"); ImGui::SameLine(); ImGui::DragFloat("##pos_y", &light.position.y);
        ImGui::Text("                  Z"); ImGui::SameLine(); ImGui::DragFloat("##pos_z", &light.position.z);

        ImGui::NewLine();
        ImGui::Text("Attenuation Contant"); ImGui::SameLine(); ImGui::DragFloat("##k0", &light.attenuation_coefs.x);
        ImGui::Text("             Linear"); ImGui::SameLine(); ImGui::DragFloat("##k1", &light.attenuation_coefs.y);
        ImGui::Text("          Quadratic"); ImGui::SameLine(); ImGui::DragFloat("##k2", &light.attenuation_coefs.z);

        ImGui::NewLine();
        ImGui::Text("            Color R"); ImGui::SameLine(); ImGui::DragFloat("##color_r", &light.color.r, light_color_drag_speed, light_color_min, light_color_max);
        ImGui::Text("                  G"); ImGui::SameLine(); ImGui::DragFloat("##color_g", &light.color.g, light_color_drag_speed, light_color_min, light_color_max);
        ImGui::Text("                  B"); ImGui::SameLine(); ImGui::DragFloat("##color_b", &light.color.b, light_color_drag_speed, light_color_min, light_color_max);

        ImGui::NewLine();
        ImGui::Text(" Ambient Strength R"); ImGui::SameLine(); ImGui::DragFloat("##as_r", &light.ambient_strength.r, light_strength_speed, light_strength_min, light_strength_max);
        ImGui::Text("                  G"); ImGui::SameLine(); ImGui::DragFloat("##as_g", &light.ambient_strength.g, light_strength_speed, light_strength_min, light_strength_max);
        ImGui::Text("                  B"); ImGui::SameLine(); ImGui::DragFloat("##as_b", &light.ambient_strength.b, light_strength_speed, light_strength_min, light_strength_max);

        ImGui::NewLine();
        ImGui::Text(" Diffuse Strength R"); ImGui::SameLine(); ImGui::DragFloat("##ds_r", &light.diffuse_strength.r, light_strength_speed, light_strength_min, light_strength_max);
        ImGui::Text("                  G"); ImGui::SameLine(); ImGui::DragFloat("##ds_g", &light.diffuse_strength.g, light_strength_speed, light_strength_min, light_strength_max);
        ImGui::Text("                  B"); ImGui::SameLine(); ImGui::DragFloat("##ds_b", &light.diffuse_strength.b, light_strength_speed, light_strength_min, light_strength_max);

        ImGui::NewLine();
        ImGui::Text("Specular Strength R"); ImGui::SameLine(); ImGui::DragFloat("##ss_r", &light.specular_strength.r, light_strength_speed, light_strength_min, light_strength_max);
        ImGui::Text("                  G"); ImGui::SameLine(); ImGui::DragFloat("##ss_g", &light.specular_strength.g, light_strength_speed, light_strength_min, light_strength_max);
        ImGui::Text("                  B"); ImGui::SameLine(); ImGui::DragFloat("##ss_b", &light.specular_strength.b, light_strength_speed, light_strength_min, light_strength_max);

    }
    );

    ImGui::PopItemWidth();

    ImGui::EndGroup();
}

void arcadia::imgui_window_property::_display_model_component()
{
    if(!_contains_component<arcadia::model_component>())
    {
        return;
    }

    auto& model_comp = _get_component<arcadia::model_component>();
    ImGui::BeginGroup();

    ImGui::PushItemWidth(200.f);

    ImGui::SeparatorText("Model");

    ImGui::Text("Filepath");
    auto filepath_str = model_comp.get_filepath().empty()
        ? "(No filepath)"s
        : model_comp.get_filepath().generic_string();
    ImGui::TextWrapped(filepath_str.c_str());
    ImGui::SameLine();
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

    ImGui::NewLine();
    ImGui::Text("Location X"); ImGui::SameLine(); ImGui::DragFloat("##location_x", &model_comp.location.x);
    ImGui::Text("         Y"); ImGui::SameLine(); ImGui::DragFloat("##location_y", &model_comp.location.y);
    ImGui::Text("         Z"); ImGui::SameLine(); ImGui::DragFloat("##location_z", &model_comp.location.z);

    ImGui::NewLine();
    float rotation_drag_speed{ .05f };
    ImGui::Text("Rotation X"); ImGui::SameLine(); ImGui::DragFloat("##rotation_x", &model_comp.rotation.x, rotation_drag_speed);
    ImGui::Text("         Y"); ImGui::SameLine(); ImGui::DragFloat("##rotation_y", &model_comp.rotation.y, rotation_drag_speed);
    ImGui::Text("         Z"); ImGui::SameLine(); ImGui::DragFloat("##rotation_z", &model_comp.rotation.z, rotation_drag_speed);

    ImGui::NewLine();
    ImGui::Text("   Scale X"); ImGui::SameLine(); ImGui::DragFloat("##scale_x", &model_comp.scale.x);
    ImGui::Text("         Y"); ImGui::SameLine(); ImGui::DragFloat("##scale_y", &model_comp.scale.y);
    ImGui::Text("         Z"); ImGui::SameLine(); ImGui::DragFloat("##scale_z", &model_comp.scale.z);

    ImGui::NewLine();
    ImGui::Text("   Pivot X"); ImGui::SameLine(); ImGui::DragFloat("##pivot_x", &model_comp.pivot.x);
    ImGui::Text("         Y"); ImGui::SameLine(); ImGui::DragFloat("##pivot_y", &model_comp.pivot.y);
    ImGui::Text("         Z"); ImGui::SameLine(); ImGui::DragFloat("##pivot_z", &model_comp.pivot.z);

    ImGui::PopItemWidth();

    ImGui::EndGroup();

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
