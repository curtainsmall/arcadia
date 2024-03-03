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
            _name(name)
        {}
        ~project() = default;

        [[nodiscard]]
        inline auto get_name() const -> const std::string&
        {
            return _name;
        }
        inline auto set_name(const std::string& name) -> self_type&
        {
            _name = name;
            _modified = true;
            return *this;
        }

        [[nodiscard]]
        auto is_modified() const -> bool;

    public:
        std::unordered_map<std::string, arcadia::scene> scene_umap{};
        arcadia::scene* active_scene_ptr{};
    private:
        bool _modified{ true };
        std::string _name;
    };
}
