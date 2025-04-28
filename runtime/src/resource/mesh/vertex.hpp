#pragma once

#include "platform/api_def.hpp"
#include "core/math.hpp"

namespace Arcadia
{
    class Vertex
    {
    public:
        using SelfType = Vertex;
    public:
        Vertex(
            const glm::vec3& coord = GlmVec3::CreateZero(),
            const glm::vec3& normal = GlmVec3::CreateZero(),
            const glm::vec2& tex_coord = GlmVec2::CreateZero()
        ):
            Coordinate(coord), Normal(normal), TextureCoordinate(tex_coord)
        {}
    public:
        glm::vec3 Coordinate{ GlmVec3::CreateZero() };
        glm::vec3 Normal{ GlmVec3::CreateZero() };
        glm::vec2 TextureCoordinate{ GlmVec2::CreateZero() };
    };
}