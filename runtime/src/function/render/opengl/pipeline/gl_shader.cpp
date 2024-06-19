#include "pch.hpp"

#include "gl_shader.hpp"

GlShader::GlShader(const std::string& source_code, GLenum shader_type)
{
    const char* src = source_code.c_str();
    ACDA_GL_CALL(_gl_id = glCreateShader(shader_type));
    ACDA_GL_CALL(glShaderSource(_gl_id, 1, &src, nullptr));

    // Compile
    ACDA_GL_CALL(glCompileShader(_gl_id));

    // Check whether compile success
    GLint compile_status{ 0 };
    ACDA_GL_CALL(glGetShaderiv(_gl_id, GL_COMPILE_STATUS, &compile_status));
    if(compile_status == GL_FALSE)
    {
        GLint length{ 0 };
        ACDA_GL_CALL(glGetShaderiv(_gl_id, GL_INFO_LOG_LENGTH, &length));
        std::string msg{};
        msg.reserve(length);
        ACDA_GL_CALL(glGetShaderInfoLog(_gl_id, length, nullptr, msg.data()));
        ACDA_GL_CALL(glDeleteShader(_gl_id));
        _gl_id = 0;
        throw CompileFail{ msg };
    }

}

GlShader::~GlShader()
{
    ACDA_GL_CALL(glDeleteShader(_gl_id));
}

GlShader::GlShader(self_type&& rhs) noexcept
{
    _gl_id = rhs._gl_id;
    rhs._gl_id = 0;
}

auto GlShader::operator=(self_type&& rhs) noexcept -> self_type&
{
    _gl_id = rhs._gl_id;
    rhs._gl_id = 0;

    return *this;
}
