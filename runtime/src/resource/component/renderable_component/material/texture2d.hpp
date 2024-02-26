#pragma once

#include<vector>

#include"core/base.hpp"
#include"core/math.hpp"

namespace arcadia
{
    struct ARCADIA_API texture2d
    {
    public:
        using self_type = texture2d;
    public:
        std::vector<glm::vec4> pixels{};
        glm::uvec2 size{ 0,0 };
    };
}
