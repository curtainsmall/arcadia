#include "pch.hpp"
#include "mesh.hpp"

auto Arcadia::Mesh::Box(
    const glm::vec3& half_extent
) -> Arcadia::Mesh
{
    auto x = half_extent.x;
    auto y = half_extent.y;
    auto z = half_extent.z;

    Arcadia::Mesh mesh{};
    auto& vertices = mesh.Vertices;
    vertices.reserve(36);

    // pos-x
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{x, y, z},
            Arcadia::Vec3::PosX(),
            glm::vec2{.5f,.67f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{x, -y,z},
            Arcadia::Vec3::PosX(),
            glm::vec2{.5f,.33f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{x,y,-z},
            Arcadia::Vec3::PosX(),
            glm::vec2{.75f,.67f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{x,y,-z},
            Arcadia::Vec3::PosX(),
            glm::vec2{.75f,.67f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{x,-y,z},
            Arcadia::Vec3::PosX(),
            glm::vec2{.5f,.33f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{x,-y,-z},
            Arcadia::Vec3::PosX(),
            glm::vec2{.75f,.33f}
        }
    );

    // neg-x
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{-x, y, -z},
            Arcadia::Vec3::NegX(),
            glm::vec2{.0f,.67f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{-x, -y,-z},
            Arcadia::Vec3::NegX(),
            glm::vec2{.0f,.33f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{-x,y,z},
            Arcadia::Vec3::NegX(),
            glm::vec2{.25f,.67f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{-x,y,z},
            Arcadia::Vec3::NegX(),
            glm::vec2{.25f,.67f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{-x,-y,-z},
            Arcadia::Vec3::NegX(),
            glm::vec2{.0f,.33f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{-x,-y,z},
            Arcadia::Vec3::PosY(),
            glm::vec2{.25f,.33f}
        }
    );

    // pos-y
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{-x, y, -z},
            Arcadia::Vec3::PosY(),
            glm::vec2{.25f,1.f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{-x, y,z},
            Arcadia::Vec3::PosY(),
            glm::vec2{.25f,.67f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{x,y,-z},
            Arcadia::Vec3::PosY(),
            glm::vec2{.5f,1.f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{x,y,-z},
            Arcadia::Vec3::PosY(),
            glm::vec2{.5f,1.f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{-x,y,z},
            Arcadia::Vec3::PosY(),
            glm::vec2{.25f,.67f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{x,y,z},
            Arcadia::Vec3::PosY(),
            glm::vec2{.5f,.67f}
        }
    );

    // neg-y
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{-x, -y, z},
            Arcadia::Vec3::NegY(),
            glm::vec2{.25f,.33f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{-x, -y,-z},
            Arcadia::Vec3::NegY(),
            glm::vec2{.25f,.0f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{x,-y,z},
            Arcadia::Vec3::NegY(),
            glm::vec2{.25f,.5f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{x,-y,z},
            Arcadia::Vec3::NegY(),
            glm::vec2{.25f,.5f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{-x,-y,-z},
            Arcadia::Vec3::NegY(),
            glm::vec2{.25f,.0f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{x,-y,-z},
            Arcadia::Vec3::NegY(),
            glm::vec2{.5f,.0f}
        }
    );

    // pos-z
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{-x, y, z},
            Arcadia::Vec3::PosZ(),
            glm::vec2{.25f,.67f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{-x, -y,z},
            Arcadia::Vec3::PosZ(),
            glm::vec2{.25f,.33f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{x,y,z},
            Arcadia::Vec3::PosZ(),
            glm::vec2{.5f,.67f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{x,y,z},
            Arcadia::Vec3::PosZ(),
            glm::vec2{.5f,.67f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{-x, -y,z},
            Arcadia::Vec3::PosZ(),
            glm::vec2{.25f,.33f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{x,-y,z},
            Arcadia::Vec3::PosZ(),
            glm::vec2{.5f,.33f}
        }
    );

    // neg-z
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{x, y, -z},
            Arcadia::Vec3::NegZ(),
            glm::vec2{.75f,.67f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{x, -y,-z},
            Arcadia::Vec3::NegZ(),
            glm::vec2{.75f,.33f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{-x,y,-z},
            Arcadia::Vec3::NegZ(),
            glm::vec2{1.f,.67f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{-x,y,-z},
            Arcadia::Vec3::NegZ(),
            glm::vec2{1.f,.67f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{x, -y,-z},
            Arcadia::Vec3::NegZ(),
            glm::vec2{.75f,.33f}
        }
    );
    vertices.emplace_back(
        Arcadia::Vertex{
            glm::vec3{-x,-y,-z},
            Arcadia::Vec3::NegZ(),
            glm::vec2{1.f,.33f}
        }
    );

    return mesh;

}

auto Arcadia::Mesh::Capsule(
    float radius,
    float half_height_of_sylinder
) -> Arcadia::Mesh
{
    return Arcadia::Mesh();
}

auto Arcadia::Mesh::Cylinder(
    float half_height,
    float radius
) -> Arcadia::Mesh
{
    return Arcadia::Mesh{};
}

auto Arcadia::Mesh::Sphere(
    float radius,
    std::size_t stack_count,
    std::size_t sector_count
) -> Arcadia::Mesh
{
    const auto pi = glm::pi<float>();
    float sector_step = 2 * pi / sector_count;
    float stack_step = pi / sector_count;

    float radius_inv = 1.f / radius;

    Arcadia::Mesh mesh{};

    auto& vertices = mesh.Vertices;
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
                Arcadia::Vertex{
                    glm::vec3{x,y,z},
                    glm::vec3{nx,ny,nz},
                    glm::vec2{u,v}
                }
            );
        }
    }

    auto& indices = mesh.Indices;
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
