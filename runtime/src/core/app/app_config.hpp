#pragma once

#include<filesystem>
#include<functional>
#include<set>

#include"core/base.hpp"
#include"core/file/file.hpp"
#include"core/math.hpp"
#include"platform/graphic_api/graphic_api.hpp"

class AppConfig
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
    glm::ivec2 WindowPosition{ -1,-1 }; // Use negative value for `don't care`
    glm::ivec2 WindowSize{ 1280,768 };
    glm::ivec2 WindowSizeMin{ 800,600 };
    glm::ivec2 WindowSizeMax{ -1,-1 }; // Use negative value for `don't care`
    std::string WindowTitle{ "Arcadia" };
    int WindowMultisampleCount{ 0 };
    bool WindowMaxmized{ true };

    // ImGui
    std::set<std::string> ImguiOpenedWindowIdStrings{};
    float UiScale ={ 1.0f };
};