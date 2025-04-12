#include "pch.hpp"

#include "opengl_header.hpp"

#include"core/log/log.hpp"

ACDA_API void Arcadia::GlCheckError(const char* fn_name, const char* file_name, int line)
{
    while(GLenum error = glGetError())
    {
        std::string err_des{};
        switch(error)
        {
            case GL_INVALID_ENUM:
            {
                err_des = "GL_INVALID_ENUM";
                break;
            }
            case GL_INVALID_VALUE:
            {
                err_des = "GL_INVALID_VALUE";
                break;
            }
            case GL_INVALID_OPERATION:
            {
                err_des = "GL_INVALID_OPERATION";
                break;
            }
            case GL_INVALID_FRAMEBUFFER_OPERATION:
            {
                err_des = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            }
            case GL_OUT_OF_MEMORY:
            {
                err_des = "GL_OUT_OF_MEMORY";
                break;
            }
            case GL_STACK_UNDERFLOW:
            {
                err_des = "GL_STACK_UNDERFLOW";
                break;
            }
            case GL_STACK_OVERFLOW:
            {
                err_des = "GL_STACK_OVERFLOW";
                break;
            }
            default:
            {
                err_des = std::to_string(error);
                break;
            }
        }
        ACDA_LOG_ERROR(std::format("OpenGL ERROR CODE: {0} in {1} at {2}: {3}", err_des, std::string(fn_name), std::string(file_name), std::to_string(line)));
        ACDA_LOG_FLUSH();
    }
}

ACDA_API auto Arcadia::GlGetTypeSize(GLenum Type) -> std::size_t
{
    switch(Type)
    {
        case GL_FLOAT:
            return 4;
        case GL_UNSIGNED_BYTE:
            return 1;
        case GL_UNSIGNED_INT:
            return 4;
        default:
            return 0;
    }
}

ACDA_API auto Arcadia::GetGlVersion() -> Version
{
    int major = 0;
    int minor = 0;
    ACDA_GL_CALL(glGetIntegerv(GL_MAJOR_VERSION, &major));
    ACDA_GL_CALL(glGetIntegerv(GL_MINOR_VERSION, &minor));

    Version version{};

    version.Major = major;
    version.Minor = minor;

    return version;
}

void GLAPIENTRY Arcadia::GlDebugCallback(
    GLenum source,
    GLenum Type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* user_param
)
{
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204)
    {
        return;
    }

    std::string source_string{};
    switch(source)
    {
        case GL_DEBUG_SOURCE_API:
            source_string = "API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            source_string = "Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            source_string = "Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            source_string = "Third Party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            source_string = "Application";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            source_string = "Other";
            break;
    }

    std::string type_string{};
    switch(Type)
    {
        case GL_DEBUG_TYPE_ERROR:
            type_string= "Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            type_string= "Deprecated Behaviour";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            type_string= "Undefined Behaviour";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            type_string= "Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            type_string= "Performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            type_string= "Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            type_string= "Push Group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            type_string= "Pop Group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            type_string= "Other";
            break;
    }

    std::string severity_string{};
    switch(severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            severity_string= "high";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            severity_string= "medium";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            severity_string= "low";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            severity_string= "notification";
            break;
    }

    ACDA_LOG_ERROR(std::format("GL callback\nSource: {0}\nType: {1}\nSeverity: {2}\nMessage: {3}", source_string, type_string, severity_string, message));
    ACDA_LOG_FLUSH();
}

ACDA_API auto Arcadia::GetGlMaxCombineTextureImageUnitsCount() -> GLint
{
    GLint res = 0;
    ACDA_GL_CALL(glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &res));
    return res;
}

ACDA_API auto Arcadia::GetGlMaxTextureImageUnitsCount() -> GLint
{
    GLint res = 0;
    ACDA_GL_CALL(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &res));
    return res;
}

Arcadia::OpenglContext::OpenglContext()
{
    auto error = glewInit();
    if(error != GLEW_OK)
    {
        throw Exceptions::GlError(reinterpret_cast<const char*>(glewGetErrorString(error)));
    }
    ACDA_GL_CALL(std::string gl_version_string(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
    ACDA_LOG_INFO(std::format("OpenGL Version: {}", gl_version_string));

#if ACDA_GL_USE_DEBUG_CALLBACK
    if(GetGlVersion() >= Version(4, 6, 0))
    {
        ACDA_GL_CALL(glEnable(GL_DEBUG_OUTPUT));
        ACDA_GL_CALL(glDebugMessageCallback(GlDebugCallback, nullptr));
    }
#endif

    ACDA_LOG_FLUSH();
}