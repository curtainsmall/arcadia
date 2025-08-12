#pragma once

#include "core/math.hpp"
#include "platform/defines.hpp"
#include "platform/opengl.hpp"
#include "resource/material/texture2d.hpp"

namespace Arcadia
{
    struct ACDA_API GlTexture2d
    {
    public:
        using SelfType = GlTexture2d;
    public:
        GlTexture2d(
            const glm::i32vec2& size,
            void* ptr = nullptr
        );
        GlTexture2d(
            const Texture2d& texture2d
        );
        ~GlTexture2d();

        GlTexture2d(const SelfType&) = delete;
        auto operator=(const SelfType&) -> SelfType & = delete;

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