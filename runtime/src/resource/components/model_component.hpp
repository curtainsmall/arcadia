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
#include"resource/components/component_interface.hpp"
#include"resource/mesh/mesh.hpp"

struct ModelComponent:
    iComponent,
    iMementoOriginator
{
public:
    using identifiable_meshes_type = BasicIdentifiable<std::vector<Mesh>>;

    using self_type = ModelComponent;
public:
    ACDA_COMPONENT_TYPE_STR_GETTERS("model");

    ModelComponent() = default;
    ModelComponent(const std::filesystem::path& filepath);
    ModelComponent(const nlohmann::json& json);
    ~ModelComponent() = default;
    [[nodiscard]]
    auto to_json() const->nlohmann::json;

    ModelComponent(self_type&&) noexcept = default;
    auto operator=(self_type&&) noexcept -> self_type & = default;

    [[nodiscard]]
    auto filepath() const -> const std::filesystem::path&;
    [[nodiscard]]
    auto has_identifiable_meshes() const -> bool;
    [[nodiscard]]
    auto identifiable_meshes() const -> const identifiable_meshes_type&;

    void import(const std::filesystem::path & filepath);

protected:
    [[nodiscard]]
    virtual auto on_snapshot() const->std::shared_ptr<MementoDataBase> override;
    virtual void on_restore(const std::shared_ptr<MementoDataBase>& memento_data) override;

private:
    void _load();
    void _unload();

    void _process_assimp_node(
        std::vector<Mesh>& meshes,
        const aiScene* const ai_scene,
        const aiNode* const ai_node,
        size_t& next_mesh_index
    );

    void _load_texture(
        const std::filesystem::path& directory,
        const aiMaterial* const ai_material,
        aiTextureType ai_texture_type,
        Texture2d& texture
    );

private:
    std::filesystem::path _filepath{};
    std::unique_ptr<identifiable_meshes_type> _identifiable_meshes{};
};
