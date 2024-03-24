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

        std::vector<arcadia::vertex> vertices{};
        std::vector<index_type> indices{};

        arcadia::material material{};
    };
}
