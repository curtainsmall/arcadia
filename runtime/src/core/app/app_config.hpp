#pragma once

#include<filesystem>
#include<functional>

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
        std::filesystem::path working_directory{ arcadia::to_filepath("./") };
        arcadia::graphic_api::type graphic_api{};
        glm::ivec2 window_size{ 800,600 };
        std::string window_title{ "Arcadia" };
        int window_multisample_count{ 0 };
    };
}
