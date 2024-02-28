#pragma once

#include"core/base.hpp"
#include"core/math.hpp"
#include"platform/opengl/opengl_header.hpp"
#include"resource/component/model_component/material/texture2d.hpp"

namespace arcadia
{
    struct ARCADIA_API gl_texture2d: arcadia::noncopyable
    {
    public:
        using self_type = gl_texture2d;
    public:
        gl_texture2d(
            const glm::uvec2& size
        );
        gl_texture2d(
            const arcadia::texture2d& texture2d
        );
        ~gl_texture2d();

        gl_texture2d(self_type&& rhs) noexcept;
        auto operator=(self_type&& rhs) noexcept -> self_type&;

        [[nodiscard]]
        inline auto get_id() const -> GLuint
        {
            return _gl_texture2d_id;
        }

        void bind(GLenum slot = 0);
        void unbind();

        void set_tex_parameter(GLenum pname, GLint param) const;
        void set_tex_parameter(GLenum pname, GLfloat param) const;
    private:
        GLuint _gl_texture2d_id{ 0 };
        GLenum _slot{ -1u };

    };
}
