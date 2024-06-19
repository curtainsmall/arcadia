#include "pch.hpp"
#include "transform_component.hpp"

#include"core/math.hpp"

TransformComponent::TransformComponent(const nlohmann::json& json):
    Flags(json.at("flags")),
    position(vec3::from_json(json.at("position"))),
    rotation(quat::from_json(json.at("rotation"))),
    direction(vec3::from_json(json.at("direction"))),
    scale(vec3::from_json(json.at("scale"))),
    pivot(vec3::from_json(json.at("pivot")))
{}

auto TransformComponent::to_json() const -> nlohmann::json
{
    nlohmann::json json{
        {"flags", Flags},
        {"position",vec3::to_json(position)},
        {"rotation",quat::to_json(rotation)},
        {"direction",vec3::to_json(direction)},
        {"scale"   ,vec3::to_json(scale)},
        {"pivot"   ,vec3::to_json(pivot)}
    };

    return json;
}

auto TransformComponent::generate_transform_matrix() const -> glm::mat4
{
    glm::mat4 mat = mat4::identity();

    // Move pivot to origin
    //mat = glm::translate(mat, -Pivot);

    // Scale about origin (which is pivot now)
    mat = glm::scale(mat, scale);

    // Rotate
    mat = glm::mat4_cast(rotation) * mat;

    // Move pivot back from origin
    //mat = glm::translate(mat, Pivot);

    // Translate
    mat = glm::translate(mat, position);

    return mat;
}

auto TransformComponent::on_snapshot() const -> std::shared_ptr<MementoDataBase>
{
    auto memento_data = std::make_shared<TransformComponentMementoData>();

    memento_data->Flags = Flags;
    memento_data->position  = position;
    memento_data->rotation  = rotation;
    memento_data->direction = direction;
    memento_data->scale     = scale;
    memento_data->pivot     = pivot;

    return memento_data;
}

void TransformComponent::on_restore(const std::shared_ptr<MementoDataBase>& memento_data)
{
    auto& memento_data_ = memento_data->as<TransformComponentMementoData>();

    Flags = memento_data_.Flags;
    position  = memento_data_.position;
    rotation  = memento_data_.rotation;
    direction = memento_data_.direction;
    scale     = memento_data_.scale;
    pivot     = memento_data_.pivot;
}
