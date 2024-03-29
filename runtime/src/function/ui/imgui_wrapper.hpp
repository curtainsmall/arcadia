#pragma once

#include<string>

#include"core/base.hpp"
#include"core/math.hpp"
#include"function/ui/imgui_header.hpp"

namespace arcadia
{
    namespace imgui_wrapper
    {
        ARCADIA_API void checkbox(const std::string& name, bool& b);
        ARCADIA_API void checkbox(const std::string& name, std::function<bool()> getter, std::function<void(bool)> setter);

        ARCADIA_API void drag_int(const std::string& name, int& i, float speed, float min, float max, const char* format, ImGuiSliderFlags flags);
        ARCADIA_API void drag_int(const std::string& name, std::function<int()> getter, std::function<void(int)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags);

        ARCADIA_API void drag_ivec2(const std::string& name, glm::ivec2& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags);
        ARCADIA_API void drag_ivec2(const std::string& name, std::function<glm::ivec2()> getter, std::function<void(const glm::ivec2&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags);

        ARCADIA_API void drag_ivec3(const std::string& name, glm::ivec3& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags);
        ARCADIA_API void drag_ivec3(const std::string& name, std::function<glm::ivec3()> getter, std::function<void(const glm::ivec3&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags);

        ARCADIA_API void drag_ivec4(const std::string& name, glm::ivec4& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags);
        ARCADIA_API void drag_ivec4(const std::string& name, std::function<glm::ivec4()> getter, std::function<void(const glm::ivec4&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags);

        ARCADIA_API void drag_float(const std::string& name, float& f, float speed, float min, float max, const char* format, ImGuiSliderFlags flags);
        ARCADIA_API void drag_float(const std::string& name, std::function<float()> getter, std::function<void(float)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags);

        ARCADIA_API void drag_vec2(const std::string& name, glm::vec2& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags);
        ARCADIA_API void drag_vec2(const std::string& name, std::function<glm::vec2()> getter, std::function<void(const glm::vec2&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags);

        ARCADIA_API void drag_vec3(const std::string& name, glm::vec3& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags);
        ARCADIA_API void drag_vec3(const std::string& name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags);
        ARCADIA_API void drag_vec3_color(const std::string& name, glm::vec3& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags);
        ARCADIA_API void drag_vec3_color(const std::string& name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags);

        ARCADIA_API void drag_vec4(const std::string& name, glm::vec4& vec, float speed, float min, float max, const char* format, ImGuiSliderFlags flags);
        ARCADIA_API void drag_vec4(const std::string& name, std::function<glm::vec4()> getter, std::function<void(const glm::vec4&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags);

        ARCADIA_API void drag_quat(const std::string& name, glm::quat& quat, float speed, float min, float max, const char* format, ImGuiSliderFlags flags);
        ARCADIA_API void drag_quat(const std::string& name, std::function<glm::quat()> getter, std::function<void(const glm::quat&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags);
        ARCADIA_API void drag_quat_normalized(const std::string& name, glm::quat& quat, float speed, float min, float max, const char* format, ImGuiSliderFlags flags);
        ARCADIA_API void drag_quat_normalized(const std::string& name, std::function<glm::quat()> getter, std::function<void(const glm::quat&)> setter, float speed, float min, float max, const char* format, ImGuiSliderFlags flags);


        ARCADIA_API void color_edit3(const std::string& name, glm::vec3& color);
        ARCADIA_API void color_edit3(const std::string& name, std::function<glm::vec3()> getter, std::function<void(const glm::vec3&)> setter);

        ARCADIA_API void color_edit4(const std::string& name, glm::vec4& color);
        ARCADIA_API void color_edit4(const std::string& name, std::function<glm::vec4()> getter, std::function<void(const glm::vec4&)> setter);

        ARCADIA_API void text_vec3(const std::string& name, const glm::vec3& vec);
        ARCADIA_API void text_vec4(const std::string& name, const glm::vec4& vec);
        ARCADIA_API void text_quat(const std::string& name, const glm::quat& quat);

    }
}
