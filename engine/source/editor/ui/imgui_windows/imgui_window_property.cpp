#include "imgui_window_property.hpp"

#include <string>

#include "core/assert.hpp"
#include "core/command.hpp"
#include "core/enum.hpp"
#include "core/function.hpp"
#include "core/match.hpp"
#include "core/pfd.hpp"
#include "resource/fonts/icon.hpp"
#include "resource/scene_layer.hpp"

#include "ui/imgui.hpp"
#include "ui/imgui_wrapper.hpp"

#define _ACDA_COMMAND_HELPER(msg, comp_name, getter_name, setter_name, value_name, origin_name) \
if(ImGui::IsItemDeactivatedAfterEdit())\
{\
    CommandList::Instance().Emplace(\
        msg,\
        [&comp = comp_name, value = value_name, origin_ptr = &origin_name]() -> void\
    {\
        comp.setter_name(value);\
        *origin_ptr = value;\
    },\
        [&comp = comp_name, origin = origin_name, origin_ptr = &origin_name]() -> void\
    {\
        comp.setter_name(origin);\
        *origin_ptr = origin;\
    }\
    );\
    origin_name = comp_name.getter_name();\
}

void Arcadia::ImguiWindowPropertyFunctor_CameraComponent::operator()(CameraComponent& camera_comp)
{
    const float drag_speed = 1.f;
    const float min = .0;
    const float max = .0f;
    const char* format = "%.3f";
    const ImGuiSliderFlags flags =
        ImGuiSliderFlags_AlwaysClamp;

    ImGui::BeginGroup();

    float near_plane = camera_comp.GetNearPlane();
    ImGui::DragFloat("Near Plane", &near_plane, drag_speed, min, max, format, flags);
    camera_comp.SetNearPlane(near_plane);
    _ACDA_COMMAND_HELPER("[Camera] Near Plane", camera_comp, GetNearPlane, SetNearPlane, near_plane, _OriginNearPlane);

    float far_plane = camera_comp.GetFarPlane();
    ImGui::DragFloat("Far Plane", &far_plane, drag_speed, min, max, format, flags);
    camera_comp.SetFarPlane(far_plane);
    _ACDA_COMMAND_HELPER("[Camera] Far Plane", camera_comp, GetFarPlane, SetFarPlane, far_plane, _OriginFarPlane);

    float fovy = glm::degrees(camera_comp.GetFovY());
    ImGui::DragFloat("FOV-Y", &fovy, drag_speed, camera_comp.GetFovYMin(), camera_comp.GetFovYMax(), format, flags);
    camera_comp.SetFovY(glm::radians(fovy));
    _ACDA_COMMAND_HELPER("[Camera] FOV-Y", camera_comp, GetFovY, SetFovY, fovy, _OriginFovY);

    float fovy_min = glm::degrees(camera_comp.GetFovYMin());
    ImGui::DragFloat("FOV-Y Min", &fovy_min, drag_speed, 0.0f, 180.0f, format, flags);
    camera_comp.SetFovYMin(glm::radians(fovy_min));
    _ACDA_COMMAND_HELPER("[Camera] FOV-Y Min", camera_comp, GetFovYMin, SetFovYMin, fovy_min, _OriginFovYMin);

    float fovy_max = glm::degrees(camera_comp.GetFovYMax());
    ImGui::DragFloat("FOV-Y Max", &fovy_max, drag_speed, 0.0f, 180.0f, format, flags);
    camera_comp.SetFovYMax(glm::radians(fovy_max));
    _ACDA_COMMAND_HELPER("[Camera] FOV-Y Max", camera_comp, GetFovYMax, SetFovYMax, fovy_max, _OriginFovYMax);

    float speed = camera_comp.GetSpeed();
    ImGui::DragFloat("Speed", &speed, drag_speed, min, max, format, flags);
    camera_comp.SetSpeed(speed);
    _ACDA_COMMAND_HELPER("[Camera] Speed", camera_comp, GetSpeed, SetSpeed, speed, _OriginSpeed);

    glm::i32vec2 viewport_size = camera_comp.GetViewportSize();
    ImGui::DragInt2("Viewport Size", glm::value_ptr(viewport_size), drag_speed, 1.0f, INT_MAX, format, flags);
    camera_comp.SetViewportSize(viewport_size);
    _ACDA_COMMAND_HELPER("[Camera] Viewport Size", camera_comp, GetViewportSize, SetViewportSize, viewport_size, _OriginViewportSize);

    bool up_axis_fixed = camera_comp.IsUpAxisFixed();
    ImGui::Checkbox("Up Axis Fixed", &up_axis_fixed);
    camera_comp.SetUpAxisFixed(up_axis_fixed);
    _ACDA_COMMAND_HELPER("[Camera] Up Axis Fixed", camera_comp, IsUpAxisFixed, SetUpAxisFixed, up_axis_fixed, _OriginUpAxisFixed);

    float up_epsilon = glm::degrees(camera_comp.GetUpAxisAngleEpsilon());
    ImGui::DragFloat("Up Axis Angle Epsilon", &up_epsilon, drag_speed, min, max, format, flags);
    camera_comp.SetUpAxisAngleEpsilon(glm::radians(up_epsilon));
    _ACDA_COMMAND_HELPER("[Camera] Up Axis Angle Epsilon", camera_comp, GetUpAxisAngleEpsilon, SetUpAxisAngleEpsilon, up_epsilon, _OriginUpAxisAngleEpsilon);

    ImGui::EndGroup();
}

