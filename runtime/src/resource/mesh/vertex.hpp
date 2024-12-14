#pragma once

#include"core/base.hpp"
#include"core/math.hpp"

namespace Arcadia
{
    class Vertex
    {
    public:
        using SelfType = Vertex;
    public:
        glm::vec3 Coordinate{ GlmVec3::CreateZero() };
        glm::vec3 Normal{ GlmVec3::CreateZero() };
        glm::vec2 TextureCoordinate{ GlmVec2::CreateZero() };
    };
}