#include "pch.hpp"

#include "imgui_wrapper.hpp"

ACDA_API auto Arcadia::ImguiWrappers::Checkbox(const std::string& name, bool& b)  -> bool
{
    ImGui::Text(name.c_str()); ImGui::SameLine(); ImGui::Checkbox(std::format("##{}", name).c_str(), &b);
    return ImGui::IsItemDeactivatedAfterEdit();
}

ACDA_API auto Arcadia::ImguiWrappers::Checkbox(const std::string& name, std::function<bool()> getter, std::function<void(bool)> setter) -> bool
{
    auto b = getter();
    auto deactivated = ImguiWrappers::Checkbox(name, b);
    setter(b);
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::DragInt(const std::string& name, int32_t& i, float speed, int32_t min, int32_t max, const char* format, ImGuiSliderFlags flags) -> bool
{
    int _i = i;
    ImGui::Text(name.c_str());
    ImGui::SameLine();
    ImGui::DragInt(std::format("##{}", name).c_str(), &_i, speed, min, max, format, flags);
    i = _i;
    return ImGui::IsItemDeactivatedAfterEdit();
}

ACDA_API auto Arcadia::ImguiWrappers::DragInt(const std::string& name, std::function<int32_t()> getter, std::function<void(int32_t)> setter, float speed, int32_t min, int32_t max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto i = getter();
    auto deactivated = ImguiWrappers::DragInt(name, i, speed, min, max, format, flags);
    setter(i);
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::DragIntVec2(const std::string& name, glm::ivec2& vec, float speed, int32_t min, int32_t max, const char* format, ImGuiSliderFlags flags) -> bool
{
    bool deactivated{ false };
    ImGui::Text(std::format("{:>{}} X", name, name.size()).c_str()); ImGui::SameLine(); ImGui::DragInt(std::format("##{}_x", name).c_str(), &vec.x, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} Y", ""s, name.size()).c_str()); ImGui::SameLine(); ImGui::DragInt(std::format("##{}_y", name).c_str(), &vec.y, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::DragIntVec2(const std::string& name, std::function<glm::ivec2()> getter, std::function<void(const glm::ivec2&)> setter, float speed, int32_t min, int32_t max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = ImguiWrappers::DragIntVec2(name, vec, speed, min, max, format, flags);
    setter(vec);
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::DragIntVec3(const std::string& name, glm::ivec3& vec, float speed, int32_t min, int32_t max, const char* format, ImGuiSliderFlags flags) -> bool
{
    bool deactivated{ false };
    ImGui::Text(std::format("{:>{}} X", name, name.size()).c_str()); ImGui::SameLine(); ImGui::DragInt(std::format("##{}_x", name).c_str(), &vec.x, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} Y", ""s, name.size()).c_str()); ImGui::SameLine(); ImGui::DragInt(std::format("##{}_y", name).c_str(), &vec.y, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} Z", ""s, name.size()).c_str()); ImGui::SameLine(); ImGui::DragInt(std::format("##{}_z", name).c_str(), &vec.z, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::DragIntVec3(const std::string& name, std::function<glm::ivec3()> getter, std::function<void(const glm::ivec3&)> setter, float speed, int32_t min, int32_t max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = ImguiWrappers::DragIntVec3(name, vec, speed, min, max, format, flags);
    setter(vec);
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::DragIntVec4(const std::string& name, glm::ivec4& vec, float speed, int32_t min, int32_t max, const char* format, ImGuiSliderFlags flags) -> bool
{
    bool deactivated{ false };
    ImGui::Text(std::format("{:>{}} X", name, name.size()).c_str()); ImGui::SameLine(); ImGui::DragInt(std::format("##{}_x", name).c_str(), &vec.x, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} Y", ""s, name.size()).c_str()); ImGui::SameLine(); ImGui::DragInt(std::format("##{}_y", name).c_str(), &vec.y, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} Z", ""s, name.size()).c_str()); ImGui::SameLine(); ImGui::DragInt(std::format("##{}_z", name).c_str(), &vec.z, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} W", ""s, name.size()).c_str()); ImGui::SameLine(); ImGui::DragInt(std::format("##{}_w", name).c_str(), &vec.w, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::DragIntVec4(const std::string& name, std::function<glm::ivec4()> getter, std::function<void(const glm::ivec4&)> setter, float speed, int32_t min, int32_t max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = ImguiWrappers::DragIntVec4(name, vec, speed, min, max, format, flags);
    setter(vec);
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::DragFloat(const std::string& name, float& f, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    ImGui::Text(name.c_str());
    ImGui::SameLine();
    ImGui::DragFloat(std::format("##{}", name).c_str(), &f, speed, min, max, format, flags);
    return ImGui::IsItemDeactivatedAfterEdit();
}

ACDA_API auto Arcadia::ImguiWrappers::DragFloat(const std::string& name, std::function<float()> getter, std::function<void(float)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto f = getter();
    auto deactivated = ImguiWrappers::DragFloat(name, f, speed, min, max, format, flags);
    setter(f);
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::DragFloat3(const std::string& name, glm::vec3& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    ImGui::Text(name.c_str()); ImGui::SameLine(); ImGui::DragFloat3(std::format("##{}", name).c_str(), glm::value_ptr(vec), speed, min, max, format, flags);
    return ImGui::IsItemDeactivatedAfterEdit();
}

ACDA_API auto Arcadia::ImguiWrappers::DragFloat3(const std::string& name, std::function<glm::vec3()> getter, std::function<void(glm::vec3)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = ImguiWrappers::DragFloat3(name, vec, speed, min, max, format, flags);
    setter(vec);
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::DragVec2(const std::string& name, glm::vec2& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    bool deactivated{ false };
    ImGui::Text(std::format("{:>{}} X", name, name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_x", name).c_str(), &vec.x, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} Y", ""s, name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_y", name).c_str(), &vec.y, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::DragVec2(const std::string& name, std::function<glm::vec2()> getter, std::function<void(const glm::vec2&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = ImguiWrappers::DragVec2(name, vec, speed, min, max, format, flags);
    setter(vec);
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::DragVec3(const std::string& name, glm::vec3& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    bool deactivated{ false };
    ImGui::Text(std::format("{:>{}} X", name, name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_x", name).c_str(), &vec.x, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} Y", ""s, name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_y", name).c_str(), &vec.y, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} Z", ""s, name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_z", name).c_str(), &vec.z, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::DragVec3(const std::string& name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = ImguiWrappers::DragVec3(name, vec, speed, min, max, format, flags);
    setter(vec);
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::DragVec3Normalized(const std::string& name, glm::vec3& vec, float speed, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto temp_vec = vec;
    auto deactivated = ImguiWrappers::DragVec3(name, temp_vec, speed, -1.f, 1.f, format, flags);
    if(temp_vec.x != vec.x)
    {
        vec = GlmVec3::NormalizeFixedly<0>(temp_vec);
    }
    else if(temp_vec.y != vec.y)
    {
        vec = GlmVec3::NormalizeFixedly<1>(temp_vec);
    }
    else if(temp_vec.z != vec.z)
    {
        vec = GlmVec3::NormalizeFixedly<2>(temp_vec);
    }
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::DragVec3Normalized(const std::string& name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter, float speed, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = ImguiWrappers::DragVec3Normalized(name, vec, speed, format, flags);
    setter(vec);
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::DragVec3Color(const std::string& name, glm::vec3& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    bool deactivated{ false };
    ImGui::Text(std::format("{:>{}} R", name, name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_r", name).c_str(), &vec.x, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} G", ""s, name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_g", name).c_str(), &vec.y, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} B", ""s, name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_b", name).c_str(), &vec.z, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::DragVec3Color(const std::string& name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = ImguiWrappers::DragVec3Color(name, vec, speed, min, max, format, flags);
    setter(vec);
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::DragVec4(const std::string& name, glm::vec4& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    bool deactivated{ false };
    ImGui::Text(std::format("{:>{}} X", name, name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_x", name).c_str(), &vec.x, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} Y", ""s, name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_y", name).c_str(), &vec.y, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} Z", ""s, name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_z", name).c_str(), &vec.z, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} W", ""s, name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_w", name).c_str(), &vec.w, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::DragVec4(const std::string& name, std::function<glm::vec4()> getter, std::function<void(const glm::vec4&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = ImguiWrappers::DragVec4(name, vec, speed, min, max, format, flags);
    setter(vec);
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::DragQuat(const std::string& name, glm::quat& quat, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    bool deactivated{ false };
    ImGui::Text(std::format("{:>{}} W", name, name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_w", name).c_str(), &quat.w, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} X", ""s, name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_x", name).c_str(), &quat.x, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} y", ""s, name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_y", name).c_str(), &quat.y, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} Z", ""s, name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_z", name).c_str(), &quat.z, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::DragQuat(const std::string& name, std::function<glm::quat()> getter, std::function<void(const glm::quat&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto quat = getter();
    auto deactivated = ImguiWrappers::DragQuat(name, quat, speed, min, max, format, flags);
    setter(quat);
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::DragQuatNormalized(const std::string& name, glm::quat& quat, float speed, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto temp_quat = quat;
    auto deactivated = ImguiWrappers::DragQuat(name, temp_quat, speed, -1.f, 1.f, format, flags);
    if(temp_quat.w != quat.w)
    {
        quat = GlmQuat::NormalizeFixedly<0>(temp_quat);
    }
    else if(temp_quat.x != quat.x)
    {
        quat = GlmQuat::NormalizeFixedly<1>(temp_quat);
    }
    else if(temp_quat.y != quat.y)
    {
        quat = GlmQuat::NormalizeFixedly<2>(temp_quat);
    }
    else if(temp_quat.z != quat.x)
    {
        quat = GlmQuat::NormalizeFixedly<3>(temp_quat);
    }
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::DragQuatNormalized(const std::string& name, std::function<glm::quat()> getter, std::function<void(const glm::quat&)> setter, float speed, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto quat = getter();
    auto deactivated = ImguiWrappers::DragQuatNormalized(name, quat, speed, format, flags);
    setter(quat);
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::ColorEditVec3(const std::string& name, glm::vec3& color) -> bool
{
    ImGui::Text(name.c_str()); ImGui::SameLine(); ImGui::ColorEdit3(std::format("##{}", name).c_str(), &color[0]);
    return ImGui::IsItemDeactivatedAfterEdit();
}

ACDA_API auto Arcadia::ImguiWrappers::ColorEditVec3(const std::string& name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter) -> bool
{
    auto color = getter();
    auto deactivated = ImguiWrappers::ColorEditVec3(name, color);
    setter(color);
    return deactivated;
}

ACDA_API auto Arcadia::ImguiWrappers::ColorEditVec4(const std::string& name, glm::vec4& color) -> bool
{
    ImGui::Text(name.c_str()); ImGui::SameLine(); ImGui::ColorEdit4(std::format("##{}", name).c_str(), &color[0]);
    return ImGui::IsItemDeactivatedAfterEdit();
}

ACDA_API auto Arcadia::ImguiWrappers::ColorEditVec4(const std::string& name, std::function<glm::vec4()> getter, std::function<void(const glm::vec4&)> setter) -> bool
{
    auto color = getter();
    auto deactivated = ImguiWrappers::ColorEditVec4(name, color);
    setter(color);
    return deactivated;
}

ACDA_API void Arcadia::ImguiWrappers::TextVec3(const std::string& name, const glm::vec3& vec)
{
    ImGui::Text(std::format("{:>{}} X: {:.2f}", name, name.size(), vec.x).c_str());
    ImGui::Text(std::format("{:>{}} Y: {:.2f}", ""s, name.size(), vec.y).c_str());
    ImGui::Text(std::format("{:>{}} Z: {:.2f}", ""s, name.size(), vec.z).c_str());
}

ACDA_API void Arcadia::ImguiWrappers::TextVec4(const std::string& name, const glm::vec4& vec)
{
    ImGui::Text(std::format("{:>{}} X: {:.2f}", name, name.size(), vec.x).c_str());
    ImGui::Text(std::format("{:>{}} Y: {:.2f}", ""s, name.size(), vec.y).c_str());
    ImGui::Text(std::format("{:>{}} Z: {:.2f}", ""s, name.size(), vec.z).c_str());
    ImGui::Text(std::format("{:>{}} W: {:.2f}", ""s, name.size(), vec.w).c_str());
}

ACDA_API void Arcadia::ImguiWrappers::TextQuat(const std::string& name, const glm::quat& quat)
{
    ImGui::Text(std::format("{:>{}} W: {:.2f}", name, name.size(), quat.w).c_str());
    ImGui::Text(std::format("{:>{}} X: {:.2f}", ""s, name.size(), quat.x).c_str());
    ImGui::Text(std::format("{:>{}} Y: {:.2f}", ""s, name.size(), quat.y).c_str());
    ImGui::Text(std::format("{:>{}} Z: {:.2f}", ""s, name.size(), quat.z).c_str());
}

ACDA_API void Arcadia::ImguiWrappers::NewLine(float height)
{
    glm::vec2 size{ 0.0f,height };
    if(height < 0.0f)
    {
        size.y = ImGui::GetStyle().ItemSpacing.y / 2.0f;
    }

    ImGui::Dummy(size);
}

ACDA_API void Arcadia::ImguiWrappers::HelpMark(const std::string& icon, const std::string& text)
{
    ImGui::TextDisabled("%s", icon.c_str());
    if(ImGui::BeginItemTooltip())
    {
        ImGui::TextUnformatted(text.c_str());
        ImGui::EndTooltip();
    }
}