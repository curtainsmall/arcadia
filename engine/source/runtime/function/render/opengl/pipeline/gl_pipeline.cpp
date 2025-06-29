#include "gl_pipeline.hpp"

Arcadia::GlPipeline::GlPipeline(
    const std::filesystem::path& gl_shader_folder_path,
    const GlShadersBuilderType& gl_shaders_builder
)
{
    ACDA_GL_CALL(_GlId = glCreateProgram());

    // Build Shaders
    gl_shaders_builder(gl_shader_folder_path, _GlShaders);

    // Build pipeline
    for(const GlShader& shader : _GlShaders)
    {
        ACDA_GL_CALL(glAttachShader(_GlId, shader.GetGlId()));
    }

    ACDA_GL_CALL(glLinkProgram(_GlId));
    GLint status = GL_FALSE;
    ACDA_GL_CALL(glGetProgramiv(_GlId, GL_LINK_STATUS, &status));
    if(status == GL_FALSE)
    {
        GLint length = 0;
        ACDA_GL_CALL(glGetProgramiv(_GlId, GL_INFO_LOG_LENGTH, &length));
        std::string msg{};
        msg.reserve(length);
        ACDA_GL_CALL(glGetProgramInfoLog(_GlId, length, nullptr, msg.data()));
        ACDA_GL_CALL(glDeleteProgram(_GlId));
        _GlId = 0;
        throw Exceptions::GlPipelineLinkFailed(msg);
    }

    ACDA_GL_CALL(glValidateProgram(_GlId));
    ACDA_GL_CALL(glGetProgramiv(_GlId, GL_VALIDATE_STATUS, &status));
    if(status == GL_FALSE)
    {
        GLint length = 0;
        ACDA_GL_CALL(glGetProgramiv(_GlId, GL_INFO_LOG_LENGTH, &length));
        std::string msg{};
        msg.reserve(length);
        ACDA_GL_CALL(glGetProgramInfoLog(_GlId, length, nullptr, msg.data()));
        ACDA_GL_CALL(glDeleteProgram(_GlId));
        _GlId = 0;
        throw Exceptions::GlPipelineLinkFailed(msg);
    }
}

Arcadia::GlPipeline::~GlPipeline()
{
    ACDA_GL_CALL(glDeleteProgram(_GlId));
}

Arcadia::GlPipeline::GlPipeline(SelfType&& rhs) noexcept:
    _GlUniformLocationCache(std::move(rhs._GlUniformLocationCache)),
    _GlShaders(std::move(rhs._GlShaders))
{
    _GlId = rhs._GlId;
    rhs._GlId = 0;
}

auto Arcadia::GlPipeline::operator=(SelfType&& rhs) noexcept -> SelfType&
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
        throw Exceptions::GlInvalid("Cannot use null OpenGL pipeline");
    }
    ACDA_GL_CALL(glUseProgram(_GlId));
}

void Arcadia::GlPipeline::Unuse() const
{
    ACDA_GL_CALL(glUseProgram(0));
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, GLfloat f0, GLfloat f1, GLfloat f2, GLfloat f3) -> SelfType&
{
    ACDA_GL_CALL(glUniform4f(_GetUniformLocation(name), f0, f1, f2, f3));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, GLfloat f0, GLfloat f1, GLfloat f2) -> SelfType&
{
    ACDA_GL_CALL(glUniform3f(_GetUniformLocation(name), f0, f1, f2));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, GLfloat f0, GLfloat f1) -> SelfType&
{
    ACDA_GL_CALL(glUniform2f(_GetUniformLocation(name), f0, f1));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, GLfloat f) -> SelfType&
{
    ACDA_GL_CALL(glUniform1f(_GetUniformLocation(name), f));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, GLint i0, GLint i1, GLint i2, GLint i3) -> SelfType&
{
    ACDA_GL_CALL(glUniform4i(_GetUniformLocation(name), i0, i1, i2, i3));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, GLint i0, GLint i1, GLint i2) -> SelfType&
{
    ACDA_GL_CALL(glUniform3i(_GetUniformLocation(name), i0, i1, i2));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, GLint i0, GLint i1) -> SelfType&
{
    ACDA_GL_CALL(glUniform2i(_GetUniformLocation(name), i0, i1));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, GLint i) -> SelfType&
{
    ACDA_GL_CALL(glUniform1i(_GetUniformLocation(name), i));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, GLuint u0, GLuint u1, GLuint u2, GLuint u3) -> SelfType&
{
    ACDA_GL_CALL(glUniform4ui(_GetUniformLocation(name), u0, u1, u2, u3));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, GLuint u0, GLuint u1, GLuint u2) -> SelfType&
{
    ACDA_GL_CALL(glUniform3ui(_GetUniformLocation(name), u0, u1, u2));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, GLuint u0, GLuint u1) -> SelfType&
{
    ACDA_GL_CALL(glUniform2ui(_GetUniformLocation(name), u0, u1));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, GLuint u) -> SelfType&
{
    ACDA_GL_CALL(glUniform1ui(_GetUniformLocation(name), u));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, const glm::vec4& vec) -> SelfType&
{
    ACDA_GL_CALL(glUniform4fv(_GetUniformLocation(name), 1, glm::value_ptr(vec)));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, const glm::vec3& vec) -> SelfType&
{
    ACDA_GL_CALL(glUniform3fv(_GetUniformLocation(name), 1, glm::value_ptr(vec)));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, const glm::vec2& vec) -> SelfType&
{
    ACDA_GL_CALL(glUniform2fv(_GetUniformLocation(name), 1, glm::value_ptr(vec)));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, const glm::mat4& mat) -> SelfType&
{
    ACDA_GL_CALL(glUniformMatrix4fv(_GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat)));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, const glm::mat3& mat) -> SelfType&
{
    ACDA_GL_CALL(glUniformMatrix3fv(_GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat)));
    return *this;
}

auto Arcadia::GlPipeline::SetUniform(const std::string& name, const glm::mat2& mat) -> SelfType&
{
    ACDA_GL_CALL(glUniformMatrix2fv(_GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat)));
    return *this;
}

auto Arcadia::GlPipeline::SetUniformBlockBinding(const std::string& name, GLuint index) -> SelfType&
{
    ACDA_GL_CALL(GLuint block_index = glGetUniformBlockIndex(_GlId, name.c_str()));
    ACDA_GL_CALL(glUniformBlockBinding(_GlId, block_index, index));
    return *this;
}

auto Arcadia::GlPipeline::_GetUniformLocation(const std::string& name) -> GLint
{
    try
    {
        return _GlUniformLocationCache.at(name);
    }
    catch(const std::out_of_range&)
    {
        ACDA_GL_CALL(GLint location = glGetUniformLocation(_GlId, name.c_str()));
        if(location == -1)
        {
            throw Exceptions::GlPipelineInvalidUniformLocation(std::format("Failed to get OpenGL uniform location of {}, because it does not exist", name));
        }
        _GlUniformLocationCache.insert_or_assign(name, location);
        return location;
    }
}