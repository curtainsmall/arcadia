#pragma once

#include<variant>

#include"core/base.hpp"
#include"core/version/version.hpp"

namespace Arcadia
{
    namespace GraphicApi
    {
        struct ARCADIA_API Directx
        {
        public:
            using self_type = Directx;
        public:
            Arcadia::Version version{ 12,0,0 };

        };

        struct ARCADIA_API Opengl
        {
        public:
            using self_type = Opengl;
        public:
            Arcadia::Version version{ 4,6,0 };

        };

        struct ARCADIA_API Vulkan
        {
        public:
            using self_type = Vulkan;
        public:
            Arcadia::Version version{ 1,3,0 };

        };

        using Type = std::variant<
            Arcadia::GraphicApi::Opengl,
            Arcadia::GraphicApi::Directx,
            Arcadia::GraphicApi::Vulkan
        >;



    }
}
