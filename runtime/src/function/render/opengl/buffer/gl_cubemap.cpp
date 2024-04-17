#include "pch.hpp"

#include "gl_cubemap.hpp"

Arcadia::GlCubemap::GlCubemap(const Arcadia::Cubemap& cubemap)
{
    ARCADIA_GL_CALL(glGenTextures(1, &_GlId));
    Bind();

    // TODO: Multisample ?
    ARCADIA_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, cubemap.Size.x, cubemap.Size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap.PosXPixels.data()));
    ARCADIA_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, cubemap.Size.x, cubemap.Size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap.NegXPixels.data()));
    ARCADIA_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, cubemap.Size.x, cubemap.Size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap.PosYPixels.data()));
    ARCADIA_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, cubemap.Size.x, cubemap.Size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap.NegYPixels.data()));
    ARCADIA_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, cubemap.Size.x, cubemap.Size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap.PosZPixels.data()));
    ARCADIA_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, cubemap.Size.x, cubemap.Size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap.NegZPixels.data()));

    SetTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    SetTexParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    SetTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    SetTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    Unbind();
}

Arcadia::GlCubemap::~GlCubemap()
{
    ARCADIA_GL_CALL(glDeleteTextures(1, &_GlId));
}

Arcadia::GlCubemap::GlCubemap(self_type&& rhs) noexcept
{
    _GlId = rhs._GlId;
    rhs._GlId = 0;

    _Slot = rhs._Slot;
    rhs._Slot = -1u;
}

auto Arcadia::GlCubemap::operator=(self_type&& rhs) noexcept -> self_type&
{
    _GlId = rhs._GlId;
    rhs._GlId = 0;

    _Slot = rhs._Slot;
    rhs._Slot = -1u;

    return *this;
}

void Arcadia::GlCubemap::Bind(GLenum slot)
{
    if(_GlId == 0)
    {
        throw Arcadia::GlInvalid{ "Cannot bind null OpenGL cubemap" };
    }
    _Slot = slot;
    ARCADIA_GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
    ARCADIA_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, _GlId));
}

void Arcadia::GlCubemap::Unbind()
{
    if(_Slot < 0)
    {
        return;
    }

    ARCADIA_GL_CALL(glActiveTexture(GL_TEXTURE0 + _Slot));
    ARCADIA_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
    _Slot = -1;
}

void Arcadia::GlCubemap::SetTexParameter(GLenum pname, GLint param)
{
    if(_GlId == 0 || _Slot == -1)
    {
        throw Arcadia::GlInvalid{ "Cannot set texture parameter to an unbound OpenGL Texture" };
    }

    ARCADIA_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, pname, param));
}

void Arcadia::GlCubemap::SetTexParameter(GLenum pname, GLfloat param)
{
    if(_GlId == 0 || _Slot == -1)
    {
        throw Arcadia::GlInvalid{ "Cannot set texture parameter to an unbound OpenGL Texture" };
    }

    ARCADIA_GL_CALL(glTexParameterf(GL_TEXTURE_CUBE_MAP, pname, param));
}
