#include "pch.hpp"

#include "glfw_header.hpp"

#include<format>


Arcadia::GlfwContext::GlfwContext()
{
    if(!glfwInit())
    {
        const char* desr{ nullptr };
        auto err_code = glfwGetError(&desr);
        throw Arcadia::GlfwError{ std::format("Failed to init GLFW, because {}",desr) };
    }

    glfwSetErrorCallback(
        [](int err_type, const char* desr) -> void
    {
        throw Arcadia::GlfwError{ std::format("GLFW error[{0}]: {1}", err_type,desr) };
    }
    );

}

Arcadia::GlfwContext::~GlfwContext()
{
    glfwTerminate();
}
