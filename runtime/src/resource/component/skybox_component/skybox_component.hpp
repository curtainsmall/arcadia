#pragma once

#include"core/base.hpp"
#include"resource/component/component.hpp"
#include"resource/component/skybox_component/cubemap.hpp"

namespace arcadia
{
    struct ARCADIA_API skybox_component: arcadia::component
    {
    public:
        using self_type = skybox_component;
    public:
        arcadia::cubemap cubemap{};
    };
}
