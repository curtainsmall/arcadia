#pragma once

#include<filesystem>
#include<string>
#include<unordered_map>

#include"core/nlohmann_json_header.hpp"
#include"core/noncopyable.hpp"
#include"project/project_events.hpp"
#include"resource/components/camera_component.hpp"
#include"resource/scene.hpp"

namespace Arcadia
{
    class Project: public Noncopyable
    {
    public:
        using SceneStorageType = std::unordered_map<std::string, std::shared_ptr<Scene>>;
        using SelfType = Project;
    public:
        inline Project(
            std::string name
        ):
            _Name(name)
        {}
        Project(nlohmann::json& json);
        ~Project() = default;
        [[nodiscard]]
        auto ToJson() const->nlohmann::json;

        [[nodiscard]]
        auto GetName() const -> const std::string&;
        void SetName(const std::string& name);

        [[nodiscard]]
        auto HasActiveScene() const -> bool;
        [[nodiscard]]
        auto GetActiveScene() -> Scene&;
        [[nodiscard]]
        auto GetActiveScene() const -> const Scene&;
        void SetActiveScene(const std::string& name={});

    public:
        static inline std::string ProjectExtensionString{ ".arcadia" };

        SceneStorageType SceneStorage{};
    private:
        std::string _Name;
        std::shared_ptr<Scene> _spActiveScene{};
    };
}