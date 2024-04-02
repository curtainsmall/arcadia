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

namespace arcadia
{
    struct ARCADIA_API model_component;
    struct ARCADIA_API model_component_memento
    {
        friend arcadia::model_component;
    private:
        glm::vec3             location{ arcadia::vec3::zero() };
        glm::quat             rotation{ arcadia::quat::identity() };
        glm::vec3             scale{ 1,1,1 };
        glm::vec3             pivot{ arcadia::vec3::zero() };
    };

    struct ARCADIA_API model_component:
        arcadia::component_base,
        arcadia::memento_originator_interface<arcadia::model_component_memento>
    {
    public:
        using identifiable_meshes = arcadia::basic_identifiable<std::vector<arcadia::mesh>>;

        using self_type = model_component;
    public:
        ARCADIA_COMPONENT_TYPE_STR_GETTERS("model");

        model_component() = default;
        model_component(const std::filesystem::path& filepath);
        model_component(const nlohmann::json& json);
        ~model_component() = default;
        [[nodiscard]]
        auto to_json() const->nlohmann::json;

        [[nodiscard]]
        virtual auto snapshot() const->memento_data_type override;
        virtual void restore(const memento_data_type& memento) override;

        model_component(self_type&&) noexcept = default;
        auto operator=(self_type&&) noexcept -> self_type & = default;

        [[nodiscard]]
        auto get_filepath() const -> const std::filesystem::path&;
        [[nodiscard]]
        auto has_identifiable_meshes() const -> bool;
        [[nodiscard]]
        auto get_identifiable_meshes() const -> const identifiable_meshes&;

        void import(const std::filesystem::path & filepath);

    private:
        void _load();
        void _unload();

        void _process_assimp_node(
            std::vector<arcadia::mesh>& meshes,
            const aiScene* const ai_scene,
            const aiNode* const ai_node,
            std::size_t& next_mesh_index
        );

        void _load_texture(
            const std::filesystem::path& directory,
            const aiMaterial* const ai_material,
            aiTextureType ai_texture_type,
            texture2d& texture
        );

    public:
        glm::vec3 location{ arcadia::vec3::zero() };
        glm::quat rotation{ arcadia::quat::identity() };
        glm::vec3 scale{ 1,1,1 };
        glm::vec3 pivot{ arcadia::vec3::zero() };

    private:
        std::filesystem::path _filepath{};
        std::unique_ptr<identifiable_meshes> _identifiable_meshes_uptr{};
    };
}
