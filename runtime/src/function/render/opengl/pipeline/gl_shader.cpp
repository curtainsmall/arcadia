#include "pch.hpp"
#include "gl_shader.hpp"

arcadia::gl_shader::gl_shader(const std::string& source_code, GLenum shader_type)
{
    const char* src = source_code.c_str();
    ARCADIA_GL_CALL(_gl_shader_id = glCreateShader(shader_type));
    ARCADIA_GL_CALL(glShaderSource(_gl_shader_id, 1, &src, nullptr));

    // Compile
    ARCADIA_GL_CALL(glCompileShader(_gl_shader_id));

    // Check whether compile success
    GLint compile_status{ 0 };
    ARCADIA_GL_CALL(glGetShaderiv(_gl_shader_id, GL_COMPILE_STATUS, &compile_status));
    if(compile_status == GL_FALSE)
    {
        GLint length{ 0 };
        ARCADIA_GL_CALL(glGetShaderiv(_gl_shader_id, GL_INFO_LOG_LENGTH, &length));
        std::string msg{};
        msg.reserve(length);
        ARCADIA_GL_CALL(glGetShaderInfoLog(_gl_shader_id, length, nullptr, msg.data()));
        ARCADIA_GL_CALL(glDeleteShader(_gl_shader_id));
        _gl_shader_id = 0;
        throw compile_fail{ msg };
    }

}

arcadia::gl_shader::~gl_shader()
{
    ARCADIA_GL_CALL(glDeleteShader(_gl_shader_id));
}

arcadia::gl_shader::gl_shader(self_type&& rhs) noexcept
{
    _gl_shader_id = rhs._gl_shader_id;
    rhs._gl_shader_id = 0;
}

auto arcadia::gl_shader::operator=(self_type&& rhs) noexcept -> self_type&
{
    _gl_shader_id = rhs._gl_shader_id;
    rhs._gl_shader_id = 0;

    return *this;
}
