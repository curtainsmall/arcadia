#pragma once

#include"core/base.hpp"
#include"resource/material/texture2d.hpp"

namespace Arcadia
{
    struct ARCADIA_API Material
    {
    public:
        using self_type = Material;
    public:
        Arcadia::Texture2d AmbientTexture2d{};
        Arcadia::Texture2d DiffuseTexture2d{};
        Arcadia::Texture2d SpecularTexture2d{};
    };
}
