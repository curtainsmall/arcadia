#pragma once

#include"core/base.hpp"
#include"resource/material/texture2d.hpp"

class Material
{
public:
    using SelfType = Material;
public:
    Texture2d AmbientTexture2d{};
    Texture2d DiffuseTexture2d{};
    Texture2d SepcularTexture2d{};
};