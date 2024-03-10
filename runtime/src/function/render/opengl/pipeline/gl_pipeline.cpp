#include "pch.hpp"
#include "gl_pipeline.hpp"

arcadia::gl_pipeline::gl_pipeline(
    const std::filesystem::path& gl_shader_folder_path,
    const gl_shaders_builder_type& gl_shaders_builder
)
{
    ARCADIA_GL_CALL(_gl_id = glCreateProgram());

    // Build Shaders
    gl_shaders_builder(gl_shader_folder_path, _gl_shaders);

    // Build pipeline
    for(const auto& shader : _gl_shaders)
    {
        ARCADIA_GL_CALL(glAttachShader(_gl_id, shader.get_gl_id()));
    }

    GLint status{ GL_FALSE };
    ARCADIA_GL_CALL(glLinkProgram(_gl_id));
    ARCADIA_GL_CALL(glGetProgramiv(_gl_id, GL_LINK_STATUS, &status));
    if(status == GL_FALSE)
    {
        GLint length{ 0 };
        ARCADIA_GL_CALL(glGetProgramiv(_gl_id, GL_INFO_LOG_LENGTH, &length));
        std::string msg{};
        msg.reserve(length);
        ARCADIA_GL_CALL(glGetProgramInfoLog(_gl_id, length, nullptr, msg.data()));
        ARCADIA_GL_CALL(glDeleteProgram(_gl_id));
        _gl_id = 0;
        throw link_fail{ msg };
    }

    ARCADIA_GL_CALL(glValidateProgram(_gl_id));
    ARCADIA_GL_CALL(glGetProgramiv(_gl_id, GL_VALIDATE_STATUS, &status));
    if(status == GL_FALSE)
    {
        GLint length{ 0 };
        ARCADIA_GL_CALL(glGetProgramiv(_gl_id, GL_INFO_LOG_LENGTH, &length));
        std::string msg{};
        msg.reserve(length);
        ARCADIA_GL_CALL(glGetProgramInfoLog(_gl_id, length, nullptr, msg.data()));
        ARCADIA_GL_CALL(glDeleteProgram(_gl_id));
        _gl_id = 0;
        throw link_fail{ msg };
    }


}

arcadia::gl_pipeline::~gl_pipeline()
{
    ARCADIA_GL_CALL(glDeleteProgram(_gl_id));
}

arcadia::gl_pipeline::gl_pipeline(self_type&& rhs) noexcept:
    _gl_uniform_location_cache_umap(std::move(rhs._gl_uniform_location_cache_umap)),
    _gl_shaders(std::move(rhs._gl_shaders))
{
    _gl_id = rhs._gl_id;
    rhs._gl_id = 0;
}

auto arcadia::gl_pipeline::operator=(self_type&& rhs) noexcept -> self_type&
{
    _gl_uniform_location_cache_umap = std::move(rhs._gl_uniform_location_cache_umap);
    _gl_shaders = std::move(rhs._gl_shaders);

    _gl_id = rhs._gl_id;
    rhs._gl_id = 0;

    return *this;
}

void arcadia::gl_pipeline::use() const
{
    if(_gl_id == 0)
    {
        throw arcadia::gl_invalid{ "Cannot use null OpenGL pipeline" };
    }
    ARCADIA_GL_CALL(glUseProgram(_gl_id));
}

void arcadia::gl_pipeline::unuse() const
{
    ARCADIA_GL_CALL(glUseProgram(0));
}

void arcadia::gl_pipeline::set_uniform(const std::string& name, GLfloat f0, GLfloat f1, GLfloat f2, GLfloat f3)
{
    ARCADIA_GL_CALL(glUniform4f(_get_uniform_location(name), f0, f1, f2, f3));
}

void arcadia::gl_pipeline::set_uniform(const std::string& name, GLfloat f0, GLfloat f1, GLfloat f2)
{
    ARCADIA_GL_CALL(glUniform3f(_get_uniform_location(name), f0, f1, f2));
}

