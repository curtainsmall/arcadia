#include "pch.hpp"
#include "gl_texture2d.hpp"

Arcadia::GlTexture2d::GlTexture2d(
    const glm::ivec2& size,
    void* ptr
)
{
    ARCADIA_GL_CALL(glGenTextures(1, &_GlId));
    Bind();

    // TODO: Multisample count ?
    ARCADIA_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0/* TODO: Mipmap level ?*/, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, ptr));
    // TODO: Generate mipmap ?
    SetTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    Unbind();
}

Arcadia::GlTexture2d::GlTexture2d(const Arcadia::Texture2d& texture2d)
{
    ARCADIA_GL_CALL(glGenTextures(1, &_GlId));
    Bind();

    // TODO: Multisample count ?
    ARCADIA_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0/* TODO: Mipmap level ?*/, GL_RGBA, texture2d.Size.x, texture2d.Size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture2d.Pixels.data()));
    // TODO: Generate mipmap ?
    SetTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    Unbind();
}

Arcadia::GlTexture2d::~GlTexture2d()
{
    ARCADIA_GL_CALL(glDeleteTextures(1, &_GlId));
}

Arcadia::GlTexture2d::GlTexture2d(self_type&& rhs) noexcept:
    _GlId(rhs._GlId),
    _Slot(rhs._Slot)
{
    rhs._GlId = 0;
    rhs._Slot = -1u;
}

auto Arcadia::GlTexture2d::operator=(self_type&& rhs) noexcept -> self_type&
{
    _GlId = rhs._GlId;
    rhs._GlId = 0;

    _Slot = rhs._Slot;
    rhs._Slot = -1u;

    return *this;
}

void Arcadia::GlTexture2d::Bind(GLenum slot)
{
    if(_GlId == 0)
    {
        throw Arcadia::GlInvalid{ "Cannot bind null OpenGL texture2d" };
    }
    _Slot = slot;
    ARCADIA_GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
    ARCADIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, _GlId));
}

void Arcadia::GlTexture2d::Unbind()
{
    if(_Slot == -1)
    {
        return;
    }

    ARCADIA_GL_CALL(glActiveTexture(GL_TEXTURE0 + _Slot));
    ARCADIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
    _Slot = -1;
}

void Arcadia::GlTexture2d::SetTexParameter(GLenum pname, GLint param) const
{
    if(_GlId == 0 || _Slot == -1)
    {
        throw Arcadia::GlInvalid{ "Cannot set texture parameter to an unbound OpenGL Texture" };
    }

    ARCADIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, pname, param));
}

void Arcadia::GlTexture2d::SetTexParameter(GLenum pname, GLfloat param) const
{
    if(_GlId == 0 || _Slot == -1)
    {
        throw Arcadia::GlInvalid{ "Cannot set texture parameter to an unbound OpenGL Texture" };
    }

    ARCADIA_GL_CALL(glTexParameterf(GL_TEXTURE_2D, pname, param));
}
