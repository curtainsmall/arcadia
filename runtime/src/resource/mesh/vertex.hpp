#pragma once

#include"core/base.hpp"
#include"core/math.hpp"

struct Vertex
{
public:
    using self_type = Vertex;
public:
    glm::vec3 Coord{ Vec3::Zero() };
    glm::vec3 Normal{ Vec3::Zero() };
    glm::vec2 TexCoord{ Vec2::Zero() };
};
