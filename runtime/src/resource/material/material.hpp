#pragma once

#include"core/base.hpp"
#include"resource/material/texture2d.hpp"

struct Material
{
public:
    using self_type = Material;
public:
    Texture2d ambient_texture2d{};
    Texture2d diffuse_texture2d{};
    Texture2d specular_texture2d{};
};
