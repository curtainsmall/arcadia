#pragma once

#include"core/base.hpp"

namespace arcadia
{
    struct ARCADIA_API editor_config
    {
    public:
        using self_type = editor_config;
    public:
        static auto instance() -> self_type&;
    public:
    };
}
