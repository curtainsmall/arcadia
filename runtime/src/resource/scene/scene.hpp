#pragma once

#include"core/base.hpp"
#include"resource/scene/entt_header.hpp"

namespace arcadia
{
    struct ARCADIA_API scene
    {
    public:
        using self_type = scene;
    public:
        scene();
    private:
        entt::registry _registry{};
    };
}
