#pragma once

#include"core/base.hpp"
#include"core/math.hpp"

class Vertex
{
public:
    using SelfType = Vertex;
public:
    glm::vec3 Coordinate{ Vec3::CreateZero() };
    glm::vec3 Normal{ Vec3::CreateZero() };
    glm::vec2 TextureCoordinate{ Vec2::CreateZero() };
};