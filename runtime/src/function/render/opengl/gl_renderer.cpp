#include "pch.hpp"

#include "gl_renderer.hpp"

#include<vector>

#include"resource/components/camera_component.hpp"
#include"resource/components/light_component.hpp"
#include"resource/components/model_component.hpp"
#include"resource/components/physics_component.hpp"
#include"resource/components/skybox_component.hpp"
#include"resource/components/transform_component.hpp"

GlRenderer::GlRenderer(const std::filesystem::path& gl_shader_folder_path):
    _gl_model_pipeline(gl_shader_folder_path, generate_model_shaders_builder()),
    _gl_skybox_pipeline(gl_shader_folder_path, generate_skybox_shaders_builder()),
    _gl_grid_pipeline(gl_shader_folder_path, generate_grid_shaders_builder()),
    _gl_shape_pipeline(gl_shader_folder_path, generate_shape_shaders_builder())
{
    ACDA_GL_CALL(glEnable(GL_DEPTH_TEST));
    ACDA_GL_CALL(glEnable(GL_CULL_FACE));
}

void GlRenderer::prepare()
{
    _assert_frame_not_in_build();
    _in_build = true;

    // Clear submitted meshes uuids
    _submitted_mesh_uuids.clear();

    // Clear cameras
    _gl_render_unit_cameras.clear();

    // Clear lights
    _gl_render_unit_lights.clear();

    // Clear skybox
    _gl_render_unit_skybox.reset();

    // Clear submitted physics body shape uuids
    _submitted_physics_body_shape_uuids.clear();

}

