

#include "gl_cubemap.hpp"

Arcadia::GlCubemap::GlCubemap(const Cubemap& cubemap)
{
    ACDA_GL_CALL(glGenTextures(1, &_GlId));
    Bind();

    // TODO: Multisample ?
    ACDA_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, cubemap.Size.x, cubemap.Size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap.PositiveXFacePixels.data()));
    ACDA_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, cubemap.Size.x, cubemap.Size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap.NegativeXFacePixels.data()));
    ACDA_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, cubemap.Size.x, cubemap.Size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap.PositiveYFacePixels.data()));
    ACDA_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, cubemap.Size.x, cubemap.Size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap.NegativeYFacePixels.data()));
    ACDA_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, cubemap.Size.x, cubemap.Size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap.PositiveZFacePixels.data()));
    ACDA_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, cubemap.Size.x, cubemap.Size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap.NegatieZFacePixels.data()));

    SetTextureParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    SetTextureParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    SetTextureParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    SetTextureParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    Unbind();
}

Arcadia::GlCubemap::~GlCubemap()
{
    ACDA_GL_CALL(glDeleteTextures(1, &_GlId));
}

Arcadia::GlCubemap::GlCubemap(SelfType&& rhs) noexcept
{
    _GlId = rhs._GlId;
    rhs._GlId = 0;

    _Slot = rhs._Slot;
    rhs._Slot = -1u;
}

auto Arcadia::GlCubemap::operator=(SelfType&& rhs) noexcept -> SelfType&
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
        throw Exceptions::GlInvalid("Cannot bind null OpenGL cubemap");
    }
    _Slot = slot;
    ACDA_GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
    ACDA_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, _GlId));
}

void Arcadia::GlCubemap::Unbind()
{
    if(_Slot < 0)
    {
        return;
    }

    ACDA_GL_CALL(glActiveTexture(GL_TEXTURE0 + _Slot));
    ACDA_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
    _Slot = -1;
}

void Arcadia::GlCubemap::SetTextureParameter(GLenum pname, GLint param)
{
    if(_GlId == 0 || _Slot == -1)
    {
        throw Exceptions::GlInvalid("Cannot set texture parameter to an unbound OpenGL Texture");
    }

    ACDA_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, pname, param));
}

void Arcadia::GlCubemap::SetTextureParameter(GLenum pname, GLfloat param)
{
    if(_GlId == 0 || _Slot == -1)
    {
        throw Exceptions::GlInvalid("Cannot set texture parameter to an unbound OpenGL Texture");
    }

    ACDA_GL_CALL(glTexParameterf(GL_TEXTURE_CUBE_MAP, pname, param));
}