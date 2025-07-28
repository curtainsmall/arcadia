#include "imgui_wrapper.hpp"

auto Arcadia::ImguiWrappers::Checkbox(std::string_view name, bool& b)  -> bool
{
    ImGui::Text(name.data()); ImGui::SameLine(); ImGui::Checkbox(std::format("##{}", name).c_str(), &b);
    return ImGui::IsItemDeactivatedAfterEdit();
}

auto Arcadia::ImguiWrappers::Checkbox(std::string_view name, std::function<bool()> getter, std::function<void(bool)> setter) -> bool
{
    auto b = getter();
    auto deactivated = ImguiWrappers::Checkbox(name, b);
    setter(b);
    return deactivated;
}

auto Arcadia::ImguiWrappers::DragInt(std::string_view name, std::int32_t& i, float speed, std::int32_t min, std::int32_t max, const char* format, ImGuiSliderFlags flags) -> bool
{
    int _i = i;
    ImGui::Text(name.data());
    ImGui::SameLine();
    ImGui::DragInt(std::format("##{}", name).c_str(), &_i, speed, min, max, format, flags);
    i = _i;
    return ImGui::IsItemDeactivatedAfterEdit();
}

auto Arcadia::ImguiWrappers::DragInt(std::string_view name, std::function<std::int32_t()> getter, std::function<void(std::int32_t)> setter, float speed, std::int32_t min, std::int32_t max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto i = getter();
    auto deactivated = ImguiWrappers::DragInt(name, i, speed, min, max, format, flags);
    setter(i);
    return deactivated;
}

auto Arcadia::ImguiWrappers::DragInt32Vec2(std::string_view name, glm::i32vec2& vec, float speed, std::int32_t min, std::int32_t max, const char* format, ImGuiSliderFlags flags) -> bool
{
    bool deactivated = false;
    ImGui::Text(std::format("{:>{}} X", name, name.size()).c_str()); ImGui::SameLine(); ImGui::DragInt(std::format("##{}_x", name).c_str(), &vec.x, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} Y", "", name.size()).c_str()); ImGui::SameLine(); ImGui::DragInt(std::format("##{}_y", name).c_str(), &vec.y, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    return deactivated;
}

auto Arcadia::ImguiWrappers::DragInt32Vec2(std::string_view name, std::function<glm::i32vec2()> getter, std::function<void(const glm::i32vec2&)> setter, float speed, std::int32_t min, std::int32_t max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = ImguiWrappers::DragInt32Vec2(name, vec, speed, min, max, format, flags);
    setter(vec);
    return deactivated;
}

auto Arcadia::ImguiWrappers::DragInt32Vec3(std::string_view name, glm::i32vec3& vec, float speed, std::int32_t min, std::int32_t max, const char* format, ImGuiSliderFlags flags) -> bool
{
    bool deactivated = false;
    ImGui::Text(std::format("{:>{}} X", name, name.size()).c_str()); ImGui::SameLine(); ImGui::DragInt(std::format("##{}_x", name).c_str(), &vec.x, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} Y", "", name.size()).c_str()); ImGui::SameLine(); ImGui::DragInt(std::format("##{}_y", name).c_str(), &vec.y, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} Z", "", name.size()).c_str()); ImGui::SameLine(); ImGui::DragInt(std::format("##{}_z", name).c_str(), &vec.z, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    return deactivated;
}

auto Arcadia::ImguiWrappers::DragInt32Vec3(std::string_view name, std::function<glm::i32vec3()> getter, std::function<void(const glm::i32vec3&)> setter, float speed, std::int32_t min, std::int32_t max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = ImguiWrappers::DragInt32Vec3(name, vec, speed, min, max, format, flags);
    setter(vec);
    return deactivated;
}

auto Arcadia::ImguiWrappers::DragInt32Vec4(std::string_view name, glm::i32vec4& vec, float speed, std::int32_t min, std::int32_t max, const char* format, ImGuiSliderFlags flags) -> bool
{
    bool deactivated = false;
    ImGui::Text(std::format("{:>{}} X", name, name.size()).c_str()); ImGui::SameLine(); ImGui::DragInt(std::format("##{}_x", name).c_str(), &vec.x, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} Y", "", name.size()).c_str()); ImGui::SameLine(); ImGui::DragInt(std::format("##{}_y", name).c_str(), &vec.y, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} Z", "", name.size()).c_str()); ImGui::SameLine(); ImGui::DragInt(std::format("##{}_z", name).c_str(), &vec.z, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} W", "", name.size()).c_str()); ImGui::SameLine(); ImGui::DragInt(std::format("##{}_w", name).c_str(), &vec.w, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    return deactivated;
}

auto Arcadia::ImguiWrappers::DragInt32Vec4(std::string_view name, std::function<glm::i32vec4()> getter, std::function<void(const glm::i32vec4&)> setter, float speed, std::int32_t min, std::int32_t max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = ImguiWrappers::DragInt32Vec4(name, vec, speed, min, max, format, flags);
    setter(vec);
    return deactivated;
}

