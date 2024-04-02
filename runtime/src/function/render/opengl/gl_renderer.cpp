#include "pch.hpp"
#include "gl_renderer.hpp"

#include<vector>

arcadia::gl_renderer::gl_renderer(const std::filesystem::path& gl_shader_folder_path):
    _gl_model_pipeline(gl_shader_folder_path, arcadia::get_model_shaders_builder()),
    _gl_skybox_pipeline(gl_shader_folder_path, arcadia::get_skybox_shaders_builder()),
    _gl_grid_pipeline(gl_shader_folder_path, arcadia::get_grid_shaders_builder()),
    _gl_shape_pipeline(gl_shader_folder_path, arcadia::get_shape_shaders_builder())
{
    ARCADIA_GL_CALL(glEnable(GL_DEPTH_TEST));
    ARCADIA_GL_CALL(glEnable(GL_CULL_FACE));
}

void arcadia::gl_renderer::prepare()
{
    _assert_frame_not_in_build();
    _in_build = true;

    // Clear submitted meshes uuids
    _submitted_meshes_uuid_set.clear();

    // Clear cameras
    _gl_render_unit_cameras.clear();

    // Clear lights
    _gl_render_unit_lights.clear();

    // Clear skybox
    _gl_render_unit_skybox_opt.reset();

    // Clear submitted physics body shape uuids
    _submitted_physcis_body_shape_uuid_set.clear();

}

