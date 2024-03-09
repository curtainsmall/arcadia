#pragma once

#include<string>

#include"core/base.hpp"
#include"platform/opengl/opengl_header.hpp"

namespace arcadia
{
    struct ARCADIA_API gl_shader: arcadia::noncopyable
    {
    public:
        ARCADIA_EXCEPTION(compile_fail);

        using self_type = gl_shader;
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
        gl_shader(
            const std::string& source_code,
            GLenum shader_type
        );
        ~gl_shader();

        gl_shader(self_type&& rhs) noexcept;
        auto operator=(self_type&& rhs) noexcept -> self_type&;

        [[nodiscard]]
        inline auto get_gl_id() const -> GLuint
        {
            return _gl_id;
        }
    private:
        GLuint _gl_id{ 0 };
    };
}
