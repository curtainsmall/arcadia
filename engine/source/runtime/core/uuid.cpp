#include "pch.hpp"
#include "uuid.hpp"

auto Arcadia::Uuid::CreateZero() -> Uuid
{
    return Uuid{ 0 };
}

Arcadia::Uuid::Uuid():
    _Value(_NextValue++)
{}

Arcadia::Uuid::Uuid(ValueType val):
    _Value(val)
{}

auto Arcadia::Uuid::GetValue() const -> ValueType
{
    return _Value;
}
Arcadia::Uuid::operator ValueType() const
{
    return _Value;
}