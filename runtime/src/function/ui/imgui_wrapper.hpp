#pragma once

#include<string>

#include"core/base.hpp"
#include"core/math.hpp"
#include"function/ui/imgui_header.hpp"

namespace Arcadia
{
    namespace ImguiWrapper
    {
        ARCADIA_API auto Checkbox(const std::string& name, bool& b) -> bool;
        ARCADIA_API auto Checkbox(const std::string& name, std::function<bool()> getter, std::function<void(bool)> setter) -> bool;

        ARCADIA_API auto DragInt(const std::string& name, int& i, float speed, int min, int max, const char* format, ImGuiSliderFlags flags) -> bool;
        ARCADIA_API auto DragInt(const std::string& name, std::function<int()> getter, std::function<void(int)> setter, float speed, int min, int max, const char* format, ImGuiSliderFlags flags) -> bool;

        ARCADIA_API auto DragIVec2(const std::string& name, glm::ivec2& vec, float speed, int min, int max, const char* format, ImGuiSliderFlags flags) -> bool;
        ARCADIA_API auto DragIVec2(const std::string& name, std::function<glm::ivec2()> getter, std::function<void(const glm::ivec2&)> setter, float speed, int min, int max, const char* format, ImGuiSliderFlags flags) -> bool;

        ARCADIA_API auto DragIVec3(const std::string& name, glm::ivec3& vec, float speed, int min, int max, const char* format, ImGuiSliderFlags flags) -> bool;
        ARCADIA_API auto DragIVec3(const std::string& name, std::function<glm::ivec3()> getter, std::function<void(const glm::ivec3&)> setter, float speed, int min, int max, const char* format, ImGuiSliderFlags flags) -> bool;

        ARCADIA_API auto DragIVec4(const std::string& name, glm::ivec4& vec, float speed, int min, int max, const char* format, ImGuiSliderFlags flags) -> bool;
        ARCADIA_API auto DragIVec4(const std::string& name, std::function<glm::ivec4()> getter, std::function<void(const glm::ivec4&)> setter, float speed, int min, int max, const char* format, ImGuiSliderFlags flags) -> bool;

        ARCADIA_API auto DragFloat(const std::string& name, float& f, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        ARCADIA_API auto DragFloat(const std::string& name, std::function<float()> getter, std::function<void(float)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;

        ARCADIA_API auto DragVec2(const std::string& name, glm::vec2& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        ARCADIA_API auto DragVec2(const std::string& name, std::function<glm::vec2()> getter, std::function<void(const glm::vec2&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;

        ARCADIA_API auto DragVec3(const std::string& name, glm::vec3& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        ARCADIA_API auto DragVec3(const std::string& name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        ARCADIA_API auto DragVec3Color(const std::string& name, glm::vec3& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        ARCADIA_API auto DragVec3Color(const std::string& name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;

        ARCADIA_API auto DragVec4(const std::string& name, glm::vec4& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        ARCADIA_API auto DragVec4(const std::string& name, std::function<glm::vec4()> getter, std::function<void(const glm::vec4&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;

        ARCADIA_API auto DragQuat(const std::string& name, glm::quat& quat, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        ARCADIA_API auto DragQuat(const std::string& name, std::function<glm::quat()> getter, std::function<void(const glm::quat&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        ARCADIA_API auto DragQuatNormalized(const std::string& name, glm::quat& quat, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;
        ARCADIA_API auto DragQuatNormalized(const std::string& name, std::function<glm::quat()> getter, std::function<void(const glm::quat&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) -> bool;


        ARCADIA_API auto ColorEditVec3(const std::string& name, glm::vec3& color) -> bool;
        ARCADIA_API auto ColorEditVec3(const std::string& name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter) -> bool;

        ARCADIA_API auto ColorEditVec4(const std::string& name, glm::vec4& color) -> bool;
        ARCADIA_API auto ColorEditVec4(const std::string& name, std::function<glm::vec4()> getter, std::function<void(const glm::vec4&)> setter) -> bool;

        ARCADIA_API void TextVec3(const std::string& name, const glm::vec3& vec);
        ARCADIA_API void TextVec4(const std::string& name, const glm::vec4& vec);
        ARCADIA_API void TextQuat(const std::string& name, const glm::quat& quat);

    }
}
