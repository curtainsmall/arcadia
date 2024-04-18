#pragma once

#include<filesystem>
#include<string>
#include<unordered_map>

#include"core/nlohmann_json_header.hpp"
#include"project/project_events.hpp"
#include"resource/components/camera_component.hpp"
#include"resource/scene.hpp"

namespace Arcadia
{
    struct ARCADIA_API Project: Arcadia::Noncopyable
    {
    public:
        using self_type = Project;
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
        auto GetActiveScene() -> Arcadia::Scene&;
        [[nodiscard]]
        auto GetActiveScene() const -> const Arcadia::Scene&;
        void SetActiveScene(const std::string& name={});

        void _SnapshotEntities();


    public:
        static inline std::string ProjectExtensionStr{ ".arcadia" };

        std::unordered_map<std::string, std::shared_ptr<Arcadia::Scene>> SceneSptrStorage{};
    private:
        std::string _Name;
        std::shared_ptr<Arcadia::Scene> _ActiveScene{};
    };
}
