#pragma once

#include"core/base.hpp"
#include"core/math.hpp"

struct Vertex
{
public:
    using self_type = Vertex;
public:
    glm::vec3 coord{ vec3::zero() };
    glm::vec3 normal{ vec3::zero() };
    glm::vec2 tex_coord{ vec2::zero() };
};
