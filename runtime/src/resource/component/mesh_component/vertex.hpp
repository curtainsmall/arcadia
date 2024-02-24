#pragma once

#include"core/base.hpp"
#include"core/math.hpp"

namespace arcadia
{
    struct ARCADIA_API vertex
    {
    public:
        using self_type = vertex;
    public:
        glm::vec3 coord{ arcadia::vec3::create_zero() };
        glm::vec3 normal{ arcadia::vec3::create_zero() };
        glm::vec2 tex_coord{ arcadia::vec2::create_zero() };
    };
}
