#pragma once

#include <string>

#include "core/math.hpp"
#include "resource/fonts/icon.hpp"

#include "ui/imgui.hpp"

namespace Arcadia
{
    namespace ImguiWrappers
    {
        auto Checkbox(std::string_view name, bool& b) -> bool;
        auto Checkbox(std::string_view name, std::function<bool()> getter, std::function<void(bool)> setter) -> bool;

        auto DragInt(std::string_view name, std::int32_t& i, float speed, std::int32_t min, std::int32_t max, const char* format, ImGuiSliderFlags flags) -> bool;
        auto DragInt(std::string_view name, std::function<std::int32_t()> getter, std::function<void(std::int32_t)> setter, float speed, std::int32_t min, std::int32_t max, const char* format, ImGuiSliderFlags flags) -> bool;

        auto DragInt32Vec2(std::string_view name, glm::i32vec2& vec, float speed, std::int32_t min, std::int32_t max, const char* format, ImGuiSliderFlags flags) -> bool;
        auto DragInt32Vec2(std::string_view name, std::function<glm::i32vec2()> getter, std::function<void(const glm::i32vec2&)> setter, float speed, std::int32_t min, std::int32_t max, const char* format, ImGuiSliderFlags flags) -> bool;

        auto DragInt32Vec3(std::string_view name, glm::i32vec3& vec, float speed, std::int32_t min, std::int32_t max, const char* format, ImGuiSliderFlags flags) -> bool;
        auto DragInt32Vec3(std::string_view name, std::function<glm::i32vec3()> getter, std::function<void(const glm::i32vec3&)> setter, float speed, std::int32_t min, std::int32_t max, const char* format, ImGuiSliderFlags flags) -> bool;

        auto DragInt32Vec4(std::string_view name, glm::i32vec4& vec, float speed, std::int32_t min, std::int32_t max, const char* format, ImGuiSliderFlags flags) -> bool;
        auto DragInt32Vec4(std::string_view name, std::function<glm::i32vec4()> getter, std::function<void(const glm::i32vec4&)> setter, float speed, std::int32_t min, std::int32_t max, const char* format, ImGuiSliderFlags flags) -> bool;

        auto DragFloat(std::string_view name, float& f, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        auto DragFloat(std::string_view name, std::function<float()> getter, std::function<void(float)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;

        auto DragFloat3(std::string_view name, glm::vec3& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        auto DragFloat3(std::string_view name, std::function<glm::vec3()> getter, std::function<void(glm::vec3)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;

        auto DragVec2(std::string_view name, glm::vec2& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        auto DragVec2(std::string_view name, std::function<glm::vec2()> getter, std::function<void(const glm::vec2&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;

        auto DragVec3(std::string_view name, glm::vec3& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        auto DragVec3(std::string_view name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        auto DragVec3Normalized(std::string_view name, glm::vec3& vec, float speed, const char* format, ImGuiSliderFlags flags) -> bool;
        auto DragVec3Normalized(std::string_view name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter, float speed, const char* format, ImGuiSliderFlags flags) -> bool;
        auto DragVec3Color(std::string_view name, glm::vec3& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        auto DragVec3Color(std::string_view name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;

        auto DragVec4(std::string_view name, glm::vec4& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        auto DragVec4(std::string_view name, std::function<glm::vec4()> getter, std::function<void(const glm::vec4&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;

        auto DragQuat(std::string_view name, glm::quat& quat, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        auto DragQuat(std::string_view name, std::function<glm::quat()> getter, std::function<void(const glm::quat&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        auto DragQuatNormalized(std::string_view name, glm::quat& quat, float speed, const char* format, ImGuiSliderFlags flags) -> bool;
        auto DragQuatNormalized(std::string_view name, std::function<glm::quat()> getter, std::function<void(const glm::quat&)> setter, float speed, const char* format, ImGuiSliderFlags flags) -> bool;

        auto ColorEditVec3(std::string_view name, glm::vec3& color) -> bool;
        auto ColorEditVec3(std::string_view name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter) -> bool;

        auto ColorEditVec4(std::string_view name, glm::vec4& color) -> bool;
        auto ColorEditVec4(std::string_view name, std::function<glm::vec4()> getter, std::function<void(const glm::vec4&)> setter) -> bool;

        void TextVec3(std::string_view name, const glm::vec3& vec);
        void TextVec4(std::string_view name, const glm::vec4& vec);
        void TextQuat(std::string_view name, const glm::quat& quat);

        void NewLine(float height = -1.0f);

        void HelpMark(std::string_view text, std::string_view icon = ICON_FA_QUESTION);
    }
}