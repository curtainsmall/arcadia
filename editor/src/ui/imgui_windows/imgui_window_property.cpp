#include "imgui_window_property.hpp"

#include<algorithm>
#include<string>

#include"core/assert.hpp"
#include"core/enum.hpp"
#include"core/file/pfd_header.hpp"
#include"core/function.hpp"
#include"core/match.hpp"
#include"core/command/command.hpp"
#include"resource/fonts/icon_header.hpp"

#include"ui/imgui_header.hpp"
#include"ui/imgui_wrapper.hpp"

#define _ACDA_COMMAND_HELPER(msg, comp_name, getter_name, setter_name, value_name, origin_name) \
if(ImGui::IsItemDeactivatedAfterEdit())\
{\
    CommandList::Instance().Emplace(\
        msg,\
        [&comp = comp_name, value = value_name]() -> void\
    {\
        comp.setter_name(value);\
    },\
        [&comp = comp_name, origin = origin_name]() -> void\
    {\
        comp.setter_name(origin);\
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
    _ACDA_COMMAND_HELPER("[Camera] Near Plane", camera_comp, GetNearPlane, SetNearPlane, near_plane, _TempNearPlane);

    float far_plane = camera_comp.GetFarPlane();
    ImGui::DragFloat("Far Plane", &far_plane, drag_speed, min, max, format, flags);
    camera_comp.SetFarPlane(far_plane);
    _ACDA_COMMAND_HELPER("[Camera] Far Plane", camera_comp, GetFarPlane, SetFarPlane, far_plane, _TempFarPlane);

    float fovy = glm::degrees(camera_comp.GetFovY());
    ImGui::DragFloat("FOV-Y", &fovy, drag_speed, camera_comp.GetFovYMin(), camera_comp.GetFovYMax(), format, flags);
    camera_comp.SetFovY(glm::radians(fovy));
    _ACDA_COMMAND_HELPER("[Camera] FOV-Y", camera_comp, GetFovY, SetFovY, fovy, _TempFovY);

    float fovy_min = glm::degrees(camera_comp.GetFovYMin());
    ImGui::DragFloat("FOV-Y Min", &fovy_min, drag_speed, 0.0f, 180.0f, format, flags);
    camera_comp.SetFovYMin(glm::radians(fovy_min));
    _ACDA_COMMAND_HELPER("[Camera] FOV-Y Min", camera_comp, GetFovYMin, SetFovYMin, fovy_min, _TempFovYMin);

    float fovy_max = glm::degrees(camera_comp.GetFovYMax());
    ImGui::DragFloat("FOV-Y Max", &fovy_max, drag_speed, 0.0f, 180.0f, format, flags);
    camera_comp.SetFovYMax(glm::radians(fovy_max));
    _ACDA_COMMAND_HELPER("[Camera] FOV-Y Max", camera_comp, GetFovYMax, SetFovYMax, fovy_max, _TempFovYMax);

    float speed = camera_comp.GetSpeed();
    ImGui::DragFloat("Speed", &speed, drag_speed, min, max, format, flags);
    camera_comp.SetSpeed(speed);
    _ACDA_COMMAND_HELPER("[Camera] Speed", camera_comp, GetSpeed, SetSpeed, speed, _TempSpeed);

    glm::i32vec2 viewport_size = camera_comp.GetViewportSize();
    ImGui::DragInt2("Viewport Size", glm::value_ptr(viewport_size), drag_speed, 1.0f, INT_MAX, format, flags);
    camera_comp.SetViewportSize(viewport_size);
    _ACDA_COMMAND_HELPER("[Camera] Viewport Size", camera_comp, GetViewportSize, SetViewportSize, viewport_size, _TempViewportSize);

    bool up_axis_fixed = camera_comp.IsUpAxisFixed();
    ImGui::Checkbox("Up Axis Fixed", &up_axis_fixed);
    camera_comp.SetUpAxisFixed(up_axis_fixed);
    _ACDA_COMMAND_HELPER("[Camera] Up Axis Fixed", camera_comp, IsUpAxisFixed, SetUpAxisFixed, up_axis_fixed, _TempUpAxisFixed);

    float up_epsilon = glm::degrees(camera_comp.GetUpAxisAngleEpsilon());
    ImGui::DragFloat("Up Axis Angle Epsilon", &up_epsilon, drag_speed, min, max, format, flags);
    camera_comp.SetUpAxisAngleEpsilon(glm::radians(up_epsilon));
    _ACDA_COMMAND_HELPER("[Camera] Up Axis Angle Epsilon", camera_comp, GetUpAxisAngleEpsilon, SetUpAxisAngleEpsilon, up_epsilon, _TempUpAxisAngleEpsilon);

    ImGui::EndGroup();
}

void Arcadia::ImguiWindowPropertyFunctor_CameraComponent::Refresh(const CameraComponent& comp)
{
    _TempNearPlane = comp.GetNearPlane();
    _TempFarPlane = comp.GetFarPlane();
    _TempFovY = comp.GetFovY();
    _TempFovYMin = comp.GetFovYMin();
    _TempFovYMax = comp.GetFovYMax();
    _TempSpeed = comp.GetSpeed();
    _TempViewportSize = comp.GetViewportSize();
    _TempUpAxisFixed = comp.IsUpAxisFixed();
    _TempUpAxisAngleEpsilon = comp.GetUpAxisAngleEpsilon();
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
                ImGui::EndCombo();
            }
            if(ImGui::Selectable("Direct Light"))
            {
                light_comp.SetLight<DirectLight>();
                Refresh(light_comp);
                ImGui::EndCombo();
            }
            if(ImGui::Selectable("Area Light"))
            {
                light_comp.SetLight<AreaLight>();
                Refresh(light_comp);
                ImGui::EndCombo();
            }
            if(ImGui::Selectable("Point Light"))
            {
                light_comp.SetLight<PointLight>();
                Refresh(light_comp);
                ImGui::EndCombo();
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
                ImGui::EndCombo();
            }
            if(_ChangeLightTypeSelectable<AreaLight>(light_comp, "Area Light"))
            {
                ImGui::EndCombo();
            }
            if(_ChangeLightTypeSelectable<PointLight>(light_comp, "Point Light"))
            {
                ImGui::EndCombo();
            }
            ImGui::EndCombo();
        }
        ImGui::NewLine();

        glm::vec3 coeffs = light.GetAttenuationCoefficients();
        ImGui::DragFloat3("Attenuation Coefficients", glm::value_ptr(coeffs), speed, min, max, format, flags);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
            glm::vec3 origin = light.GetAttenuationCoefficients();
            CommandList::Instance().Emplace(
                "[Spot Light] Attenuation Coefficients",
                [&]() -> void
            {
                light.SetAttenuationCoefficients(coeffs);
            },
                [&]() -> void
            {
                light.SetAttenuationCoefficients(origin);
            }
            );
        }
        light.SetAttenuationCoefficients(coeffs);
        ImGui::SameLine();
        ImguiWrappers::HelpMark(ICON_FA_QUESTION, "In order of constant, linear and quadratic terms");

        const float cutoff_angle_drag_speend = .1f;
        const float cutoff_angle_min = 0.f;
        const float cutoff_angle_max = 180.f;
        glm::vec2 cutoff = light.GetCutoffAngles();
        ImGui::DragFloat2("Cutoff Angle", glm::value_ptr(cutoff), cutoff_angle_drag_speend, cutoff_angle_min, cutoff_angle_max, format, flags);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
            glm::vec2 origin = light.GetCutoffAngles();
            CommandList::Instance().Emplace(
                "[Spot Light] Cutoff Angle",
                [&]() -> void
            {
                light.SetCutoffAngles(cutoff);
            },
                [&]() -> void
            {
                light.SetCutoffAngles(origin);
            }
            );
        }
        light.SetCutoffAngles(cutoff);
        ImGui::SameLine();
        ImguiWrappers::HelpMark(ICON_FA_QUESTION, "Inner and outter");

        glm::vec3 color = light.GetColor();
        ImGui::ColorEdit3("Color", glm::value_ptr(color));
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
            glm::vec3 origin = light.GetColor();
            CommandList::Instance().Emplace(
                "[Spot Light] Color",
                [&]() -> void
            {
                light.SetColor(color);
            },
                [&]() -> void
            {
                light.SetColor(origin);
            }
            );
        }
        light.SetColor(color);

        ImGui::NewLine();

        glm::vec3 ambient = light.GetAmbientStrength();
        ImGui::DragFloat3("Ambient Strength", glm::value_ptr(ambient), speed, min, max, format, flags);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
            glm::vec3 origin = light.GetAmbientStrength();
            CommandList::Instance().Emplace(
                "[Spot Light] Ambient Strength",
                [&]() -> void
            {
                light.SetAmbientStrength(ambient);
            },
                [&]() -> void
            {
                light.SetAmbientStrength(origin);
            }
            );
        }
        light.SetAmbientStrength(ambient);

        glm::vec3 diffuse = light.GetDiffuseStrength();
        ImGui::DragFloat3("Diffuse Strength", glm::value_ptr(diffuse), speed, min, max, format, flags);
        glm::vec3 origin = light.GetDiffuseStrength();
        CommandList::Instance().Emplace(
            "[Spot Light] Diffuse Strength",
            [&]() -> void
        {
            light.SetDiffuseStrength(diffuse);
        },
            [&]() -> void
        {
            light.SetDiffuseStrength(origin);
        }
        );
        light.SetDiffuseStrength(diffuse);

        glm::vec3 specular = light.GetSpecularStrength();
        ImGui::DragFloat3("Specular Strength", glm::value_ptr(specular), speed, min, max, format, flags);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
            glm::vec3 origin = light.GetSpecularStrength();
            CommandList::Instance().Emplace(
                "[Spot Light] Specular Strength",
                [&]() -> void
            {
                light.SetSpecularStrength(specular);
            },
                [&]() -> void
            {
                light.SetSpecularStrength(origin);
            }
            );
        }
        light.SetSpecularStrength(specular);
    },
        [&](DirectLight& light)
    {
        if(ImGui::BeginCombo("Light Type", "Direct Light"))
        {
            if(_ChangeLightTypeSelectable<SpotLight>(light_comp, "Spot Light"))
            {
                ImGui::EndCombo();
            }
            if(_ChangeLightTypeSelectable<AreaLight>(light_comp, "Area Light"))
            {
                ImGui::EndCombo();
            }
            if(_ChangeLightTypeSelectable<PointLight>(light_comp, "Point Light"))
            {
                ImGui::EndCombo();
            }
            ImGui::EndCombo();
        }
        ImGui::NewLine();

        glm::vec3 color = light.GetColor();
        ImGui::ColorEdit3("Color", glm::value_ptr(color));
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
            glm::vec3 origin = light.GetColor();
            CommandList::Instance().Emplace(
                "[Direct Light] Color",
                [&]() -> void
            {
                light.SetColor(color);
            },
                [&]() -> void
            {
                light.SetColor(origin);
            }
            );
        }
        light.SetColor(color);

        ImGui::NewLine();

        glm::vec3 ambient = light.GetAmbientStrength();
        ImGui::DragFloat3("Ambient Strength", glm::value_ptr(ambient), speed, min, max, format, flags);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
            glm::vec3 origin = light.GetAmbientStrength();
            CommandList::Instance().Emplace(
                "[Direct Light] Ambient Strength",
                [&]() -> void
            {
                light.SetAmbientStrength(ambient);
            },
                [&]() -> void
            {
                light.SetAmbientStrength(origin);
            }
            );
        }
        light.SetAmbientStrength(ambient);

        glm::vec3 diffuse = light.GetDiffuseStrength();
        ImGui::DragFloat3("Diffuse Strength", glm::value_ptr(diffuse), speed, min, max, format, flags);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
            glm::vec3 origin = light.GetDiffuseStrength();
            CommandList::Instance().Emplace(
                "[Direct Light] Diffuse Strength",
                [&]() -> void
            {
                light.SetDiffuseStrength(diffuse);
            },
                [&]() -> void
            {
                light.SetDiffuseStrength(origin);
            }
            );
        }
        light.SetDiffuseStrength(diffuse);

        glm::vec3 specular = light.GetSpecularStrength();
        ImGui::DragFloat3("Specular Strength", glm::value_ptr(specular), speed, min, max, format, flags);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
            glm::vec3 origin = light.GetSpecularStrength();
            CommandList::Instance().Emplace(
                "[Direct Light] Specular Strength",
                [&]() -> void
            {
                light.SetSpecularStrength(specular);
            },
                [&]() -> void
            {
                light.SetSpecularStrength(origin);
            }
            );
        }
        light.SetSpecularStrength(specular);
    },
        [&](AreaLight& light)
    {
        if(ImGui::BeginCombo("Light Type", "Area Light"))
        {
            if(_ChangeLightTypeSelectable<SpotLight>(light_comp, "Spot Light"))
            {
                ImGui::EndCombo();
            }
            if(_ChangeLightTypeSelectable<DirectLight>(light_comp, "Direct Light"))
            {
                ImGui::EndCombo();
            }
            if(_ChangeLightTypeSelectable<PointLight>(light_comp, "Point Light"))
            {
                ImGui::EndCombo();
            }
            ImGui::EndCombo();
        }
        ImGui::NewLine();

        glm::vec2 size = light.GetSize();
        ImGui::DragFloat2("Size", glm::value_ptr(size), speed, min, max, format, flags);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
            glm::vec2 origin = light.GetSize();
            CommandList::Instance().Emplace(
                "[Area Light] Size",
                [&]() -> void
            {
                light.SetSize(size);
            },
                [&]() -> void
            {
                light.SetSize(origin);
            }
            );
        }
        light.SetSize(size);

        glm::vec3 color = light.GetColor();
        ImGui::ColorEdit3("Color", glm::value_ptr(color));
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
            glm::vec3 origin = light.GetColor();
            CommandList::Instance().Emplace(
                "[Area Light] Color",
                [&]() -> void
            {
                light.SetColor(color);
            },
                [&]() -> void
            {
                light.SetColor(origin);
            }
            );
        }
        light.SetColor(color);

        ImGui::NewLine();

        glm::vec3 ambient = light.GetAmbientStrength();
        ImGui::DragFloat3("Ambient Strength", glm::value_ptr(ambient), speed, min, max, format, flags);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
            glm::vec3 origin = light.GetAmbientStrength();
            CommandList::Instance().Emplace(
                "[Area Light] Ambient Strength",
                [&]() -> void
            {
                light.SetAmbientStrength(ambient);
            },
                [&]() -> void
            {
                light.SetAmbientStrength(origin);
            }
            );
        }
        light.SetAmbientStrength(ambient);

        glm::vec3 diffuse = light.GetDiffuseStrength();
        ImGui::DragFloat3("Diffuse Strength", glm::value_ptr(diffuse), speed, min, max, format, flags);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
            glm::vec3 origin = light.GetDiffuseStrength();
            CommandList::Instance().Emplace(
                "[Area Light] Diffuse Strength",
                [&]() -> void
            {
                light.SetDiffuseStrength(diffuse);
            },
                [&]() -> void
            {
                light.SetDiffuseStrength(origin);
            }
            );
        }
        light.SetDiffuseStrength(diffuse);

        glm::vec3 specular = light.GetSpecularStrength();
        ImGui::DragFloat3("Specular Strength", glm::value_ptr(specular), speed, min, max, format, flags);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
            glm::vec3 origin = light.GetSpecularStrength();
            CommandList::Instance().Emplace(
                "[Area Light] Specular Strength",
                [&]() -> void
            {
                light.SetSpecularStrength(specular);
            },
                [&]() -> void
            {
                light.SetSpecularStrength(origin);
            }
            );
        }
        light.SetSpecularStrength(specular);
    },
        [&](PointLight& light)
    {
        if(ImGui::BeginCombo("Light Type", "Point Light"))
        {
            if(_ChangeLightTypeSelectable<SpotLight>(light_comp, "Spot Light"))
            {
                ImGui::EndCombo();
            }
            if(_ChangeLightTypeSelectable<DirectLight>(light_comp, "Direct Light"))
            {
                ImGui::EndCombo();
            }
            if(_ChangeLightTypeSelectable<AreaLight>(light_comp, "Area Light"))
            {
                ImGui::EndCombo();
            }
            ImGui::EndCombo();
        }
        ImGui::NewLine();

        glm::vec3 coeffs = light.GetAttenuationCoefficients();
        ImGui::DragFloat3("Attenuation", glm::value_ptr(coeffs), speed, min, max, format, flags);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
            glm::vec3 origin = light.GetAttenuationCoefficients();
            CommandList::Instance().Emplace(
                "[Point Light] Attenuation",
                [&]() -> void
            {
                light.SetAttenuationCoefficients(coeffs);
            },
                [&]() -> void
            {
                light.SetAttenuationCoefficients(origin);
            }
            );
        }
        light.SetAttenuationCoefficients(coeffs);
        ImGui::SameLine();
        ImguiWrappers::HelpMark(ICON_FA_QUESTION, "In order of constant, linear and quadratic terms");

        glm::vec3 color = light.GetColor();
        ImGui::ColorEdit3("Color", glm::value_ptr(color));
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
            glm::vec3 origin = light.GetColor();
            CommandList::Instance().Emplace(
                "[Point Light] Color",
                [&]() -> void
            {
                light.SetColor(color);
            },
                [&]() -> void
            {
                light.SetColor(origin);
            }
            );
        }
        light.SetColor(color);

        ImGui::NewLine();

        glm::vec3 ambient = light.GetAmbientStrength();
        ImGui::DragFloat3("Ambient Strength", glm::value_ptr(ambient), speed, min, max, format, flags);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
            glm::vec3 origin = light.GetAmbientStrength();
            CommandList::Instance().Emplace(
                "[Point Light] Ambient Strength",
                [&]() -> void
            {
                light.SetAmbientStrength(ambient);
            },
                [&]() -> void
            {
                light.SetAmbientStrength(origin);
            }
            );
        }
        light.SetAmbientStrength(ambient);

        glm::vec3 diffuse = light.GetDiffuseStrength();
        ImGui::DragFloat3("Diffuse Strength", glm::value_ptr(diffuse), speed, min, max, format, flags);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
            glm::vec3 origin = light.GetDiffuseStrength();
            CommandList::Instance().Emplace(
                "[Point Light] Diffuse Strength",
                [&]() -> void
            {
                light.SetDiffuseStrength(diffuse);
            },
                [&]() -> void
            {
                light.SetDiffuseStrength(origin);
            }
            );
        }
        light.SetDiffuseStrength(diffuse);

        glm::vec3 specular = light.GetSpecularStrength();
        ImGui::DragFloat3("Specular Strength", glm::value_ptr(specular), speed, min, max, format, flags);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
            glm::vec3 origin = light.GetSpecularStrength();
            CommandList::Instance().Emplace(
                "[Point Light] Specular Strength",
                [&]() -> void
            {
                light.SetSpecularStrength(specular);
            },
                [&]() -> void
            {
                light.SetSpecularStrength(origin);
            }
            );
        }
        light.SetSpecularStrength(specular);
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
        _TempAttenuationCoefficients = {};
        _TempCutoffAngle = {};
        _TempColor = {};
        _TempAmbientStrength = {};
        _TempDiffuseStrength = {};
        _TempSpecularStrength = {};
    },
        [&](const SpotLight& light)
    {
        _TempAttenuationCoefficients = light.GetAttenuationCoefficients();
        _TempCutoffAngle = light.GetCutoffAngles();
        _TempColor = light.GetColor();
        _TempAmbientStrength = light.GetAmbientStrength();
        _TempDiffuseStrength = light.GetDiffuseStrength();
        _TempSpecularStrength = light.GetSpecularStrength();
    },
        [&](const DirectLight& light)
    {
        _TempAttenuationCoefficients = {};
        _TempCutoffAngle = {};
        _TempColor = light.GetColor();
        _TempAmbientStrength = light.GetAmbientStrength();
        _TempDiffuseStrength = light.GetDiffuseStrength();
        _TempSpecularStrength = light.GetSpecularStrength();
    },
        [&](const AreaLight& light)
    {
        _TempAttenuationCoefficients = {};
        _TempCutoffAngle = {};
        _TempColor = light.GetColor();
        _TempAmbientStrength = light.GetAmbientStrength();
        _TempDiffuseStrength = light.GetDiffuseStrength();
        _TempSpecularStrength = light.GetSpecularStrength();
    },
        [&](const PointLight& light)
    {
        _TempAttenuationCoefficients = light.GetAttenuationCoefficients();
        _TempCutoffAngle = {};
        _TempColor = light.GetColor();
        _TempAmbientStrength = light.GetAmbientStrength();
        _TempDiffuseStrength = light.GetDiffuseStrength();
        _TempSpecularStrength = light.GetSpecularStrength();
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
            _TempJphBodyInfo.JphMotionType,
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
            float half_extent_min = std::max({ .01f,info.ConvexRadius });
            float half_extent_max = FLT_MAX;
            ImGui::DragFloat3("Half Extent", glm::value_ptr(info.HalfExtent), speed, half_extent_min, half_extent_max, format, slider_flags);

            // Convex radius
            float convex_radius_min = .0f;
            float convex_radius_max = std::min({ info.HalfExtent.x,info.HalfExtent.y,info.HalfExtent.z });
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

            float radius_min = .0f;
            float radius_max = FLT_MAX;
            ImGui::DragFloat("Radius", &info.Radius, speed, radius_min, radius_max, format, slider_flags);

            float half_height_of_cylinder_min = .0f;
            float half_height_of_cylinder_max = FLT_MAX;
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

            float half_height_min = .0f;
            float half_height_max = FLT_MAX;
            ImGui::DragFloat("Half Height", &info.HalfHeight, speed, half_height_min, half_height_max, format, slider_flags);

            float radius_min = .0f;
            float radius_max = FLT_MAX;
            ImGui::DragFloat("Radius", &info.Radius, speed, radius_min, radius_max, format, slider_flags);

            float convex_radius_min = .0f;
            float convex_radius_max = FLT_MAX;
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

            float radius_min = .0f;
            float radius_max = FLT_MAX;
            ImGui::DragFloat("Radius", &info.Radius, speed, radius_min, radius_max, format, slider_flags);

            return _TempJphBodyInfo.JphShapeInfo;
        }
        );

        ImGui::NewLine();
        bool confirmed = ImGui::Button("Confirm");
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

