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
#include"resource/components/light_component.hpp"

using GlRenderUnitCamera = std::tuple<
    GlFramebuffer, // gl_framebuffer
    glm::ivec2, // viewport_size
    glm::mat4, // camera_view_mat4
    glm::mat4, // camera_proj_mat4
    glm::vec3, // camera_position
    bool, // should_display_grid
    float, // near_plane
    float // far_plane
>;

using GlRenderUnitLight = std::tuple<
    glm::vec3, // position
    glm::vec3, // direction
    LightType // light
>;

using GlRenderUnitMesh = std::tuple<
    GlVertexArray, // gl_vertex_array
    glm::mat4, // transform_mat
    GlTexture2d, // gl_texture2d_ambient
    GlTexture2d, // gl_texture2d_diffuse
    GlTexture2d  // gl_texture2d_specular
>;

using GlRenderUnitSkybox = std::tuple<
    GlVertexArray, // gl_vertex_array
    GlCubemap // gl_cubemap
>;

using GlRenderUnitPhysicsBodyShape = std::tuple<
    GlVertexArray, // gl_vertex_array
    glm::mat4, // transform_mat
    glm::vec3 // color
>;

struct GlRenderer: iRenderer
{
public:
    ACDA_EXCEPTION(TooManyLights);
    using self_type = GlRenderer;
public:
    GlRenderer(const std::filesystem::path& gl_shader_folder_path);
    virtual ~GlRenderer() = default;

    /// @copydoc renderer::is_in_build
    [[nodiscard]]
    virtual auto is_in_build() const -> bool override
    {
        return _in_build;
    }

    virtual void prepare() override;
    virtual void finalize() override;
    virtual void submit(const Scene& scene, const std::string& name) override;
    virtual void draw() override;

    virtual void reset() override;

    [[nodiscard]]
    virtual auto render_result_id(size_t index) const->void* override;

    [[nodiscard]]
    virtual auto graphic_api_type() const->graphic_api::Type override
    {
        return graphic_api::Opengl{ Version{4, 6, 0} };
    }

public:
    void _assert_frame_in_build() const;
    void _assert_frame_not_in_build() const;

    void _draw_grid(
        const GlVertexArray& gl_grid_vertex_array,
        const glm::mat4& camera_view,
        const glm::mat4& camera_proj,
        float near_plane,
        float far_plane
    );
    void _draw_lights(
        const GLsizeiptr light_t_size,
        const int max_light_count,
        const int light_count_size_aligned,
        GlUniformBuffer& gl_light_uniform_buffer,
        const GlVertexArray& gl_light_shape_vertex_array,
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

    std::unordered_map<Uuid, std::vector<GlRenderUnitMesh>> _gl_render_unit_mesh_storage{};
    std::set<Uuid> _submitted_mesh_uuids{};

    std::unordered_map<Uuid, GlRenderUnitPhysicsBodyShape> _gl_render_unit_physics_body_shape_storage{};
    std::set<Uuid> _submitted_physics_body_shape_uuids{};

    std::vector<GlRenderUnitCamera> _gl_render_unit_cameras{};
    std::vector<GlRenderUnitLight> _gl_render_unit_lights{};
    std::optional<GlRenderUnitSkybox> _gl_render_unit_skybox{};

    GlPipeline _gl_model_pipeline;
    GlPipeline _gl_skybox_pipeline;
    GlPipeline _gl_grid_pipeline;
    GlPipeline _gl_shape_pipeline;
};
