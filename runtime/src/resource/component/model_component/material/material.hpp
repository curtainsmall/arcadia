#pragma once

#include"core/base.hpp"
#include"resource/component/model_component/material/texture2d.hpp"

namespace arcadia
{
    struct ARCADIA_API material
    {
    public:
        using self_type = material;
    public:
        arcadia::texture2d ambient_texture2d{};
        arcadia::texture2d diffuse_texture2d{};
        arcadia::texture2d specular_texture2d{};
    };
}
