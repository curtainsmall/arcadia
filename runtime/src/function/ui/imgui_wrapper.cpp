#include "pch.hpp"
#include "imgui_wrapper.hpp"

ARCADIA_API void arcadia::imgui_wrapper::checkbox(const std::string& name, bool& b)
{
    ImGui::Text(name.c_str()); ImGui::SameLine(); ImGui::Checkbox(std::format("##{}", name).c_str(), &b);
}

ARCADIA_API void arcadia::imgui_wrapper::checkbox(const std::string& name, std::function<bool()> getter, std::function<void(bool)> setter)
{
    auto b = getter();
    arcadia::imgui_wrapper::checkbox(name, b);
    setter(b);
}

ARCADIA_API auto arcadia::imgui_wrapper::drag_int(const std::string& name, int& i, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    ImGui::Text(name.c_str()); ImGui::SameLine(); ImGui::DragInt(std::format("##{}", name).c_str(), &i, speed, min, max, format, flags);
    return ImGui::IsItemDeactivatedAfterEdit();
}

ARCADIA_API auto arcadia::imgui_wrapper::drag_int(const std::string& name, std::function<int()> getter, std::function<void(int)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto i = getter();
    auto deactivated = arcadia::imgui_wrapper::drag_int(name, i, speed, min, max, format, flags);
    setter(i);
    return deactivated;
}

