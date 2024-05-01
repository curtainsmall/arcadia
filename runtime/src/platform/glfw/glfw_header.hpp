#pragma once

#include"GLFW/glfw3.h"

#include"core/base.hpp"
#include"core/exception.hpp"

namespace Arcadia
{
    ARCADIA_EXCEPTION(GlfwError);

    struct GlfwContext: Arcadia::Noncopyable
    {
    public:
        using self_type = GlfwContext;
    public:
        GlfwContext();
        ~GlfwContext();
    };
}
