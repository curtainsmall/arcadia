#pragma once

#include<optional>
#include<tuple>
#include<vector>

#include"assimp/Importer.hpp"
#include"assimp/scene.h"

#include"core/base.hpp"
#include"core/math.hpp"
#include"core/nlohmann_json_header.hpp"
#include"core/uuid.hpp"
#include"resource/component/component.hpp"
#include"resource/component/model_component/mesh/mesh.hpp"

namespace arcadia
{
    struct ARCADIA_API model_component: arcadia::component_base
    {
    public:
        struct ARCADIA_API identifiable_meshes: arcadia::identifiable_base
        {
        public:
            using meshes_type = std::vector<arcadia::mesh>;
            using self_type = identifiable_meshes;
        public:
            inline identifiable_meshes(meshes_type&& meshes):
                _meshes(std::move(meshes))
            {}

            [[nodiscard]]
            inline auto get_meshes() const -> const meshes_type&
            {
                return _meshes;
            }

        private:
            meshes_type _meshes{};
        };

        using self_type = model_component;
    public:
        ARCADIA_COMPONENT_TYPE_STR_GETERS("model");

        model_component() = default;
        model_component(const std::filesystem::path& filepath);
        model_component(const nlohmann::json& json);
        ~model_component() = default;
        [[nodiscard]]
        auto to_json() const->nlohmann::json;

        model_component(self_type&&) noexcept = default;
        auto operator=(self_type&&) noexcept -> self_type & = default;

        [[nodiscard]]
        inline auto has_identifiable_meshes() const -> bool
        {
            return _meshes_opt.has_value();
        }

        [[nodiscard]]
        inline auto get_identifiable_meshes() const -> const identifiable_meshes&
        {
            return *_meshes_opt;
        }

        [[nodiscard]]
        inline auto get_filepath() const -> const std::filesystem::path&
        {
            return _filepath;
        }

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
        glm::vec3 rotation{ arcadia::vec3::zero() };
        glm::vec3 scale{ 1,1,1 };
        glm::vec3 pivot{ arcadia::vec3::zero() };

    private:
        std::optional<identifiable_meshes> _meshes_opt{};
        std::filesystem::path _filepath{};
    };
}
