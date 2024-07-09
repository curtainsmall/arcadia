#include "pch.hpp"

#include "gl_pipeline.hpp"

GlPipeline::GlPipeline(
    const std::filesystem::path& gl_shader_folder_path,
    const gl_shaders_builder_type& gl_shaders_builder
)
{
    ACDA_GL_CALL(_gl_id = glCreateProgram());

    // Build Shaders
    gl_shaders_builder(gl_shader_folder_path, _gl_shaders);

    // Build pipeline
    for(const auto& shader : _gl_shaders)
    {
        ACDA_GL_CALL(glAttachShader(_gl_id, shader.gl_id()));
    }

    ACDA_GL_CALL(glLinkProgram(_gl_id));
    GLint status{ GL_FALSE };
    ACDA_GL_CALL(glGetProgramiv(_gl_id, GL_LINK_STATUS, &status));
    if(status == GL_FALSE)
    {
        GLint length{ 0 };
        ACDA_GL_CALL(glGetProgramiv(_gl_id, GL_INFO_LOG_LENGTH, &length));
        std::string msg{};
        msg.reserve(length);
        ACDA_GL_CALL(glGetProgramInfoLog(_gl_id, length, nullptr, msg.data()));
        ACDA_GL_CALL(glDeleteProgram(_gl_id));
        _gl_id = 0;
        throw LinkFail{ msg };
    }

    ACDA_GL_CALL(glValidateProgram(_gl_id));
    ACDA_GL_CALL(glGetProgramiv(_gl_id, GL_VALIDATE_STATUS, &status));
    if(status == GL_FALSE)
    {
        GLint length{ 0 };
        ACDA_GL_CALL(glGetProgramiv(_gl_id, GL_INFO_LOG_LENGTH, &length));
        std::string msg{};
        msg.reserve(length);
        ACDA_GL_CALL(glGetProgramInfoLog(_gl_id, length, nullptr, msg.data()));
        ACDA_GL_CALL(glDeleteProgram(_gl_id));
        _gl_id = 0;
        throw LinkFail{ msg };
    }


}

GlPipeline::~GlPipeline()
{
    ACDA_GL_CALL(glDeleteProgram(_gl_id));
}

GlPipeline::GlPipeline(self_type&& rhs) noexcept:
    _gl_uniform_location_cache(std::move(rhs._gl_uniform_location_cache)),
    _gl_shaders(std::move(rhs._gl_shaders))
{
    _gl_id = rhs._gl_id;
    rhs._gl_id = 0;
}

auto GlPipeline::operator=(self_type&& rhs) noexcept -> self_type&
{
    _gl_uniform_location_cache = std::move(rhs._gl_uniform_location_cache);
    _gl_shaders = std::move(rhs._gl_shaders);

    _gl_id = rhs._gl_id;
    rhs._gl_id = 0;

    return *this;
}

void GlPipeline::use() const
{
    if(_gl_id == 0)
    {
        throw GlInvalid{ "Cannot use null OpenGL pipeline" };
    }
    ACDA_GL_CALL(glUseProgram(_gl_id));
}

void GlPipeline::unuse() const
{
    ACDA_GL_CALL(glUseProgram(0));
}

auto GlPipeline::set_uniform(const std::string& name, GLfloat f0, GLfloat f1, GLfloat f2, GLfloat f3) -> self_type&
{
    ACDA_GL_CALL(glUniform4f(_get_uniform_location(name), f0, f1, f2, f3));
    return *this;
}

auto GlPipeline::set_uniform(const std::string& name, GLfloat f0, GLfloat f1, GLfloat f2) -> self_type&
{
    ACDA_GL_CALL(glUniform3f(_get_uniform_location(name), f0, f1, f2));
    return *this;
}

auto GlPipeline::set_uniform(const std::string& name, GLfloat f0, GLfloat f1) -> self_type&
{
    ACDA_GL_CALL(glUniform2f(_get_uniform_location(name), f0, f1));
    return *this;
}

auto GlPipeline::set_uniform(const std::string& name, GLfloat f) -> self_type&
{
    ACDA_GL_CALL(glUniform1f(_get_uniform_location(name), f));
    return *this;
}

