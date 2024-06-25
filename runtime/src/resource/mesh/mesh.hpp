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
    static auto box(
        const glm::vec3& half_extent
    ) -> Mesh;

    static auto capsule(
        float radius,
        float half_height_of_sylinder,
        size_t half_sphere_stack_count = 18,
        size_t sector_count = 36
    ) -> Mesh;

    static auto cylinder(
        float half_height,
        float radius,
        size_t sector_count = 36
    ) -> Mesh;

    static auto sphere(
        float radius,
        size_t stack_count  = 36, // Along latitude
        size_t sector_count = 36  // Alone longitude
    ) -> Mesh;

    std::vector<Vertex> vertices{};
    std::vector<index_type> indices{};

    Material material{};
};
