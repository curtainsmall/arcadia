#pragma once

#include<memory>
#include<vector>

#include"assimp/Importer.hpp"
#include"assimp/scene.h"

#include"core/base.hpp"
#include"core/math.hpp"
#include"core/memento/memento.hpp"
#include"core/nlohmann_json_header.hpp"
#include"core/uuid.hpp"
#include"resource/component/component.hpp"
#include"resource/component/model_component/mesh/mesh.hpp"

namespace Arcadia
{
    struct ARCADIA_API ModelComponent;
    struct ARCADIA_API ModelComponentMemento
    {
        friend Arcadia::ModelComponent;
    public:
        auto operator==(const ModelComponentMemento&) const -> bool = default;
    private:
        glm::vec3 Location{ Arcadia::Vec3::Zero() };
        glm::quat Rotation{ Arcadia::Quat::Identity() };
        glm::vec3 Scale{ 1,1,1 };
        glm::vec3 Pivot{ Arcadia::Vec3::Zero() };
    };

    struct ARCADIA_API ModelComponent:
        Arcadia::iComponent,
        Arcadia::iMementoOriginator<Arcadia::ModelComponentMemento>
    {
    public:
        using identifiable_meshes = Arcadia::BasicIdentifiable<std::vector<Arcadia::Mesh>>;

        using self_type = ModelComponent;
    public:
        ARCADIA_COMPONENT_TYPE_STR_GETTERS("model");

        ModelComponent() = default;
        ModelComponent(const std::filesystem::path& filepath);
        ModelComponent(const nlohmann::json& json);
        ~ModelComponent() = default;
        [[nodiscard]]
        auto ToJson() const->nlohmann::json;

        [[nodiscard]]
        virtual auto OnSnapshot() const->memento_data_type override;
        virtual void OnRestore(const memento_data_type& memento) override;

        ModelComponent(self_type&&) noexcept = default;
        auto operator=(self_type&&) noexcept -> self_type & = default;

        [[nodiscard]]
        auto GetFilepath() const -> const std::filesystem::path&;
        [[nodiscard]]
        auto HasIdentifiableMeshes() const -> bool;
        [[nodiscard]]
        auto GetIdentifiableMeshes() const -> const identifiable_meshes&;

        void Import(const std::filesystem::path& filepath);

    private:
        void _Load();
        void _Unload();

        void _ProcessAssimpNode(
            std::vector<Arcadia::Mesh>& meshes,
            const aiScene* const ai_scene,
            const aiNode* const ai_node,
            std::size_t& next_mesh_index
        );

        void _LoadTexture(
            const std::filesystem::path& directory,
            const aiMaterial* const ai_material,
            aiTextureType ai_texture_type,
            Texture2d& texture
        );

    public:
        glm::vec3 Location{ Arcadia::Vec3::Zero() };
        glm::quat Rotation{ Arcadia::Quat::Identity() };
        glm::vec3 Scale{ 1,1,1 };
        glm::vec3 Pivot{ Arcadia::Vec3::Zero() };

    private:
        std::filesystem::path _Filepath{};
        std::unique_ptr<identifiable_meshes> _upIdentifiableMeshes{};
    };
}