void Arcadia::ImguiWindowPropertyFunctor_PhysicsComponent::operator()(PhysicsComponent& physics_comp)
{
    _ImguiWindowPopupPhysicsComponentCreateBody(physics_comp);

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
            const JphBodyState& jph_body_state = physics_comp.GetBodyState();
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
}

void Arcadia::ImguiWindowPropertyFunctor_TransformComponent::Refresh(const TransformComponent& comp)
{
    _TempPosition = comp.GetPosition();
    _TempDirection = comp.GetDirection();
    _TempRotationEularAngle = comp.GetRotationEularAngle();
    _TempScale = comp.GetScale();
    _TempPivot = comp.GetPivot();
}

void Arcadia::ImguiWindowPropertyFunctor_TransformComponent::operator()(TransformComponent& transform_comp)
{
    ImGui::BeginGroup();

    const float speed = 1.f;
    const float min = .0f;
    const float max = .0f;
    const char* format = "%.3f";
    const ImGuiSliderFlags slider_flags =
        ImGuiSliderFlags_AlwaysClamp;

    glm::vec3 position = transform_comp.GetPosition();
    glm::vec3 position_delta = position; // Previous position
    ImGui::DragFloat3("Position", glm::value_ptr(position), speed, min, max, format, slider_flags);
    transform_comp.SetPosition(position);
    _ACDA_COMMAND_HELPER("[Transform] Position", transform_comp, GetPosition, SetPosition, position, _TempPosition);
    position_delta = transform_comp.GetPosition() - position_delta; // current - previous

    if(transform_comp.CheckFlag(TransformComponentFlags::UseRotation))
    {
        float rotation_drag_speed = .5f;
        glm::vec3 eular_angle = glm::degrees(transform_comp.GetRotationEularAngle());
        ImGui::DragFloat3("Rotation", glm::value_ptr(eular_angle), rotation_drag_speed, min, max, format, slider_flags);
        transform_comp.SetRotationEularAngle(glm::radians(eular_angle));
        _ACDA_COMMAND_HELPER("[Transform] Rotation", transform_comp, GetRotationEularAngle, SetRotationEularAngle, eular_angle, _TempRotationEularAngle);
    }
    else if(transform_comp.CheckFlag(TransformComponentFlags::UseDirection))
    {
        float rotation_drag_speed = .5f;
        glm::vec3 direction = transform_comp.GetDirection();
        ImGui::DragFloat3("Direction", glm::value_ptr(direction), rotation_drag_speed, min, max, format, slider_flags);
        transform_comp.SetDirection(direction);
        _ACDA_COMMAND_HELPER("[Transform] Direction", transform_comp, GetDirection, SetDirection, direction, _TempDirection);
    }
    else
    {
        ACDA_UNREACHABLE("");
    }

    glm::vec3 scale = transform_comp.GetScale();
    ImGui::DragFloat3("Scale", glm::value_ptr(scale), speed, min, max, format, slider_flags);
    transform_comp.SetScale(scale);
    _ACDA_COMMAND_HELPER("[Transform] Scale", transform_comp, GetScale, SetScale, scale, _TempScale);

    if(position_delta != GlmVec3::CreateZero())
    {
        transform_comp.IncreasePivot(position_delta); // Make pivot move with translation
    }
    glm::vec3 pivot = transform_comp.GetPivot();
    ImGui::DragFloat3("Pivot", glm::value_ptr(pivot), speed, min, max, format, slider_flags);
    transform_comp.SetPivot(pivot);
    _ACDA_COMMAND_HELPER("[Transform] ivott", transform_comp, GetPivot, SetPivot, pivot, _TempPivot);

    ImGui::EndGroup();
}

