#pragma once

#include<variant>

#include"core/base.hpp"
#include"core/version/version.hpp"

namespace graphic_api
{
    struct Directx
    {
    public:
        using self_type = Directx;
    public:
        Version version{ 12,0,0 };
    };

    struct Opengl
    {
    public:
        using self_type = Opengl;
    public:
        Version version{ 4,6,0 };
    };

    struct Vulkan
    {
    public:
        using self_type = Vulkan;
    public:
        Version version{ 1,3,0 };
    };

    using Type = std::variant<
        graphic_api::Opengl,
        graphic_api::Directx,
        graphic_api::Vulkan
    >;
}
