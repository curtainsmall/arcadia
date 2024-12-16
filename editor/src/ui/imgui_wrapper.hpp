#pragma once

#include<string>

#include"core/base.hpp"
#include"core/math.hpp"
#include"ui/imgui_header.hpp"

namespace Arcadia
{
    namespace ImguiWrappers
    {
        ACDA_API auto Checkbox(const std::string& name, bool& b) -> bool;
        ACDA_API auto Checkbox(const std::string& name, std::function<bool()> getter, std::function<void(bool)> setter) -> bool;

        ACDA_API auto DragInt(const std::string& name, std::int32_t& i, float speed, std::int32_t min, std::int32_t max, const char* format, ImGuiSliderFlags flags) -> bool;
        ACDA_API auto DragInt(const std::string& name, std::function<std::int32_t()> getter, std::function<void(std::int32_t)> setter, float speed, std::int32_t min, std::int32_t max, const char* format, ImGuiSliderFlags flags) -> bool;

        ACDA_API auto DragInt32Vec2(const std::string& name, glm::i32vec2& vec, float speed, std::int32_t min, std::int32_t max, const char* format, ImGuiSliderFlags flags) -> bool;
        ACDA_API auto DragInt32Vec2(const std::string& name, std::function<glm::i32vec2()> getter, std::function<void(const glm::i32vec2&)> setter, float speed, std::int32_t min, std::int32_t max, const char* format, ImGuiSliderFlags flags) -> bool;

        ACDA_API auto DragInt32Vec3(const std::string& name, glm::i32vec3& vec, float speed, std::int32_t min, std::int32_t max, const char* format, ImGuiSliderFlags flags) -> bool;
        ACDA_API auto DragInt32Vec3(const std::string& name, std::function<glm::i32vec3()> getter, std::function<void(const glm::i32vec3&)> setter, float speed, std::int32_t min, std::int32_t max, const char* format, ImGuiSliderFlags flags) -> bool;

        ACDA_API auto DragInt32Vec4(const std::string& name, glm::i32vec4& vec, float speed, std::int32_t min, std::int32_t max, const char* format, ImGuiSliderFlags flags) -> bool;
        ACDA_API auto DragInt32Vec4(const std::string& name, std::function<glm::i32vec4()> getter, std::function<void(const glm::i32vec4&)> setter, float speed, std::int32_t min, std::int32_t max, const char* format, ImGuiSliderFlags flags) -> bool;

        ACDA_API auto DragFloat(const std::string& name, float& f, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        ACDA_API auto DragFloat(const std::string& name, std::function<float()> getter, std::function<void(float)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;

        ACDA_API auto DragFloat3(const std::string& name, glm::vec3& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        ACDA_API auto DragFloat3(const std::string& name, std::function<glm::vec3()> getter, std::function<void(glm::vec3)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;

        ACDA_API auto DragVec2(const std::string& name, glm::vec2& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        ACDA_API auto DragVec2(const std::string& name, std::function<glm::vec2()> getter, std::function<void(const glm::vec2&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;

        ACDA_API auto DragVec3(const std::string& name, glm::vec3& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        ACDA_API auto DragVec3(const std::string& name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        ACDA_API auto DragVec3Normalized(const std::string& name, glm::vec3& vec, float speed, const char* format, ImGuiSliderFlags flags) -> bool;
        ACDA_API auto DragVec3Normalized(const std::string& name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter, float speed, const char* format, ImGuiSliderFlags flags) -> bool;
        ACDA_API auto DragVec3Color(const std::string& name, glm::vec3& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        ACDA_API auto DragVec3Color(const std::string& name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;

        ACDA_API auto DragVec4(const std::string& name, glm::vec4& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        ACDA_API auto DragVec4(const std::string& name, std::function<glm::vec4()> getter, std::function<void(const glm::vec4&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;

        ACDA_API auto DragQuat(const std::string& name, glm::quat& quat, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        ACDA_API auto DragQuat(const std::string& name, std::function<glm::quat()> getter, std::function<void(const glm::quat&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        ACDA_API auto DragQuatNormalized(const std::string& name, glm::quat& quat, float speed, const char* format, ImGuiSliderFlags flags) -> bool;
        ACDA_API auto DragQuatNormalized(const std::string& name, std::function<glm::quat()> getter, std::function<void(const glm::quat&)> setter, float speed, const char* format, ImGuiSliderFlags flags) -> bool;

        ACDA_API auto ColorEditVec3(const std::string& name, glm::vec3& color) -> bool;
        ACDA_API auto ColorEditVec3(const std::string& name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter) -> bool;

        ACDA_API auto ColorEditVec4(const std::string& name, glm::vec4& color) -> bool;
        ACDA_API auto ColorEditVec4(const std::string& name, std::function<glm::vec4()> getter, std::function<void(const glm::vec4&)> setter) -> bool;

        ACDA_API void TextVec3(const std::string& name, const glm::vec3& vec);
        ACDA_API void TextVec4(const std::string& name, const glm::vec4& vec);
        ACDA_API void TextQuat(const std::string& name, const glm::quat& quat);

        ACDA_API void NewLine(float height = -1.0f);

        ACDA_API void HelpMark(const std::string& icon, const std::string& text);
    }
}