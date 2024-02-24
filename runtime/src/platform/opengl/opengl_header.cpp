#include "pch.hpp"
#include "opengl_header.hpp"

#include"core/log/log.hpp"

ARCADIA_API void arcadia::gl_check_error(const char* fn_name, const char* file_name, int line)
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
        arcadia::log::error(std::format("OpenGL ERROR CODE: {0} in {1} at {2}: {3}", err_des, std::string(fn_name), std::string(file_name), std::to_string(line)));
        arcadia::log::flush();
        //ASSERT_STATIC(false);
    }
}

ARCADIA_API auto arcadia::get_gl_type_size(GLenum type) -> std::size_t
{
    switch(type)
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

ARCADIA_API auto arcadia::get_gl_version() -> arcadia::version
{
    int
        major{ 0 },
        minor{ 0 };
    ARCADIA_GL_CALL(glGetIntegerv(GL_MAJOR_VERSION, &major));
    ARCADIA_GL_CALL(glGetIntegerv(GL_MINOR_VERSION, &minor));

    arcadia::version version{};

    version.major = major;
    version.minor = minor;

    return version;
}

void GLAPIENTRY arcadia::gl_debug_callback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* user_param
)
{
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::string source_str{};
    switch(source)
    {
        case GL_DEBUG_SOURCE_API:
            source_str = "API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            source_str = "Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            source_str = "Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            source_str = "Third Party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            source_str = "Application";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            source_str = "Other";
            break;
    }

    std::string type_str{};
    switch(type)
    {
        case GL_DEBUG_TYPE_ERROR:
            type_str = "Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            type_str = "Deprecated Behaviour";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            type_str = "Undefined Behaviour";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            type_str = "Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            type_str = "Performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            type_str = "Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            type_str = "Push Group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            type_str = "Pop Group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            type_str = "Other";
            break;
    }

    std::string severity_str{};
    switch(severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            severity_str = "high";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            severity_str = "medium";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            severity_str = "low";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            severity_str = "notification";
            break;
    }

    arcadia::log::error(std::format("GL callback\nSource: {0}\nType: {1}\nSeverity: {2}\nMessage: {3}", source_str, type_str, severity_str, message));
    arcadia::log::flush();
}

ARCADIA_API auto arcadia::gl_get_max_combined_texture_image_units_count() -> GLint
{
    GLint res{ 0 };
    ARCADIA_GL_CALL(glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &res));
    return res;
}

ARCADIA_API auto arcadia::gl_get_max_texture_image_units_count() -> GLint
{
    GLint res{ 0 };
    ARCADIA_GL_CALL(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &res));
    return res;
}

arcadia::opengl_context::opengl_context()
{
    auto error = glewInit();
    if(error != GLEW_OK)
    {
        throw arcadia::gl_error{ reinterpret_cast<const char*>(glewGetErrorString(error)) };
    }
    ARCADIA_GL_CALL(auto gl_version_str = glGetString(GL_VERSION));
    arcadia::log::info(std::format("OpenGL Version: {}", reinterpret_cast<const char*>(gl_version_str)));

#ifdef ARCADIA_GL_USE_DEBUG_CALLBACK
    if(arcadia::get_gl_version() >= arcadia::version{ 4,6,0 })
    {
        ARCADIA_GL_CALL(glEnable(GL_DEBUG_OUTPUT));
        ARCADIA_GL_CALL(glDebugMessageCallback(arcadia::gl_debug_callback, nullptr));
    }
#endif

    arcadia::log::flush();

    ARCADIA_GL_CALL(glEnable(GL_DEPTH_TEST));
    ARCADIA_GL_CALL(glEnable(GL_MULTISAMPLE));
    ARCADIA_GL_CALL(glClearColor(.2f, .3, .3f, 1.f));
}
