#pragma once

#include<vector>

#include"core/base.hpp"
#include"resource/components/component_interface.hpp"
#include"resource/material/material.hpp"
#include"resource/mesh/vertex.hpp"

struct Mesh
{
public:
    using index_type = unsigned int;
    using self_type = Mesh;
public:
    static auto Box(
        const glm::vec3& half_extent
    ) -> Mesh;

    static auto Capsule(
        float radius,
        float half_height_of_sylinder,
        std::size_t half_sphere_stack_count = 18,
        std::size_t sector_count = 36
    ) -> Mesh;

    static auto Cylinder(
        float half_height,
        float radius,
        std::size_t sector_count = 36
    ) -> Mesh;

    static auto Sphere(
        float radius,
        std::size_t stack_count  = 36, // Along latitude
        std::size_t sector_count = 36  // Alone longitude
    ) -> Mesh;

    std::vector<Vertex> Vertices{};
    std::vector<index_type> Indices{};

    Material Material{};
};
