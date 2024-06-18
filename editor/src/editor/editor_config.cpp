#include "editor_config.hpp"

auto EditorConfig::Instance() -> self_type&
{
    static self_type editor_config{};
    return editor_config;
}