void arcadia::gl_pipeline::set_uniform(const std::string& name, GLfloat f0, GLfloat f1)
{
    ARCADIA_GL_CALL(glUniform2f(_get_uniform_location(name), f0, f1));
}

void arcadia::gl_pipeline::set_uniform(const std::string& name, GLfloat f)
{
    ARCADIA_GL_CALL(glUniform1f(_get_uniform_location(name), f));
}

void arcadia::gl_pipeline::set_uniform(const std::string& name, GLint i0, GLint i1, GLint i2, GLint i3)
{
    ARCADIA_GL_CALL(glUniform4i(_get_uniform_location(name), i0, i1, i2, i3));
}

void arcadia::gl_pipeline::set_uniform(const std::string& name, GLint i0, GLint i1, GLint i2)
{
    ARCADIA_GL_CALL(glUniform3i(_get_uniform_location(name), i0, i1, i2));
}

void arcadia::gl_pipeline::set_uniform(const std::string& name, GLint i0, GLint i1)
{
    ARCADIA_GL_CALL(glUniform2i(_get_uniform_location(name), i0, i1));
}

void arcadia::gl_pipeline::set_uniform(const std::string& name, GLint i)
{
    ARCADIA_GL_CALL(glUniform1i(_get_uniform_location(name), i));
}

void arcadia::gl_pipeline::set_uniform(const std::string& name, GLuint u0, GLuint u1, GLuint u2, GLuint u3)
{
    ARCADIA_GL_CALL(glUniform4ui(_get_uniform_location(name), u0, u1, u2, u3));
}

void arcadia::gl_pipeline::set_uniform(const std::string& name, GLuint u0, GLuint u1, GLuint u2)
{
    ARCADIA_GL_CALL(glUniform3ui(_get_uniform_location(name), u0, u1, u2));
}

void arcadia::gl_pipeline::set_uniform(const std::string& name, GLuint u0, GLuint u1)
{
    ARCADIA_GL_CALL(glUniform2ui(_get_uniform_location(name), u0, u1));
}

void arcadia::gl_pipeline::set_uniform(const std::string& name, GLuint u)
{
    ARCADIA_GL_CALL(glUniform1ui(_get_uniform_location(name), u));
}

void arcadia::gl_pipeline::set_uniform(const std::string& name, const glm::vec4& vec)
{
    set_uniform(name, vec.x, vec.y, vec.z, vec.w);
}

void arcadia::gl_pipeline::set_uniform(const std::string& name, const glm::vec3& vec)
{
    set_uniform(name, vec.x, vec.y, vec.z);
}

void arcadia::gl_pipeline::set_uniform(const std::string& name, const glm::vec2& vec)
{
    set_uniform(name, vec.x, vec.y);
}

void arcadia::gl_pipeline::set_uniform(const std::string& name, const glm::mat4& mat)
{
    ARCADIA_GL_CALL(glUniformMatrix4fv(_get_uniform_location(name), 1, GL_FALSE, &mat[0][0]));
}

void arcadia::gl_pipeline::set_uniform(const std::string& name, const glm::mat3& mat)
{
    ARCADIA_GL_CALL(glUniformMatrix3fv(_get_uniform_location(name), 1, GL_FALSE, &mat[0][0]));
}

void arcadia::gl_pipeline::set_uniform(const std::string& name, const glm::mat2& mat)
{
    ARCADIA_GL_CALL(glUniformMatrix2fv(_get_uniform_location(name), 1, GL_FALSE, &mat[0][0]));
}

auto arcadia::gl_pipeline::_get_uniform_location(const std::string& name) -> GLuint
{
    try
    {
        return _gl_uniform_location_cache_umap.at(name);
    }
    catch(const std::out_of_range&)
    {
        ARCADIA_GL_CALL(auto location = glGetUniformLocation(_gl_id, name.c_str()));
        if(location == -1)
        {
            arcadia::log::error(std::format("Failed to get OpenGL uniform location of {}, because it does not exist", name));
        }
        _gl_uniform_location_cache_umap.insert_or_assign(name, location);
        return location;
    }
}
