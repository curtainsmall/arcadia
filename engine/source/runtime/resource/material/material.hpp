#pragma once

#include "platform/defines.hpp"
#include "resource/material/texture2d.hpp"

namespace Arcadia
{
    struct ACDA_API Material
    {
    public:
        using SelfType = Material;
    public:
        Texture2d AmbientTexture2d{};
        Texture2d DiffuseTexture2d{};
        Texture2d SepcularTexture2d{};
    };
}