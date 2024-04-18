#include "pch.hpp"
#include "transform_component.hpp"

Arcadia::TransformComponent::TransformComponent(const nlohmann::json& json):
    Position(Arcadia::Vec3::FromJson(json.at("position"))),
    Rotation(Arcadia::Quat::FromJson(json.at("rotation"))),
    Scale(Arcadia::Vec3::FromJson(json.at("scale"))),
    Pivot(Arcadia::Vec3::FromJson(json.at("pivot")))
{}

auto Arcadia::TransformComponent::ToJson() const -> nlohmann::json
{
    nlohmann::json json{
        {"position",Arcadia::Vec3::ToJson(Position)},
        {"rotation",Arcadia::Quat::ToJson(Rotation)},
        {"scale"   ,Arcadia::Vec3::ToJson(Scale)},
        {"pivot"   ,Arcadia::Vec3::ToJson(Pivot)}
    };

    return json;
}

auto Arcadia::TransformComponent::OnSnapshot() const -> std::shared_ptr<Arcadia::MementoDataBase>
{
    auto memento_data = std::make_shared<Arcadia::TransformComponentMementoData>();

    memento_data->Position = Position;
    memento_data->Rotation = Rotation;
    memento_data->Scale    = Scale;
    memento_data->Pivot    = Pivot;

    return memento_data;
}

void Arcadia::TransformComponent::OnRestore(const std::shared_ptr<Arcadia::MementoDataBase>& memento_data)
{
    auto& memento_data_ = memento_data->As<Arcadia::TransformComponentMementoData>();

    Position = memento_data_.Position;
    Rotation = memento_data_.Rotation;
    Scale = memento_data_.Scale;
    Pivot = memento_data_.Pivot;
}