void Arcadia::ImguiWindowPropertyFunctor_CameraComponent::Refresh(const CameraComponent& comp)
{
    _OriginNearPlane = comp.GetNearPlane();
    _OriginFarPlane = comp.GetFarPlane();
    _OriginFovY = comp.GetFovY();
    _OriginFovYMin = comp.GetFovYMin();
    _OriginFovYMax = comp.GetFovYMax();
    _OriginSpeed = comp.GetSpeed();
    _OriginViewportSize = comp.GetViewportSize();
    _OriginUpAxisFixed = comp.IsUpAxisFixed();
    _OriginUpAxisAngleEpsilon = comp.GetUpAxisAngleEpsilon();
}

void Arcadia::ImguiWindowPropertyFunctor_LightComponent::operator()(LightComponent& light_comp)
{
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
    const char* format = "%.3f";
    const ImGuiSliderFlags flags =
        ImGuiSliderFlags_AlwaysClamp;
    MatchVariant<void>(
        light_comp.GetLight(),
        [&](NullLight&)
        {
            if(ImGui::BeginCombo("Light Type", "(No light)"))
            {
                if(ImGui::Selectable("Spot Light"))
                {
                    light_comp.SetLight<SpotLight>();
                    Refresh(light_comp);
                }
                if(ImGui::Selectable("Direct Light"))
                {
                    light_comp.SetLight<DirectLight>();
                    Refresh(light_comp);
                }
                if(ImGui::Selectable("Area Light"))
                {
                    light_comp.SetLight<AreaLight>();
                    Refresh(light_comp);
                }
                if(ImGui::Selectable("Point Light"))
                {
                    light_comp.SetLight<PointLight>();
                    Refresh(light_comp);
                }
                ImGui::EndCombo();
            }
        },
        [&](SpotLight& light)
        {
            if(ImGui::BeginCombo("Light Type", "Spot Light"))
            {
                if(_ChangeLightTypeSelectable<DirectLight>(light_comp, "Direct Light"))
                {
                }
                if(_ChangeLightTypeSelectable<AreaLight>(light_comp, "Area Light"))
                {
                }
                if(_ChangeLightTypeSelectable<PointLight>(light_comp, "Point Light"))
                {
                }
                ImGui::EndCombo();
            }
            ImGui::NewLine();

            glm::vec3 coeffs = light.GetAttenuationCoefficients();
            ImGui::DragFloat3("Attenuation Coefficients", glm::value_ptr(coeffs), speed, min, max, format, flags);
            light.SetAttenuationCoefficients(coeffs);
            _ACDA_COMMAND_HELPER("[Spot Light] Attenuation", light, GetAttenuationCoefficients, SetAttenuationCoefficients, coeffs, _OriginAttenuationCoefficients);

            ImGui::SameLine();
            ImguiWrappers::HelpMark(ICON_FA_QUESTION, "In order of constant, linear and quadratic terms");

            const float cutoff_angle_drag_speend = .1f;
            const float cutoff_angle_min = 0.f;
            const float cutoff_angle_max = 180.f;
            glm::vec2 cutoff = light.GetCutoffAngles();
            ImGui::DragFloat2("Cutoff Angle", glm::value_ptr(cutoff), cutoff_angle_drag_speend, cutoff_angle_min, cutoff_angle_max, format, flags);
            light.SetCutoffAngles(cutoff);
            _ACDA_COMMAND_HELPER("[Spot Light] Cutoff Angle", light, GetCutoffAngles, SetCutoffAngles, cutoff, _OriginCutoffAngle);
            ImGui::SameLine();
            ImguiWrappers::HelpMark(ICON_FA_QUESTION, "Inner and outter");

            glm::vec3 color = light.GetColor();
            ImGui::ColorEdit3("Color", glm::value_ptr(color));
            light.SetColor(color);
            _ACDA_COMMAND_HELPER("[Spot Light] Color", light, GetColor, SetColor, color, _OriginColor);

            ImGui::NewLine();

            glm::vec3 ambient = light.GetAmbientStrength();
            ImGui::DragFloat3("Ambient Strength", glm::value_ptr(ambient), speed, min, max, format, flags);
            light.SetAmbientStrength(ambient);
            _ACDA_COMMAND_HELPER("[Spot Light] Ambient Strength", light, GetAmbientStrength, SetAmbientStrength, ambient, _OriginAmbientStrength);

            glm::vec3 diffuse = light.GetDiffuseStrength();
            ImGui::DragFloat3("Diffuse Strength", glm::value_ptr(diffuse), speed, min, max, format, flags);
            light.SetDiffuseStrength(diffuse);
            _ACDA_COMMAND_HELPER("[Spot Light] Diffuse Strength", light, GetDiffuseStrength, SetDiffuseStrength, diffuse, _OriginDiffuseStrength);

            glm::vec3 specular = light.GetSpecularStrength();
            ImGui::DragFloat3("Specular Strength", glm::value_ptr(specular), speed, min, max, format, flags);
            light.SetSpecularStrength(specular);
            _ACDA_COMMAND_HELPER("[Spot Light] Specular Strength", light, GetSpecularStrength, SetSpecularStrength, specular, _OriginSpecularStrength);
        },
        [&](DirectLight& light)
        {
            if(ImGui::BeginCombo("Light Type", "Direct Light"))
            {
                if(_ChangeLightTypeSelectable<SpotLight>(light_comp, "Spot Light"))
                {
                }
                if(_ChangeLightTypeSelectable<AreaLight>(light_comp, "Area Light"))
                {
                }
                if(_ChangeLightTypeSelectable<PointLight>(light_comp, "Point Light"))
                {
                }
                ImGui::EndCombo();
            }
            ImGui::NewLine();

            glm::vec3 color = light.GetColor();
            ImGui::ColorEdit3("Color", glm::value_ptr(color));
            light.SetColor(color);
            _ACDA_COMMAND_HELPER("[Direct Light] Color", light, GetColor, SetColor, color, _OriginColor);

            ImGui::NewLine();

            glm::vec3 ambient = light.GetAmbientStrength();
            ImGui::DragFloat3("Ambient Strength", glm::value_ptr(ambient), speed, min, max, format, flags);
            light.SetAmbientStrength(ambient);
            _ACDA_COMMAND_HELPER("[Direct Light] Ambient Strength", light, GetAmbientStrength, SetAmbientStrength, ambient, _OriginAmbientStrength);

            glm::vec3 diffuse = light.GetDiffuseStrength();
            ImGui::DragFloat3("Diffuse Strength", glm::value_ptr(diffuse), speed, min, max, format, flags);
            light.SetDiffuseStrength(diffuse);
            _ACDA_COMMAND_HELPER("[Direct Light] Diffuse Strength", light, GetDiffuseStrength, SetDiffuseStrength, diffuse, _OriginDiffuseStrength);

            glm::vec3 specular = light.GetSpecularStrength();
            ImGui::DragFloat3("Specular Strength", glm::value_ptr(specular), speed, min, max, format, flags);
            light.SetSpecularStrength(specular);
            _ACDA_COMMAND_HELPER("[Direct Light] Specular Strength", light, GetSpecularStrength, SetSpecularStrength, specular, _OriginSpecularStrength);
        },
        [&](AreaLight& light)
        {
            if(ImGui::BeginCombo("Light Type", "Area Light"))
            {
                if(_ChangeLightTypeSelectable<SpotLight>(light_comp, "Spot Light"))
                {
                }
                if(_ChangeLightTypeSelectable<DirectLight>(light_comp, "Direct Light"))
                {
                }
                if(_ChangeLightTypeSelectable<PointLight>(light_comp, "Point Light"))
                {
                }
                ImGui::EndCombo();
            }
            ImGui::NewLine();

            glm::vec2 size = light.GetSize();
            ImGui::DragFloat2("Size", glm::value_ptr(size), speed, min, max, format, flags);
            light.SetSize(size);
            _ACDA_COMMAND_HELPER("[Area Light] Size", light, GetSize, SetSize, size, _OriginSize);

            glm::vec3 color = light.GetColor();
            ImGui::ColorEdit3("Color", glm::value_ptr(color));
            light.SetColor(color);
            _ACDA_COMMAND_HELPER("[Area Light] Color", light, GetColor, SetColor, color, _OriginColor);

            ImGui::NewLine();

            glm::vec3 ambient = light.GetAmbientStrength();
            ImGui::DragFloat3("Ambient Strength", glm::value_ptr(ambient), speed, min, max, format, flags);
            light.SetAmbientStrength(ambient);
            _ACDA_COMMAND_HELPER("[Area Light] Ambient Strength", light, GetAmbientStrength, SetAmbientStrength, ambient, _OriginAmbientStrength);

            glm::vec3 diffuse = light.GetDiffuseStrength();
            ImGui::DragFloat3("Diffuse Strength", glm::value_ptr(diffuse), speed, min, max, format, flags);
            light.SetDiffuseStrength(diffuse);
            _ACDA_COMMAND_HELPER("[Area Light] Diffuse Strength", light, GetDiffuseStrength, SetDiffuseStrength, diffuse, _OriginDiffuseStrength);

            glm::vec3 specular = light.GetSpecularStrength();
            ImGui::DragFloat3("Specular Strength", glm::value_ptr(specular), speed, min, max, format, flags);
            light.SetSpecularStrength(specular);
            _ACDA_COMMAND_HELPER("[Area Light] Specular Strength", light, GetSpecularStrength, SetSpecularStrength, specular, _OriginSpecularStrength);
        },
        [&](PointLight& light)
        {
            if(ImGui::BeginCombo("Light Type", "Point Light"))
            {
                if(_ChangeLightTypeSelectable<SpotLight>(light_comp, "Spot Light"))
                {
                }
                if(_ChangeLightTypeSelectable<DirectLight>(light_comp, "Direct Light"))
                {
                }
                if(_ChangeLightTypeSelectable<AreaLight>(light_comp, "Area Light"))
                {
                }
                ImGui::EndCombo();
            }
            ImGui::NewLine();

            glm::vec3 coeffs = light.GetAttenuationCoefficients();
            ImGui::DragFloat3("Attenuation", glm::value_ptr(coeffs), speed, min, max, format, flags);
            light.SetAttenuationCoefficients(coeffs);
            _ACDA_COMMAND_HELPER("[Point Light] Attenuation", light, GetAttenuationCoefficients, SetAttenuationCoefficients, coeffs, _OriginAttenuationCoefficients);
            ImGui::SameLine();
            ImguiWrappers::HelpMark(ICON_FA_QUESTION, "In order of constant, linear and quadratic terms");

            glm::vec3 color = light.GetColor();
            ImGui::ColorEdit3("Color", glm::value_ptr(color));
            light.SetColor(color);
            _ACDA_COMMAND_HELPER("[Point Light] Color", light, GetColor, SetColor, color, _OriginColor);

            ImGui::NewLine();

            glm::vec3 ambient = light.GetAmbientStrength();
            ImGui::DragFloat3("Ambient Strength", glm::value_ptr(ambient), speed, min, max, format, flags);
            light.SetAmbientStrength(ambient);
            _ACDA_COMMAND_HELPER("[Point Light] Ambient Strength", light, GetAmbientStrength, SetAmbientStrength, ambient, _OriginAmbientStrength);

            glm::vec3 diffuse = light.GetDiffuseStrength();
            ImGui::DragFloat3("Diffuse Strength", glm::value_ptr(diffuse), speed, min, max, format, flags);
            light.SetDiffuseStrength(diffuse);
            _ACDA_COMMAND_HELPER("[Point Light] Diffuse Strength", light, GetDiffuseStrength, SetDiffuseStrength, diffuse, _OriginDiffuseStrength);

            glm::vec3 specular = light.GetSpecularStrength();
            ImGui::DragFloat3("Specular Strength", glm::value_ptr(specular), speed, min, max, format, flags);
            light.SetSpecularStrength(specular);
            _ACDA_COMMAND_HELPER("[Point Light] Specular Strength", light, GetSpecularStrength, SetSpecularStrength, specular, _OriginSpecularStrength);
        }
    );
    ImGui::EndGroup();
}

