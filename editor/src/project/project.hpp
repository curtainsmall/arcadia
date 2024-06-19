#pragma once

#include<filesystem>
#include<string>
#include<unordered_map>

#include"core/nlohmann_json_header.hpp"
#include"project/project_events.hpp"
#include"resource/components/camera_component.hpp"
#include"resource/scene.hpp"

struct Project: Noncopyable
{
public:
    using self_type = Project;
public:
    inline Project(
        std::string name
    ):
        _name(name)
    {}
    Project(nlohmann::json& json);
    ~Project() = default;
    [[nodiscard]]
    auto to_json() const->nlohmann::json;

    [[nodiscard]]
    auto get_name() const -> const std::string&;
    void set_name(const std::string& name);

    [[nodiscard]]
    auto has_active_scene() const -> bool;
    [[nodiscard]]
    auto get_active_scene() -> Scene&;
    [[nodiscard]]
    auto get_active_scene() const -> const Scene&;
    void set_active_scene(const std::string& name={});

    void _snapshot_entities();
public:
    static inline std::string project_extension_str{ ".arcadia" };

    std::unordered_map<std::string, std::shared_ptr<Scene>> scene_sptr_storage{};
private:
    std::string _name;
    std::shared_ptr<Scene> _active_scene{};
};
