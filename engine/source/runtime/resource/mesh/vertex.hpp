#pragma once

#include "platform/defines.hpp"
#include "core/math.hpp"

namespace Arcadia
{
    struct ACDA_API Vertex
    {
    public:
        using SelfType = Vertex;
    public:
        Vertex(
            const glm::vec3& coord = Glm::Vec3_CreateZero(),
            const glm::vec3& normal = Glm::Vec3_CreateZero(),
            const glm::vec2& tex_coord = Glm::Vec2_CreateZero()
        ):
            Coordinate(coord), Normal(normal), TextureCoordinate(tex_coord)
        {
        }
    public:
        glm::vec3 Coordinate{ Glm::Vec3_CreateZero() };
        glm::vec3 Normal{ Glm::Vec3_CreateZero() };
        glm::vec2 TextureCoordinate{ Glm::Vec2_CreateZero() };
    };
}