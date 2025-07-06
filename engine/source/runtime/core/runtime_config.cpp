#include "runtime_config.hpp"

auto Arcadia::RuntimeConfig::Instance() -> SelfType&
{
    static SelfType app_config{};
    return app_config;
}