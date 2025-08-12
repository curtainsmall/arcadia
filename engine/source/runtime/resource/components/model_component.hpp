#pragma once

#include <memory>
#include <vector>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"

#include "core/exception.hpp"
#include "core/identifiable.hpp"
#include "core/math.hpp"
#include "core/memento.hpp"
#include "core/nlohmann_json.hpp"
#include "platform/defines.hpp"
#include "resource/components/component.hpp"
#include "resource/mesh/mesh.hpp"

namespace Arcadia
{
    namespace Exceptions
    {
        ACDA_DEFINE_EXCEPTION(ModelComponent_ModelLoadInvalidFormat);
    }

    using ModelComponent_IdentifiableMeshesType = Identifiable<std::vector<Mesh>>;

    struct ModelComponent_Memento
    {
    public:
    public:
        std::filesystem::path Filepath{};
        std::unique_ptr<ModelComponent_IdentifiableMeshesType> upIdentifiableMeshes{};
    };

    struct ACDA_API ModelComponent:
        public Mementoable<ModelComponent_Memento>
    {
    public:
        using MementoType = ModelComponent_Memento;
        using SelfType = ModelComponent;
    public:
        ACDA_COMPONENT_TYPE_STR_GETTERS("model");

        ModelComponent() = default;
        ModelComponent(const std::filesystem::path& filepath);
        ModelComponent(const nlohmann::json& json);
        virtual ~ModelComponent() = default;
        [[nodiscard]]
        auto ToJson() const -> nlohmann::json;

        ModelComponent(const SelfType&) = delete;
        auto operator=(const SelfType&) -> SelfType & = delete;

        ModelComponent(SelfType&&) noexcept = default;
        auto operator=(SelfType&&) noexcept -> SelfType & = default;

        [[nodiscard]]
        auto GetFilepath() const -> const std::filesystem::path&;
        [[nodiscard]]
        auto HasIdentifiableMeshes() const -> bool;
        [[nodiscard]]
        auto GetIdentifiableMeshes() const -> const ModelComponent_IdentifiableMeshesType&;

        void LoadModel(const std::filesystem::path& filepath);
        void UnloadModel();
        [[nodiscard]]
        auto IsModelLoaded() const -> bool;

    protected:
        [[nodiscard]]
        virtual auto OnSnapshot() const -> std::unique_ptr<MementoType> override;
        virtual void OnRestore(const std::unique_ptr<MementoType>& memento_uptr) override;

    private:
        void _LoadModel();
        void _UnloadModel();

        void _ProcessAssimpNode(
            std::vector<Mesh>& meshes,
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

    private:
        std::filesystem::path _Filepath{};
        std::unique_ptr<ModelComponent_IdentifiableMeshesType> _upIdentifiableMeshes{};
    };
}