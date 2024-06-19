#pragma once

#include"core/base.hpp"
#include"core/math.hpp"
#include"platform/opengl/opengl_header.hpp"
#include"resource/cubemap.hpp"

struct GlCubemap: Noncopyable
{
public:
    using self_type = GlCubemap;
public:
    GlCubemap(const Cubemap& cubemap);
    ~GlCubemap();

    GlCubemap(self_type&& rhs) noexcept;
    auto operator=(self_type&& rhs) noexcept -> self_type&;

    [[nodiscard]]
    auto gl_id() const -> GLuint
    {
        return _gl_id;
    }

    void bind(GLenum slot = 0);
    void unbind();

    void set_tex_param(GLenum pname, GLint param);
    void set_tex_param(GLenum pname, GLfloat param);

private:
    GLuint _gl_id{};
    GLenum _slot{ -1u };
};
