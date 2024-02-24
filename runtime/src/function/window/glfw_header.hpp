#pragma once

#include"GLFW/glfw3.h"

#include"core/base.hpp"
#include"core/exception.hpp"

namespace arcadia
{
    ARCADIA_EXCEPTION(glfw_error);

    struct ARCADIA_API glfw_context: arcadia::noncopyable
    {
    public:
        using self_type = glfw_context;
    public:
        glfw_context();
        ~glfw_context();
    };
}
