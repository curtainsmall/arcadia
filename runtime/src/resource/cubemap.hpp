#pragma once

#include<vector>

#include"core/base.hpp"
#include"core/math.hpp"

namespace Arcadia
{
    struct Cubemap
    {
    public:
        using self_type = Cubemap;
    public:
        std::vector<glm::vec4> PosXPixels{};
        std::vector<glm::vec4> NegXPixels{};
        std::vector<glm::vec4> PosYPixels{};
        std::vector<glm::vec4> NegYPixels{};
        std::vector<glm::vec4> PosZPixels{};
        std::vector<glm::vec4> NegZPixels{};
        glm::ivec2 Size{};
    };
}
