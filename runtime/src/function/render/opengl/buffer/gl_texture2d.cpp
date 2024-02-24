#include "pch.hpp"
#include "gl_texture2d.hpp"

arcadia::gl_texture2d::gl_texture2d(const glm::uvec2& size)
{
    ARCADIA_GL_CALL(glGenTextures(1, &_gl_texture2d_id));
    bind();

    // TODO: Multisample count ?
    ARCADIA_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0/* TODO: Mipmap level ?*/, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
    // TODO: Generate mipmap ?
    set_tex_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    unbind();
}

arcadia::gl_texture2d::gl_texture2d(const arcadia::texture2d& texture2d)
{
    ARCADIA_GL_CALL(glGenTextures(1, &_gl_texture2d_id));
    bind();

    // TODO: Multisample count ?
    ARCADIA_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0/* TODO: Mipmap level ?*/, GL_RGBA, texture2d.size.x, texture2d.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture2d.pixels.data()));
    // TODO: Generate mipmap ?
    set_tex_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    unbind();
}

arcadia::gl_texture2d::~gl_texture2d()
{
    ARCADIA_GL_CALL(glDeleteTextures(1, &_gl_texture2d_id));
}

arcadia::gl_texture2d::gl_texture2d(self_type&& rhs) noexcept:
    _gl_texture2d_id(rhs._gl_texture2d_id),
    _slot(rhs._slot)
{
    rhs._gl_texture2d_id = 0;
    rhs._slot = -1u;
}

auto arcadia::gl_texture2d::operator=(self_type&& rhs) noexcept -> self_type&
{
    _gl_texture2d_id = rhs._gl_texture2d_id;
    rhs._gl_texture2d_id = 0;

    _slot = rhs._slot;
    rhs._slot = -1u;

    return *this;
}

void arcadia::gl_texture2d::bind(GLenum slot)
{
    if(_gl_texture2d_id == 0)
    {
        throw arcadia::gl_invalid{ "Cannot bind null OpenGL texture2d" };
    }
    _slot = slot;
    ARCADIA_GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
    ARCADIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, _gl_texture2d_id));
}

void arcadia::gl_texture2d::unbind()
{
    if(_slot == -1)
    {
        return;
    }

    ARCADIA_GL_CALL(glActiveTexture(GL_TEXTURE0 + _slot));
    ARCADIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
    _slot = -1;
}

void arcadia::gl_texture2d::set_tex_parameter(GLenum pname, GLint param) const
{
    if(_gl_texture2d_id == 0 || _slot == -1)
    {
        throw arcadia::gl_invalid{ "Cannot set texture parameter to an unbound OpenGL Texture" };
    }

    ARCADIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, pname, param));
}

void arcadia::gl_texture2d::set_tex_parameter(GLenum pname, GLfloat param) const
{
    if(_gl_texture2d_id == 0 || _slot == -1)
    {
        throw arcadia::gl_invalid{ "Cannot set texture parameter to an unbound OpenGL Texture" };
    }

    ARCADIA_GL_CALL(glTexParameterf(GL_TEXTURE_2D, pname, param));
}
