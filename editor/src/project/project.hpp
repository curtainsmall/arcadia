#pragma once

#include<filesystem>
#include<string>
#include<unordered_map>

#include"core/nlohmann_json_header.hpp"
#include"project/project_events.hpp"
#include"resource/component/camera_component/camera_component.hpp"
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
        project(nlohmann::json& json);
        ~project() = default;
        [[nodiscard]]
        auto to_json() const->nlohmann::json;

        [[nodiscard]]
        auto get_name() const -> const std::string&;
        void set_name(const std::string& name);

        [[nodiscard]]
        auto has_active_scene() const -> bool;
        [[nodiscard]]
        auto get_active_scene() -> arcadia::scene&;
        [[nodiscard]]
        auto get_active_scene() const -> const arcadia::scene&;
        auto set_active_scene(const std::string& name={}) -> std::weak_ptr<arcadia::scene>&;


    public:
        std::unordered_map<std::string, std::shared_ptr<arcadia::scene>> scene_sptr_umap{};
        arcadia::camera_component viewport_camera{};
    private:
        std::string _name;
        std::weak_ptr<arcadia::scene> _active_scene_wptr{};
    };
}
