#pragma once

#include"core/base.hpp"
#include"resource/components/component_interface.hpp"
#include"resource/cubemap.hpp"

namespace Arcadia
{
    class SkyboxComponent: iComponent
    {
    public:
        using SelfType = SkyboxComponent;
    public:
        ACDA_COMPONENT_TYPE_STR_GETTERS("skybox");

        SkyboxComponent() = default;
        ~SkyboxComponent() = default;
    public:
        Cubemap Cubemap{};
    };
}