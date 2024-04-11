#include "editor_config.hpp"

auto Arcadia::EditorConfig::Instance() -> self_type&
{
    static self_type editor_config{};
    return editor_config;
}
