#pragma once

#include<vector>

#include"flatbuffers_generated/asset_generated.h"

#include"core/base.hpp"
#include"resource/component/component.hpp"
#include"resource/component/model_component/material/material.hpp"
#include"resource/component/model_component/mesh/vertex.hpp"

namespace arcadia
{
    struct ARCADIA_API mesh
    {
    public:
        using index_type = unsigned int;
        using self_type = mesh;
    public:
        static auto box(
            const glm::vec3& half_extent
        ) -> arcadia::mesh;

        static auto capsule(
            float radius,
            float half_height_of_sylinder
        ) -> arcadia::mesh;

        static auto cylinder(
            float half_height,
            float radius
        ) -> arcadia::mesh;

        static auto sphere(
            float radius,
            std::size_t stack_count  = 36, // Along latitude
            std::size_t sector_count = 36  // Alone longitude
        ) -> arcadia::mesh;

        std::vector<arcadia::vertex> vertices{};
        std::vector<index_type> indices{};

        arcadia::material material{};
    };
}
