#pragma once

#include "core/math.hpp"
#include "platform/defines.hpp"
#include "platform/opengl.hpp"
#include "resource/cubemap.hpp"

namespace Arcadia
{
    struct ACDA_API GlCubemap
    {
    public:
        using SelfType = GlCubemap;
    public:
        GlCubemap(const Cubemap& cubemap);
        ~GlCubemap();

        GlCubemap(const SelfType&) = delete;
        auto operator=(const SelfType&) -> SelfType & = delete;

        GlCubemap(SelfType&& rhs) noexcept;
        auto operator=(SelfType&& rhs) noexcept -> SelfType&;

        [[nodiscard]]
        auto GetGlId() const -> GLuint
        {
            return _GlId;
        }

        void Bind(GLenum slot = 0);
        void Unbind();

        void SetTextureParameter(GLenum pname, GLint param);
        void SetTextureParameter(GLenum pname, GLfloat param);

    private:
        GLuint _GlId{};
        GLenum _Slot{ -1u };
    };
}