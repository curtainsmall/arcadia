#pragma once

#include"core/base.hpp"
#include"core/math.hpp"
#include"platform/opengl/opengl_header.hpp"
#include"resource/component/skybox_component/cubemap.hpp"

namespace arcadia
{
    struct ARCADIA_API gl_cubemap: arcadia::noncopyable
    {
    public:
        using self_type = gl_cubemap;
    public:
        gl_cubemap(const arcadia::cubemap& cubemap);
        ~gl_cubemap();

        gl_cubemap(self_type&& rhs) noexcept;
        auto operator=(self_type&& rhs) noexcept -> self_type&;

        [[nodiscard]]
        inline auto get_gl_id() const -> GLuint
        {
            return _gl_id;
        }

        void bind(GLenum slot = 0);
        void unbind();

        void set_tex_parameter(GLenum pname, GLint param);
        void set_tex_parameter(GLenum pname, GLfloat param);

    private:
        GLuint _gl_id{};
        GLenum _slot{ -1u };

    };
}
