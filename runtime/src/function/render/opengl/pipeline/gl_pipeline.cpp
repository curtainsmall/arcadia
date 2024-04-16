#include "pch.hpp"
#include "gl_pipeline.hpp"

Arcadia::GlPipeline::GlPipeline(
    const std::filesystem::path& gl_shader_folder_path,
    const gl_shaders_builder_type& gl_shaders_builder
)
{
    ARCADIA_GL_CALL(_GlId = glCreateProgram());

    // Build Shaders
    gl_shaders_builder(gl_shader_folder_path, _GlShaders);

    // Build pipeline
    for(const auto& shader : _GlShaders)
    {
        ARCADIA_GL_CALL(glAttachShader(_GlId, shader.GetGlId()));
    }

    GLint status{ GL_FALSE };
    ARCADIA_GL_CALL(glLinkProgram(_GlId));
    ARCADIA_GL_CALL(glGetProgramiv(_GlId, GL_LINK_STATUS, &status));
    if(status == GL_FALSE)
    {
        GLint length{ 0 };
        ARCADIA_GL_CALL(glGetProgramiv(_GlId, GL_INFO_LOG_LENGTH, &length));
        std::string msg{};
        msg.reserve(length);
        ARCADIA_GL_CALL(glGetProgramInfoLog(_GlId, length, nullptr, msg.data()));
        ARCADIA_GL_CALL(glDeleteProgram(_GlId));
        _GlId = 0;
        throw LinkFail{ msg };
    }

    ARCADIA_GL_CALL(glValidateProgram(_GlId));
    ARCADIA_GL_CALL(glGetProgramiv(_GlId, GL_VALIDATE_STATUS, &status));
    if(status == GL_FALSE)
    {
        GLint length{ 0 };
        ARCADIA_GL_CALL(glGetProgramiv(_GlId, GL_INFO_LOG_LENGTH, &length));
        std::string msg{};
        msg.reserve(length);
        ARCADIA_GL_CALL(glGetProgramInfoLog(_GlId, length, nullptr, msg.data()));
        ARCADIA_GL_CALL(glDeleteProgram(_GlId));
        _GlId = 0;
        throw LinkFail{ msg };
    }


}

Arcadia::GlPipeline::~GlPipeline()
{
    ARCADIA_GL_CALL(glDeleteProgram(_GlId));
}

Arcadia::GlPipeline::GlPipeline(self_type&& rhs) noexcept:
    _GlUniformLocationCache(std::move(rhs._GlUniformLocationCache)),
    _GlShaders(std::move(rhs._GlShaders))
{
    _GlId = rhs._GlId;
    rhs._GlId = 0;
}

auto Arcadia::GlPipeline::operator=(self_type&& rhs) noexcept -> self_type&
{
    _GlUniformLocationCache = std::move(rhs._GlUniformLocationCache);
    _GlShaders = std::move(rhs._GlShaders);

    _GlId = rhs._GlId;
    rhs._GlId = 0;

    return *this;
}

void Arcadia::GlPipeline::Use() const
{
    if(_GlId == 0)
    {
        throw Arcadia::GlInvalid{ "Cannot use null OpenGL pipeline" };
    }
    ARCADIA_GL_CALL(glUseProgram(_GlId));
}

void Arcadia::GlPipeline::Unuse() const
{
    ARCADIA_GL_CALL(glUseProgram(0));
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, GLfloat f0, GLfloat f1, GLfloat f2, GLfloat f3) -> self_type&
{
    ARCADIA_GL_CALL(glUniform4f(_GetUniformLocation(name), f0, f1, f2, f3));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, GLfloat f0, GLfloat f1, GLfloat f2) -> self_type&
{
    ARCADIA_GL_CALL(glUniform3f(_GetUniformLocation(name), f0, f1, f2));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, GLfloat f0, GLfloat f1) -> self_type&
{
    ARCADIA_GL_CALL(glUniform2f(_GetUniformLocation(name), f0, f1));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, GLfloat f) -> self_type&
{
    ARCADIA_GL_CALL(glUniform1f(_GetUniformLocation(name), f));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, GLint i0, GLint i1, GLint i2, GLint i3) -> self_type&
{
    ARCADIA_GL_CALL(glUniform4i(_GetUniformLocation(name), i0, i1, i2, i3));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, GLint i0, GLint i1, GLint i2) -> self_type&
{
    ARCADIA_GL_CALL(glUniform3i(_GetUniformLocation(name), i0, i1, i2));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, GLint i0, GLint i1) -> self_type&
{
    ARCADIA_GL_CALL(glUniform2i(_GetUniformLocation(name), i0, i1));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, GLint i) -> self_type&
{
    ARCADIA_GL_CALL(glUniform1i(_GetUniformLocation(name), i));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, GLuint u0, GLuint u1, GLuint u2, GLuint u3) -> self_type&
{
    ARCADIA_GL_CALL(glUniform4ui(_GetUniformLocation(name), u0, u1, u2, u3));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, GLuint u0, GLuint u1, GLuint u2) -> self_type&
{
    ARCADIA_GL_CALL(glUniform3ui(_GetUniformLocation(name), u0, u1, u2));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, GLuint u0, GLuint u1) -> self_type&
{
    ARCADIA_GL_CALL(glUniform2ui(_GetUniformLocation(name), u0, u1));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, GLuint u) -> self_type&
{
    ARCADIA_GL_CALL(glUniform1ui(_GetUniformLocation(name), u));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, const glm::vec4& vec) -> self_type&
{
    SetUniform(name, vec.x, vec.y, vec.z, vec.w);
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, const glm::vec3& vec) -> self_type&
{
    SetUniform(name, vec.x, vec.y, vec.z);
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, const glm::vec2& vec) -> self_type&
{
    SetUniform(name, vec.x, vec.y);
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, const glm::mat4& mat) -> self_type&
{
    ARCADIA_GL_CALL(glUniformMatrix4fv(_GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, const glm::mat3& mat) -> self_type&
{
    ARCADIA_GL_CALL(glUniformMatrix3fv(_GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, const glm::mat2& mat) -> self_type&
{
    ARCADIA_GL_CALL(glUniformMatrix2fv(_GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
    return *this;
}

auto Arcadia::GlPipeline::SetUniformBlockBinding(const std::string& name, GLuint index) -> self_type&
{
    ARCADIA_GL_CALL(GLuint block_index = glGetUniformBlockIndex(_GlId, name.c_str()));
    ARCADIA_GL_CALL(glUniformBlockBinding(_GlId, block_index, index));
    return *this;
}

auto Arcadia::GlPipeline::_GetUniformLocation(const std::string& name) -> GLuint
{
    try
    {
        return _GlUniformLocationCache.at(name);
    }
    catch(const std::out_of_range&)
    {
        ARCADIA_GL_CALL(auto location = glGetUniformLocation(_GlId, name.c_str()));
        if(location == -1)
        {
            Arcadia::Log::Error(std::format("Failed to get OpenGL uniform location of {}, because it does not exist", name));
        }
        _GlUniformLocationCache.insert_or_assign(name, location);
        return location;
    }
}
