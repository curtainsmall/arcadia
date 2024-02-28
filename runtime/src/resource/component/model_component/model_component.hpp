#pragma once

#include<vector>

#include"core/base.hpp"
#include"resource/component/component.hpp"
#include"resource/component/model_component/mesh/mesh.hpp"

namespace arcadia
{
    struct ARCADIA_API model_component: arcadia::component
    {
    public:
        using self_type = model_component;
    public:
        std::vector<arcadia::mesh> meshes{};
    };
}
