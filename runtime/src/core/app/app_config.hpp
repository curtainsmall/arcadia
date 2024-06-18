#pragma once

#include<filesystem>
#include<functional>
#include<set>

#include"core/base.hpp"
#include"core/file/file.hpp"
#include"core/math.hpp"
#include"platform/graphic_api/graphic_api.hpp"

struct AppConfig
{
public:
    using self_type = AppConfig;
public:
    static auto Instance() -> self_type&;
public:
    static inline std::filesystem::path Filepath{ ToFilepath(".acdacfg") };

    std::filesystem::path WorkingDirectory{ ToFilepath("./") };
    GraphicApi::Type GraphicApi{};

    // Window
    glm::ivec2 WindowPos{ -1,-1 }; // Use negative value for `don't care`
    glm::ivec2 WindowSize{ 1280,768 };
    glm::ivec2 WindowMinSize{ 800,600 };
    glm::ivec2 WindowMaxSize{ -1,-1 }; // Use negative value for `don't care`
    std::string WindowTitle{ "Arcadia" };
    int WindowMultisampleCount{ 0 };
    bool WindowMaxmized{ true };

    // ImGui
    std::set<std::string> ImguiOpenedWindowIdStrs{};
};
