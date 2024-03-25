#pragma once

#include<filesystem>
#include<optional>
#include<set>
#include<tuple>
#include<unordered_map>
#include<vector>

#include"core/base.hpp"
#include"core/exception.hpp"
#include"core/math.hpp"
#include"function/render/opengl/buffer/gl_cubemap.hpp"
#include"function/render/opengl/buffer/gl_framebuffer.hpp"
#include"function/render/opengl/buffer/gl_uniform_buffer.hpp"
#include"function/render/opengl/buffer/gl_vertex_array.hpp"
#include"function/render/opengl/pipeline/gl_pipeline.hpp"
#include"function/render/renderer.hpp"
#include"platform/opengl/opengl_header.hpp"

namespace arcadia
{
    using gl_render_unit_camera = std::tuple<
        arcadia::gl_framebuffer, // gl_framebuffer
        glm::ivec2, // viewport_size
        glm::mat4, // camera_view_mat4
        glm::mat4, // camera_proj_mat4
        glm::vec3, // camera_position
        bool, // should_display_grid
        float, // near_plane
        float // far_plane
    >;

    using gl_render_unit_light = std::tuple<
        arcadia::light_type // light
    >;

    using gl_render_unit_mesh = std::tuple<
        arcadia::gl_vertex_array, // gl_vertex_array
        glm::mat4, // transform_mat
        arcadia::gl_texture2d, // gl_texture2d_ambient
        arcadia::gl_texture2d, // gl_texture2d_diffuse
        arcadia::gl_texture2d  // gl_texture2d_specular
    >;

    using gl_render_unit_skybox = std::tuple<
        arcadia::gl_vertex_array, // gl_vertex_array
        arcadia::gl_cubemap // gl_cubemap
    >;

    using gl_render_unit_physcis_body_shape = std::tuple<
        arcadia::gl_vertex_array, // gl_vertex_array
        glm::mat4, // transform_mat
        glm::vec3 // color
    >;

    struct ARCADIA_API gl_renderer: arcadia::renderer_interface
    {
    public:
        ARCADIA_EXCEPTION(too_many_lights);
        using self_type = gl_renderer;
    public:
        gl_renderer(const std::filesystem::path& gl_shader_folder_path);
        virtual ~gl_renderer() = default;

        /// @copydoc arcadia::renderer::is_in_build
        [[nodiscard]]
        virtual inline auto is_in_build() const -> bool override
        {
            return _in_build;
        }

        /// @copydoc arcadia::renderer::prepare
        virtual void prepare() override;
        /// @copydoc arcadia::renderer::finalize
        virtual void finalize() override;

        /// @copydoc arcadia::renderer::submit
        virtual void submit(const arcadia::camera_component& camera_comp) override;

        /// @copydoc arcadia::renderer::submit
        virtual void submit(const arcadia::light_component& light_comp) override;

        /// @copydoc arcadia::renderer::submit
        virtual void submit(const arcadia::model_component& model_comp) override;

        /// @copydoc arcadia::renderer::submit
        virtual void submit(const arcadia::skybox_component& skybox_comp) override;

        /// @copydoc arcadia::renderer::submit
        virtual void submit(const arcadia::physics_component& physcis_comp) override;

        /// @copydoc arcadia::renderer::draw
        virtual void draw() override;

        /// @copydoc arcadia::renderer::reset
        virtual void reset() override;

        [[nodiscard]]
        virtual auto get_render_result_id(std::size_t index) const->void* override;

        [[nodiscard]]
        virtual inline auto get_graphic_api_type() const->arcadia::graphic_api::type override
        {
            return arcadia::graphic_api::opengl{ arcadia::version{4, 6, 0} };
        }

    public:
        void _assert_frame_in_build() const;
        void _assert_frame_not_in_build() const;

        void _draw_grid(
            const arcadia::gl_vertex_array& gl_grid_vertex_array,
            const glm::mat4& camera_view,
            const glm::mat4& camera_proj,
            float near_plane,
            float far_plane
        );
        void _draw_lights(
            const GLsizeiptr light_t_size,
            const int max_light_count,
            const int light_count_size_aligned,
            arcadia::gl_uniform_buffer& gl_light_uniform_buffer,
            const arcadia::gl_vertex_array& gl_light_shape_vertex_array,
            const glm::mat4& camera_view,
            const glm::mat4& camera_proj
        );
        void _draw_models(
            const glm::mat4& camera_view,
            const glm::mat4& camera_proj,
            const glm::vec3& camera_pos
        );
        void _draw_skybox(
            const glm::mat4& camera_view,
            const glm::mat4& camera_proj
        );
        void _draw_physics_body_shape(
            const glm::mat4& camera_view,
            const glm::mat4& camera_proj
        );

    private:
        bool _in_build{ false };

        std::unordered_map<arcadia::uuid, std::vector<arcadia::gl_render_unit_mesh>> _gl_render_unit_meshes_umap{};
        std::set<arcadia::uuid> _submitted_meshes_uuid_set{};

        std::unordered_map<arcadia::uuid, arcadia::gl_render_unit_physcis_body_shape> _gl_render_unit_physics_body_shape_umap{};
        std::set<arcadia::uuid> _submitted_physcis_body_shape_uuid_set{};

        std::vector<arcadia::gl_render_unit_camera> _gl_render_unit_cameras{};
        std::vector<arcadia::gl_render_unit_light> _gl_render_unit_lights{};
        std::optional<arcadia::gl_render_unit_skybox> _gl_render_unit_skybox_opt{};

        arcadia::gl_pipeline _gl_model_pipeline;
        arcadia::gl_pipeline _gl_skybox_pipeline;
        arcadia::gl_pipeline _gl_grid_pipeline;
        arcadia::gl_pipeline _gl_shape_pipeline;
    };
}
