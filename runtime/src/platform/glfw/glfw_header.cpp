#include "pch.hpp"

#include "glfw_header.hpp"

#include<format>
#include<string>

Arcadia::GlfwContext::GlfwContext()
{
    if(!glfwInit())
    {
        const char* desr = nullptr;
        auto err_code = glfwGetError(&desr);
        throw Exceptions::GlfwError(std::format("Failed to init GLFW, because {}", desr));
    }

    glfwSetErrorCallback(
        [](int err_type, const char* desr) -> void
    {
        throw Exceptions::GlfwError(std::format("GLFW error[{0}]: {1}", err_type, desr));
    }
    );
}

Arcadia::GlfwContext::~GlfwContext()
{
    glfwTerminate();
}