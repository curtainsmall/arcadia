#include "pch.hpp"

#include "app_config.hpp"

auto Arcadia::AppConfig::Instance() -> SelfType&
{
    static SelfType app_config{};
    return app_config;
}