#include"pch.hpp"

#include"model_component.hpp"

#include"core/file/file.hpp"
#include"core/file/pfd_header.hpp"
#include"core/log/log.hpp"

#include"assimp/postprocess.h"
#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include"stb/stb_image.h"

ModelComponent::ModelComponent(const std::filesystem::path& filepath):
    _filepath(filepath)
{
    if(!_filepath.empty())
    {
        _load();
    }
}

ModelComponent::ModelComponent(const nlohmann::json& json):
    _filepath(to_filepath(json.at("filepath")))
{
    if(!_filepath.empty())
    {
        _load();
    }
}

auto ModelComponent::to_json() const -> nlohmann::json
{
    nlohmann::json json{
        {"filepath", _filepath.generic_string() }
    };
    return json;
}

auto ModelComponent::on_snapshot() const -> std::shared_ptr<MementoDataBase>
{
    return nullptr;
}

void ModelComponent::on_restore(const std::shared_ptr<MementoDataBase>& sp_memento_data)
{}

auto ModelComponent::filepath() const -> const std::filesystem::path&
{
    return _filepath;
}

auto ModelComponent::has_identifiable_meshes() const -> bool
{
    return _identifiable_meshes.get();
}

auto ModelComponent::identifiable_meshes() const -> const identifiable_meshes_type&
{
    ACDA_ASSERT(has_identifiable_meshes());
    return *_identifiable_meshes;
}


void ModelComponent::import(const std::filesystem::path & filepath)
{
    if(!_filepath.empty() && !filepath.empty())
    {
        auto res = pfd::message{
             "Replacing Model",
             std::format("Do you want to replace model from {} with model from {}",_filepath.generic_string(),filepath.generic_string()),
             pfd::choice::yes_no,
             pfd::icon::info
        }.result();

        switch(res)
        {
            case pfd::button::yes:
            {
                _unload();
                _filepath = filepath;
                _load();
                break;
            }
            case pfd::button::no:
            default:
            {
                break;
            }
        }
    }
    else if(!_filepath.empty() && filepath.empty())
    {
        auto res = pfd::message{
            "Unloading Model",
            std::format("Do you want to unload model from {}",_filepath.generic_string()),
            pfd::choice::yes_no,
            pfd::icon::info
        }.result();
        switch(res)
        {
            case pfd::button::yes:
            {
                _unload();
                _filepath = filepath;
                break;
            }
            case pfd::button::no:
            default:
            {
                break;
            }
        }
    }
    else if(!filepath.empty())
    {
        _filepath = filepath;
        _load();
    }
}

void ModelComponent::_load()
{
    Assimp::Importer importer{};
    auto ai_scene = importer.ReadFile(
        _filepath.generic_string(),
        aiProcess_Triangulate
        | aiProcess_GenNormals
        | aiProcess_CalcTangentSpace
        | aiProcess_FlipUVs
    );

    if(
        !ai_scene
        || ai_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE
        || !ai_scene->mRootNode
        )
    {
        LOG_ERROR(importer.GetErrorString());
        return;
    }

    std::vector<Mesh> meshes{};

    std::size_t next_mesh_index{ 0 };
    _process_assimp_node(
        meshes,
        ai_scene,
        ai_scene->mRootNode,
        next_mesh_index
    );

    _identifiable_meshes = std::make_unique<identifiable_meshes_type>(std::move(meshes));
}

void ModelComponent::_unload()
{
    _identifiable_meshes.reset();
}

void ModelComponent::_process_assimp_node(
    std::vector<Mesh>& meshes,
    const aiScene* const ai_scene,
    const aiNode* const ai_node,
    std::size_t& next_mesh_index
)
{
    // Current node
    for(
        unsigned int i = 0;
        i < ai_node->mNumMeshes;
        ++i, ++next_mesh_index
        )
    {
        auto ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[i]];
        auto& mesh = meshes.emplace_back();

        // Vertex
        mesh.vertices.reserve(ai_mesh->mNumVertices);
        for(unsigned int i = 0; i < ai_mesh->mNumVertices; ++i)
        {
            auto& vertex = mesh.vertices.emplace_back();

            auto ai_vertex = ai_mesh->mVertices[i];
            vertex.coord = glm::vec3{
                ai_vertex.x,
                ai_vertex.y,
                ai_vertex.z
            };

            if(ai_mesh->HasNormals())
            {
                auto ai_normal = ai_mesh->mNormals[i];
                vertex.normal = glm::vec3{
                    ai_normal.x,
                    ai_normal.y,
                    ai_normal.z
                };
            }

            if(ai_mesh->HasTextureCoords(0))
            {
                auto ai_tex_coord = ai_mesh->mTextureCoords[0][i];
                vertex.tex_coord = glm::vec2{
                    ai_tex_coord.x,
                    ai_tex_coord.y
                };
            }
        }

        // Index
        mesh.indices.reserve(ai_mesh->mNumFaces * 3u);
        for(unsigned int i = 0; i < ai_mesh->mNumFaces; ++i)
        {
            auto& ai_face = ai_mesh->mFaces[i];
            for(unsigned int j = 0; j < ai_face.mNumIndices; ++j)
            {
                mesh.indices.emplace_back(ai_face.mIndices[j]);
            }
        }

        // Material
        if(ai_mesh->mMaterialIndex >= 0)
        {
            auto& material = mesh.material;
            auto ai_material = ai_scene->mMaterials[ai_mesh->mMaterialIndex];
            auto model_directory = _filepath.parent_path();

            _load_texture(
                model_directory,
                ai_material,
                aiTextureType_AMBIENT,
                material.ambient_texture2d
            );
            _load_texture(
                model_directory,
                ai_material,
                aiTextureType_DIFFUSE,
                material.diffuse_texture2d
            );
            _load_texture(
                model_directory,
                ai_material,
                aiTextureType_SPECULAR,
                material.specular_texture2d
            );
        }
    }

    // Sub-nodes
    for(unsigned int i = 0; i < ai_node->mNumChildren; ++i)
    {
        _process_assimp_node(
            meshes,
            ai_scene,
            ai_node->mChildren[i],
            next_mesh_index
        );
    }
}

void ModelComponent::_load_texture(
    const std::filesystem::path& directory,
    const aiMaterial* const ai_material,
    aiTextureType ai_texture_type,
    Texture2d& texture
)
{
    aiString str{};
    for(unsigned int i = 0; i < ai_material->GetTextureCount(ai_texture_type); ++i)
    {
        ai_material->GetTexture(ai_texture_type, i, &str);
        int x{ 0 },
            y{ 0 };
        auto filepath = directory / std::filesystem::path{ str.C_Str() };
        float* ptr = reinterpret_cast<float*>(stbi_load(filepath.string().c_str(), &x, &y, nullptr, 4));
        texture.size = glm::ivec2{ x,y };
        for(unsigned int i = 0; i < x * y; i+=4)
        {
            texture.pixels.emplace_back(
                ptr[i],
                ptr[i + 1],
                ptr[i + 2],
                ptr[i + 3]
            );
        }
        break; // TODO: Only read the first texture because material only accept one texture per type. Maybe use texture2d::cascade to cimbine multiple textures in to one later?
    }
}