auto GlPipeline::set_uniform(const std::string& name, GLint i0, GLint i1, GLint i2, GLint i3) -> self_type&
{
    ACDA_GL_CALL(glUniform4i(_get_uniform_location(name), i0, i1, i2, i3));
    return *this;
}

auto GlPipeline::set_uniform(const std::string& name, GLint i0, GLint i1, GLint i2) -> self_type&
{
    ACDA_GL_CALL(glUniform3i(_get_uniform_location(name), i0, i1, i2));
    return *this;
}

auto GlPipeline::set_uniform(const std::string& name, GLint i0, GLint i1) -> self_type&
{
    ACDA_GL_CALL(glUniform2i(_get_uniform_location(name), i0, i1));
    return *this;
}

auto GlPipeline::set_uniform(const std::string& name, GLint i) -> self_type&
{
    ACDA_GL_CALL(glUniform1i(_get_uniform_location(name), i));
    return *this;
}

auto GlPipeline::set_uniform(const std::string& name, GLuint u0, GLuint u1, GLuint u2, GLuint u3) -> self_type&
{
    ACDA_GL_CALL(glUniform4ui(_get_uniform_location(name), u0, u1, u2, u3));
    return *this;
}

auto GlPipeline::set_uniform(const std::string& name, GLuint u0, GLuint u1, GLuint u2) -> self_type&
{
    ACDA_GL_CALL(glUniform3ui(_get_uniform_location(name), u0, u1, u2));
    return *this;
}

auto GlPipeline::set_uniform(const std::string& name, GLuint u0, GLuint u1) -> self_type&
{
    ACDA_GL_CALL(glUniform2ui(_get_uniform_location(name), u0, u1));
    return *this;
}

auto GlPipeline::set_uniform(const std::string& name, GLuint u) -> self_type&
{
    ACDA_GL_CALL(glUniform1ui(_get_uniform_location(name), u));
    return *this;
}

auto GlPipeline::set_uniform(const std::string& name, const glm::vec4& vec) -> self_type&
{
    ACDA_GL_CALL(glUniform4fv(_get_uniform_location(name), 1, glm::value_ptr(vec)));
    return *this;
}

auto GlPipeline::set_uniform(const std::string& name, const glm::vec3& vec) -> self_type&
{
    ACDA_GL_CALL(glUniform3fv(_get_uniform_location(name), 1, glm::value_ptr(vec)));
    return *this;
}

auto GlPipeline::set_uniform(const std::string& name, const glm::vec2& vec) -> self_type&
{
    ACDA_GL_CALL(glUniform2fv(_get_uniform_location(name), 1, glm::value_ptr(vec)));
    return *this;
}

auto GlPipeline::set_uniform(const std::string& name, const glm::mat4& mat) -> self_type&
{
    ACDA_GL_CALL(glUniformMatrix4fv(_get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat)));
    return *this;
}

auto GlPipeline::set_uniform(const std::string& name, const glm::mat3& mat) -> self_type&
{
    ACDA_GL_CALL(glUniformMatrix3fv(_get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat)));
    return *this;
}

auto GlPipeline::set_uniform(const std::string& name, const glm::mat2& mat) -> self_type&
{
    ACDA_GL_CALL(glUniformMatrix2fv(_get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat)));
    return *this;
}

auto GlPipeline::set_uniform_block_binding(const std::string& name, GLuint index) -> self_type&
{
    ACDA_GL_CALL(GLuint block_index = glGetUniformBlockIndex(_gl_id, name.c_str()));
    ACDA_GL_CALL(glUniformBlockBinding(_gl_id, block_index, index));
    return *this;
}

auto GlPipeline::_get_uniform_location(const std::string& name) -> GLuint
{
    try
    {
        return _gl_uniform_location_cache.at(name);
    }
    catch(const std::out_of_range&)
    {
        ACDA_GL_CALL(auto location = glGetUniformLocation(_gl_id, name.c_str()));
        if(location == -1)
        {
            ACDA_LOG_ERROR(std::format("Failed to get OpenGL uniform location of {}, because it does not exist", name));
        }
        _gl_uniform_location_cache.insert_or_assign(name, location);
        return location;
    }
}