void Arcadia::ImguiWindowProperty::OnEvent(EventBase& e)
{
    EventDispatcher{ e }
        .Dispatch<Events::OpenImguiWindow>(ACDA_BIND_MEMBER_FN(_OnOpenImguiWindow))
        .Dispatch<Events::SceneActivated>(ACDA_BIND_MEMBER_FN(_OnSceneActivated))
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

    std::shared_ptr<Scene> scene_sptr = _SceneWeakPtr.lock();

    std::string imgui_title = scene_sptr && _SelectedEntityId
        ? _Title + " - " + ToString(_SelectedEntityId) + GetIdString()
        : _Title + GetIdString();

    ImGui::SetNextWindowSize(glm::vec2{ 1024,768 }, ImGuiCond_Once);
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_title.c_str(), &_Opened, window_flags))
    {
        if(!scene_sptr)
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

                _DisplayProperty<CameraComponent>("Camera", _ImguiWindowPropertyFunctor_CameraComponent);
                _DisplayProperty<LightComponent>("Light", _ImguiWindowPropertyFunctor_LightComponent);
                _DisplayProperty<ModelComponent>("Model", _ImguiWindowPropertyFunctor_ModelComponent);
                _DisplayProperty<PhysicsComponent>("Physics", _ImguiWindowPropertyFunctor_PhysicsComponent);
                _DisplayProperty<TransformComponent>("Transform", _ImguiWindowPropertyFunctor_TransformComponent);

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
    _SceneWeakPtr = e.spScene;
    Scene& scene = *e.spScene;
    for(auto& [entity_id, entity_info] : scene.GetEntityInfoStorage())
    {
        if(scene.ContainsAllComponents<TransformComponent>(entity_id))
        {
            _ImguiWindowPropertyFunctor_TransformComponent.Refresh(scene.GetComponent<TransformComponent>(entity_id));
        }

        if(scene.ContainsAllComponents<CameraComponent>(entity_id))
        {
            _ImguiWindowPropertyFunctor_CameraComponent.Refresh(scene.GetComponent<CameraComponent>(entity_id));
        }
    }
}

void Arcadia::ImguiWindowProperty::_OnSceneDeactivated(Events::SceneDeactivated& e)
{
    _SceneWeakPtr.reset();
    _SelectedEntityId.SetNull();
}

void Arcadia::ImguiWindowProperty::_OnSelectEntity(Events::SelectEntity& e)
{
    _SelectedEntityId = e.EntityId;
}

void Arcadia::ImguiWindowProperty::_OnDeleteEntity(Events::DeleteEntity& e)
{
    if(_SelectedEntityId == e.EntityId)
    {
        _SelectedEntityId.SetNull();
    }
}