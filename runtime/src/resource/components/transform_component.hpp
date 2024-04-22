#pragma once

#include"core/base.hpp"
#include"core/math.hpp"
#include"core/memento/memento.hpp"
#include"core/nlohmann_json_header.hpp"
#include"resource/components/component_interface.hpp"

namespace Arcadia
{
    namespace TransformComponentFlags
    {
        using value_type = std::uint8_t;
        enum: value_type
        {
            None = 0,
            UseRotation = 0x01,
            UseDirection = 0x02,
        };
    }

    struct ARCADIA_API TransformComponentMementoData: Arcadia::MementoDataBase
    {
    public:
        auto operator==(const TransformComponentMementoData&) const -> bool = default;
    public:
        Arcadia::TransformComponentFlags::value_type Flags{ Arcadia::TransformComponentFlags::None };

        glm::vec3 Position{ Arcadia::Vec3::Zero() };
        glm::quat Rotation{ Arcadia::Quat::Identity() };
        glm::vec3 Direction{ Arcadia::Vec3::PosZ() };
        glm::vec3 Scale{ 1,1,1 };
        glm::vec3 Pivot{ Arcadia::Vec3::Zero() };
    };

    struct ARCADIA_API TransformComponent:
        Arcadia::iComponent,
        Arcadia::iMementoOriginator
    {
    public:
        using self_type = TransformComponent;
    public:
        ARCADIA_COMPONENT_TYPE_STR_GETTERS("transform");

        TransformComponent() = default;
        TransformComponent(const nlohmann::json& json);
        ~TransformComponent() = default;
        [[nodiscard]]
        auto ToJson() const->nlohmann::json;

        TransformComponent(self_type&&) noexcept = default;
        auto operator=(self_type&&) noexcept -> self_type & = default;

    protected:
        [[nodiscard]]
        virtual auto OnSnapshot() const->std::shared_ptr<Arcadia::MementoDataBase> override;
        virtual void OnRestore(const std::shared_ptr<Arcadia::MementoDataBase>& memento_data) override;

    public:
        Arcadia::TransformComponentFlags::value_type Flags{ Arcadia::TransformComponentFlags::None };

        glm::vec3 Position{ Arcadia::Vec3::Zero() };
        glm::quat Rotation{ Arcadia::Quat::Identity() };
        glm::vec3 Direction{ Arcadia::Vec3::PosZ() };
        glm::vec3 Scale{ 1,1,1 };
        glm::vec3 Pivot{ Arcadia::Vec3::Zero() };
    };
}

