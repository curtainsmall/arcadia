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
    static auto instance() -> self_type&;
public:
    static inline std::filesystem::path filepath{ to_filepath(".acdacfg") };

    std::filesystem::path working_directory{ to_filepath("./") };
    graphic_api::Type graphic_api{};

    // Window
    glm::ivec2 window_pos{ -1,-1 }; // Use negative value for `don't care`
    glm::ivec2 window_size{ 1280,768 };
    glm::ivec2 window_size_min{ 800,600 };
    glm::ivec2 window_size_max{ -1,-1 }; // Use negative value for `don't care`
    std::string window_title{ "Arcadia" };
    int window_multisample_count{ 0 };
    bool window_maxmized{ true };

    // ImGui
    std::set<std::string> imgui_opened_window_id_strs{};
    float ui_scale ={ 1.0f };
};