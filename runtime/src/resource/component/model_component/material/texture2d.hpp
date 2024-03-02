#pragma once

#include<functional>
#include<vector>

#include"core/base.hpp"
#include"core/exception.hpp"
#include"core/math.hpp"

namespace arcadia
{
    struct ARCADIA_API texture2d
    {
    public:
        ARCADIA_EXCEPTION(incompatible_texture_size);

        using self_type = texture2d;
    public:
        static auto cascade(
            const texture2d& texture2d_1,
            const texture2d& texture2d_2,
            const std::function<glm::vec4(const glm::vec4&, const glm::vec4&)>& method
        ) -> texture2d;
    public:
        std::vector<glm::vec4> pixels{};
        glm::ivec2 size{ 0,0 };
    };
}
