#pragma once

#include <filesystem>
#include <functional>
#include <set>

#include "core/file.hpp"
#include "core/math.hpp"
#include "platform/defines.hpp"
#include "platform/graphic_api.hpp"

namespace Arcadia
{
    struct ACDA_API RuntimeConfig
    {
    public:
        using SelfType = RuntimeConfig;
    public:
        static auto Instance() -> SelfType&;
    public:
        static inline std::filesystem::path Filepath{ ToFilepath(".acdacfg") };

        std::filesystem::path WorkingDirectory{ ACDA_DEFAULT_WORKING_DIR };
        GraphicApi::Type GraphicApi{ GraphicApi::Opengl(Version(4, 6, 0)) };

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
        float UiScale = { 1.0f };
    };
}