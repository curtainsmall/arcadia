#pragma once

#include<vector>

#include"core/base.hpp"
#include"resource/component/component.hpp"
#include"resource/component/model_component/material/material.hpp"
#include"resource/component/model_component/mesh/vertex.hpp"

namespace Arcadia
{
    struct ARCADIA_API Mesh
    {
    public:
        using index_type = unsigned int;
        using self_type = Mesh;
    public:
        static auto Box(
            const glm::vec3& half_extent
        ) -> Arcadia::Mesh;

        static auto Capsule(
            float radius,
            float half_height_of_sylinder
        ) -> Arcadia::Mesh;

        static auto Cylinder(
            float half_height,
            float radius
        ) -> Arcadia::Mesh;

        static auto Sphere(
            float radius,
            std::size_t stack_count  = 36, // Along latitude
            std::size_t sector_count = 36  // Alone longitude
        ) -> Arcadia::Mesh;

        std::vector<Arcadia::Vertex> Vertices{};
        std::vector<index_type> Indices{};

        Arcadia::Material Material{};
    };
}
