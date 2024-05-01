#pragma once

#include"core/base.hpp"

namespace Arcadia
{
    struct EditorConfig
    {
    public:
        using self_type = EditorConfig;
    public:
        static auto Instance() -> self_type&;
    public:
    };
}