void arcadia::gl_renderer::finalize()
{
    _assert_frame_in_build();
    _in_build = false;

    for(auto iter = _gl_render_unit_meshes_umap.begin(); iter != _gl_render_unit_meshes_umap.end();)
    {
        if(!_submitted_meshes_uuid_set.contains(iter->first))
        {
            iter = _gl_render_unit_meshes_umap.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    for(auto iter = _gl_render_unit_physics_body_shape_umap.begin(); iter != _gl_render_unit_physics_body_shape_umap.end();)
    {
        if(!_submitted_physcis_body_shape_uuid_set.contains(iter->first))
        {
            iter = _gl_render_unit_physics_body_shape_umap.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void arcadia::gl_renderer::submit(const arcadia::camera_component& camera_comp)
{
    _assert_frame_in_build();

    _gl_render_unit_cameras.emplace_back(
        arcadia::gl_framebuffer{
            camera_comp.viewport_size,
            camera_comp.near_plane,
            camera_comp.far_plane
        },
        camera_comp.viewport_size,
        camera_comp.gen_view_mat4(),
        camera_comp.gen_proj_mat4(),
        camera_comp.position,
        camera_comp.should_display_grid,
        camera_comp.near_plane,
        camera_comp.far_plane
    );
}

void arcadia::gl_renderer::submit(const arcadia::light_component& light_comp)
{
    _assert_frame_in_build();

    _gl_render_unit_lights.emplace_back(light_comp.light);
}

void arcadia::gl_renderer::submit(const arcadia::model_component& model_comp)
{
    _assert_frame_in_build();

    if(model_comp.has_identifiable_meshes())
    {
        const auto& [uuid, meshes] = model_comp.get_identifiable_meshes();

        auto transform_mat =
            // Translate
            glm::translate(
                // Move pivot back from origin
                glm::translate(
                    // Rotate
                    glm::mat4_cast(model_comp.rotation)
                    // Scale about origin (same as pivot)
                    * glm::scale(
                        // Move pivot to origin
                        glm::translate(
                            arcadia::mat4::identity(),
                            -model_comp.pivot
                        ),
                        model_comp.scale
                    ),
                    model_comp.pivot
                ),
                model_comp.location
            );

        if(!_gl_render_unit_meshes_umap.contains(uuid))
        {

            std::vector<arcadia::gl_render_unit_mesh> gl_meshes{};
            for(const auto& mesh : meshes)
            {
                // For any uuid, its corresponding meshes must be the same
                gl_meshes.emplace_back(
                    arcadia::gl_vertex_array{ mesh.vertices, mesh.indices },
                    transform_mat,
                    mesh.material.ambient_texture2d,
                    mesh.material.diffuse_texture2d,
                    mesh.material.specular_texture2d
                );
            }
            _gl_render_unit_meshes_umap.try_emplace(uuid, std::move(gl_meshes));
        }
        else
        {
            for(auto& gl_render_unit_mesh : _gl_render_unit_meshes_umap.at(uuid))
            {
                std::get<1>(gl_render_unit_mesh) = transform_mat;
            }
        }

        _submitted_meshes_uuid_set.emplace(uuid);
    }
}

void arcadia::gl_renderer::submit(const arcadia::skybox_component& skybox_comp)
{
    _assert_frame_in_build();

    const auto skybox_box_shape = arcadia::mesh::box(glm::vec3{ 1,1,1 });
    _gl_render_unit_skybox_opt.emplace(
        arcadia::gl_vertex_array{ skybox_box_shape.vertices, skybox_box_shape.indices },
        skybox_comp.cubemap
    );
}

void arcadia::gl_renderer::submit(const arcadia::physics_component& physcis_comp)
{
    _assert_frame_in_build();

    if(physcis_comp.has_body_info())
    {
        const auto& [uuid, jph_body] = physcis_comp.get_identifiable_jph_body_info_initial();
        if(!_gl_render_unit_physics_body_shape_umap.contains(uuid))
        {
            const auto& shape_info = jph_body.jph_shape_info;
            const auto& shape_mesh = arcadia::match<arcadia::mesh>(
                shape_info,
                [&](const arcadia::jph_box_shape_info& info)
            {
                return arcadia::mesh::box(info.half_extent);
            },
                [&](const arcadia::jph_capsule_shape_info& info)
            {
                return arcadia::mesh{};
            },
                [&](const arcadia::jph_cylinder_shape_info& info)
            {
                return arcadia::mesh{};
            },
                [&](const arcadia::jph_sphere_shape_info& info)
            {
                return arcadia::mesh::sphere(info.radius);
            }
            );

            _gl_render_unit_physics_body_shape_umap.try_emplace(
                uuid, arcadia::gl_vertex_array{ shape_mesh.vertices,shape_mesh.indices },
                glm::mat4{},
                glm::vec3{}
            );
        }

        const auto& body_info_ongoing = physcis_comp.get_jph_body_info_ongoing();
        auto& [gl_vertex_array, transform_mat, color] = _gl_render_unit_physics_body_shape_umap.at(uuid);
        transform_mat = glm::translate(
            glm::mat4_cast(body_info_ongoing.rotation),
            body_info_ongoing.position
        );
        color = physcis_comp.body_shape_color;

        _submitted_physcis_body_shape_uuid_set.emplace(uuid);
    }
}

void arcadia::gl_renderer::draw()
{
    _assert_frame_not_in_build();


    if(_gl_render_unit_cameras.empty())
    {
        throw draw_fail{ "No framebuffer to draw to" };
    }


    ARCADIA_GL_CALL(glClearColor(41 / 255.0, 43 / 255.0, 44 / 255.0, 1.f));

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
        ARCADIA_GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
        ARCADIA_GL_CALL(glViewport(0, 0, viewport_size.x, viewport_size.y));

        // Draw grid
        if(should_display_grid)
        {
            std::vector<arcadia::vertex> grid_vertices{
                   arcadia::vertex{ glm::vec3{-1,1,0} },
                    arcadia::vertex{ glm::vec3{-1,-1,0} },
                    arcadia::vertex{ glm::vec3{1,-1,0} },
                    arcadia::vertex{ glm::vec3{1,1,0} }
            };
            std::vector<arcadia::mesh::index_type> grid_indices{
                0,1,2,
                2,3,0
            };
            arcadia::gl_vertex_array gl_grid_vertex_array{ grid_vertices, grid_indices };

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
        arcadia::gl_uniform_buffer gl_uniform_buffer{ light_count_size_aligned + light_t_size * max_light_count };

        auto light_box_shape = arcadia::mesh::box(glm::vec3{ 1,1,1 });
        arcadia::gl_vertex_array gl_light_box_shape_vertex_array{ light_box_shape.vertices ,light_box_shape.indices };

        _draw_lights(
            light_t_size,
            max_light_count,
            light_count_size_aligned,
            gl_uniform_buffer,
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
        if(_gl_render_unit_skybox_opt)
        {
            _draw_skybox(
                camera_view,
                camera_proj
            );
        }

        gl_framebuffer.unbind();
    }

}

void arcadia::gl_renderer::reset()
{
    _gl_render_unit_cameras.clear();
    _gl_render_unit_lights.clear();
    _gl_render_unit_meshes_umap.clear();
    _submitted_meshes_uuid_set.clear();
    _gl_render_unit_skybox_opt.reset();
}

auto arcadia::gl_renderer::get_render_result_id(std::size_t index) const -> void*
{
    return reinterpret_cast<void*>(std::get<0>(_gl_render_unit_cameras.at(index)).get_gl_texture2d().get_gl_id());
}

void arcadia::gl_renderer::_assert_frame_in_build() const
{
    ARCADIA_ASSERT(_in_build && "Frame is not in build, did you call `prepare()`?");
}

void arcadia::gl_renderer::_assert_frame_not_in_build() const
{
    ARCADIA_ASSERT(!_in_build && "Frame is in build, did you call `finalize()`?");
}

void arcadia::gl_renderer::_draw_grid(
    const arcadia::gl_vertex_array& gl_grid_vertex_array,
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

void arcadia::gl_renderer::_draw_lights(
    const GLsizeiptr light_t_size,
    const int max_light_count,
    const int light_count_size_aligned,
    arcadia::gl_uniform_buffer& gl_light_uniform_buffer,
    const arcadia::gl_vertex_array& gl_light_shape_vertex_array,
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
    for(const auto& [light] : _gl_render_unit_lights)
    {
        if(light_count > max_light_count)
        {
            throw too_many_lights{ std::format("The max light count is {}",max_light_count) };
        }

        arcadia::match<void>(
            light,
            [&](const arcadia::null_light& light)
        {},
            [&](const arcadia::spot_light& light)
        {
            GLintptr base_offfset = light_count_size_aligned + light_count * light_t_size;
            gl_light_uniform_buffer.sub_data(base_offfset + 0, sizeof(int), &light_type_spot);
            float cosine_inner_cutoff_angle = glm::cos(light.cutoff_angles.x);
            float cosine_outer_cutoff_angle = glm::cos(light.cutoff_angles.y);
            gl_light_uniform_buffer
                .sub_data(base_offfset + 4, sizeof(float), &cosine_inner_cutoff_angle)
                .sub_data(base_offfset + 8, sizeof(float), &cosine_outer_cutoff_angle)
                .sub_data(base_offfset + 16, sizeof(glm::vec3), &light.position)
                .sub_data(base_offfset + 32, sizeof(glm::vec3), &light.direction)
                .sub_data(base_offfset + 48, sizeof(glm::vec3), &light.attenuation_coefs)
                .sub_data(base_offfset + 64, sizeof(glm::vec3), &light.color)
                .sub_data(base_offfset + 80, sizeof(glm::vec3), &light.ambient_strength)
                .sub_data(base_offfset + 96, sizeof(glm::vec3), &light.diffuse_strength)
                .sub_data(base_offfset + 112, sizeof(glm::vec3), &light.specular_strength);
            ++light_count;

            _gl_shape_pipeline
                .set_uniform("u_transform_mat", glm::translate(arcadia::mat4::identity(), light.position))
                .set_uniform("u_color", light.color);
        },
            [&](const arcadia::direct_light& light)
        {
            GLintptr base_offfset = light_count_size_aligned + light_count * light_t_size;
            gl_light_uniform_buffer
                .sub_data(base_offfset + 0, sizeof(int), &light_type_direct)
                .sub_data(base_offfset + 32, sizeof(glm::vec3), &light.direction)
                .sub_data(base_offfset + 64, sizeof(glm::vec3), &light.color)
                .sub_data(base_offfset + 80, sizeof(glm::vec3), &light.ambient_strength)
                .sub_data(base_offfset + 96, sizeof(glm::vec3), &light.diffuse_strength)
                .sub_data(base_offfset + 112, sizeof(glm::vec3), &light.specular_strength);
            ++light_count;
        },
            [&](const arcadia::area_light& light)
        {},
            [&](const arcadia::point_light& light)
        {
            GLintptr base_offfset = light_count_size_aligned + light_count * light_t_size;
            gl_light_uniform_buffer
                .sub_data(base_offfset + 0, sizeof(int), &light_type_point)
                .sub_data(base_offfset + 16, sizeof(glm::vec3), &light.position)
                .sub_data(base_offfset + 48, sizeof(glm::vec3), &light.attenuation_coefs)
                .sub_data(base_offfset + 64, sizeof(glm::vec3), &light.color)
                .sub_data(base_offfset + 80, sizeof(glm::vec3), &light.ambient_strength)
                .sub_data(base_offfset + 96, sizeof(glm::vec3), &light.diffuse_strength)
                .sub_data(base_offfset + 112, sizeof(glm::vec3), &light.specular_strength);
            ++light_count;

            _gl_shape_pipeline
                .set_uniform("u_transform_mat", glm::translate(arcadia::mat4::identity(), light.position))
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

void arcadia::gl_renderer::_draw_models(
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
    for(auto& [uuid, gl_meshes] : _gl_render_unit_meshes_umap)
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

void arcadia::gl_renderer::_draw_skybox(
    const glm::mat4& camera_view,
    const glm::mat4& camera_proj
)
{
    _gl_skybox_pipeline.use();
    auto& [gl_vertex_array, gl_cubemap] = *_gl_render_unit_skybox_opt;

    _gl_skybox_pipeline.set_uniform("u_skybox", 0);
    gl_cubemap.bind(0);

    _gl_skybox_pipeline
        .set_uniform("u_view_mat", glm::mat4{ glm::mat3{camera_view} })
        .set_uniform("u_proj_mat", camera_proj);

    gl_vertex_array.bind();
    ARCADIA_GL_CALL(glDepthFunc(GL_LEQUAL));
    gl_vertex_array.draw(GL_TRIANGLES);
    ARCADIA_GL_CALL(glDepthFunc(GL_LESS));
    gl_vertex_array.unbind();

    _gl_skybox_pipeline.unuse();
}

void arcadia::gl_renderer::_draw_physics_body_shape(
    const glm::mat4& camera_view,
    const glm::mat4& camera_proj
)
{
    _gl_shape_pipeline.use();
    for(const auto& [gl_vertex_array, transform_mat, color] : _gl_render_unit_physics_body_shape_umap | std::views::values)
    {
        _gl_shape_pipeline
            .set_uniform("u_transform_mat", transform_mat)
            .set_uniform("u_view_mat", camera_view)
            .set_uniform("u_proj_mat", camera_proj)
            .set_uniform("u_color", color);

        gl_vertex_array.bind();
        gl_vertex_array.draw(GL_TRIANGLES);
        gl_vertex_array.unbind();
    }
    _gl_shape_pipeline.unuse();
}