void Arcadia::ImguiWindowPropertyFunctor_LightComponent::Refresh(const LightComponent& comp)
{
    MatchVariant<void>(
        comp.GetLight(),
        [&](const NullLight&)
        {
            _OriginAttenuationCoefficients = {};
            _OriginCutoffAngle = {};
            _OriginSize = {};
            _OriginColor = {};
            _OriginAmbientStrength = {};
            _OriginDiffuseStrength = {};
            _OriginSpecularStrength = {};
        },
        [&](const SpotLight& light)
        {
            _OriginAttenuationCoefficients = light.GetAttenuationCoefficients();
            _OriginCutoffAngle = light.GetCutoffAngles();
            _OriginSize = {};
            _OriginColor = light.GetColor();
            _OriginAmbientStrength = light.GetAmbientStrength();
            _OriginDiffuseStrength = light.GetDiffuseStrength();
            _OriginSpecularStrength = light.GetSpecularStrength();
        },
        [&](const DirectLight& light)
        {
            _OriginAttenuationCoefficients = {};
            _OriginCutoffAngle = {};
            _OriginSize = {};
            _OriginColor = light.GetColor();
            _OriginAmbientStrength = light.GetAmbientStrength();
            _OriginDiffuseStrength = light.GetDiffuseStrength();
            _OriginSpecularStrength = light.GetSpecularStrength();
        },
        [&](const AreaLight& light)
        {
            _OriginAttenuationCoefficients = {};
            _OriginCutoffAngle = {};
            _OriginSize = light.GetSize();
            _OriginColor = light.GetColor();
            _OriginAmbientStrength = light.GetAmbientStrength();
            _OriginDiffuseStrength = light.GetDiffuseStrength();
            _OriginSpecularStrength = light.GetSpecularStrength();
        },
        [&](const PointLight& light)
        {
            _OriginAttenuationCoefficients = light.GetAttenuationCoefficients();
            _OriginCutoffAngle = {};
            _OriginSize = {};
            _OriginColor = light.GetColor();
            _OriginAmbientStrength = light.GetAmbientStrength();
            _OriginDiffuseStrength = light.GetDiffuseStrength();
            _OriginSpecularStrength = light.GetSpecularStrength();
        }
    );
}

