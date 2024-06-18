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
    _Filepath(filepath)
{
    if(!_Filepath.empty())
    {
        _Load();
    }
}

ModelComponent::ModelComponent(const nlohmann::json& json):
    _Filepath(ToFilepath(json.at("filepath")))
{
    if(!_Filepath.empty())
    {
        _Load();
    }
}

auto ModelComponent::ToJson() const -> nlohmann::json
{
    nlohmann::json json{
        {"filepath", _Filepath.generic_string() }
    };
    return json;
}

auto ModelComponent::OnSnapshot() const -> std::shared_ptr<MementoDataBase>
{
    return nullptr;
}

void ModelComponent::OnRestore(const std::shared_ptr<MementoDataBase>& sp_memento_data)
{}

auto ModelComponent::GetFilepath() const -> const std::filesystem::path&
{
    return _Filepath;
}

auto ModelComponent::HasIdentifiableMeshes() const -> bool
{
    return _IdentifiableMeshes.get();
}

auto ModelComponent::GetIdentifiableMeshes() const -> const identifiable_meshes&
{
    ARCADIA_ASSERT(HasIdentifiableMeshes());
    return *_IdentifiableMeshes;
}


void ModelComponent::Import(const std::filesystem::path& filepath)
{
    if(!_Filepath.empty() && !filepath.empty())
    {
        auto res = pfd::message{
             "Replacing Model",
             std::format("Do you want to replace model from {} with model from {}",_Filepath.generic_string(),filepath.generic_string()),
             pfd::choice::yes_no,
             pfd::icon::info
        }.result();

        switch(res)
        {
            case pfd::button::yes:
            {
                _Unload();
                _Filepath = filepath;
                _Load();
                break;
            }
            case pfd::button::no:
            default:
            {
                break;
            }
        }
    }
    else if(!_Filepath.empty() && filepath.empty())
    {
        auto res = pfd::message{
            "Unloading Model",
            std::format("Do you want to unload model from {}",_Filepath.generic_string()),
            pfd::choice::yes_no,
            pfd::icon::info
        }.result();
        switch(res)
        {
            case pfd::button::yes:
            {
                _Unload();
                _Filepath = filepath;
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
        _Filepath = filepath;
        _Load();
    }
}

void ModelComponent::_Load()
{
    Assimp::Importer importer{};
    auto ai_scene = importer.ReadFile(
        _Filepath.generic_string(),
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
        Log::Error(importer.GetErrorString());
        return;
    }

    std::vector<Mesh> meshes{};

    std::size_t next_mesh_index{ 0 };
    _ProcessAssimpNode(
        meshes,
        ai_scene,
        ai_scene->mRootNode,
        next_mesh_index
    );

    _IdentifiableMeshes = std::make_unique<identifiable_meshes>(std::move(meshes));
}

void ModelComponent::_Unload()
{
    _IdentifiableMeshes.reset();
}

void ModelComponent::_ProcessAssimpNode(
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
        mesh.Vertices.reserve(ai_mesh->mNumVertices);
        for(unsigned int i = 0; i < ai_mesh->mNumVertices; ++i)
        {
            auto& vertex = mesh.Vertices.emplace_back();

            auto ai_vertex = ai_mesh->mVertices[i];
            vertex.Coord = glm::vec3{
                ai_vertex.x,
                ai_vertex.y,
                ai_vertex.z
            };

            if(ai_mesh->HasNormals())
            {
                auto ai_normal = ai_mesh->mNormals[i];
                vertex.Normal = glm::vec3{
                    ai_normal.x,
                    ai_normal.y,
                    ai_normal.z
                };
            }

            if(ai_mesh->HasTextureCoords(0))
            {
                auto ai_tex_coord = ai_mesh->mTextureCoords[0][i];
                vertex.TexCoord = glm::vec2{
                    ai_tex_coord.x,
                    ai_tex_coord.y
                };
            }
        }

        // Index
        mesh.Indices.reserve(ai_mesh->mNumFaces * 3u);
        for(unsigned int i = 0; i < ai_mesh->mNumFaces; ++i)
        {
            auto& ai_face = ai_mesh->mFaces[i];
            for(unsigned int j = 0; j < ai_face.mNumIndices; ++j)
            {
                mesh.Indices.emplace_back(ai_face.mIndices[j]);
            }
        }

        // Material
        if(ai_mesh->mMaterialIndex >= 0)
        {
            auto& material = mesh.Material;
            auto ai_material = ai_scene->mMaterials[ai_mesh->mMaterialIndex];
            auto model_directory = _Filepath.parent_path();

            _LoadTexture(
                model_directory,
                ai_material,
                aiTextureType_AMBIENT,
                material.AmbientTexture2d
            );
            _LoadTexture(
                model_directory,
                ai_material,
                aiTextureType_DIFFUSE,
                material.DiffuseTexture2d
            );
            _LoadTexture(
                model_directory,
                ai_material,
                aiTextureType_SPECULAR,
                material.SpecularTexture2d
            );
        }
    }

    // Sub-nodes
    for(unsigned int i = 0; i < ai_node->mNumChildren; ++i)
    {
        _ProcessAssimpNode(
            meshes,
            ai_scene,
            ai_node->mChildren[i],
            next_mesh_index
        );
    }
}

void ModelComponent::_LoadTexture(
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
        texture.Size = glm::ivec2{ x,y };
        for(unsigned int i = 0; i < x * y; i+=4)
        {
            texture.Pixels.emplace_back(
                ptr[i],
                ptr[i + 1],
                ptr[i + 2],
                ptr[i + 3]
            );
        }
        break; // TODO: Only read the first texture because material only accept one texture per type. Maybe use texture2d::cascade to cimbine multiple textures in to one later?
    }
}
