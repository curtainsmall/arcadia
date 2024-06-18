#pragma once

#include"GLFW/glfw3.h"

#include"core/base.hpp"
#include"core/exception.hpp"

ACDA_EXCEPTION(GlfwError);

struct GlfwContext: Noncopyable
{
public:
    using self_type = GlfwContext;
public:
    GlfwContext();
    ~GlfwContext();
};
