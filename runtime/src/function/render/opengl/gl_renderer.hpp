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

namespace Arcadia
{
    using GlRenderUnitCamera = std::tuple<
        Arcadia::GlFramebuffer, // gl_framebuffer
        glm::ivec2, // viewport_size
        glm::mat4, // camera_view_mat4
        glm::mat4, // camera_proj_mat4
        glm::vec3, // camera_position
        bool, // should_display_grid
        float, // near_plane
        float // far_plane
    >;

    using GlRenderUnitLight = std::tuple<
        Arcadia::LightType // light
    >;

    using GlRenderUnitMesh = std::tuple<
        Arcadia::GlVertexArray, // gl_vertex_array
        glm::mat4, // transform_mat
        Arcadia::GlTexture2d, // gl_texture2d_ambient
        Arcadia::GlTexture2d, // gl_texture2d_diffuse
        Arcadia::GlTexture2d  // gl_texture2d_specular
    >;

    using GlRenderUnitSkybox = std::tuple<
        Arcadia::GlVertexArray, // gl_vertex_array
        Arcadia::GlCubemap // gl_cubemap
    >;

    using GlRenderUnitPhysicsBodyShape = std::tuple<
        Arcadia::GlVertexArray, // gl_vertex_array
        glm::mat4, // transform_mat
        glm::vec3 // color
    >;

    struct ARCADIA_API GlRenderer: Arcadia::iRenderer
    {
    public:
        ARCADIA_EXCEPTION(too_many_lights);
        using self_type = GlRenderer;
    public:
        GlRenderer(const std::filesystem::path& gl_shader_folder_path);
        virtual ~GlRenderer() = default;

        /// @copydoc Arcadia::renderer::is_in_build
        [[nodiscard]]
        virtual auto IsInBuild() const -> bool override
        {
            return _InBuild;
        }

        /// @copydoc Arcadia::Renderer::Prepare
        virtual void Prepare() override;
        /// @copydoc Arcadia::Renderer::Finalize
        virtual void Finalize() override;

        /// @copydoc Arcadia::Renderer::Submit
        virtual void Submit(const Arcadia::Scene& scene, const std::string& name) override;

        /// @copydoc Arcadia::Renderer::Draw
        virtual void Draw() override;

        /// @copydoc Arcadia::Renderer::Reset
        virtual void Reset() override;

        [[nodiscard]]
        virtual auto GetRenderResultId(std::size_t index) const->void* override;

        [[nodiscard]]
        virtual auto GetGraphicApiType() const->Arcadia::GraphicApi::Type override
        {
            return Arcadia::GraphicApi::Opengl{ Arcadia::Version{4, 6, 0} };
        }

    public:
        void _AssertFrameInBuild() const;
        void _AssertFrameNotInBuild() const;

        void _DrawGrid(
            const Arcadia::GlVertexArray& gl_grid_vertex_array,
            const glm::mat4& camera_view,
            const glm::mat4& camera_proj,
            float near_plane,
            float far_plane
        );
        void _DrawLights(
            const GLsizeiptr light_t_size,
            const int max_light_count,
            const int light_count_size_aligned,
            Arcadia::GlUniformBuffer& gl_light_uniform_buffer,
            const Arcadia::GlVertexArray& gl_light_shape_vertex_array,
            const glm::mat4& camera_view,
            const glm::mat4& camera_proj
        );
        void _DrawModels(
            const glm::mat4& camera_view,
            const glm::mat4& camera_proj,
            const glm::vec3& camera_pos
        );
        void _DrawSkybox(
            const glm::mat4& camera_view,
            const glm::mat4& camera_proj
        );
        void _DrawPhysicsBodyShape(
            const glm::mat4& camera_view,
            const glm::mat4& camera_proj
        );

    private:
        bool _InBuild{ false };

        std::unordered_map<Arcadia::Uuid, std::vector<Arcadia::GlRenderUnitMesh>> _GlRenderUnitMeshStorage{};
        std::set<Arcadia::Uuid> _SubmittedMeshesUuids{};

        std::unordered_map<Arcadia::Uuid, Arcadia::GlRenderUnitPhysicsBodyShape> _GlRenderUnitPhysicsBodyShapeStorage{};
        std::set<Arcadia::Uuid> _SubmittedPhysicsBodyShapeUuids{};

        std::vector<Arcadia::GlRenderUnitCamera> _GlRenderUnitCameras{};
        std::vector<Arcadia::GlRenderUnitLight> _GlRenderUnitLights{};
        std::optional<Arcadia::GlRenderUnitSkybox> _optGlRenderUnitSkybox{};

        Arcadia::GlPipeline _GlModelPipeline;
        Arcadia::GlPipeline _GlSkyboxPipeline;
        Arcadia::GlPipeline _GlGridPipeline;
        Arcadia::GlPipeline _GlShapePipeline;
    };
}
