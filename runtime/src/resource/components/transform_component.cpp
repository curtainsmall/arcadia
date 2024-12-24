#include "pch.hpp"
#include "transform_component.hpp"

#include"core/math.hpp"

Arcadia::TransformComponent::TransformComponent(const nlohmann::json& json) :
    Flags(json.at("flags")),
    Position(GlmVec3::FromJson(json.at("position"))),
    Rotation(GlmQuat::FromJson(json.at("rotation"))),
    Direction(GlmVec3::FromJson(json.at("direction"))),
    Scale(GlmVec3::FromJson(json.at("scale"))),
    Pivot(GlmVec3::FromJson(json.at("pivot")))
{}

auto Arcadia::TransformComponent::ToJson() const -> nlohmann::json
{
    nlohmann::json json{
        {"flags", Flags},
        {"position",GlmVec3::ToJson(Position)},
        {"rotation",GlmQuat::ToJson(Rotation)},
        {"direction",GlmVec3::ToJson(Direction)},
        {"scale"   ,GlmVec3::ToJson(Scale)},
        {"pivot"   ,GlmVec3::ToJson(Pivot)}
    };

    return json;
}

auto Arcadia::TransformComponent::GenerateTransformMat4() const -> glm::mat4
{
    glm::mat4 mat = GlmMat4::CreateIdentity();

    // Move pivot to origin
    //mat = glm::translate(mat, -Pivot);

    // Scale about origin (which is pivot now)
    mat = glm::scale(mat, Scale);

    // Rotate
    mat = glm::mat4_cast(Rotation) * mat;

    // Move pivot back from origin
    //mat = glm::translate(mat, Pivot);

    // Translate
    mat = glm::translate(mat, Position);

    return mat;
}

auto Arcadia::TransformComponent::OnSnapshot() const -> std::shared_ptr<MementoDataBase>
{
    std::shared_ptr< TransformComponentMementoData> memento_data
        = std::make_shared<TransformComponentMementoData>();

    memento_data->Flags     = Flags;
    memento_data->Position  = Position;
    memento_data->Rotation  = Rotation;
    memento_data->Direction = Direction;
    memento_data->Scale     = Scale;
    memento_data->Pivot     = Pivot;

    return memento_data;
}

void Arcadia::TransformComponent::OnRestore(const std::shared_ptr<MementoDataBase>& memento_data_sptr)
{
    TransformComponentMementoData& memento_data = memento_data_sptr->CastTo<TransformComponentMementoData>();

    Flags = memento_data.Flags;
    Position  = memento_data.Position;
    Rotation  = memento_data.Rotation;
    Direction = memento_data.Direction;
    Scale     = memento_data.Scale;
    Pivot     = memento_data.Pivot;
}