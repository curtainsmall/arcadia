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
        using SelfType = Project;
    public:
        Project(std::string name);
        Project(nlohmann::json& json);
        ~Project();
        [[nodiscard]]
        auto ToJson() const->nlohmann::json;

        [[nodiscard]]
        auto GetName() const -> const std::string&;
        void SetName(const std::string& name);

    public:
        static inline std::string ProjectExtensionString{ ".arcadia" };

    private:
        std::string _Name;
    };
}