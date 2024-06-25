#pragma once

#include"GL/glew.h"

#include"core/base.hpp"
#include"core/exception.hpp"
#include"core/version/version.hpp"

#ifndef NDEBUG
#   define ACDA_GL_USE_DEBUG_CALLBACK 0
#endif

#if !defined(NDEBUG) && !ACDA_GL_USE_DEBUG_CALLBACK
#   define ACDA_GL_CALL(x) x;gl_check_error(#x,__FILE__,__LINE__)
#else 
#   define ACDA_GL_CALL(x) x
#endif

ACDA_EXCEPTION(GlError);
ACDA_EXCEPTION(GlInvalid);

/// @brief Check OpenGL error and throw Arcadia exception if found
/// @param fn_name Name of the API call that generated error
/// @param file_name File where the error generated
/// @param line Line where the error generated
ACDA_API void gl_check_error(const char* fn_name, const char* file_name, int line);

ACDA_API auto gl_sizeof(GLenum Type) -> size_t;

ACDA_API auto set_gl_version() -> Version;

/// @brief Opengl debug callback
///
/// @param source     Source of the error
/// @param type       Type of the error
/// @param id         Id of the error
/// @param severity   Severity level of the error
/// @param length     Length of the error message
/// @param message    Pointer to a null-terminate string representing error message
/// @param user_param User defined external parameter
ACDA_API void GLAPIENTRY gl_debug_callback(GLenum source,
                                           GLenum Type,
                                           GLuint id,
                                           GLenum severity,
                                           GLsizei length,
                                           const GLchar* message,
                                           const void* user_param);

ACDA_API auto get_gl_max_combine_texture_image_units_count() -> GLint;

ACDA_API auto get_gl_max_texture_image_units_count() -> GLint;

struct OpenglContext: Noncopyable
{
public:
    OpenglContext();
    ~OpenglContext() = default;
};
