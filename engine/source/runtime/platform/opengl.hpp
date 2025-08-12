#pragma once

#include "GL/gl3w.h"

#include "core/exception.hpp"
#include "core/version.hpp"
#include "platform/defines.hpp"

#ifndef NDEBUG
#   define ACDA_GL_USE_DEBUG_CALLBACK 0
#endif

#if !defined(NDEBUG) && !ACDA_GL_USE_DEBUG_CALLBACK
#   define ACDA_GL_CALL(x) x;GlCheckError(#x,__FILE__,__LINE__)
#else
#   define ACDA_GL_CALL(x) x
#endif

namespace Arcadia
{
    namespace Exceptions
    {
        ACDA_DEFINE_EXCEPTION(GlError);
        ACDA_DEFINE_EXCEPTION(GlInvalid);
    }

    ACDA_API void GlCheckError(const char* fn_name, const char* file_name, int line);

    ACDA_API auto GlGetTypeSize(GLenum Type) -> std::size_t;

    ACDA_API auto GetGlVersion() -> Version;

    ACDA_API void GlDebugCallback(GLenum source,
                                  GLenum Type,
                                  GLuint id,
                                  GLenum severity,
                                  GLsizei length,
                                  const GLchar* message,
                                  const void* user_param);

    ACDA_API auto GetGlMaxCombineTextureImageUnitsCount() -> GLint;

    ACDA_API auto GetGlMaxTextureImageUnitsCount() -> GLint;

    struct ACDA_API OpenglContext
    {
    public:
        using SelfType = OpenglContext;
    public:
        OpenglContext();
        ~OpenglContext() = default;

        OpenglContext(const SelfType&) = delete;
        auto operator=(const SelfType&) -> SelfType & = delete;
    };
}