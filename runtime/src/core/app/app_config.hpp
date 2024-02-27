#pragma once

#include<filesystem>

#include"core/base.hpp"
#include"core/file/file.hpp"
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
    };
}
