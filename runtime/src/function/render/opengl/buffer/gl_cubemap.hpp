#pragma once

#include"core/base.hpp"
#include"core/math.hpp"
#include"platform/opengl/opengl_header.hpp"
#include"resource/cubemap.hpp"

namespace Arcadia
{
    struct ARCADIA_API GlCubemap: Arcadia::Noncopyable
    {
    public:
        using self_type = GlCubemap;
    public:
        GlCubemap(const Arcadia::Cubemap& cubemap);
        ~GlCubemap();

        GlCubemap(self_type&& rhs) noexcept;
        auto operator=(self_type&& rhs) noexcept -> self_type&;

        [[nodiscard]]
        auto GetGlId() const -> GLuint
        {
            return _GlId;
        }

        void Bind(GLenum slot = 0);
        void Unbind();

        void SetTexParameter(GLenum pname, GLint param);
        void SetTexParameter(GLenum pname, GLfloat param);

    private:
        GLuint _GlId{};
        GLenum _Slot{ -1u };

    };
}
