#include "pch.hpp"
#include "gl_cubemap.hpp"

arcadia::gl_cubemap::gl_cubemap(const arcadia::cubemap& cubemap)
{
    ARCADIA_GL_CALL(glGenTextures(1, &_gl_cubemap_id));
    bind();

    // TODO: Multisample ?
    ARCADIA_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, cubemap.size.x, cubemap.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap.pos_x_pixels.data()));
    ARCADIA_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, cubemap.size.x, cubemap.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap.neg_x_pixels.data()));
    ARCADIA_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, cubemap.size.x, cubemap.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap.pos_y_pixels.data()));
    ARCADIA_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, cubemap.size.x, cubemap.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap.neg_y_pixels.data()));
    ARCADIA_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, cubemap.size.x, cubemap.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap.pos_z_pixels.data()));
    ARCADIA_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, cubemap.size.x, cubemap.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap.neg_z_pixels.data()));

    set_tex_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    set_tex_parameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    set_tex_parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    set_tex_parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    unbind();
}

arcadia::gl_cubemap::~gl_cubemap()
{
    ARCADIA_GL_CALL(glDeleteTextures(1, &_gl_cubemap_id));
}

arcadia::gl_cubemap::gl_cubemap(self_type&& rhs) noexcept
{
    _gl_cubemap_id = rhs._gl_cubemap_id;
    rhs._gl_cubemap_id = 0;

    _slot = rhs._slot;
    rhs._slot = -1u;
}

auto arcadia::gl_cubemap::operator=(self_type&& rhs) noexcept -> self_type&
{
    _gl_cubemap_id = rhs._gl_cubemap_id;
    rhs._gl_cubemap_id = 0;

    _slot = rhs._slot;
    rhs._slot = -1u;

    return *this;
}

void arcadia::gl_cubemap::bind(GLenum slot)
{
    if(_gl_cubemap_id == 0)
    {
        throw arcadia::gl_invalid{ "Cannot bind null OpenGL cubemap" };
    }
    _slot = slot;
    ARCADIA_GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
    ARCADIA_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, _gl_cubemap_id));
}

void arcadia::gl_cubemap::unbind()
{
    if(_slot < 0)
    {
        return;
    }

    ARCADIA_GL_CALL(glActiveTexture(GL_TEXTURE0 + _slot));
    ARCADIA_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
    _slot = -1;
}

void arcadia::gl_cubemap::set_tex_parameter(GLenum pname, GLint param)
{
    if(_gl_cubemap_id == 0 || _slot == -1)
    {
        throw arcadia::gl_invalid{ "Cannot set texture parameter to an unbound OpenGL Texture" };
    }

    ARCADIA_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, pname, param));
}

void arcadia::gl_cubemap::set_tex_parameter(GLenum pname, GLfloat param)
{
    if(_gl_cubemap_id == 0 || _slot == -1)
    {
        throw arcadia::gl_invalid{ "Cannot set texture parameter to an unbound OpenGL Texture" };
    }

    ARCADIA_GL_CALL(glTexParameterf(GL_TEXTURE_CUBE_MAP, pname, param));
}
