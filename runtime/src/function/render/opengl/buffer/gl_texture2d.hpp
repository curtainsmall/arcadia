#pragma once

#include"core/base.hpp"
#include"core/math.hpp"
#include"platform/opengl/opengl_header.hpp"
#include"resource/material/texture2d.hpp"

namespace Arcadia
{
    class GlTexture2d: public Noncopyable
    {
    public:
        using SelfType = GlTexture2d;
    public:
        GlTexture2d(
            const glm::ivec2& size,
            void* ptr = nullptr
        );
        GlTexture2d(
            const Texture2d& texture2d
        );
        ~GlTexture2d();

        GlTexture2d(SelfType&& rhs) noexcept;
        auto operator=(SelfType&& rhs) noexcept -> SelfType&;

        [[nodiscard]]
        auto GetGlId() const -> GLuint
        {
            return _GlId;
        }

        void Bind(GLenum slot = 0);
        void Unbind();

        void SetTextureParameter(GLenum pname, GLint param) const;
        void SetTextureParameter(GLenum pname, GLfloat param) const;
    private:
        GLuint _GlId{ 0 };
        GLenum _Slot{ -1u };
    };
}