#include "editor_context.hpp"

auto EditorContext::Instance() -> SelfType&
{
    static SelfType editor_context{};
    return editor_context;
}