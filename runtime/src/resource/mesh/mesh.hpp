#pragma once

#include<vector>

#include"core/base.hpp"
#include"resource/components/component_interface.hpp"
#include"resource/material/material.hpp"
#include"resource/mesh/vertex.hpp"

namespace Arcadia
{
    class Mesh
    {
    public:
        using IndexType = unsigned int;
        using SelfType = Mesh;
    public:
        static auto CreateBox(
            const glm::vec3& half_extent
        ) -> Mesh;

        static auto CreateCapsule(
            float radius,
            float half_height_of_sylinder,
            size_t half_sphere_stack_count = 18,
            size_t sector_count = 36
        ) -> Mesh;

        static auto CreateCylinder(
            float half_height,
            float radius,
            size_t sector_count = 36
        ) -> Mesh;

        static auto CreateSphere(
            float radius,
            size_t stack_count  = 36, // Along latitude
            size_t sector_count = 36  // Alone longitude
        ) -> Mesh;

        std::vector<Vertex> Vertices{};
        std::vector<IndexType> Indices{};

        Material Material{};
    };
}