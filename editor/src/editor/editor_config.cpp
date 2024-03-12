#include "editor_config.hpp"

auto arcadia::editor_config::instance() -> self_type&
{
    static self_type editor_config{};
    return editor_config;
}
