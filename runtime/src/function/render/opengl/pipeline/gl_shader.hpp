#pragma once

#include<string>

#include"core/base.hpp"
#include"platform/opengl/opengl_header.hpp"

struct GlShader: Noncopyable
{
public:
    ARCADIA_EXCEPTION(CompileFail);

    using self_type = GlShader;
public:
    /// @brief Create an OpenGL shader object
    /// @param source_code Source code of the shader
    /// @param shader_type Type of shader, must be one of
    /// - GL_COMPUT_SHADER
    /// - GL_FRAGMENT_SHADER
    /// - GL_GEOMETRY_SHADER
    /// - GL_TESS_CONTROL_SHADER
    /// - GL_TESS_EVALUATION_SHADER
    /// - GL_VERTEX_SHADER
    GlShader(
        const std::string& source_code,
        GLenum shader_type
    );
    ~GlShader();

    GlShader(self_type&& rhs) noexcept;
    auto operator=(self_type&& rhs) noexcept -> self_type&;

    [[nodiscard]]
    auto GetGlId() const -> GLuint
    {
        return _GlId;
    }
private:
    GLuint _GlId{ 0 };
};
