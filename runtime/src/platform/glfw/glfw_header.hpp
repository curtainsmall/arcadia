#pragma once

#include"GLFW/glfw3.h"

#include"core/base.hpp"
#include"core/exception.hpp"

ARCADIA_EXCEPTION(GlfwError);

struct GlfwContext: Noncopyable
{
public:
    using self_type = GlfwContext;
public:
    GlfwContext();
    ~GlfwContext();
};
