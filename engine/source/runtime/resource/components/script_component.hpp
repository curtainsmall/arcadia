#pragma once

#include <variant>
#include <filesystem>

#include "platform/defines.hpp"
#include "core/exception.hpp"
#include "core/nlohmann_json.hpp"
#include "core/memento.hpp"
#include "resource/components/component_interface.hpp"

namespace Arcadia
{
    struct ScriptComponent_Memento
    {
    public:
        std::filesystem::path Filepath{};
        std::string Name{};
        std::string ScriptText{};
    };

    struct ACDA_API ScriptComponent:
        public ComponentInterface,
        public Mementoable<ScriptComponent_Memento>
    {
    public:
        using MementoType = ScriptComponent_Memento;
        using SelfType = ScriptComponent;
    public:
        ACDA_COMPONENT_TYPE_STR_GETTERS("script");

        ScriptComponent() = default;
        ScriptComponent(const nlohmann::json& json);
        virtual ~ScriptComponent() override = default;
        [[nodiscard]]
        auto ToJson() const -> nlohmann::json;

        ScriptComponent(SelfType&&) noexcept = default;
        auto operator=(SelfType&&) noexcept -> SelfType & = default;

        [[nodiscard]]
        auto GetFilepath() const -> const std::filesystem::path&;

        [[nodiscard]]
        auto GetName() const -> std::string_view;

        [[nodiscard]]
        auto GetScriptText() const -> std::string_view;

        void LoadScript(const std::filesystem::path& filepath, std::string_view name);

    protected:
        [[nodiscard]]
        virtual auto OnSnapshot() const -> std::unique_ptr<MementoType> override;
        virtual void OnRestore(const std::unique_ptr<MementoType>& memento_uptr) override;

    private:
        std::filesystem::path _Filepath{};
        std::string _Name{};
        std::string _ScriptText{};
    };
}