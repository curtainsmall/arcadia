#pragma once

#include <cstdint>
#include <filesystem>
#include <set>
#include <string>

#include <nlohmann/json.hpp>

// Runtime configuration, owned by Engine. The JSON layout of `.acdacfg` is
// unchanged, so existing configuration files keep working.

namespace Arcadia
{
    struct RuntimeConfig
    {
    public:
        static inline const char* FileExtension{ ".acdacfg" };
        static inline const char* DefaultWorkingDirectory{ "." };

    public:
        std::filesystem::path WorkingDirectory{ DefaultWorkingDirectory };
        std::string GraphicApi{ "opengl" };
        std::int32_t GraphicApiMajor{ 4 };
        std::int32_t GraphicApiMinor{ 6 };
        std::int32_t GraphicApiPatch{ 0 };

        std::int32_t WindowPositionX{ -1 };
        std::int32_t WindowPositionY{ -1 };
        std::int32_t WindowWidth{ 1280 };
        std::int32_t WindowHeight{ 768 };
        bool WindowMaximized{ true };
        std::int32_t WindowMinWidth{ 800 };
        std::int32_t WindowMinHeight{ 600 };
        std::int32_t WindowMaxWidth{ -1 };
        std::int32_t WindowMaxHeight{ -1 };
        std::string WindowTitle{ "Arcadia" };
        std::int32_t MultisampleCount{ 0 };

        float UiScale{ 1.f };
        std::set<std::string> OpenedWindowIds{};

    public:
        // Never throws: an unreadable or missing file keeps the defaults.
        void Load(const std::filesystem::path& filepath);
        void Save(const std::filesystem::path& filepath) const;

        [[nodiscard]] auto ToJson() const -> nlohmann::json;
        void FromJson(const nlohmann::json& json);
    };
}