void GlRenderer::finalize()
{
    _assert_frame_in_build();
    _in_build = false;

    for(auto iter = _gl_render_unit_mesh_storage.begin(); iter != _gl_render_unit_mesh_storage.end();)
    {
        if(!_submitted_mesh_uuids.contains(iter->first))
        {
            iter = _gl_render_unit_mesh_storage.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    for(auto iter = _gl_render_unit_physics_body_shape_storage.begin(); iter != _gl_render_unit_physics_body_shape_storage.end();)
    {
        if(!_submitted_physics_body_shape_uuids.contains(iter->first))
        {
            iter = _gl_render_unit_physics_body_shape_storage.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void GlRenderer::submit(const Scene& scene, const std::string& name)
{
    _assert_frame_in_build();

    const auto& entity_info = scene.entity_info(name);

    match<void>(
        entity_info.type,
        [&]()
    {
        ACDA_ASSERT(false && "Entity type not supported");
    },
        "camera"s,
        [&]()
    {
        const auto& [camera_comp, transform_comp] = scene.get<CameraComponent, TransformComponent>(name);

        _gl_render_unit_cameras.emplace_back(
            GlFramebuffer{
                camera_comp.viewport_size,
                camera_comp.near_plane,
                camera_comp.far_plane
            },
            camera_comp.viewport_size,
            camera_comp.generate_view_mat4(transform_comp.position, transform_comp.direction),
            camera_comp.generate_proj_mat4(),
            transform_comp.position,
            camera_comp.should_display_grid,
            camera_comp.near_plane,
            camera_comp.far_plane
        );
    },
        "light"s,
        [&]()
    {
        const auto& [light_comp, transform_comp] = scene.get<LightComponent, TransformComponent>(name);
        _gl_render_unit_lights.emplace_back(transform_comp.position, transform_comp.direction, light_comp.light);
    },
        "actor"s,
        [&]()
    {
        const auto [model_comp, transform_comp, physics_comp] = scene.get<ModelComponent, TransformComponent, PhysicsComponent>(name);

        if(model_comp.has_identifiable_meshes())
        {
            const auto& [uuid, meshes] = model_comp.identifiable_meshes();

            const auto transform_mat = transform_comp.generate_transform_matrix();

            if(!_gl_render_unit_mesh_storage.contains(uuid))
            {

                std::vector<GlRenderUnitMesh> gl_meshes{};
                for(const auto& mesh : meshes)
                {
                    // For any uuid, its corresponding meshes must be the same
                    gl_meshes.emplace_back(
                        GlVertexArray{ mesh.vertices, mesh.indices },
                        transform_mat,
                        mesh.material.ambient_texture2d,
                        mesh.material.diffuse_texture2d,
                        mesh.material.specular_texture2d
                    );
                }
                _gl_render_unit_mesh_storage.try_emplace(uuid, std::move(gl_meshes));
            }
            else
            {
                for(auto& gl_render_unit_mesh : _gl_render_unit_mesh_storage.at(uuid))
                {
                    std::get<1>(gl_render_unit_mesh) = transform_mat;
                }
            }

            _submitted_mesh_uuids.emplace(uuid);
        }

        if(physics_comp.has_body_info())
        {
            const auto& [uuid, jph_body] = physics_comp.get_identifiable_jph_body_info();
            if(!_gl_render_unit_physics_body_shape_storage.contains(uuid))
            {
                const auto& shape_info = jph_body.jph_shape_info;
                const auto& shape_mesh = match<Mesh>(
                    shape_info,
                    [&](const JphBoxShapeInfo& info)
                {
                    return Mesh::box(info.half_extent);
                },
                    [&](const JphCapsuleShapeInfo& info)
                {
                    return Mesh::capsule(info.radius, info.half_height_of_cylinder);
                },
                    [&](const JphCylinderShapeInfo& info)
                {
                    return Mesh::cylinder(info.half_height, info.radius);
                },
                    [&](const JphSphereShapeInfo& info)
                {
                    return Mesh::sphere(info.radius);
                }
                );

                _gl_render_unit_physics_body_shape_storage.try_emplace(
                    uuid, GlVertexArray{ shape_mesh.vertices,shape_mesh.indices },
                    glm::mat4{},
                    glm::vec3{}
                );
            }

            auto& [GlVertexBuffer, transform_mat, color] = _gl_render_unit_physics_body_shape_storage.at(uuid);
            transform_mat = glm::translate(
                glm::mat4_cast(transform_comp.rotation),
                transform_comp.position
            );
            color = physics_comp.body_shape_color;

            _submitted_physics_body_shape_uuids.emplace(uuid);
        }
    }
    );
}

void GlRenderer::draw()
{
    _assert_frame_not_in_build();


    if(_gl_render_unit_cameras.empty())
    {
        throw DrawFail{ "No framebuffer to draw to" };
    }


    ACDA_GL_CALL(glClearColor(41 / 255.0, 43 / 255.0, 44 / 255.0, 1.f));

    // For each framebuffer
    for(const auto& [
        gl_framebuffer,
            viewport_size,
            camera_view,
            camera_proj,
            camera_position,
            should_display_grid,
            near_plane,
            far_plane
    ] : _gl_render_unit_cameras)
    {
        gl_framebuffer.bind();

        // Clear framebufers
        ACDA_GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
        ACDA_GL_CALL(glViewport(0, 0, viewport_size.x, viewport_size.y));

        // Draw grid
        if(should_display_grid)
        {
            std::vector<Vertex> grid_vertices{
                   Vertex{ glm::vec3{-1,1,0} },
                    Vertex{ glm::vec3{-1,-1,0} },
                    Vertex{ glm::vec3{1,-1,0} },
                    Vertex{ glm::vec3{1,1,0} }
            };
            std::vector<Mesh::index_type> grid_indices{
                0,1,2,
                2,3,0
            };
            GlVertexArray gl_grid_vertex_array{ grid_vertices, grid_indices };

            _draw_grid(
                gl_grid_vertex_array,
                camera_view,
                camera_proj,
                near_plane,
                far_plane
            );
        }

        // Lights
        const GLsizeiptr light_t_size{ 128 };
        const int max_light_count = 32;
        const int light_count_size_aligned = 16; // Sizeof `u_light_count` in fragment shader with alignment considered
        GlUniformBuffer GlUniformBuffer{ light_count_size_aligned + light_t_size * max_light_count };

        auto light_box_shape = Mesh::box(glm::vec3{ 1,1,1 });
        GlVertexArray gl_light_box_shape_vertex_array{ light_box_shape.vertices ,light_box_shape.indices };

        _draw_lights(
            light_t_size,
            max_light_count,
            light_count_size_aligned,
            GlUniformBuffer,
            gl_light_box_shape_vertex_array,
            camera_view,
            camera_proj
        );

        // Draw with mesh pipeline
        _draw_models(
            camera_view,
            camera_proj,
            camera_position
        );

        _draw_physics_body_shape(
            camera_view,
            camera_proj
        );

        // Draw with skybox pipeline
        if(_gl_render_unit_skybox)
        {
            _draw_skybox(
                camera_view,
                camera_proj
            );
        }

        gl_framebuffer.unbind();
    }

}

void GlRenderer::reset()
{
    _gl_render_unit_cameras.clear();
    _gl_render_unit_lights.clear();
    _gl_render_unit_mesh_storage.clear();
    _submitted_mesh_uuids.clear();
    _gl_render_unit_skybox.reset();
}

auto GlRenderer::render_result_id(std::size_t index) const -> void*
{
    return reinterpret_cast<void*>(std::get<0>(_gl_render_unit_cameras.at(index)).gl_texure2d().gl_id());
}

void GlRenderer::_assert_frame_in_build() const
{
    ACDA_ASSERT(_in_build && "Frame is not in build, did you call `prepare()`?");
}

void GlRenderer::_assert_frame_not_in_build() const
{
    ACDA_ASSERT(!_in_build && "Frame is in build, did you call `finalize()`?");
}

void GlRenderer::_draw_grid(
    const GlVertexArray& gl_grid_vertex_array,
    const glm::mat4& camera_view,
    const glm::mat4& camera_proj,
    float near_plane,
    float far_plane
)
{
    _gl_grid_pipeline.use();
    _gl_grid_pipeline
        .set_uniform("u_view_mat4", camera_view)
        .set_uniform("u_proj_mat4", camera_proj)
        .set_uniform("u_near_plane", near_plane)
        .set_uniform("u_far_plane", far_plane);

    gl_grid_vertex_array.bind();
    gl_grid_vertex_array.draw(GL_TRIANGLES);
    gl_grid_vertex_array.unbind();

    _gl_grid_pipeline.unuse();
}

void GlRenderer::_draw_lights(
    const GLsizeiptr light_t_size,
    const int max_light_count,
    const int light_count_size_aligned,
    GlUniformBuffer& gl_light_uniform_buffer,
    const GlVertexArray& gl_light_shape_vertex_array,
    const glm::mat4& camera_view,
    const glm::mat4& camera_proj
)
{
    const int light_type_none = 0;
    const int light_type_spot = 1;
    const int light_type_direct = 2;
    const int light_type_area = 3;
    const int light_type_point = 4;

    _gl_shape_pipeline.use();
    _gl_shape_pipeline
        .set_uniform("u_view_mat", camera_view)
        .set_uniform("u_proj_mat", camera_proj);
    gl_light_shape_vertex_array.bind();

    GLsizeiptr light_count = 0;
    for(const auto& [position, direction, light] : _gl_render_unit_lights)
    {
        if(light_count > max_light_count)
        {
            throw too_many_lights{ std::format("The max light count is {}",max_light_count) };
        }

        match<void>(
            light,
            [&](const NullLight& light)
        {},
            [&](const SpotLight& light)
        {
            GLintptr base_offfset = light_count_size_aligned + light_count * light_t_size;
            gl_light_uniform_buffer.sub_data(base_offfset + 0, sizeof(int), &light_type_spot);
            float cosine_inner_cutoff_angle = glm::cos(light.cutoff_angles.x);
            float cosine_outer_cutoff_angle = glm::cos(light.cutoff_angles.y);
            gl_light_uniform_buffer
                .sub_data(base_offfset + 4, sizeof(float), &cosine_inner_cutoff_angle)
                .sub_data(base_offfset + 8, sizeof(float), &cosine_outer_cutoff_angle)
                .sub_data(base_offfset + 16, sizeof(glm::vec3), &position)
                .sub_data(base_offfset + 32, sizeof(glm::vec3), &direction)
                .sub_data(base_offfset + 48, sizeof(glm::vec3), &light.attenuation_coefs)
                .sub_data(base_offfset + 64, sizeof(glm::vec3), &light.color)
                .sub_data(base_offfset + 80, sizeof(glm::vec3), &light.ambient_strength)
                .sub_data(base_offfset + 96, sizeof(glm::vec3), &light.diffuse_strength)
                .sub_data(base_offfset + 112, sizeof(glm::vec3), &light.specular_strength);
            ++light_count;

            _gl_shape_pipeline
                .set_uniform("u_transform_mat", glm::translate(mat4::identity(), position))
                .set_uniform("u_color", light.color);
        },
            [&](const DirectLight& light)
        {
            GLintptr base_offfset = light_count_size_aligned + light_count * light_t_size;
            gl_light_uniform_buffer
                .sub_data(base_offfset + 0, sizeof(int), &light_type_direct)
                .sub_data(base_offfset + 32, sizeof(glm::vec3), &direction)
                .sub_data(base_offfset + 64, sizeof(glm::vec3), &light.color)
                .sub_data(base_offfset + 80, sizeof(glm::vec3), &light.ambient_strength)
                .sub_data(base_offfset + 96, sizeof(glm::vec3), &light.diffuse_strength)
                .sub_data(base_offfset + 112, sizeof(glm::vec3), &light.specular_strength);
            ++light_count;
        },
            [&](const AreaLight& light)
        {},
            [&](const PointLight& light)
        {
            GLintptr base_offfset = light_count_size_aligned + light_count * light_t_size;
            gl_light_uniform_buffer
                .sub_data(base_offfset + 0, sizeof(int), &light_type_point)
                .sub_data(base_offfset + 16, sizeof(glm::vec3), &position)
                .sub_data(base_offfset + 48, sizeof(glm::vec3), &light.attenuation_coefs)
                .sub_data(base_offfset + 64, sizeof(glm::vec3), &light.color)
                .sub_data(base_offfset + 80, sizeof(glm::vec3), &light.ambient_strength)
                .sub_data(base_offfset + 96, sizeof(glm::vec3), &light.diffuse_strength)
                .sub_data(base_offfset + 112, sizeof(glm::vec3), &light.specular_strength);
            ++light_count;

            _gl_shape_pipeline
                .set_uniform("u_transform_mat", glm::translate(mat4::identity(), position))
                .set_uniform("u_color", light.color);
        }
        );

        gl_light_shape_vertex_array.draw(GL_TRIANGLES);

    }
    gl_light_shape_vertex_array.unbind();
    _gl_shape_pipeline.unuse();
    gl_light_uniform_buffer.sub_data(0, sizeof(int), &light_count);
    gl_light_uniform_buffer.bind_buffer_base(0);
}

void GlRenderer::_draw_models(
    const glm::mat4& camera_view,
    const glm::mat4& camera_proj,
    const glm::vec3& camera_pos
)
{
    _gl_model_pipeline.use();
    auto tex_uniform_index = 0;
    _gl_model_pipeline
        .set_uniform("u_view_mat", camera_view)
        .set_uniform("u_proj_mat", camera_proj)
        .set_uniform("u_view_pos", camera_pos);
    for(auto& [Uuid, gl_meshes] : _gl_render_unit_mesh_storage)
    {
        for(auto& [
            gl_vertex_array,
                transform_mat4,
                gl_texture2d_ambient,
                gl_texture2d_diffuse,
                gl_texture2d_specular
        ] : gl_meshes)
        {
            _gl_model_pipeline
                .set_uniform("u_transform_mat", transform_mat4)
                .set_uniform("u_normal_mat", glm::mat3{ glm::transpose(glm::inverse(transform_mat4)) });

            _gl_model_pipeline.set_uniform("u_material.ambient", tex_uniform_index);
            gl_texture2d_ambient.bind(tex_uniform_index++);

            _gl_model_pipeline.set_uniform("u_material.diffuse", tex_uniform_index);
            gl_texture2d_diffuse.bind(tex_uniform_index++);

            _gl_model_pipeline.set_uniform("u_material.specular", tex_uniform_index);
            gl_texture2d_specular.bind(tex_uniform_index++);

            _gl_model_pipeline.set_uniform("u_material.shininess", 32.f);

            gl_vertex_array.bind();
            gl_vertex_array.draw(GL_TRIANGLES);
            gl_vertex_array.unbind();

            gl_texture2d_ambient.unbind();
            gl_texture2d_diffuse.unbind();
            gl_texture2d_specular.unbind();

        }
    }
    _gl_model_pipeline.unuse();
}

void GlRenderer::_draw_skybox(
    const glm::mat4& camera_view,
    const glm::mat4& camera_proj
)
{
    _gl_skybox_pipeline.use();
    auto& [gl_vartex_array, gl_cubemap] = *_gl_render_unit_skybox;

    _gl_skybox_pipeline.set_uniform("u_skybox", 0);
    gl_cubemap.bind(0);

    _gl_skybox_pipeline
        .set_uniform("u_view_mat", glm::mat4{ glm::mat3{camera_view} })
        .set_uniform("u_proj_mat", camera_proj);

    gl_vartex_array.bind();
    ACDA_GL_CALL(glDepthFunc(GL_LEQUAL));
    gl_vartex_array.draw(GL_TRIANGLES);
    ACDA_GL_CALL(glDepthFunc(GL_LESS));
    gl_vartex_array.unbind();

    _gl_skybox_pipeline.unuse();
}

void GlRenderer::_draw_physics_body_shape(
    const glm::mat4& camera_view,
    const glm::mat4& camera_proj
)
{
    _gl_shape_pipeline.use();
    for(const auto& [gl_vertex_arrray, transform_mat, color] : _gl_render_unit_physics_body_shape_storage | std::views::values)
    {
        _gl_shape_pipeline
            .set_uniform("u_transform_mat", transform_mat)
            .set_uniform("u_view_mat", camera_view)
            .set_uniform("u_proj_mat", camera_proj)
            .set_uniform("u_color", color);

        gl_vertex_arrray.bind();
        gl_vertex_arrray.draw(GL_TRIANGLES);
        gl_vertex_arrray.unbind();
    }
    _gl_shape_pipeline.unuse();
}




