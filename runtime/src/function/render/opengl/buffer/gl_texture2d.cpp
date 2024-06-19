#include "pch.hpp"

#include "gl_texture2d.hpp"

GlTexture2d::GlTexture2d(
    const glm::ivec2& size,
    void* ptr
)
{
    ACDA_GL_CALL(glGenTextures(1, &_gl_id));
    bind();

    // TODO: Multisample Count ?
    ACDA_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0/* TODO: Mipmap level ?*/, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, ptr));
    // TODO: Generate mipmap ?
    set_tex_param(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    unbind();
}

GlTexture2d::GlTexture2d(const Texture2d& texture2d)
{
    ACDA_GL_CALL(glGenTextures(1, &_gl_id));
    bind();

    // TODO: Multisample Count ?
    ACDA_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0/* TODO: Mipmap level ?*/, GL_RGBA, texture2d.size.x, texture2d.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture2d.pixels.data()));
    // TODO: Generate mipmap ?
    set_tex_param(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    unbind();
}

GlTexture2d::~GlTexture2d()
{
    ACDA_GL_CALL(glDeleteTextures(1, &_gl_id));
}

GlTexture2d::GlTexture2d(self_type&& rhs) noexcept:
    _gl_id(rhs._gl_id),
    _slot(rhs._slot)
{
    rhs._gl_id = 0;
    rhs._slot = -1u;
}

auto GlTexture2d::operator=(self_type&& rhs) noexcept -> self_type&
{
    _gl_id = rhs._gl_id;
    rhs._gl_id = 0;

    _slot = rhs._slot;
    rhs._slot = -1u;

    return *this;
}

void GlTexture2d::bind(GLenum slot)
{
    if(_gl_id == 0)
    {
        throw GlInvalid{ "Cannot bind null OpenGL texture2d" };
    }
    _slot = slot;
    ACDA_GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
    ACDA_GL_CALL(glBindTexture(GL_TEXTURE_2D, _gl_id));
}

void GlTexture2d::unbind()
{
    if(_slot == -1)
    {
        return;
    }

    ACDA_GL_CALL(glActiveTexture(GL_TEXTURE0 + _slot));
    ACDA_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
    _slot = -1;
}

void GlTexture2d::set_tex_param(GLenum pname, GLint param) const
{
    if(_gl_id == 0 || _slot == -1)
    {
        throw GlInvalid{ "Cannot set texture parameter to an unbound OpenGL Texture" };
    }

    ACDA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, pname, param));
}

void GlTexture2d::set_tex_param(GLenum pname, GLfloat param) const
{
    if(_gl_id == 0 || _slot == -1)
    {
        throw GlInvalid{ "Cannot set texture parameter to an unbound OpenGL Texture" };
    }

    ACDA_GL_CALL(glTexParameterf(GL_TEXTURE_2D, pname, param));
}
