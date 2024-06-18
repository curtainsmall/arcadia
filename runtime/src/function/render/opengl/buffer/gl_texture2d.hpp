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
    auto GetGlId() const -> GLuint
    {
        return _GlId;
    }

    void Bind(GLenum slot = 0);
    void Unbind();

    void SetTexParameter(GLenum pname, GLint param) const;
    void SetTexParameter(GLenum pname, GLfloat param) const;
private:
    GLuint _GlId{ 0 };
    GLenum _Slot{ -1u };

};
