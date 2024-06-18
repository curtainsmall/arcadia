#pragma once

#include"core/base.hpp"
#include"resource/material/texture2d.hpp"

struct Material
{
public:
    using self_type = Material;
public:
    Texture2d AmbientTexture2d{};
    Texture2d DiffuseTexture2d{};
    Texture2d SpecularTexture2d{};
};
