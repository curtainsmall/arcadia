#pragma once

#include <filesystem>
#include <functional>
#include <set>

#include "platform/api_def.hpp"
#include "core/file.hpp"
#include "core/math.hpp"
#include "platform/graphic_api.hpp"

namespace Arcadia
{
    struct ACDA_API AppConfig
    {
    public:
        using SelfType = AppConfig;
    public:
        static auto Instance() -> SelfType&;
    public:
        static inline std::filesystem::path Filepath{ ToFilepath(".acdacfg") };

        std::filesystem::path WorkingDirectory{ ToFilepath("./") };
        GraphicApi::Type GraphicApi{};

        // Window
        glm::i32vec2 WindowPosition{ -1,-1 }; // Use negative value for `don't care`
        glm::i32vec2 WindowSize{ 1280,768 };
        glm::i32vec2 WindowSizeMin{ 800,600 };
        glm::i32vec2 WindowSizeMax{ -1,-1 }; // Use negative value for `don't care`
        std::string WindowTitle{ "Arcadia" };
        std::int32_t WindowMultisampleCount{ 0 };
        bool WindowMaxmized{ true };

        // ImGui
        std::set<std::string> ImguiOpenedWindowIdStrings{};
        float UiScale ={ 1.0f };
    };
}