void Arcadia::ImguiWindowPropertyFunctor_ModelComponent::operator()(ModelComponent& model_comp)
{
    ImGui::BeginGroup();

    ImGui::SeparatorText("Filepath");
    std::string filepath_string = model_comp.GetFilepath().empty()
        ? "(No Filepath)"
        : model_comp.GetFilepath().generic_string();
    ImGui::TextWrapped(filepath_string.c_str());
    if(ImGui::Button("..."))
    {
        std::vector<std::string> res = pfd::open_file(
            "Import Model"
        ).result();
        if(res.size())
        {
            try
            {
                model_comp.LoadModel(res.at(0));
            }
            catch(const Exceptions::ModelComponent_ModelLoadInvalidFormat&)
            {
                (void) pfd::message(
                    "Arcadia",
                    std::format("Cannot load model from {} because it has invalid format", res.at(0)),
                    pfd::choice::ok,
                    pfd::icon::error
                );
            }
        }
    }
    ImGui::SameLine();
    if(model_comp.IsModelLoaded() && ImGui::Button("Unload Model"))
    {
        model_comp.UnloadModel();
    }

    ImGui::EndGroup();
}

void Arcadia::ImguiWindowPopupFunctor_PhysicsComponentCreateBody::operator()(PhysicsComponent& physics_comp)
{
    if(!Opened)
    {
        return;
    }

    std::string imgui_window_title("Physics Component - Create Body");

    ImGuiPopupFlags popup_flags =
        ImGuiPopupFlags_NoOpenOverExistingPopup;
    ImGui::OpenPopup(imgui_window_title.c_str(), popup_flags);

    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::BeginPopupModal(imgui_window_title.c_str(), &Opened, window_flags))
    {
        float speed = .05f;
        float min = .0f;
        float max = .0f;
        const char* format = "%.3f";
        ImGuiSliderFlags slider_flags =
            ImGuiSliderFlags_AlwaysClamp;

        // Motion type
        std::string jph_motion_type_preview_string = Match<std::string>(
            _TempJphMotionType,
            JPH::EMotionType::Static,
            "Static",
            JPH::EMotionType::Dynamic,
            "Dynamic",
            JPH::EMotionType::Kinematic,
            "Kinematic"
        );
        if(ImGui::BeginCombo("Motion Type", jph_motion_type_preview_string.c_str()))
        {
            if(ImGui::Selectable("Static"))
            {
                _TempJphMotionType = JPH::EMotionType::Static;
                _TempJphObjectLayer = JphObjectLayers::NonMoving;
            }
            if(ImGui::Selectable("Dynamic"))
            {
                _TempJphMotionType = JPH::EMotionType::Dynamic;
                _TempJphObjectLayer = JphObjectLayers::Moving;
            }
            if(ImGui::Selectable("Kinematic"))
            {
                _TempJphMotionType = JPH::EMotionType::Kinematic;
                _TempJphObjectLayer = JphObjectLayers::Moving;
            }
            ImGui::EndCombo();
        }

        //Shape
        _TempJphShapeInfo = MatchVariant<JphShapeInfo>(
            _TempJphShapeInfo,
            [&](const JphNoShapeInfo&) -> JphShapeInfo
            {
                if(ImGui::BeginCombo("Shape Type", "Sphere Shape"))
                {
                    if(ImGui::Selectable("Box Shape"))
                    {
                        ImGui::EndCombo();
                        return JphBoxShapeInfo{};
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
                return _TempJphShapeInfo;
            },
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
                float half_extent_min = std::max({ .01f,info.ConvexRadius });
                float half_extent_max = std::numeric_limits<float>::max();
                ImGui::DragFloat3("Half Extent", glm::value_ptr(info.HalfExtent), speed, half_extent_min, half_extent_max, format, slider_flags);

                // Convex radius
                float convex_radius_min = .0f;
                float convex_radius_max = std::min({ info.HalfExtent.x,info.HalfExtent.y,info.HalfExtent.z });
                ImGui::DragFloat("Convex Radius", &info.ConvexRadius, speed, convex_radius_min, convex_radius_max, format, slider_flags);

                return _TempJphShapeInfo;
            },
            [&](JphCapsuleShapeInfo& info) -> JphShapeInfo
            {
                if(ImGui::BeginCombo("Shape Type", "Capsule Shape"))
                {
                    if(ImGui::Selectable("Box Shape"))
                    {
                        ImGui::EndCombo();
                        return JphBoxShapeInfo{};
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

                float radius_min = .0f;
                float radius_max = std::numeric_limits<float>::max();
                ImGui::DragFloat("Radius", &info.Radius, speed, radius_min, radius_max, format, slider_flags);

                float half_height_of_cylinder_min = .0f;
                float half_height_of_cylinder_max = std::numeric_limits<float>::max();
                ImGui::DragFloat("Half Height if Cylinder", &info.HalfHeightOfCylinder, speed, half_height_of_cylinder_min, half_height_of_cylinder_max, format, slider_flags);

                return _TempJphShapeInfo;
            },
            [&](JphCylinderShapeInfo& info) -> JphShapeInfo
            {
                if(ImGui::BeginCombo("Shape Type", "Cylinder Shape"))
                {
                    if(ImGui::Selectable("Box Shape"))
                    {
                        ImGui::EndCombo();
                        return JphBoxShapeInfo{};
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

                float half_height_min = .0f;
                float half_height_max = std::numeric_limits<float>::max();
                ImGui::DragFloat("Half Height", &info.HalfHeight, speed, half_height_min, half_height_max, format, slider_flags);

                float radius_min = .0f;
                float radius_max = std::numeric_limits<float>::max();
                ImGui::DragFloat("Radius", &info.Radius, speed, radius_min, radius_max, format, slider_flags);

                float convex_radius_min = .0f;
                float convex_radius_max = std::numeric_limits<float>::max();
                ImGui::DragFloat("Convex Radius", &info.ConvexRadius, speed, convex_radius_min, convex_radius_max, format, slider_flags);

                return _TempJphShapeInfo;
            },
            [&](JphSphereShapeInfo& info) -> JphShapeInfo
            {
                if(ImGui::BeginCombo("Shape Type", "Sphere Shape"))
                {
                    if(ImGui::Selectable("Box Shape"))
                    {
                        ImGui::EndCombo();
                        return JphBoxShapeInfo{};
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

                float radius_min = .0f;
                float radius_max = std::numeric_limits<float>::max();
                ImGui::DragFloat("Radius", &info.Radius, speed, radius_min, radius_max, format, slider_flags);

                return _TempJphShapeInfo;
            }
        );

        ImGui::NewLine();
        bool confirmed = ImGui::Button("Confirm");
        if(confirmed)
        {
            physics_comp.SetJphMotionType(_TempJphMotionType);
            physics_comp.SetJphObjectLayer(_TempJphObjectLayer);
            physics_comp.SetJphShapeInfo(_TempJphShapeInfo);
            physics_comp.SetInUse(true);
        }
        ImGui::SameLine();
        if(confirmed || ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            Opened = false;
            _TempJphMotionType = JPH::EMotionType::Static;
            _TempJphObjectLayer = JphObjectLayers::NonMoving;
            _TempJphShapeInfo = JphNoShapeInfo{};
        }

        ImGui::EndPopup();
    }
}

void Arcadia::ImguiWindowPropertyFunctor_PhysicsComponent::operator()(PhysicsComponent& physics_comp)
{
    _ImguiWindowPopupPhysicsComponentCreateBody(physics_comp);

    ImGui::BeginGroup();

    if(physics_comp.IsInUse())
    {
        if(ImGui::TreeNodeEx("Initial", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding))
        {
            ImGui::Text(std::format(
                "Motion Type: {}",
                Match<std::string>(
                    physics_comp.GetJphMotionType(),
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
            ImGui::Text(std::format("Object Layer: {}", physics_comp.GetJphObjectLayer()).c_str());
            ImGui::TreePop();
        }

        if(ImGui::TreeNodeEx("Current", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding))
        {
            ImGui::Text(std::format("Active: {}", physics_comp.IsActive()).c_str());
            ImGui::Text(std::format("Linear Velocity - {}", physics_comp.GetLinearVelocity()).c_str());
            ImGui::Text(std::format("Angular Velocity - {}", physics_comp.GetAngularVelocity()).c_str());
            ImGui::TreePop();
        }

        bool tree_open = MatchVariant<bool>(
            physics_comp.GetJphShapeInfo(),
            [&](const JphNoShapeInfo)
            {
                ACDA_UNREACHABLE("Invalid shape info type");
                return false;
            },
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

    if(physics_comp.IsInUse())
    {
        glm::vec3 color = physics_comp.GetBodyShapeColor();
        ImGui::ColorEdit3("Body Shape Color", glm::value_ptr(color));
        physics_comp.SetBodyShapeColor(color);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
            glm::vec3 origin = physics_comp.GetBodyShapeColor();
            CommandList::Instance().Emplace(
                "[Physics] Body Shape Color",
                [&]() -> void
                {
                    physics_comp.SetBodyShapeColor(color);
                },
                [&]() -> void
                {
                    physics_comp.SetBodyShapeColor(origin);
                }
            );
        }

        if(ImGui::Button("Recreate Body"))
        {
            _ImguiWindowPopupPhysicsComponentCreateBody.Opened = true;
        }
        ImGui::SameLine();
        if(ImGui::Button("Destroy Body"))
        {
            physics_comp.SetInUse(false);
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
}

void Arcadia::ImguiWindowPropertyFunctor_TransformComponent::Refresh(const TransformComponent& comp)
{
    _OriginPosition = comp.GetPosition();
    _OriginDirection = comp.GetDirection();
    _OriginRotationEularAngle = comp.GetRotationEularAngle();
    _OriginScale = comp.GetScale();
    _OriginPivot = comp.GetPivot();
}

void Arcadia::ImguiWindowPropertyFunctor_TransformComponent::operator()(TransformComponent& transform_comp)
{
    ImGui::BeginGroup();

    const float speed = 1.f;
    const float min = std::numeric_limits<float>::max();
    const float max = std::numeric_limits<float>::lowest();
    const char* format = "%.3f";
    const ImGuiSliderFlags slider_flags =
        ImGuiSliderFlags_AlwaysClamp;

    glm::vec3 position = transform_comp.GetPosition();
    glm::vec3 position_delta = position; // Previous position
    ImGui::DragFloat3("Position", glm::value_ptr(position), speed, min, max, format, slider_flags);
    transform_comp.SetPosition(position);
    _ACDA_COMMAND_HELPER("[Transform] Position", transform_comp, GetPosition, SetPosition, position, _OriginPosition);
    position_delta = transform_comp.GetPosition() - position_delta; // current - previous

    if(transform_comp.CheckFlag(TransformComponentFlags::UseRotation))
    {
        float rotation_drag_speed = .5f;
        glm::vec3 eular_angle = glm::degrees(transform_comp.GetRotationEularAngle());
        ImGui::DragFloat3("Rotation", glm::value_ptr(eular_angle), rotation_drag_speed, min, max, format, slider_flags);
        transform_comp.SetRotationEularAngle(glm::radians(eular_angle));
        _ACDA_COMMAND_HELPER("[Transform] Rotation", transform_comp, GetRotationEularAngle, SetRotationEularAngle, eular_angle, _OriginRotationEularAngle);
    }
    else if(transform_comp.CheckFlag(TransformComponentFlags::UseDirection))
    {
        float rotation_drag_speed = .5f;
        glm::vec3 direction = transform_comp.GetDirection();
        ImGui::DragFloat3("Direction", glm::value_ptr(direction), rotation_drag_speed, min, max, format, slider_flags);
        transform_comp.SetDirection(direction);
        _ACDA_COMMAND_HELPER("[Transform] Direction", transform_comp, GetDirection, SetDirection, direction, _OriginDirection);
    }
    else
    {
        ACDA_UNREACHABLE("");
    }

    glm::vec3 scale = transform_comp.GetScale();
    ImGui::DragFloat3("Scale", glm::value_ptr(scale), speed, min, max, format, slider_flags);
    transform_comp.SetScale(scale);
    _ACDA_COMMAND_HELPER("[Transform] Scale", transform_comp, GetScale, SetScale, scale, _OriginScale);

    if(position_delta != Glm::Vec3_CreateZero())
    {
        transform_comp.IncreasePivot(position_delta); // Make pivot move with translation
    }
    glm::vec3 pivot = transform_comp.GetPivot();
    ImGui::DragFloat3("Pivot", glm::value_ptr(pivot), speed, min, max, format, slider_flags);
    transform_comp.SetPivot(pivot);
    _ACDA_COMMAND_HELPER("[Transform] Pivot", transform_comp, GetPivot, SetPivot, pivot, _OriginPivot);

    ImGui::EndGroup();
}

Arcadia::ImguiWindowProperty::ImguiWindowProperty(bool open, const std::string& title):
    ImguiWindowInterface(open, title)
{
}

void Arcadia::ImguiWindowProperty::OnEvent(EventBase& e)
{
    EventDispatcher{ e }
        .Dispatch<Events::OpenImguiWindow>(ACDA_BIND_MEMBER_FN(_OnOpenImguiWindow))
        .Dispatch<Events::SceneDeactivated>(ACDA_BIND_MEMBER_FN(_OnSceneDeactivated))
        .Dispatch<Events::SelectEntity>(ACDA_BIND_MEMBER_FN(_OnSelectEntity))
        .Dispatch<Events::DeleteEntity>(ACDA_BIND_MEMBER_FN(_OnDeleteEntity))
        .IsDispatched();
}

void Arcadia::ImguiWindowProperty::OnUpdate()
{
    if(!_Opened)
    {
        return;
    }

    std::shared_ptr<SceneLayer> scene_layer_sptr = LayerStack::Instance().GetLayerShared<SceneLayer>();

    std::string imgui_title = scene_layer_sptr->HasActiveScene() && _SelectedEntityId
        ? _Title + " - " + scene_layer_sptr->ActiveScene_GetEntityInfo(_SelectedEntityId).GetName() + GetIdString()
        : _Title + GetIdString();

    ImGui::SetNextWindowSize(glm::vec2{ 1024,768 }, ImGuiCond_Once);
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_title.c_str(), &_Opened, window_flags))
    {
        if(!scene_layer_sptr->HasActiveScene())
        {
            ImGui::Text("(No scene)");
        }
        else
        {
            ImGuiTabBarFlags tab_bar_flags =
                ImGuiTabBarFlags_NoCloseWithMiddleMouseButton
                | ImGuiTabBarFlags_TabListPopupButton
                | ImGuiTabBarFlags_AutoSelectNewTabs
                | ImGuiTabBarFlags_FittingPolicyScroll
                | ImGuiTabBarFlags_Reorderable;
            if(_SelectedEntityId)
            {
                ImGui::PushItemWidth(200.f);

                _DisplayProperty<CameraComponent>("Camera", ACDA_BIND_MEMBER_FN(_ImguiWindowPropertyFunctor_CameraComponent));
                _DisplayProperty<LightComponent>("Light", ACDA_BIND_MEMBER_FN(_ImguiWindowPropertyFunctor_LightComponent));
                _DisplayProperty<ModelComponent>("Model", ACDA_BIND_MEMBER_FN(_ImguiWindowPropertyFunctor_ModelComponent));
                _DisplayProperty<PhysicsComponent>("Physics", ACDA_BIND_MEMBER_FN(_ImguiWindowPropertyFunctor_PhysicsComponent));
                _DisplayProperty<TransformComponent>("Transform", ACDA_BIND_MEMBER_FN(_ImguiWindowPropertyFunctor_TransformComponent));
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

void Arcadia::ImguiWindowProperty::_OnSceneDeactivated(Events::SceneDeactivated& e)
{
    _SelectedEntityId.SetNull();
}

void Arcadia::ImguiWindowProperty::_OnSelectEntity(Events::SelectEntity& e)
{
    _SelectedEntityId = e.EntityId;
    std::shared_ptr<SceneLayer> scene_layer_sptr = LayerStack::Instance().GetLayerShared<SceneLayer>();
    if(scene_layer_sptr->ActiveScene_ContainsAllComponents<TransformComponent>(_SelectedEntityId))
    {
        _ImguiWindowPropertyFunctor_TransformComponent.Refresh(scene_layer_sptr->ActiveScene_GetComponent<TransformComponent>(_SelectedEntityId));
    }

    if(scene_layer_sptr->ActiveScene_ContainsAllComponents<CameraComponent>(_SelectedEntityId))
    {
        _ImguiWindowPropertyFunctor_CameraComponent.Refresh(scene_layer_sptr->ActiveScene_GetComponent<CameraComponent>(_SelectedEntityId));
    }

    if(scene_layer_sptr->ActiveScene_ContainsAllComponents<LightComponent>(_SelectedEntityId))
    {
        _ImguiWindowPropertyFunctor_LightComponent.Refresh(scene_layer_sptr->ActiveScene_GetComponent<LightComponent>(_SelectedEntityId));
    }
}

void Arcadia::ImguiWindowProperty::_OnDeleteEntity(Events::DeleteEntity& e)
{
    if(_SelectedEntityId == e.EntityId)
    {
        _SelectedEntityId.SetNull();
    }
}