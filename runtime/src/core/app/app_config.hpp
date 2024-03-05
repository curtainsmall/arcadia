#pragma once

#include<filesystem>
#include<functional>
#include<set>

#include"core/base.hpp"
#include"core/file/file.hpp"
#include"core/math.hpp"
#include"platform/graphic_api/graphic_api.hpp"

namespace arcadia
{
    struct ARCADIA_API app_config
    {
    public:
        using self_type = app_config;
    public:
        static auto instance() -> self_type&;
    public:
        static inline std::filesystem::path filepath{ arcadia::to_filepath(".acdacfg") };

        std::filesystem::path working_directory{ arcadia::to_filepath("./") };
        arcadia::graphic_api::type graphic_api{};

        // Window
        glm::ivec2 window_pos{ -1,-1 }; // Use negative value for `don't care`
        glm::ivec2 window_size{ 1280,768 };
        glm::ivec2 window_min_size{ 800,600 };
        glm::ivec2 window_max_size{ -1,-1 }; // Use negative value for `don't care`
        std::string window_title{ "Arcadia" };
        int window_multisample_count{ 0 };
        bool window_maxmized{ true };

        // ImGui
        std::set<std::string> imgui_opened_window_id_strs{};
    };
}
