#include "editor_context.hpp"

auto EditorContext::Instance() -> self_type&
{
    static self_type editor_context{};
    return editor_context;
}
