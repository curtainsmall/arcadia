#pragma once

#include<vector>

#include"core/base.hpp"
#include"core/math.hpp"

namespace arcadia
{
    struct ARCADIA_API cubemap
    {
    public:
        using self_type = cubemap;
    public:
        std::vector<glm::vec4> pos_x_pixels{};
        std::vector<glm::vec4> neg_x_pixels{};
        std::vector<glm::vec4> pos_y_pixels{};
        std::vector<glm::vec4> neg_y_pixels{};
        std::vector<glm::vec4> pos_z_pixels{};
        std::vector<glm::vec4> neg_z_pixels{};
        glm::ivec2 size{};
    };
}
