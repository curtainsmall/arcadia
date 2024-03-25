#include "pch.hpp"
#include "mesh.hpp"

auto arcadia::mesh::box(
    const glm::vec3& half_extent
) -> arcadia::mesh
{
    auto x = half_extent.x;
    auto y = half_extent.y;
    auto z = half_extent.z;

    arcadia::mesh mesh{};
    auto& vertices = mesh.vertices;
    vertices.reserve(36);

    // pos-x
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{x, y, z},
            arcadia::vec3::pos_unit_x(),
            glm::vec2{.5f,.67f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{x, -y,z},
            arcadia::vec3::pos_unit_x(),
            glm::vec2{.5f,.33f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{x,y,-z},
            arcadia::vec3::pos_unit_x(),
            glm::vec2{.75f,.67f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{x,y,-z},
            arcadia::vec3::pos_unit_x(),
            glm::vec2{.75f,.67f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{x,-y,z},
            arcadia::vec3::pos_unit_x(),
            glm::vec2{.5f,.33f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{x,-y,-z},
            arcadia::vec3::pos_unit_x(),
            glm::vec2{.75f,.33f}
        }
    );

    // neg-x
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{-x, y, -z},
            arcadia::vec3::neg_unit_x(),
            glm::vec2{.0f,.67f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{-x, -y,-z},
            arcadia::vec3::neg_unit_x(),
            glm::vec2{.0f,.33f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{-x,y,z},
            arcadia::vec3::neg_unit_x(),
            glm::vec2{.25f,.67f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{-x,y,z},
            arcadia::vec3::neg_unit_x(),
            glm::vec2{.25f,.67f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{-x,-y,-z},
            arcadia::vec3::neg_unit_x(),
            glm::vec2{.0f,.33f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{-x,-y,z},
            arcadia::vec3::pos_unit_y(),
            glm::vec2{.25f,.33f}
        }
    );

    // pos-y
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{-x, y, -z},
            arcadia::vec3::pos_unit_y(),
            glm::vec2{.25f,1.f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{-x, y,z},
            arcadia::vec3::pos_unit_y(),
            glm::vec2{.25f,.67f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{x,y,-z},
            arcadia::vec3::pos_unit_y(),
            glm::vec2{.5f,1.f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{x,y,-z},
            arcadia::vec3::pos_unit_y(),
            glm::vec2{.5f,1.f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{-x,y,z},
            arcadia::vec3::pos_unit_y(),
            glm::vec2{.25f,.67f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{x,y,z},
            arcadia::vec3::pos_unit_y(),
            glm::vec2{.5f,.67f}
        }
    );

    // neg-y
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{-x, -y, z},
            arcadia::vec3::neg_unit_y(),
            glm::vec2{.25f,.33f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{-x, -y,-z},
            arcadia::vec3::neg_unit_y(),
            glm::vec2{.25f,.0f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{x,-y,z},
            arcadia::vec3::neg_unit_y(),
            glm::vec2{.25f,.5f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{x,-y,z},
            arcadia::vec3::neg_unit_y(),
            glm::vec2{.25f,.5f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{-x,-y,-z},
            arcadia::vec3::neg_unit_y(),
            glm::vec2{.25f,.0f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{x,-y,-z},
            arcadia::vec3::neg_unit_y(),
            glm::vec2{.5f,.0f}
        }
    );

    // pos-z
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{-x, y, z},
            arcadia::vec3::pos_unit_z(),
            glm::vec2{.25f,.67f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{-x, -y,z},
            arcadia::vec3::pos_unit_z(),
            glm::vec2{.25f,.33f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{x,y,z},
            arcadia::vec3::pos_unit_z(),
            glm::vec2{.5f,.67f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{x,y,z},
            arcadia::vec3::pos_unit_z(),
            glm::vec2{.5f,.67f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{-x, -y,z},
            arcadia::vec3::pos_unit_z(),
            glm::vec2{.25f,.33f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{x,-y,z},
            arcadia::vec3::pos_unit_z(),
            glm::vec2{.5f,.33f}
        }
    );

    // neg-z
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{x, y, -z},
            arcadia::vec3::neg_unit_z(),
            glm::vec2{.75f,.67f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{x, -y,-z},
            arcadia::vec3::neg_unit_z(),
            glm::vec2{.75f,.33f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{-x,y,-z},
            arcadia::vec3::neg_unit_z(),
            glm::vec2{1.f,.67f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{-x,y,-z},
            arcadia::vec3::neg_unit_z(),
            glm::vec2{1.f,.67f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{x, -y,-z},
            arcadia::vec3::neg_unit_z(),
            glm::vec2{.75f,.33f}
        }
    );
    vertices.emplace_back(
        arcadia::vertex{
            glm::vec3{-x,-y,-z},
            arcadia::vec3::neg_unit_z(),
            glm::vec2{1.f,.33f}
        }
    );

    return mesh;

}

auto arcadia::mesh::capsule(
    float radius,
    float half_height_of_sylinder
) -> arcadia::mesh
{
    return arcadia::mesh();
}

auto arcadia::mesh::cylinder(
    float half_height,
    float radius
) -> arcadia::mesh
{
    return arcadia::mesh{};
}

auto arcadia::mesh::sphere(
    float radius,
    std::size_t stack_count,
    std::size_t sector_count
) -> arcadia::mesh
{
    const auto pi = glm::pi<float>();
    float sector_step = 2 * pi / sector_count;
    float stack_step = pi / sector_count;

    float radius_inv = 1.f / radius;

    arcadia::mesh mesh{};

    auto& vertices = mesh.vertices;
    vertices.reserve(stack_count * sector_count);
    for(std::size_t i = 0; i < stack_count; ++i)
    {
        float stack_angle = pi / 2 - i * stack_step;
        float xy = radius * std::cos(stack_angle);
        float z = radius * std::sin(stack_angle);

        for(std::size_t j = 0; j <= sector_count; ++j)
        {
            float sector_angle = j * sector_step;

            float x = xy * std::cos(sector_angle);
            float y = xy * std::sin(sector_angle);

            float nx = x * radius_inv;
            float ny = y * radius_inv;
            float nz = z * radius_inv;

            float u = static_cast<float>(j / sector_count);
            float v = static_cast<float>(j / stack_count);

            vertices.emplace_back(
                arcadia::vertex{
                    glm::vec3{x,y,z},
                    glm::vec3{nx,ny,nz},
                    glm::vec2{u,v}
                }
            );
        }
    }

    auto& indices = mesh.indices;
    indices.reserve(stack_count * sector_count);
    for(std::size_t i = 0; i < stack_count; ++i)
    {
        float k1 = i * (sector_count + 1);
        float k2 = i + sector_count + 1;

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
