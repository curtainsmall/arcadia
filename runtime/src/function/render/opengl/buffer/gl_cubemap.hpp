#pragma once

#include "core/math.hpp"
#include "core/noncopyable.hpp"
#include "platform/api_def.hpp"
#include "platform/opengl.hpp"
#include "resource/cubemap.hpp"

namespace Arcadia
{
    class GlCubemap: public Noncopyable
    {
    public:
        using SelfType = GlCubemap;
    public:
        GlCubemap(const Cubemap& cubemap);
        ~GlCubemap();

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