#pragma once

#include <variant>

#include "platform/defines.hpp"
#include "core/version.hpp"
#include "app/runtime_config.hpp"

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

        // Builds the configured api out of the runtime configuration.
        [[nodiscard]]
        inline auto FromConfig(const RuntimeConfig& runtime_config) -> Type
        {
            const Version version{
                runtime_config.GraphicApiMajor,
                runtime_config.GraphicApiMinor,
                runtime_config.GraphicApiPatch
            };

            if(runtime_config.GraphicApi == "directx")
            {
                return Directx{ version };
            }
            if(runtime_config.GraphicApi == "vulkan")
            {
                return Vulkan{ version };
            }
            return Opengl{ version };
        }
    }
}