ARCADIA_API auto arcadia::imgui_wrapper::drag_ivec2(const std::string& name, glm::ivec2& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
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

ARCADIA_API auto arcadia::imgui_wrapper::drag_ivec2(const std::string& name, std::function<glm::ivec2()> getter, std::function<void(const glm::ivec2&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = arcadia::imgui_wrapper::drag_ivec2(name, vec, speed, min, max, format, flags);
    setter(vec);
    return deactivated;
}

ARCADIA_API auto arcadia::imgui_wrapper::drag_ivec3(const std::string& name, glm::ivec3& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
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

ARCADIA_API auto arcadia::imgui_wrapper::drag_ivec3(const std::string& name, std::function<glm::ivec3()> getter, std::function<void(const glm::ivec3&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = arcadia::imgui_wrapper::drag_ivec3(name, vec, speed, min, max, format, flags);
    setter(vec);
    return deactivated;
}

ARCADIA_API auto arcadia::imgui_wrapper::drag_ivec4(const std::string& name, glm::ivec4& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
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

ARCADIA_API auto arcadia::imgui_wrapper::drag_ivec4(const std::string& name, std::function<glm::ivec4()> getter, std::function<void(const glm::ivec4&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = arcadia::imgui_wrapper::drag_ivec4(name, vec, speed, min, max, format, flags);
    setter(vec);
    return deactivated;
}

ARCADIA_API auto arcadia::imgui_wrapper::drag_float(const std::string& name, float& f, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    ImGui::Text(name.c_str()); ImGui::SameLine(); ImGui::DragFloat(std::format("##{}", name).c_str(), &f, speed, min, max, format, flags);
    return ImGui::IsItemDeactivatedAfterEdit();
}

ARCADIA_API auto arcadia::imgui_wrapper::drag_float(const std::string& name, std::function<float()> getter, std::function<void(float)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto f = getter();
    auto deactivated = arcadia::imgui_wrapper::drag_float(name, f, speed, min, max, format, flags);
    setter(f);
    return deactivated;
}

ARCADIA_API auto arcadia::imgui_wrapper::drag_vec2(const std::string& name, glm::vec2& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
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

ARCADIA_API auto arcadia::imgui_wrapper::drag_vec2(const std::string& name, std::function<glm::vec2()> getter, std::function<void(const glm::vec2&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = arcadia::imgui_wrapper::drag_vec2(name, vec, speed, min, max, format, flags);
    setter(vec);
    return deactivated;
}

ARCADIA_API auto arcadia::imgui_wrapper::drag_vec3(const std::string& name, glm::vec3& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
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

ARCADIA_API auto arcadia::imgui_wrapper::drag_vec3(const std::string& name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = arcadia::imgui_wrapper::drag_vec3(name, vec, speed, min, max, format, flags);
    setter(vec);
    return deactivated;
}

ARCADIA_API auto arcadia::imgui_wrapper::drag_vec3_color(const std::string& name, glm::vec3& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
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

ARCADIA_API auto arcadia::imgui_wrapper::drag_vec3_color(const std::string& name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = arcadia::imgui_wrapper::drag_vec3_color(name, vec, speed, min, max, format, flags);
    setter(vec);
    return deactivated;
}

ARCADIA_API auto arcadia::imgui_wrapper::drag_vec4(const std::string& name, glm::vec4& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
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

ARCADIA_API auto arcadia::imgui_wrapper::drag_vec4(const std::string& name, std::function<glm::vec4()> getter, std::function<void(const glm::vec4&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto vec = getter();
    auto deactivated = arcadia::imgui_wrapper::drag_vec4(name, vec, speed, min, max, format, flags);
    setter(vec);
    return deactivated;
}

ARCADIA_API auto arcadia::imgui_wrapper::drag_quat(const std::string& name, glm::quat& quat, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
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

ARCADIA_API auto arcadia::imgui_wrapper::drag_quat(const std::string& name, std::function<glm::quat()> getter, std::function<void(const glm::quat&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto quat = getter();
    auto deactivated = arcadia::imgui_wrapper::drag_quat(name, quat, speed, min, max, format, flags);
    setter(quat);
    return deactivated;
}

ARCADIA_API auto arcadia::imgui_wrapper::drag_quat_normalized(const std::string& name, glm::quat& quat, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto temp_quat = quat;
    auto deactivated = arcadia::imgui_wrapper::drag_quat(name, temp_quat, speed, min, max, format, flags);
    if(temp_quat.w != quat.w)
    {
        quat = arcadia::quat::fixed_normalize<0>(temp_quat);
    }
    else if(temp_quat.x != quat.x)
    {
        quat = arcadia::quat::fixed_normalize<1>(temp_quat);
    }
    else if(temp_quat.y != quat.y)
    {
        quat = arcadia::quat::fixed_normalize<2>(temp_quat);
    }
    else if(temp_quat.z != quat.x)
    {
        quat = arcadia::quat::fixed_normalize<3>(temp_quat);
    }
    return deactivated;
}

ARCADIA_API auto arcadia::imgui_wrapper::drag_quat_normalized(const std::string& name, std::function<glm::quat()> getter, std::function<void(const glm::quat&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool
{
    auto quat = getter();
    auto deactivated = arcadia::imgui_wrapper::drag_quat_normalized(name, quat, speed, min, max, format, flags);
    setter(quat);
    return deactivated;
}

ARCADIA_API auto arcadia::imgui_wrapper::color_edit3(const std::string& name, glm::vec3& color) -> bool
{
    ImGui::Text(name.c_str()); ImGui::SameLine(); ImGui::ColorEdit3(std::format("##{}", name).c_str(), &color[0]);
    return ImGui::IsItemDeactivatedAfterEdit();
}

ARCADIA_API auto arcadia::imgui_wrapper::color_edit3(const std::string& name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter) -> bool
{
    auto color = getter();
    auto deactivated = arcadia::imgui_wrapper::color_edit3(name, color);
    setter(color);
    return deactivated;
}

ARCADIA_API auto arcadia::imgui_wrapper::color_edit4(const std::string& name, glm::vec4& color) -> bool
{
    ImGui::Text(name.c_str()); ImGui::SameLine(); ImGui::ColorEdit4(std::format("##{}", name).c_str(), &color[0]);
    return ImGui::IsItemDeactivatedAfterEdit();
}

ARCADIA_API auto arcadia::imgui_wrapper::color_edit4(const std::string& name, std::function<glm::vec4()> getter, std::function<void(const glm::vec4&)> setter) -> bool
{
    auto color = getter();
    auto deactivated = arcadia::imgui_wrapper::color_edit4(name, color);
    setter(color);
    return deactivated;
}

ARCADIA_API auto arcadia::imgui_wrapper::text_vec3(const std::string& name, const glm::vec3& vec) -> bool
{
    ImGui::Text(std::format("{:>{}} X: {:.2f}", name, name.size(), vec.x).c_str());
    ImGui::Text(std::format("{:>{}} Y: {:.2f}", ""s, name.size(), vec.y).c_str());
    ImGui::Text(std::format("{:>{}} Z: {:.2f}", ""s, name.size(), vec.z).c_str());

}

ARCADIA_API auto arcadia::imgui_wrapper::text_vec4(const std::string& name, const glm::vec4& vec) -> bool
{
    ImGui::Text(std::format("{:>{}} X: {:.2f}", name, name.size(), vec.x).c_str());
    ImGui::Text(std::format("{:>{}} Y: {:.2f}", ""s, name.size(), vec.y).c_str());
    ImGui::Text(std::format("{:>{}} Z: {:.2f}", ""s, name.size(), vec.z).c_str());
    ImGui::Text(std::format("{:>{}} W: {:.2f}", ""s, name.size(), vec.w).c_str());
}

ARCADIA_API auto arcadia::imgui_wrapper::text_quat(const std::string& name, const glm::quat& quat) -> bool
{
    ImGui::Text(std::format("{:>{}} W: {:.2f}", name, name.size(), quat.w).c_str());
    ImGui::Text(std::format("{:>{}} X: {:.2f}", ""s, name.size(), quat.x).c_str());
    ImGui::Text(std::format("{:>{}} Y: {:.2f}", ""s, name.size(), quat.y).c_str());
    ImGui::Text(std::format("{:>{}} Z: {:.2f}", ""s, name.size(), quat.z).c_str());
}
