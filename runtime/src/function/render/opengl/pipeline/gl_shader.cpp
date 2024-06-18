#include "pch.hpp"

#include "gl_shader.hpp"

GlShader::GlShader(const std::string& source_code, GLenum shader_type)
{
    const char* src = source_code.c_str();
    ARCADIA_GL_CALL(_GlId = glCreateShader(shader_type));
    ARCADIA_GL_CALL(glShaderSource(_GlId, 1, &src, nullptr));

    // Compile
    ARCADIA_GL_CALL(glCompileShader(_GlId));

    // Check whether compile success
    GLint compile_status{ 0 };
    ARCADIA_GL_CALL(glGetShaderiv(_GlId, GL_COMPILE_STATUS, &compile_status));
    if(compile_status == GL_FALSE)
    {
        GLint length{ 0 };
        ARCADIA_GL_CALL(glGetShaderiv(_GlId, GL_INFO_LOG_LENGTH, &length));
        std::string msg{};
        msg.reserve(length);
        ARCADIA_GL_CALL(glGetShaderInfoLog(_GlId, length, nullptr, msg.data()));
        ARCADIA_GL_CALL(glDeleteShader(_GlId));
        _GlId = 0;
        throw CompileFail{ msg };
    }

}

GlShader::~GlShader()
{
    ARCADIA_GL_CALL(glDeleteShader(_GlId));
}

GlShader::GlShader(self_type&& rhs) noexcept
{
    _GlId = rhs._GlId;
    rhs._GlId = 0;
}

auto GlShader::operator=(self_type&& rhs) noexcept -> self_type&
{
    _GlId = rhs._GlId;
    rhs._GlId = 0;

    return *this;
}
