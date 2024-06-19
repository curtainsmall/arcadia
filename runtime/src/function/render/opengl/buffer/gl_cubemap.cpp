#include "pch.hpp"

#include "gl_cubemap.hpp"

GlCubemap::GlCubemap(const Cubemap& cubemap)
{
    ACDA_GL_CALL(glGenTextures(1, &_gl_id));
    bind();

    // TODO: Multisample ?
    ACDA_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, cubemap.size.x, cubemap.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap.pos_x_pixels.data()));
    ACDA_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, cubemap.size.x, cubemap.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap.neg_x_pixels.data()));
    ACDA_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, cubemap.size.x, cubemap.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap.pos_y_pixels.data()));
    ACDA_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, cubemap.size.x, cubemap.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap.neg_y_pixels.data()));
    ACDA_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, cubemap.size.x, cubemap.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap.pos_z_pixels.data()));
    ACDA_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, cubemap.size.x, cubemap.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap.neg_z_pixels.data()));

    set_tex_param(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    set_tex_param(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    set_tex_param(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    set_tex_param(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    unbind();
}

GlCubemap::~GlCubemap()
{
    ACDA_GL_CALL(glDeleteTextures(1, &_gl_id));
}

GlCubemap::GlCubemap(self_type&& rhs) noexcept
{
    _gl_id = rhs._gl_id;
    rhs._gl_id = 0;

    _slot = rhs._slot;
    rhs._slot = -1u;
}

auto GlCubemap::operator=(self_type&& rhs) noexcept -> self_type&
{
    _gl_id = rhs._gl_id;
    rhs._gl_id = 0;

    _slot = rhs._slot;
    rhs._slot = -1u;

    return *this;
}

void GlCubemap::bind(GLenum slot)
{
    if(_gl_id == 0)
    {
        throw GlInvalid{ "Cannot bind null OpenGL cubemap" };
    }
    _slot = slot;
    ACDA_GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
    ACDA_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, _gl_id));
}

void GlCubemap::unbind()
{
    if(_slot < 0)
    {
        return;
    }

    ACDA_GL_CALL(glActiveTexture(GL_TEXTURE0 + _slot));
    ACDA_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
    _slot = -1;
}

void GlCubemap::set_tex_param(GLenum pname, GLint param)
{
    if(_gl_id == 0 || _slot == -1)
    {
        throw GlInvalid{ "Cannot set texture parameter to an unbound OpenGL Texture" };
    }

    ACDA_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, pname, param));
}

void GlCubemap::set_tex_param(GLenum pname, GLfloat param)
{
    if(_gl_id == 0 || _slot == -1)
    {
        throw GlInvalid{ "Cannot set texture parameter to an unbound OpenGL Texture" };
    }

    ACDA_GL_CALL(glTexParameterf(GL_TEXTURE_CUBE_MAP, pname, param));
}
