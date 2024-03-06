#include "editor_context.hpp"

auto arcadia::editor_context::instance() -> self_type&
{
    static self_type editor_context{};
    return editor_context;
}
