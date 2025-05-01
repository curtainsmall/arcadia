#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

#include "core/exception.hpp"
#include "core/nlohmann_json.hpp"
#include "core/noncopyable.hpp"
#include "resource/scene.hpp"

namespace Arcadia
{
    namespace Exceptions
    {
        ACDA_DEFINE_RUNTIME_ERROR_EXCEPTION(ProjectConstructionFailed);
    }

    class Project: public Noncopyable
    {
    public:
        using SceneStorageType = std::unordered_map<std::string, std::shared_ptr<Scene>>;
        using SelfType = Project;
    public:
        Project(std::string name);
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

        [[nodiscard]]
        auto HasScene() const -> bool;
        [[nodiscard]]
        auto HasScene(const std::string& name) const -> bool;
        [[nodiscard]]
        auto GetScene(const std::string& name) -> Scene&;
        [[nodiscard]]
        auto GetScene(const std::string& name) const -> const Scene&;
        void CreateScene(const std::string& name);
        void DestroyScene(const std::string& name);
        void RenameScene(const std::string& name, const std::string& new_name);

        [[nodiscard]]
        auto GetSceneStorage() const -> const SceneStorageType&;

    public:
        static inline std::string ProjectExtensionString{ ".arcadia" };

    private:
        std::string _Name;
        std::shared_ptr<Scene> _spActiveScene{};
        SceneStorageType _SceneStorage{};
    };
}