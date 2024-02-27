#pragma once

#include"GL/glew.h"

#include"core/base.hpp"
#include"core/exception.hpp"
#include"core/version/version.hpp"

#ifndef NDEBUG
#   define ARCADIA_GL_USE_DEBUG_CALLBACK 0
#endif

#if !defined(NDEBUG) && !ARCADIA_GL_USE_DEBUG_CALLBACK
#   define ARCADIA_GL_CALL(x) x;arcadia::gl_check_error(#x,__FILE__,__LINE__)
#else 
#   define ARCADIA_GL_CALL(x) x
#endif

namespace arcadia
{
    ARCADIA_EXCEPTION(gl_error);
    ARCADIA_EXCEPTION(gl_invalid);

    /// @brief Check OpenGL error and throw arcadia exception if found
    /// @param fn_name Name of the API call that generated error
    /// @param file_name File where the error generated
    /// @param line Line where the error generated
    ARCADIA_API void gl_check_error(const char* fn_name, const char* file_name, int line);

    ARCADIA_API auto get_gl_type_size(GLenum type) -> std::size_t;

    ARCADIA_API auto get_gl_version() -> arcadia::version;

    /// @brief Opengl debug callback
    ///
    /// @param source     Source of the error
    /// @param type       Type of the error
    /// @param id         Id of the error
    /// @param severity   Severity level of the error
    /// @param length     Length of the error message
    /// @param message    Pointer to a null-terminate string representing error message
    /// @param user_param User defined external parameter
    ARCADIA_API void GLAPIENTRY gl_debug_callback(GLenum source,
                                                  GLenum type,
                                                  GLuint id,
                                                  GLenum severity,
                                                  GLsizei length,
                                                  const GLchar* message,
                                                  const void* user_param);

    ARCADIA_API auto gl_get_max_combined_texture_image_units_count() -> GLint;

    ARCADIA_API auto gl_get_max_texture_image_units_count() -> GLint;

    struct ARCADIA_API opengl_context: arcadia::noncopyable
    {
    public:
        opengl_context();
        ~opengl_context() = default;
    };
}

