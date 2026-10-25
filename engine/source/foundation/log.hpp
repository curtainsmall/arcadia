#pragma once

#include <cstdio>
#include <format>
#include <string_view>

// Minimal logging for the foundation/app rings. The legacy runtime keeps its own
// spdlog based core/log.hpp; these macros exist so the new rings stay free of
// third-party logging and of the old tree.

namespace Arcadia::Foundation
{
    inline void Log(std::string_view level, std::string_view message)
    {
        std::fprintf(stdout, "[%*.s] %*.s\n", int(level.size()), level.data(), int(message.size()), message.data());
    }
}

#define ACDA_LOG_DEBUG(message)   ::Arcadia::Foundation::Log("debug", (message))
#define ACDA_LOG_INFO(message)    ::Arcadia::Foundation::Log("info",  (message))
#define ACDA_LOG_WARNING(message) ::Arcadia::Foundation::Log("warn",  (message))
#define ACDA_LOG_ERROR(message)   ::Arcadia::Foundation::Log("error", (message))
