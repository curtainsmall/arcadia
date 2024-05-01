#pragma once

#include"core/base.hpp"
#include"core/math.hpp"

namespace Arcadia
{
    struct Vertex
    {
    public:
        using self_type = Vertex;
    public:
        glm::vec3 Coord{ Arcadia::Vec3::Zero() };
        glm::vec3 Normal{ Arcadia::Vec3::Zero() };
        glm::vec2 TexCoord{ Arcadia::Vec2::Zero() };
    };
}
