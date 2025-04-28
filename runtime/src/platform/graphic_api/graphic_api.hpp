#pragma once

#include <variant>

#include "platform/api_def.hpp"
#include "core/version/version.hpp"

namespace Arcadia
{
    namespace GraphicApi
    {
        class Directx
        {
        public:
            using SelfType = Directx;
        public:
            Version Version{ 12,0,0 };
        };

        class Opengl
        {
        public:
            using SelfType = Opengl;
        public:
            Version Version{ 4,6,0 };
        };

        class Vulkan
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