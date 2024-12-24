#include "pch.hpp"

#include "memento.hpp"

void Arcadia::Memento::Restore() const
{
    _OriginatorRestoreFunction();
}

auto Arcadia::Memento::GetDescription() const -> const std::string&
{
    return _Description;
}

auto Arcadia::MementoList::Instance() -> SelfType&
{
    static SelfType memento_list{};
    return memento_list;
}

auto Arcadia::MementoList::Undo()  -> bool
{
    if(_CurrentIterator == _List.end())
    {
        return false;
    }

    (_CurrentIterator++)->Restore();
    return true;
}

auto Arcadia::MementoList::Redo()  -> bool
{
    if(_CurrentIterator == _List.begin())
    {
        return false;
    }

    (--_CurrentIterator)->Restore();
    return true;
}

auto Arcadia::MementoList::GetCapacity() const -> std::size_t
{
    return _Capacity;
}

void Arcadia::MementoList::SetCapacity(std::size_t capacity)
{
    _Capacity = capacity;
}

auto Arcadia::MementoList::GetSize() const -> std::size_t
{
    return _List.size();
}

void Arcadia::MementoList::Clear()
{
    _List.clear();
}

auto Arcadia::MementoList::IsCurrent(const ContainerType::const_iterator& iter) const -> bool
{
    return iter == _CurrentIterator;
}

auto Arcadia::MementoList::begin() noexcept -> ContainerType::iterator
{
    return _List.begin();
}

auto Arcadia::MementoList::end() noexcept -> ContainerType::iterator
{
    return _List.end();
}

auto Arcadia::MementoList::begin() const noexcept -> ContainerType::const_iterator
{
    return _List.begin();
}

auto Arcadia::MementoList::end() const noexcept -> ContainerType::const_iterator
{
    return _List.end();
}

auto Arcadia::MementoList::cbegin() const noexcept -> ContainerType::const_iterator
{
    return _List.cbegin();
}

auto Arcadia::MementoList::cend() const noexcept -> ContainerType::const_iterator
{
    return _List.cend();
}

auto Arcadia::MementoOriginatorInterface::Snapshot() -> std::shared_ptr<MementoDataBase>
{
    auto memento_data = OnSnapshot();
    if(!_PreviousMementoData)
    {
        _PreviousMementoData = memento_data;
    }
    else if(memento_data != _PreviousMementoData)
    {
        std::swap(memento_data, _PreviousMementoData);
    }
    return memento_data;
}

void Arcadia::MementoOriginatorInterface::Restore(const std::shared_ptr<MementoDataBase>& memento_data)
{
    OnRestore(memento_data);
    _PreviousMementoData = memento_data;
}