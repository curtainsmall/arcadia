#pragma once

#include"core/base.hpp"
#include"core/math.hpp"
#include"core/memento/memento.hpp"
#include"core/nlohmann_json_header.hpp"
#include"resource/components/component_interface.hpp"

namespace Arcadia
{
    enum class TransformComponentFlags: uint8_t
    {
        None = 0,
        UseRotation = 0x01,
        UseDirection = 0x02,
        _EnumBitfield
    };

    class TransformComponentMementoData: public MementoDataBase
    {
    public:
        auto operator==(const TransformComponentMementoData&) const -> bool = default;
    public:
        TransformComponentFlags Flags{ TransformComponentFlags::None };

        glm::vec3 Position{ GlmVec3::CreateZero() };
        glm::quat Rotation{ GlmQuat::CreateIdentity() };
        glm::vec3 Direction{ GlmVec3::CreateUnitPositiveZ() };
        glm::vec3 Scale{ 1,1,1 };
        glm::vec3 Pivot{ GlmVec3::CreateZero() };
    };

    class TransformComponent:
        public iComponent,
        public iMementoOriginator
    {
    public:
        using SelfType = TransformComponent;
    public:
        ACDA_COMPONENT_TYPE_STR_GETTERS("transform");

        TransformComponent() = default;
        TransformComponent(const nlohmann::json& json);
        ~TransformComponent() = default;
        [[nodiscard]]
        auto ToJson() const->nlohmann::json;

        TransformComponent(SelfType&&) noexcept = default;
        auto operator=(SelfType&&) noexcept -> SelfType & = default;

        auto GenerateTransformMat4() const->glm::mat4;

    protected:
        [[nodiscard]]
        virtual auto OnSnapshot() const->std::shared_ptr<MementoDataBase> override;
        virtual void OnRestore(const std::shared_ptr<MementoDataBase>& memento_data) override;

    public:
        TransformComponentFlags Flags{ TransformComponentFlags::None };

        glm::vec3 Position{ GlmVec3::CreateZero() };
        glm::quat Rotation{ GlmQuat::CreateIdentity() };
        glm::vec3 Direction{ GlmVec3::CreateUnitPositiveZ() };
        glm::vec3 Scale{ 1,1,1 };
        glm::vec3 Pivot{ GlmVec3::CreateZero() };
    };
}