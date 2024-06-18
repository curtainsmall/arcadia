#pragma once

#include"core/base.hpp"
#include"core/math.hpp"
#include"core/memento/memento.hpp"
#include"core/nlohmann_json_header.hpp"
#include"resource/components/component_interface.hpp"

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

struct TransformComponentMementoData: MementoDataBase
{
public:
    auto operator==(const TransformComponentMementoData&) const -> bool = default;
public:
    TransformComponentFlags::value_type Flags{ TransformComponentFlags::None };

    glm::vec3 Position{ Vec3::Zero() };
    glm::quat Rotation{ Quat::Identity() };
    glm::vec3 Direction{ Vec3::PosZ() };
    glm::vec3 Scale{ 1,1,1 };
    glm::vec3 Pivot{ Vec3::Zero() };
};

struct TransformComponent:
    iComponent,
    iMementoOriginator
{
public:
    using self_type = TransformComponent;
public:
    ACDA_COMPONENT_TYPE_STR_GETTERS("transform");

    TransformComponent() = default;
    TransformComponent(const nlohmann::json& json);
    ~TransformComponent() = default;
    [[nodiscard]]
    auto ToJson() const->nlohmann::json;

    TransformComponent(self_type&&) noexcept = default;
    auto operator=(self_type&&) noexcept -> self_type & = default;

    auto GenerateTransformMatrix() const->glm::mat4;

protected:
    [[nodiscard]]
    virtual auto OnSnapshot() const->std::shared_ptr<MementoDataBase> override;
    virtual void OnRestore(const std::shared_ptr<MementoDataBase>& memento_data) override;

public:
    TransformComponentFlags::value_type Flags{ TransformComponentFlags::None };

    glm::vec3 Position{ Vec3::Zero() };
    glm::quat Rotation{ Quat::Identity() };
    glm::vec3 Direction{ Vec3::PosZ() };
    glm::vec3 Scale{ 1,1,1 };
    glm::vec3 Pivot{ Vec3::Zero() };
};

