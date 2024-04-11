#pragma once

#include"core/base.hpp"
#include"resource/component/component.hpp"
#include"resource/component/skybox_component/cubemap.hpp"

namespace Arcadia
{
    struct ARCADIA_API SkyboxComponent: Arcadia::iComponent
    {
    public:
        using self_type = SkyboxComponent;
    public:
        ARCADIA_COMPONENT_TYPE_STR_GETTERS("Skybox");

        SkyboxComponent() = default;
        ~SkyboxComponent() = default;
    public:
        Arcadia::Cubemap Cubemap{};
    };
}
