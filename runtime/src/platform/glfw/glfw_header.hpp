#pragma once

#include"GLFW/glfw3.h"

#include"core/exception.hpp"
#include"core/noncopyable.hpp"
#include"platform/api_def.hpp"

namespace Arcadia
{
    namespace Exceptions
    {
        ACDA_DEFINE_RUNTIME_ERROR_EXCEPTION(GlfwError);
    }

    class GlfwContext: public Noncopyable
    {
    public:
        using SelfType = GlfwContext;
    public:
        GlfwContext();
        ~GlfwContext();
    };
}