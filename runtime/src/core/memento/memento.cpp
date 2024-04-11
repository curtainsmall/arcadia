#include "pch.hpp"
#include "memento.hpp"

void Arcadia::Memento::Restore() const
{
    _OriginatorRestoreFn();
}

auto Arcadia::Memento::GetDescription() const -> const std::string&
{
    return _Description;
}

auto Arcadia::MementoList::Instance() -> self_type&
{
    static self_type memento_list{};
    return memento_list;
}

auto Arcadia::MementoList::Undo()  -> bool
{
    if(_CurrentIter == _List.end())
    {
        return false;
    }

    (_CurrentIter++)->Restore();
}

auto Arcadia::MementoList::Redo()  -> bool
{
    if(_CurrentIter == _List.begin())
    {
        return false;
    }

    (--_CurrentIter)->Restore();
}

auto Arcadia::MementoList::GetCapacity() const -> std::size_t
{
    return _Capacity;
}

void Arcadia::MementoList::SetCapacity(std::size_t capacity)
{
    _Capacity = capacity;
}

auto Arcadia::MementoList::Size() const -> std::size_t
{
    return _List.size();
}

void Arcadia::MementoList::Clear()
{
    _List.clear();
}

auto Arcadia::MementoList::IsCurrent(const container_type::const_iterator& iter) const -> bool
{
    return iter == _CurrentIter;
}

auto Arcadia::MementoList::begin() noexcept -> container_type::iterator
{
    return _List.begin();
}

auto Arcadia::MementoList::end() noexcept -> container_type::iterator
{
    return _List.end();
}

auto Arcadia::MementoList::begin() const noexcept -> container_type::const_iterator
{
    return _List.begin();
}

auto Arcadia::MementoList::end() const noexcept -> container_type::const_iterator
{
    return _List.end();
}

auto Arcadia::MementoList::cbegin() const noexcept -> container_type::const_iterator
{
    return _List.cbegin();
}

auto Arcadia::MementoList::cend() const noexcept -> container_type::const_iterator
{
    return _List.cend();
}
