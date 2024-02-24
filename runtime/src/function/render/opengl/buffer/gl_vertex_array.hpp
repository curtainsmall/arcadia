#pragma once

#include<vector>

#include"core/base.hpp"
#include"function/render/opengl/buffer/gl_index_buffer.hpp"
#include"function/render/opengl/buffer/gl_vertex_buffer.hpp"
#include"platform/opengl/opengl_header.hpp"
#include"resource/component/mesh_component/mesh_component.hpp"

namespace arcadia
{

    struct ARCADIA_API gl_vertex_array: arcadia::noncopyable
    {
    public:
        using self_type = gl_vertex_array;

    public:
        gl_vertex_array(
            const std::vector<arcadia::vertex>& vertices,
            const std::vector<arcadia::mesh_component::index_type>& indices
        );
        ~gl_vertex_array();

        gl_vertex_array(self_type&& rhs) noexcept;
        auto operator=(self_type&& rhs) noexcept -> self_type&;

        [[nodiscard]]
        inline auto get_id() const -> GLuint
        {
            return _gl_vertex_array_id;
        }

        void bind() const;
        void unbind() const;

        void draw_arrays(GLenum mode) const;
        void draw_arrays_instances(GLenum mode, GLsizei count);
        void draw_indices(GLenum mode) const;
        void draw_indices_instanced(GLenum mode, GLsizei count);

    private:
        GLuint _gl_vertex_array_id{ 0 };

        arcadia::gl_vertex_buffer _gl_vertex_buffer;
        arcadia::gl_index_buffer _gl_index_buffer;
    };
}
