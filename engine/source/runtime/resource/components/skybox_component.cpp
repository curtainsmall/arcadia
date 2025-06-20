#include "pch.hpp"
#include "skybox_component.hpp"

auto Arcadia::SkyboxComponent::GetCubemap() const -> const Cubemap&
{
    return _Cubemap;
}

void Arcadia::SkyboxComponent::SetCubemap(const Cubemap& cubemap)
{
    _Cubemap = cubemap;
}