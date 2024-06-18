#pragma once

#include"core/base.hpp"

struct EditorConfig
{
public:
    using self_type = EditorConfig;
public:
    static auto Instance() -> self_type&;
public:
};
