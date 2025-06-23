
#include "renderer.hpp"

auto Arcadia::RendererInterface::IsActive() const -> bool
{
    return _Active;
}

void Arcadia::RendererInterface::SetActive(bool active)
{
    _Active = active;
}