#include "pch.hpp"

#include "mesh.hpp"

auto Arcadia::Mesh::CreateBox(
    const glm::vec3& half_extent
) -> Mesh
{
    float x = half_extent.x;
    float y = half_extent.y;
    float z = half_extent.z;

    Mesh mesh{};
    std::vector<Vertex>& vertices = mesh.Vertices;
    vertices.reserve(36);

    // pos-x
    vertices.emplace_back(
        Vertex{
            glm::vec3{x, y, z},
            GlmVec3::CreateUnitPositiveX(),
            glm::vec2{.5f,.67f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{x, -y,z},
            GlmVec3::CreateUnitPositiveX(),
            glm::vec2{.5f,.33f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{x,y,-z},
            GlmVec3::CreateUnitPositiveX(),
            glm::vec2{.75f,.67f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{x,y,-z},
            GlmVec3::CreateUnitPositiveX(),
            glm::vec2{.75f,.67f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{x,-y,z},
            GlmVec3::CreateUnitPositiveX(),
            glm::vec2{.5f,.33f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{x,-y,-z},
            GlmVec3::CreateUnitPositiveX(),
            glm::vec2{.75f,.33f}
        }
    );

    // neg-x
    vertices.emplace_back(
        Vertex{
            glm::vec3{-x, y, -z},
            GlmVec3::CreateUnitNegativeX(),
            glm::vec2{.0f,.67f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{-x, -y,-z},
            GlmVec3::CreateUnitNegativeX(),
            glm::vec2{.0f,.33f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{-x,y,z},
            GlmVec3::CreateUnitNegativeX(),
            glm::vec2{.25f,.67f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{-x,y,z},
            GlmVec3::CreateUnitNegativeX(),
            glm::vec2{.25f,.67f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{-x,-y,-z},
            GlmVec3::CreateUnitNegativeX(),
            glm::vec2{.0f,.33f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{-x,-y,z},
            GlmVec3::CreateUnitPositiveY(),
            glm::vec2{.25f,.33f}
        }
    );

    // pos-y
    vertices.emplace_back(
        Vertex{
            glm::vec3{-x, y, -z},
            GlmVec3::CreateUnitPositiveY(),
            glm::vec2{.25f,1.f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{-x, y,z},
            GlmVec3::CreateUnitPositiveY(),
            glm::vec2{.25f,.67f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{x,y,-z},
            GlmVec3::CreateUnitPositiveY(),
            glm::vec2{.5f,1.f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{x,y,-z},
            GlmVec3::CreateUnitPositiveY(),
            glm::vec2{.5f,1.f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{-x,y,z},
            GlmVec3::CreateUnitPositiveY(),
            glm::vec2{.25f,.67f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{x,y,z},
            GlmVec3::CreateUnitPositiveY(),
            glm::vec2{.5f,.67f}
        }
    );

    // neg-y
    vertices.emplace_back(
        Vertex{
            glm::vec3{-x, -y, z},
            GlmVec3::CreateUnitNegativeY(),
            glm::vec2{.25f,.33f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{-x, -y,-z},
            GlmVec3::CreateUnitNegativeY(),
            glm::vec2{.25f,.0f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{x,-y,z},
            GlmVec3::CreateUnitNegativeY(),
            glm::vec2{.25f,.5f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{x,-y,z},
            GlmVec3::CreateUnitNegativeY(),
            glm::vec2{.25f,.5f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{-x,-y,-z},
            GlmVec3::CreateUnitNegativeY(),
            glm::vec2{.25f,.0f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{x,-y,-z},
            GlmVec3::CreateUnitNegativeY(),
            glm::vec2{.5f,.0f}
        }
    );

    // pos-z
    vertices.emplace_back(
        Vertex{
            glm::vec3{-x, y, z},
            GlmVec3::CreateUnitPositiveZ(),
            glm::vec2{.25f,.67f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{-x, -y,z},
            GlmVec3::CreateUnitPositiveZ(),
            glm::vec2{.25f,.33f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{x,y,z},
            GlmVec3::CreateUnitPositiveZ(),
            glm::vec2{.5f,.67f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{x,y,z},
            GlmVec3::CreateUnitPositiveZ(),
            glm::vec2{.5f,.67f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{-x, -y,z},
            GlmVec3::CreateUnitPositiveZ(),
            glm::vec2{.25f,.33f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{x,-y,z},
            GlmVec3::CreateUnitPositiveZ(),
            glm::vec2{.5f,.33f}
        }
    );

    // neg-z
    vertices.emplace_back(
        Vertex{
            glm::vec3{x, y, -z},
            GlmVec3::CreateUnitNegativeZ(),
            glm::vec2{.75f,.67f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{x, -y,-z},
            GlmVec3::CreateUnitNegativeZ(),
            glm::vec2{.75f,.33f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{-x,y,-z},
            GlmVec3::CreateUnitNegativeZ(),
            glm::vec2{1.f,.67f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{-x,y,-z},
            GlmVec3::CreateUnitNegativeZ(),
            glm::vec2{1.f,.67f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{x, -y,-z},
            GlmVec3::CreateUnitNegativeZ(),
            glm::vec2{.75f,.33f}
        }
    );
    vertices.emplace_back(
        Vertex{
            glm::vec3{-x,-y,-z},
            GlmVec3::CreateUnitNegativeZ(),
            glm::vec2{1.f,.33f}
        }
    );

    return mesh;
}

auto Arcadia::Mesh::CreateCapsule(
    float radius,
    float half_height_of_cylinder,
    std::size_t half_sphere_stack_count,
    std::size_t sector_count
) -> Mesh
{
    const float pi = glm::pi<float>();
    float sector_step = 2 * pi / sector_count;
    float half_sphere_stack_step = pi / 2 / half_sphere_stack_count;

    float radius_inv = 1.f / radius;

    Mesh mesh{};
    std::vector<Vertex>& vertices = mesh.Vertices;
    std::vector<unsigned int>& indices = mesh.Indices;

#if 1
    // Sphere part
    for(std::size_t j = 0; j <= half_sphere_stack_count; ++j)
    {
        float stack_angle = pi / 2 - j * half_sphere_stack_step;
        float xz = radius * std::cos(stack_angle);
        float y = radius * std::sin(stack_angle) + half_height_of_cylinder;

        for(std::size_t j = 0; j <= sector_count; ++j)
        {
            float sector_angle = j * sector_step;

            float x = xz * std::sin(sector_angle);
            float z = xz * std::cos(sector_angle);

            float nx = x * radius_inv;
            float ny = y * radius_inv;
            float nz = z * radius_inv;

            float u = static_cast<float>(j / sector_count);
            float v = static_cast<float>(j / half_sphere_stack_count);

            vertices.emplace_back(
                Vertex{
                    glm::vec3{x,y,z},
                    glm::vec3{nx,ny,nz},
                    glm::vec2{u,v}
                }
            );
        }
    }

    for(std::size_t i = 0; i < half_sphere_stack_count; ++i)
    {
        float k1 = i * (sector_count + 1);
        float k2 = k1 + sector_count + 1;

        for(std::size_t j = 0; j < sector_count; ++j, ++k1, ++k2)
        {
            if(i != 0)
            {
                indices.emplace_back(k1);
                indices.emplace_back(k2);
                indices.emplace_back(k1 + 1);
            }

            if(i != half_sphere_stack_count - 1)
            {
                indices.emplace_back(k1 + 1);
                indices.emplace_back(k2);
                indices.emplace_back(k2 + 1);
            }
        }
    }
#endif

#if 1
    // Cylinder part
    std::vector<float> unit_circle_vertices{};
    for(std::size_t i = 0; i <= sector_count; ++i)
    {
        float sector_angle = i * sector_step;
        unit_circle_vertices.emplace_back(std::sin(sector_angle));
        unit_circle_vertices.emplace_back(0);
        unit_circle_vertices.emplace_back(std::cos(sector_angle));
    }

    std::size_t k1 = vertices.size();
    std::size_t k2 = k1 + sector_count + 1;
    for(std::size_t i = 0; i < 2; ++i)
    {
        float h = -half_height_of_cylinder + i * half_height_of_cylinder * 2;
        float tex_coord_y = 1.f - i;

        for(std::size_t j = 0, k = 0; j <= sector_count; ++j, k += 3)
        {
            float ux = unit_circle_vertices.at(k);
            float uy = unit_circle_vertices.at(k + 1);
            float uz = unit_circle_vertices.at(k + 2);

            vertices.emplace_back(
                Vertex{
                    glm::vec3{ux * radius,h, uz * radius},
                    glm::vec3{ux,uy,uz},
                    glm::vec2{static_cast<float>(j) / sector_count, tex_coord_y}
                }
            );
        }
    }

    for(std::size_t i = 0; i < sector_count; ++i, ++k1, ++k2)
    {
        indices.emplace_back(k1);
        indices.emplace_back(k1 + 1);
        indices.emplace_back(k2);

        indices.emplace_back(k2);
        indices.emplace_back(k1 + 1);
        indices.emplace_back(k2 + 1);
    }
#endif

    return mesh;
}

auto Arcadia::Mesh::CreateCylinder(
    float half_height,
    float radius,
    std::size_t sector_count
)->Mesh
{
    const auto pi = glm::pi<float>();
    float sector_step = 2 * pi / sector_count;

    std::vector<float> unit_circle_vertices{};
    for(std::size_t i = 0; i <= sector_count; ++i)
    {
        float sector_angle = i * sector_step;
        unit_circle_vertices.emplace_back(std::sin(sector_angle));
        unit_circle_vertices.emplace_back(0);
        unit_circle_vertices.emplace_back(std::cos(sector_angle));
    }

    Mesh mesh{};
    std::vector<Vertex>& vertices = mesh.Vertices;

    for(std::size_t i = 0; i < 2; ++i)
    {
        float h = -half_height + i * half_height * 2;
        float tex_coord_y = 1.f - i;

        for(std::size_t j = 0, k = 0; j <= sector_count; ++j, k += 3)
        {
            float ux = unit_circle_vertices.at(k);
            float uy = unit_circle_vertices.at(k + 1);
            float uz = unit_circle_vertices.at(k + 2);

            vertices.emplace_back(
                Vertex{
                    glm::vec3{ux * radius,h, uz * radius},
                    glm::vec3{ux,uy,uz},
                    glm::vec2{static_cast<float>(j) / sector_count, tex_coord_y}
                }
            );
        }
    }
    std::size_t base_center_index = vertices.size();
    std::size_t top_center_index = base_center_index + sector_count + 1;
    for(std::size_t i = 0; i < 2; ++i)
    {
        float h = -half_height + i * half_height * 2;
        float ny = -1 + i * 2;

        // Center point
        vertices.emplace_back(
            Vertex{
                glm::vec3{0,h,0},
                glm::vec3{0,ny,0},
                glm::vec2{0.5f,0.5f}
            }
        );

        for(std::size_t j = 0, k = 0; j < sector_count; ++j, k += 3)
        {
            float ux = unit_circle_vertices.at(k);
            float uz = unit_circle_vertices.at(k + 2);

            vertices.emplace_back(
                Vertex{
                    glm::vec3{ux * radius, h, uz * radius},
                    glm::vec3{0, ny ,0},
                    glm::vec2{-ux * 0.5f + 0.5f, -uz * 0.5f + 0.5f}
                }
            );
        }
    }

    std::vector<unsigned int>& indices = mesh.Indices;
    std::size_t k1 = 0;
    std::size_t k2 = sector_count + 1;
    for(std::size_t i = 0; i < sector_count; ++i, ++k1, ++k2)
    {
        indices.emplace_back(k1);
        indices.emplace_back(k1 + 1);
        indices.emplace_back(k2);

        indices.emplace_back(k2);
        indices.emplace_back(k1 + 1);
        indices.emplace_back(k2 + 1);
    }
    for(std::size_t i = 0, k = base_center_index + 1; i < sector_count; ++i, ++k)
    {
        if(i < sector_count - 1)
        {
            indices.emplace_back(base_center_index);
            indices.emplace_back(k + 1);
            indices.emplace_back(k);
        }
        else
        {
            indices.emplace_back(base_center_index);
            indices.emplace_back(base_center_index + 1);
            indices.emplace_back(k);
        }
    }
    for(std::size_t i = 0, k = top_center_index + 1; i < sector_count; ++i, ++k)
    {
        if(i < sector_count - 1)
        {
            indices.emplace_back(top_center_index);
            indices.emplace_back(k);
            indices.emplace_back(k + 1);
        }
        else
        {
            indices.emplace_back(top_center_index);
            indices.emplace_back(k);
            indices.emplace_back(top_center_index + 1);
        }
    }

    return mesh;
}

auto Arcadia::Mesh::CreateSphere(
    float radius,
    std::size_t stack_count,
    std::size_t sector_count
) -> Mesh
{
    const float pi = glm::pi<float>();
    float sector_step = 2 * pi / sector_count;
    float stack_step = pi / stack_count;

    float radius_inv = 1.f / radius;

    Mesh mesh{};

    std::vector<Vertex>& vertices = mesh.Vertices;
    for(std::size_t i = 0; i <= stack_count; ++i)
    {
        float stack_angle = pi / 2 - i * stack_step;
        float xz = radius * std::cos(stack_angle);
        float y = radius * std::sin(stack_angle);

        for(std::size_t j = 0; j <= sector_count; ++j)
        {
            float sector_angle = j * sector_step;

            float x = xz * std::sin(sector_angle);
            float z = xz * std::cos(sector_angle);

            float nx = x * radius_inv;
            float ny = y * radius_inv;
            float nz = z * radius_inv;

            float u = static_cast<float>(j / sector_count);
            float v = static_cast<float>(j / stack_count);

            vertices.emplace_back(
                Vertex{
                    glm::vec3{x,y,z},
                    glm::vec3{nx,ny,nz},
                    glm::vec2{u,v}
                }
            );
        }
    }

    std::vector<unsigned int>& indices = mesh.Indices;
    for(std::size_t i = 0; i < stack_count; ++i)
    {
        float k1 = i * (sector_count + 1);
        float k2 = k1 + sector_count + 1;

        for(std::size_t j = 0; j < sector_count; ++j, ++k1, ++k2)
        {
            if(i != 0)
            {
                indices.emplace_back(k1);
                indices.emplace_back(k2);
                indices.emplace_back(k1 + 1);
            }

            if(i != stack_count - 1)
            {
                indices.emplace_back(k1 + 1);
                indices.emplace_back(k2);
                indices.emplace_back(k2 + 1);
            }
        }
    }

    return mesh;
}