#pragma once

#include "GLFW/glfw3.h"

#include "core/exception.hpp"
#include "core/noncopyable.hpp"
#include "platform/defines.hpp"

namespace Arcadia
{
    namespace Exceptions
    {
        ACDA_DEFINE_EXCEPTION(GlfwError);
    }

    struct ACDA_API GlfwContext: public Noncopyable
    {
    public:
        using SelfType = GlfwContext;
    public:
        GlfwContext();
        ~GlfwContext();
    };
}