#pragma once

#include <vector>

#include "platform/api_def.hpp"
#include "resource/components/component_interface.hpp"
#include "resource/material/material.hpp"
#include "resource/mesh/vertex.hpp"

namespace Arcadia
{
    struct Mesh
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
            std::size_t half_sphere_stack_count = 18,
            std::size_t sector_count = 36
        ) -> Mesh;

        static auto CreateCylinder(
            float half_height,
            float radius,
            std::size_t sector_count = 36
        ) -> Mesh;

        static auto CreateSphere(
            float radius,
            std::size_t stack_count  = 36, // Along latitude
            std::size_t sector_count = 36  // Alone longitude
        ) -> Mesh;

        std::vector<Vertex> Vertices{};
        std::vector<IndexType> Indices{};

        Material Material{};
    };
}