#pragma once

#include"core/base.hpp"
#include"resource/component/component.hpp"
#include"resource/component/skybox_component/cubemap.hpp"

namespace arcadia
{
    struct ARCADIA_API skybox_component: arcadia::component_base
    {
    public:
        using self_type = skybox_component;
    public:
        skybox_component() = default;
        ~skybox_component() = default;
    public:
        arcadia::cubemap cubemap{};
    };
}