auto Arcadia::ImguiWrappers::DragFloat(std::string_view name, float& f, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    float _f = f;
    ImGui::Text(name.data());
    ImGui::SameLine();
    ImGui::DragFloat(std::format("##{}", name).c_str(), &_f, speed, min, max, format, flags);
    f = _f;
    return ImGui::IsItemDeactivatedAfterEdit();
}

auto Arcadia::ImguiWrappers::DragFloat(std::string_view name, std::function<float()> getter, std::function<void(float)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto f = getter();
    auto deactivated = ImguiWrappers::DragFloat(name, f, speed, min, max, format, flags);
    setter(f);
    return deactivated;
}

auto Arcadia::ImguiWrappers::DragFloat3(std::string_view name, glm::vec3& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    ImGui::Text(name.data()); ImGui::SameLine(); ImGui::DragFloat3(std::format("##{}", name).c_str(), glm::value_ptr(vec), speed, min, max, format, flags);
    return ImGui::IsItemDeactivatedAfterEdit();
}

auto Arcadia::ImguiWrappers::DragFloat3(std::string_view name, std::function<glm::vec3()> getter, std::function<void(glm::vec3)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = ImguiWrappers::DragFloat3(name, vec, speed, min, max, format, flags);
    setter(vec);
    return deactivated;
}

