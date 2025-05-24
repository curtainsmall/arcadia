#pragma once

#include <functional>
#include <vector>

#include "core/exception.hpp"
#include "core/math.hpp"
#include "platform/api_def.hpp"

namespace Arcadia
{
    struct Texture2d
    {
    public:

        using SelfType = Texture2d;
    public:
        static auto Cascade(
            const Texture2d& texture2d_1,
            const Texture2d& texture2d_2,
            const std::function<glm::vec4(const glm::vec4&, const glm::vec4&)>& method
        ) -> Texture2d;
    public:
        std::vector<glm::vec4> Pixels{};
        glm::i32vec2 Size{ 0,0 };
    };
}