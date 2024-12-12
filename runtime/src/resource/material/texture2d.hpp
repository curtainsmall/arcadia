#pragma once

#include<functional>
#include<vector>

#include"core/base.hpp"
#include"core/exception.hpp"
#include"core/math.hpp"

class Texture2d
{
public:
    ACDA_EXCEPTION(IncompatibleTextureSize);

    using SelfType = Texture2d;
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