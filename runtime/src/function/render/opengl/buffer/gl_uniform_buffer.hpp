#pragma once

#include "core/noncopyable.hpp"
#include "platform/api_def.hpp"
#include "platform/opengl.hpp"

namespace Arcadia
{
    class GlUniformBuffer: public Noncopyable
    {
    public:
        using SelfType = GlUniformBuffer;
    public:
        GlUniformBuffer(
            GLsizeiptr size
        );
        GlUniformBuffer(
            GLsizeiptr size,
            const GLvoid* data
        );
        ~GlUniformBuffer();

        GlUniformBuffer(SelfType&& rhs)noexcept;
        auto operator=(SelfType&& rhs) noexcept -> SelfType&;

        void Bind() const;
        void Unbind() const;

        void BindBufferBase(GLuint index) const;
        void BindBufferRange(GLuint index, GLintptr Offset, GLsizeiptr size) const;

        auto SetBufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data) const-> const SelfType&;

    private:
        GLuint _GlId{ 0 };
    };
}