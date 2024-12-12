#include "editor_context.hpp"

auto Arcadia::EditorContext::Instance() -> SelfType&
{
    static SelfType editor_context{};
    return editor_context;
}