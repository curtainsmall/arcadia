#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

#include "core/exception.hpp"
#include "core/nlohmann_json.hpp"

namespace Arcadia
{
    struct Project
    {
    public:
        using SelfType = Project;
    public:
        Project(std::string_view name);
        Project(nlohmann::json& json);
        ~Project();
        [[nodiscard]]
        auto ToJson() const -> nlohmann::json;

        Project(const SelfType&) = delete;
        auto operator=(const SelfType&) -> SelfType & = delete;

        [[nodiscard]]
        auto GetName() const -> std::string_view;
        void SetName(std::string_view name);

    public:
        static inline const char* ProjectFileExtension{ ".arcadia" };

    private:
        std::string _Name;
    };
}