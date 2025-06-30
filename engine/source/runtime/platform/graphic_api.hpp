#pragma once

#include <variant>

#include "platform/defines.hpp"
#include "core/version.hpp"

namespace Arcadia
{
    namespace GraphicApi
    {
        struct ACDA_API Directx
        {
        public:
            using SelfType = Directx;
        public:
            Version Version{ 12,0,0 };
        };

        struct ACDA_API Opengl
        {
        public:
            using SelfType = Opengl;
        public:
            Version Version{ 4,6,0 };
        };

        struct ACDA_API Vulkan
        {
        public:
            using SelfType = Vulkan;
        public:
            Version Version{ 1,3,0 };
        };

        using Type = std::variant<
            GraphicApi::Opengl,
            GraphicApi::Directx,
            GraphicApi::Vulkan
        >;
    }
}