auto Arcadia::ImguiWrappers::DragVec2(std::string_view name, glm::vec2& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    bool deactivated = false;
    ImGui::Text(std::format("{:>{}} X", name, name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_x", name).c_str(), &vec.x, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} Y", "", name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_y", name).c_str(), &vec.y, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    return deactivated;
}

auto Arcadia::ImguiWrappers::DragVec2(std::string_view name, std::function<glm::vec2()> getter, std::function<void(const glm::vec2&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = ImguiWrappers::DragVec2(name, vec, speed, min, max, format, flags);
    setter(vec);
    return deactivated;
}

auto Arcadia::ImguiWrappers::DragVec3(std::string_view name, glm::vec3& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    bool deactivated = false;
    ImGui::Text(std::format("{:>{}} X", name, name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_x", name).c_str(), &vec.x, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} Y", "", name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_y", name).c_str(), &vec.y, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} Z", "", name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_z", name).c_str(), &vec.z, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    return deactivated;
}

auto Arcadia::ImguiWrappers::DragVec3(std::string_view name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = ImguiWrappers::DragVec3(name, vec, speed, min, max, format, flags);
    setter(vec);
    return deactivated;
}

auto Arcadia::ImguiWrappers::DragVec3Normalized(std::string_view name, glm::vec3& vec, float speed, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto temp_vec = vec;
    auto deactivated = ImguiWrappers::DragVec3(name, temp_vec, speed, -1.f, 1.f, format, flags);
    if(temp_vec.x != vec.x)
    {
        vec = Glm::Vec3_NormalizeFixedly(temp_vec, 0);
    }
    else if(temp_vec.y != vec.y)
    {
        vec = Glm::Vec3_NormalizeFixedly(temp_vec, 1);
    }
    else if(temp_vec.z != vec.z)
    {
        vec = Glm::Vec3_NormalizeFixedly(temp_vec, 2);
    }
    return deactivated;
}

auto Arcadia::ImguiWrappers::DragVec3Normalized(std::string_view name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter, float speed, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = ImguiWrappers::DragVec3Normalized(name, vec, speed, format, flags);
    setter(vec);
    return deactivated;
}

auto Arcadia::ImguiWrappers::DragVec3Color(std::string_view name, glm::vec3& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    bool deactivated = false;
    ImGui::Text(std::format("{:>{}} R", name, name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_r", name).c_str(), &vec.x, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} G", "", name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_g", name).c_str(), &vec.y, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} B", "", name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_b", name).c_str(), &vec.z, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    return deactivated;
}

auto Arcadia::ImguiWrappers::DragVec3Color(std::string_view name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = ImguiWrappers::DragVec3Color(name, vec, speed, min, max, format, flags);
    setter(vec);
    return deactivated;
}

auto Arcadia::ImguiWrappers::DragVec4(std::string_view name, glm::vec4& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    bool deactivated = false;
    ImGui::Text(std::format("{:>{}} X", name, name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_x", name).c_str(), &vec.x, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} Y", "", name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_y", name).c_str(), &vec.y, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} Z", "", name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_z", name).c_str(), &vec.z, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} W", "", name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_w", name).c_str(), &vec.w, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    return deactivated;
}

auto Arcadia::ImguiWrappers::DragVec4(std::string_view name, std::function<glm::vec4()> getter, std::function<void(const glm::vec4&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = ImguiWrappers::DragVec4(name, vec, speed, min, max, format, flags);
    setter(vec);
    return deactivated;
}

auto Arcadia::ImguiWrappers::DragQuat(std::string_view name, glm::quat& quat, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    bool deactivated = false;
    ImGui::Text(std::format("{:>{}} W", name, name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_w", name).c_str(), &quat.w, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} X", "", name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_x", name).c_str(), &quat.x, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} y", "", name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_y", name).c_str(), &quat.y, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    ImGui::Text(std::format("{:>{}} Z", "", name.size()).c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}_z", name).c_str(), &quat.z, speed, min, max, format, flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
        deactivated = true;
    }
    return deactivated;
}

auto Arcadia::ImguiWrappers::DragQuat(std::string_view name, std::function<glm::quat()> getter, std::function<void(const glm::quat&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto quat = getter();
    auto deactivated = ImguiWrappers::DragQuat(name, quat, speed, min, max, format, flags);
    setter(quat);
    return deactivated;
}

auto Arcadia::ImguiWrappers::DragQuatNormalized(std::string_view name, glm::quat& quat, float speed, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto temp_quat = quat;
    auto deactivated = ImguiWrappers::DragQuat(name, temp_quat, speed, -1.f, 1.f, format, flags);
    if(temp_quat.w != quat.w)
    {
        quat = Glm::Quat_NormalizeFixedly(temp_quat, 0);
    }
    else if(temp_quat.x != quat.x)
    {
        quat = Glm::Quat_NormalizeFixedly(temp_quat, 1);
    }
    else if(temp_quat.y != quat.y)
    {
        quat = Glm::Quat_NormalizeFixedly(temp_quat, 2);
    }
    else if(temp_quat.z != quat.x)
    {
        quat = Glm::Quat_NormalizeFixedly(temp_quat, 3);
    }
    return deactivated;
}

auto Arcadia::ImguiWrappers::DragQuatNormalized(std::string_view name, std::function<glm::quat()> getter, std::function<void(const glm::quat&)> setter, float speed, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto quat = getter();
    auto deactivated = ImguiWrappers::DragQuatNormalized(name, quat, speed, format, flags);
    setter(quat);
    return deactivated;
}

auto Arcadia::ImguiWrappers::ColorEditVec3(std::string_view name, glm::vec3& color) -> bool
{
    ImGui::Text(name.data()); ImGui::SameLine(); ImGui::ColorEdit3(std::format("##{}", name).c_str(), &color[0]);
    return ImGui::IsItemDeactivatedAfterEdit();
}

auto Arcadia::ImguiWrappers::ColorEditVec3(std::string_view name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter) -> bool
{
    auto color = getter();
    auto deactivated = ImguiWrappers::ColorEditVec3(name, color);
    setter(color);
    return deactivated;
}

auto Arcadia::ImguiWrappers::ColorEditVec4(std::string_view name, glm::vec4& color) -> bool
{
    ImGui::Text(name.data()); ImGui::SameLine(); ImGui::ColorEdit4(std::format("##{}", name).c_str(), &color[0]);
    return ImGui::IsItemDeactivatedAfterEdit();
}

auto Arcadia::ImguiWrappers::ColorEditVec4(std::string_view name, std::function<glm::vec4()> getter, std::function<void(const glm::vec4&)> setter) -> bool
{
    auto color = getter();
    auto deactivated = ImguiWrappers::ColorEditVec4(name, color);
    setter(color);
    return deactivated;
}

void Arcadia::ImguiWrappers::TextVec3(std::string_view name, const glm::vec3& vec)
{
    ImGui::Text(std::format("{:>{}} X: {:.2f}", name, name.size(), vec.x).c_str());
    ImGui::Text(std::format("{:>{}} Y: {:.2f}", "", name.size(), vec.y).c_str());
    ImGui::Text(std::format("{:>{}} Z: {:.2f}", "", name.size(), vec.z).c_str());
}

void Arcadia::ImguiWrappers::TextVec4(std::string_view name, const glm::vec4& vec)
{
    ImGui::Text(std::format("{:>{}} X: {:.2f}", name, name.size(), vec.x).c_str());
    ImGui::Text(std::format("{:>{}} Y: {:.2f}", "", name.size(), vec.y).c_str());
    ImGui::Text(std::format("{:>{}} Z: {:.2f}", "", name.size(), vec.z).c_str());
    ImGui::Text(std::format("{:>{}} W: {:.2f}", "", name.size(), vec.w).c_str());
}

void Arcadia::ImguiWrappers::TextQuat(std::string_view name, const glm::quat& quat)
{
    ImGui::Text(std::format("{:>{}} W: {:.2f}", name, name.size(), quat.w).c_str());
    ImGui::Text(std::format("{:>{}} X: {:.2f}", "", name.size(), quat.x).c_str());
    ImGui::Text(std::format("{:>{}} Y: {:.2f}", "", name.size(), quat.y).c_str());
    ImGui::Text(std::format("{:>{}} Z: {:.2f}", "", name.size(), quat.z).c_str());
}

void Arcadia::ImguiWrappers::NewLine(float height)
{
    glm::vec2 size(0.0f, height);
    if(height < 0.0f)
    {
        size.y = ImGui::GetStyle().ItemSpacing.y / 2.0f;
    }

    ImGui::Dummy(size);
}

void Arcadia::ImguiWrappers::HelpMark(std::string_view text, std::string_view icon)
{
    ImGui::TextDisabled("%s", icon.data());
    if(ImGui::BeginItemTooltip())
    {
        ImGui::TextUnformatted(text.data());
        ImGui::EndTooltip();
    }
}