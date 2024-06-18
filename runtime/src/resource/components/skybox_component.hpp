#pragma once

#include"core/base.hpp"
#include"resource/components/component_interface.hpp"
#include"resource/cubemap.hpp"

struct SkyboxComponent: iComponent
{
public:
    using self_type = SkyboxComponent;
public:
    ACDA_COMPONENT_TYPE_STR_GETTERS("Skybox");

    SkyboxComponent() = default;
    ~SkyboxComponent() = default;
public:
    Cubemap Cubemap{};
};
