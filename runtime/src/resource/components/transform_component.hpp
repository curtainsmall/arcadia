#pragma once

#include"core/base.hpp"
#include"core/math.hpp"
#include"core/memento/memento.hpp"
#include"core/nlohmann_json_header.hpp"
#include"resource/components/component_interface.hpp"

enum struct TransformComponentFlags: uint8_t
{
    None = 0,
    UseRotation = 0x01,
    UseDirection = 0x02,
    _EnumBitmap
};

struct TransformComponentMementoData: MementoDataBase
{
public:
    auto operator==(const TransformComponentMementoData&) const -> bool = default;
public:
    TransformComponentFlags flags{ TransformComponentFlags::None };

    glm::vec3 position{ vec3::zero() };
    glm::quat rotation{ quat::identity() };
    glm::vec3 direction{ vec3::pos_z() };
    glm::vec3 scale{ 1,1,1 };
    glm::vec3 pivot{ vec3::zero() };
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
    auto to_json() const->nlohmann::json;

    TransformComponent(self_type&&) noexcept = default;
    auto operator=(self_type&&) noexcept -> self_type & = default;

    auto generate_transform_matrix() const->glm::mat4;

protected:
    [[nodiscard]]
    virtual auto on_snapshot() const->std::shared_ptr<MementoDataBase> override;
    virtual void on_restore(const std::shared_ptr<MementoDataBase>& memento_data) override;

public:
    TransformComponentFlags flags{ TransformComponentFlags::None };

    glm::vec3 position{ vec3::zero() };
    glm::quat rotation{ quat::identity() };
    glm::vec3 direction{ vec3::pos_z() };
    glm::vec3 scale{ 1,1,1 };
    glm::vec3 pivot{ vec3::zero() };
};

