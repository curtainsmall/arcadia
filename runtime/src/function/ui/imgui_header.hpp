#pragma once

#include"core/math.hpp"

#define IM_VEC2_CLASS_EXTRA \
    constexpr ImVec2(const glm::vec2& f) : x(f.x), y(f.y) {} \
    operator glm::vec2() const { return glm::vec2(x, y); }\
    constexpr ImVec2(const glm::uvec2& u) : x(static_cast<float>(u.x)), y(static_cast<float>(u.y)) {} \
    operator glm::uvec2() const { return glm::uvec2(static_cast<unsigned int>(x), static_cast<unsigned int>(y)); }\
    constexpr ImVec2(const glm::ivec2& i) : x(static_cast<float>(i.x)), y(static_cast<float>(i.y)) {} \
    operator glm::ivec2() const { return glm::ivec2(static_cast<int>(x), static_cast<int>(y)); }

#define IM_VEC4_CLASS_EXTRA \
    constexpr ImVec4(const glm::vec4& f) : x(f.x), y(f.y), z(f.z), w(f.w) {} \
    operator glm::vec4() const { return glm::vec4(x,y,z,w); }\
    constexpr ImVec4(const glm::uvec4& u) : x(static_cast<float>(u.x)), y(static_cast<float>(u.y)),z(static_cast<float>(u.z)),w(static_cast<float>(u.w)) {} \
    operator glm::uvec4() const { return glm::uvec4(static_cast<unsigned int>(x), static_cast<unsigned int>(y),static_cast<unsigned int>(z), static_cast<unsigned int>(w)); }\
    constexpr ImVec4(const glm::ivec4& i) : x(static_cast<float>(i.x)), y(static_cast<float>(i.y)),z(static_cast<float>(i.z)),w(static_cast<float>(i.w)) {} \
    operator glm::ivec4() const { return glm::ivec4(static_cast<int>(x), static_cast<int>(y),static_cast<unsigned int>(z), static_cast<unsigned int>(w)); }

#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#include"imgui/imgui.h"
#include"imgui/imgui_internal.h"
#include"imgui/misc/cpp/imgui_stdlib.h"
