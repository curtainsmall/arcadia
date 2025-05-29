#pragma once

#include <vector>

#include "platform/api_def.hpp"
#include "core/math.hpp"

namespace Arcadia
{
    struct ACDA_API Cubemap
    {
    public:
        using SelfType = Cubemap;
    public:
        std::vector<glm::vec4> PositiveXFacePixels{};
        std::vector<glm::vec4> NegativeXFacePixels{};
        std::vector<glm::vec4> PositiveYFacePixels{};
        std::vector<glm::vec4> NegativeYFacePixels{};
        std::vector<glm::vec4> PositiveZFacePixels{};
        std::vector<glm::vec4> NegatieZFacePixels{};
        glm::i32vec2 Size{};
    };
}