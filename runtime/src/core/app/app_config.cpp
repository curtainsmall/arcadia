#include "pch.hpp"

#include "app_config.hpp"

auto AppConfig::instance() -> self_type&
{
    static self_type app_config{};
    return app_config;
}
