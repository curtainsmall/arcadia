#pragma once

#include<filesystem>
#include<optional>
#include<tuple>

#include"core/base.hpp"
#include"core/exception.hpp"
#include"core/math.hpp"
#include"function/render/opengl/buffer/gl_cubemap.hpp"
#include"function/render/opengl/buffer/gl_framebuffer.hpp"
#include"function/render/opengl/buffer/gl_vertex_array.hpp"
#include"function/render/opengl/pipeline/gl_pipeline.hpp"
#include"function/render/renderer.hpp"
#include"platform/opengl/opengl_header.hpp"
#include"resource/component/camera_component/camera_component.hpp"
#include"resource/component/model_component/model_component.hpp"
#include"resource/component/skybox_component/skybox_component.hpp"

namespace arcadia
{
    using gl_render_unit_camera = std::tuple<
        arcadia::gl_framebuffer, // gl_framebuffer
        glm::ivec2, // viewport_size
        glm::mat4, // camera_view_mat4
        glm::mat4, // camera_proj_mat4
        bool, // should_display_grid
        float, // near_plane
        float // far_plane
    >;

    using gl_render_unit_mesh = std::tuple<
        arcadia::gl_vertex_array, // gl_vertex_array
        glm::mat4, // trasform_mat
        arcadia::gl_texture2d, // gl_texture2d_ambient
        arcadia::gl_texture2d, // gl_texture2d_diffuse
        arcadia::gl_texture2d  // gl_texture2d_specular
    >;

    using gl_render_unit_skybox = std::tuple<
        arcadia::gl_vertex_array, // gl_vertex_array
        arcadia::gl_cubemap // gl_cubemap
    >;

    struct ARCADIA_API gl_renderer: arcadia::renderer_interface
    {
    public:
        using self_type = gl_renderer;
    public:
        gl_renderer(const std::filesystem::path& gl_shader_folder_path);
        virtual ~gl_renderer() = default;

        virtual void begin_frame() override;
        virtual void end_frame() override;

        virtual void submit(const arcadia::camera_component& camera_comp) override;
        virtual void submit(const arcadia::model_component& renderable_comp) override;
        virtual void submit(const arcadia::skybox_component& skybox_comp) override;

        virtual void draw() override;

        virtual void clear() override;

        virtual auto get_render_result_id(std::size_t index) const->void* override;

    public:
        void _check_frame_in_build_or_throw() const;
        void _check_frame_not_in_build_or_throw() const;

        auto _create_unit_cube_mesh() const->std::pair<std::vector<arcadia::vertex>, std::vector<arcadia::mesh::index_type>>;
    private:
        bool _frame_in_build{ false };

        std::vector<arcadia::gl_render_unit_camera> _gl_render_unit_cameras{};
        std::vector<arcadia::gl_render_unit_mesh> _gl_render_unit_meshes{};
        std::optional<arcadia::gl_render_unit_skybox> _gl_render_unit_skybox_opt{};

        bool _legacy_gl_render_unit_camera{ false };
        bool _legacy_gl_render_unit_mesh{ false };

        arcadia::gl_pipeline _gl_mesh_pipeline;
        arcadia::gl_pipeline _gl_skybox_pipeline;
        arcadia::gl_pipeline _gl_grid_pipeline;
    };
}
