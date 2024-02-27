#include "pch.hpp"
#include "app_config.hpp"

auto arcadia::app_config::instance() -> self_type&
{
    static self_type app_config{};
    return app_config;
}
