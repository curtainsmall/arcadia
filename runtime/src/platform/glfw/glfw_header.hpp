#pragma once

#include"GLFW/glfw3.h"

#include"core/base.hpp"
#include"core/exception.hpp"

ACDA_EXCEPTION(GlfwError);

class GlfwContext: public Noncopyable
{
public:
    using SelfType = GlfwContext;
public:
    GlfwContext();
    ~GlfwContext();
};