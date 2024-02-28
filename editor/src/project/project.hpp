#pragma once

#include<filesystem>
#include<string>
#include<unordered_map>

#include"resource/scene/scene.hpp"

namespace arcadia
{
    struct ARCADIA_API project: arcadia::noncopyable
    {
    public:
        using self_type = project;
    public:
        inline project(
            std::string name
        ):
            name(name)
        {}
        ~project() = default;
    public:
        std::string name;
        std::unordered_map<std::string, arcadia::scene> scene_umap{};
        arcadia::scene* active_scene_ptr{};
    };
}

namespace std
{
    template<>
    struct hash<arcadia::project>
    {
        auto operator()(const arcadia::project& project) const noexcept -> std::size_t
        {
            return 0;
        }
    };
}
