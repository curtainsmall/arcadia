#pragma once

#include<vector>

#include"flatbuffers_generated/asset_generated.h"

#include"core/base.hpp"
#include"resource/component/component.hpp"
#include"resource/component/mesh_component/material/material.hpp"
#include"resource/component/mesh_component/vertex.hpp"

namespace arcadia
{
    struct ARCADIA_API mesh_component: arcadia::component
    {
    public:
        using index_type = unsigned int;
        using self_type = mesh_component;
    public:
        std::vector<arcadia::vertex> vertices{};
        std::vector<index_type> indices{};

        arcadia::material material{};
    };
}
