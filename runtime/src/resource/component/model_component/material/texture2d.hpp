#pragma once

#include<functional>
#include<vector>

#include"core/base.hpp"
#include"core/exception.hpp"
#include"core/math.hpp"

namespace Arcadia
{
    struct ARCADIA_API Texture2d
    {
    public:
        ARCADIA_EXCEPTION(IncompatibleTextureSize);

        using self_type = Texture2d;
    public:
        static auto Cascade(
            const Texture2d& texture2d_1,
            const Texture2d& texture2d_2,
            const std::function<glm::vec4(const glm::vec4&, const glm::vec4&)>& method
        ) -> Texture2d;
    public:
        std::vector<glm::vec4> Pixels{};
        glm::ivec2 Size{ 0,0 };
    };
}
