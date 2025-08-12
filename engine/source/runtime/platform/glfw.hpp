#pragma once

#include "GLFW/glfw3.h"

#include "core/exception.hpp"
#include "platform/defines.hpp"

namespace Arcadia
{
    namespace Exceptions
    {
        ACDA_DEFINE_EXCEPTION(GlfwError);
    }

    struct ACDA_API GlfwContext
    {
    public:
        using SelfType = GlfwContext;
    public:
        GlfwContext();
        ~GlfwContext();

        GlfwContext(const SelfType&) = delete;
        auto operator=(const SelfType&) -> SelfType & = delete;
    };
}