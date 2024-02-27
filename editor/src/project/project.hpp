#pragma once

#include<unordered_map>

#include"resource/scene/scene.hpp"

namespace arcadia
{
    struct ARCADIA_API project: arcadia::noncopyable
    {
    public:
        using self_type = project;
    public:
        std::unordered_map<std::string, arcadia::scene> scene_umap{};
        arcadia::scene* active_scene_ptr{};
    };
}
