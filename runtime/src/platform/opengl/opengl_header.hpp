#pragma once

#include"GL/glew.h"

#include"core/base.hpp"
#include"core/exception.hpp"
#include"core/version/version.hpp"

#ifndef NDEBUG
#   define ARCADIA_GL_USE_DEBUG_CALLBACK 0
#endif

#if !defined(NDEBUG) && !ARCADIA_GL_USE_DEBUG_CALLBACK
#   define ARCADIA_GL_CALL(x) x;Arcadia::GlCheckError(#x,__FILE__,__LINE__)
#else 
#   define ARCADIA_GL_CALL(x) x
#endif

namespace Arcadia
{
    ARCADIA_EXCEPTION(GlError);
    ARCADIA_EXCEPTION(GlInvalid);

    /// @brief Check OpenGL error and throw Arcadia exception if found
    /// @param fn_name Name of the API call that generated error
    /// @param file_name File where the error generated
    /// @param line Line where the error generated
    ARCADIA_API void GlCheckError(const char* fn_name, const char* file_name, int line);

    ARCADIA_API auto GetGlTypeSize(GLenum Type) -> std::size_t;

    ARCADIA_API auto SetGlVersion() -> Arcadia::Version;

    /// @brief Opengl debug callback
    ///
    /// @param source     Source of the error
    /// @param type       Type of the error
    /// @param id         Id of the error
    /// @param severity   Severity level of the error
    /// @param length     Length of the error message
    /// @param message    Pointer to a null-terminate string representing error message
    /// @param user_param User defined external parameter
    ARCADIA_API void GLAPIENTRY GlDebugCallback(GLenum source,
                                                GLenum Type,
                                                GLuint id,
                                                GLenum severity,
                                                GLsizei length,
                                                const GLchar* message,
                                                const void* user_param);

    ARCADIA_API auto GetGlMaxCombinedTextureImageUnitsCount() -> GLint;

    ARCADIA_API auto GetGlMaxTextureImageUnitsCount() -> GLint;

    struct ARCADIA_API OpenglContext: Arcadia::Noncopyable
    {
    public:
        OpenglContext();
        ~OpenglContext() = default;
    };
}

