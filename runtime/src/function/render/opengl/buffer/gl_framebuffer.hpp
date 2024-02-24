#pragma once

#include"core/base.hpp"
#include"core/exception.hpp"
#include"function/render/opengl/buffer/gl_texture2d.hpp"
#include"platform/opengl/opengl_header.hpp"
#include"resource/component/camera_component/camera_component.hpp"

namespace arcadia
{
    struct ARCADIA_API gl_framebuffer:arcadia::noncopyable
    {
    public:
        ARCADIA_EXCEPTION(imcomplete);

        using self_type = gl_framebuffer;
    public:
        gl_framebuffer(const glm::uvec2& viewport_size);
        ~gl_framebuffer();

        gl_framebuffer(self_type&& rhs) noexcept;
        auto operator=(self_type&& rhs) noexcept -> self_type&;

        [[nodiscard]]
        inline auto get_id() const -> GLuint
        {
            return _gl_framebuffer_id;
        }

        [[nodiscard]]
        inline auto get_gl_texture2d() const -> const arcadia::gl_texture2d&
        {
            return _gl_texture2d;
        }

        void bind() const;
        void unbind() const;

        auto is_complete() const->GLenum;
    private:
        GLuint _gl_framebuffer_id{ 0 };
        arcadia::gl_texture2d _gl_texture2d;
    };
}
