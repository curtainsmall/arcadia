#pragma once

#include"core/base.hpp"
#include"core/math.hpp"
#include"platform/opengl/opengl_header.hpp"
#include"resource/material/texture2d.hpp"

struct GlTexture2d: Noncopyable
{
public:
    using self_type = GlTexture2d;
public:
    GlTexture2d(
        const glm::ivec2& size,
        void* ptr = nullptr
    );
    GlTexture2d(
        const Texture2d& texture2d
    );
    ~GlTexture2d();

    GlTexture2d(self_type&& rhs) noexcept;
    auto operator=(self_type&& rhs) noexcept -> self_type&;

    [[nodiscard]]
    auto gl_id() const -> GLuint
    {
        return _gl_id;
    }

    void bind(GLenum slot = 0);
    void unbind();

    void set_tex_param(GLenum pname, GLint param) const;
    void set_tex_param(GLenum pname, GLfloat param) const;
private:
    GLuint _gl_id{ 0 };
    GLenum _slot{ -1u };

};
