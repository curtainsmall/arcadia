#pragma once

#include<vector>

#include"core/base.hpp"
#include"resource/component/component.hpp"
#include"resource/component/renderable_component/mesh/mesh.hpp"

namespace arcadia
{
    struct ARCADIA_API renderable_component: arcadia::component
    {
    public:
        using self_type = renderable_component;
    public:
        std::vector<arcadia::mesh> meshes{};
    };
}
