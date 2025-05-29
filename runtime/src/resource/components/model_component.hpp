#pragma once

#include <memory>
#include <vector>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"

#include "core/exception.hpp"
#include "core/identifiable.hpp"
#include "core/math.hpp"
#include "core/nlohmann_json.hpp"
#include "platform/api_def.hpp"
#include "resource/components/component_interface.hpp"
#include "resource/mesh/mesh.hpp"

namespace Arcadia
{
    namespace Exceptions
    {
        ACDA_DEFINE_RUNTIME_ERROR_EXCEPTION(ModelComponent_ModelLoadInvalidFormat);
    }

    struct ACDA_API ModelComponent:
        public ComponentInterface
    {
    public:
        using IdentifiableMeshesType = Identifiable<std::vector<Mesh>>;

        using SelfType = ModelComponent;
    public:
        ACDA_COMPONENT_TYPE_STR_GETTERS("model");

        ModelComponent() = default;
        ModelComponent(const std::filesystem::path& filepath);
        ModelComponent(const nlohmann::json& json);
        ~ModelComponent() = default;
        [[nodiscard]]
        auto ToJson() const->nlohmann::json;

        ModelComponent(SelfType&&) noexcept = default;
        auto operator=(SelfType&&) noexcept -> SelfType & = default;

        [[nodiscard]]
        auto GetFilepath() const -> const std::filesystem::path&;
        [[nodiscard]]
        auto HasIdentifiableMeshes() const -> bool;
        [[nodiscard]]
        auto GetIdentifiableMeshes() const -> const IdentifiableMeshesType&;

        void LoadModel(const std::filesystem::path& filepath);
        void UnloadModel();
        [[nodiscard]]
        auto IsModelLoaded() const -> bool;

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
        std::unique_ptr<IdentifiableMeshesType> _upIdentifiableMeshes{};
    };
}