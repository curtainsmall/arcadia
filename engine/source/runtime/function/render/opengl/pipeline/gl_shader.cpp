#include "pch.hpp"

#include "gl_shader.hpp"

Arcadia::GlShader::GlShader(const std::string& source_code, GlShaderType shader_type)
{
    const char* src = source_code.c_str();
    ACDA_GL_CALL(_GlId = glCreateShader(ToUnderlying(shader_type)));
    ACDA_GL_CALL(glShaderSource(_GlId, 1, &src, nullptr));

    // Compile
    ACDA_GL_CALL(glCompileShader(_GlId));

    // Check whether compile success
    GLint compile_status = 0;
    ACDA_GL_CALL(glGetShaderiv(_GlId, GL_COMPILE_STATUS, &compile_status));
    if(compile_status == GL_FALSE)
    {
        GLint length = 0;
        ACDA_GL_CALL(glGetShaderiv(_GlId, GL_INFO_LOG_LENGTH, &length));
        std::string msg{};
        msg.reserve(length);
        ACDA_GL_CALL(glGetShaderInfoLog(_GlId, length, nullptr, msg.data()));
        ACDA_GL_CALL(glDeleteShader(_GlId));
        _GlId = 0;
        throw Exceptions::GlShaderCompileFail(msg);
    }
}

Arcadia::GlShader::~GlShader()
{
    ACDA_GL_CALL(glDeleteShader(_GlId));
}

Arcadia::GlShader::GlShader(SelfType&& rhs) noexcept
{
    _GlId = rhs._GlId;
    rhs._GlId = 0;
}

auto Arcadia::GlShader::operator=(SelfType&& rhs) noexcept -> SelfType&
{
    _GlId = rhs._GlId;
    rhs._GlId = 0;

    return *this;
}