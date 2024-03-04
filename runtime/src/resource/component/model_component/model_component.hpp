#pragma once

#include<vector>

#include"assimp/Importer.hpp"
#include"assimp/scene.h"

#include"core/base.hpp"
#include"core/nlohmann_json_header.hpp"
#include"resource/component/component.hpp"
#include"resource/component/model_component/mesh/mesh.hpp"

namespace arcadia
{
    struct ARCADIA_API model_component: arcadia::component_base
    {
    public:
        using self_type = model_component;
    public:

        model_component() = default;
        model_component(const std::filesystem::path& filepath);
        model_component(const nlohmann::json& json);
        ~model_component() = default;
        auto to_json() const->nlohmann::json;

        model_component(self_type&&) noexcept = default;
        auto operator=(self_type&&) noexcept -> self_type & = default;

        [[nodiscard]]
        inline auto get_meshes() const -> const std::vector<arcadia::mesh>&
        {
            return _meshes;
        }

        [[nodiscard]]
        inline auto get_filepath() const -> const std::filesystem::path&
        {
            return _filepath;
        }

    private:
        void _import();

        void _process_assimp_node(
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

    private:
        std::vector<arcadia::mesh> _meshes{};
        std::filesystem::path _filepath{};
    };
}
