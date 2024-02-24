#include "pch.hpp"
#include "glfw_header.hpp"

#include<format>


arcadia::glfw_context::glfw_context()
{
    if(!glfwInit())
    {
        const char* desr{ nullptr };
        auto err_code = glfwGetError(&desr);
        throw arcadia::glfw_error{ std::format("Failed to init GLFW, because {}",desr) };
    }

    glfwSetErrorCallback(
        [](int err_type, const char* desr) -> void
    {
        throw arcadia::glfw_error{ std::format("GLFW error[{0}]: {1}", err_type,desr) };
    }
    );

}

arcadia::glfw_context::~glfw_context()
{
    glfwTerminate();
}
