#pragma once

#include"core/base.hpp"
#include"platform/opengl/opengl_header.hpp"

namespace Arcadia
{
    struct ARCADIA_API GlUniformBuffer: Arcadia::Noncopyable
    {
    public:
        using self_type = GlUniformBuffer;
    public:
        GlUniformBuffer(
            GLsizeiptr size
        );
        GlUniformBuffer(
            GLsizeiptr size,
            const GLvoid* data
        );
        ~GlUniformBuffer();

        GlUniformBuffer(self_type&& rhs)noexcept;
        auto operator=(self_type&& rhs) noexcept -> self_type&;

        void Bind() const;
        void Unbind() const;

        void BindBufferBase(GLuint index) const;
        void BindBufferRange(GLuint index, GLintptr Offset, GLsizeiptr size) const;

        auto SubData(GLintptr Offset, GLsizeiptr size, const GLvoid* data) const-> const self_type&;

    private:
        GLuint _GlId{ 0 };
    };
}
