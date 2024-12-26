#pragma once

#include<filesystem>
#include<optional>
#include<set>
#include<tuple>
#include<unordered_map>
#include<vector>

#include"core/exception.hpp"
#include"core/math.hpp"
#include"function/render/opengl/buffer/gl_cubemap.hpp"
#include"function/render/opengl/buffer/gl_framebuffer.hpp"
#include"function/render/opengl/buffer/gl_uniform_buffer.hpp"
#include"function/render/opengl/buffer/gl_vertex_array.hpp"
#include"function/render/opengl/pipeline/gl_pipeline.hpp"
#include"function/render/renderer.hpp"
#include"platform/api_def.hpp"
#include"platform/opengl/opengl_header.hpp"
#include"resource/components/light_component.hpp"

namespace Arcadia
{
    class GlRenderUnitCamera
    {
    public:
        GlFramebuffer Framebuffer;
        glm::i32vec2 ViewportSize;
        glm::mat4 CameraViewMatrix;
        glm::mat4 CameraProjectionMatrix;
        glm::vec3 CameraPosition;
        bool ShouldDisplayGrid;
        float NearPlane;
        float FarPlane;
    };

    class GlRenderUnitLight
    {
    public:
        glm::vec3 Position;
        glm::vec3 Direction;
        LightType Light;
    };

    class GlRenderUnitMesh
    {
    public:
        GlVertexArray VertexArray;
        glm::mat4 TransformMatrix;
        GlTexture2d AmbientTexture;
        GlTexture2d DiffuseTexture;
        GlTexture2d SpecularTexture;
    };

    class GlRenderUnitSkybox
    {
    public:
        GlVertexArray VertexArray;
        GlCubemap Cubemap;
    };

    class GlRenderUnitPhysicsBodyShape
    {
    public:
        GlVertexArray VertexArray;
        glm::mat4 TransformMatrix;
        glm::vec3 Color;
    };

    class GlRenderer: public RendererInterface
    {
    public:

        using SelfType = GlRenderer;
    public:
        GlRenderer(const std::filesystem::path& gl_shader_folder_path);
        virtual ~GlRenderer() = default;

        [[nodiscard]]
        virtual auto IsInBuild() const -> bool override;

        virtual void Prepare() override;
        virtual void Finalize() override;
        virtual void Submit(const Scene& scene, EntityId entity_id) override;
        virtual void Draw() override;

        virtual void Reset() override;

        [[nodiscard]]
        virtual auto GetRenderResultId(std::size_t index) const->void* override;

        [[nodiscard]]
        virtual auto GetGraphicApiType() const->GraphicApi::Type override;

    public:
        void _AssertFrameInBuild() const;
        void _AssertFrameNotInBuild() const;

        void _DrawGrid(
            const GlVertexArray& gl_grid_vertex_array,
            const glm::mat4& camera_view,
            const glm::mat4& camera_proj,
            float near_plane,
            float far_plane
        );
        void _DrawLights(
            const GLsizeiptr light_t_size,
            const std::int32_t max_light_count,
            const std::int32_t light_count_size_aligned,
            GlUniformBuffer& gl_light_uniform_buffer,
            const GlVertexArray& gl_light_shape_vertex_array,
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

        std::unordered_map<Uuid, std::vector<GlRenderUnitMesh>> _GlRenderUnitMeshStorage{};
        std::set<Uuid> _SubmittedMeshUuids{};

        std::unordered_map<Uuid, GlRenderUnitPhysicsBodyShape> _GlRenderUnitPhysicsBodyShapeStorage{};
        std::set<Uuid> _SubmittedPhysicsBodyShapeUuids{};

        std::vector<GlRenderUnitCamera> _GlRenderUnitCameras{};
        std::vector<GlRenderUnitLight> _GlRenderUnitLights{};
        std::optional<GlRenderUnitSkybox> _GlRenderUnitSkybox{};

        GlPipeline _GlModelPipeline;
        GlPipeline _GlSkyboxPipeline;
        GlPipeline _GlGridPipeline;
        GlPipeline _GlShapePipeline;
    };
}