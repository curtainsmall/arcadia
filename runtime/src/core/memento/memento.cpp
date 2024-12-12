#include "pch.hpp"

#include "memento.hpp"

void Memento::Restore() const
{
    _OriginatorRestoreFunction();
}

auto Memento::GetDescription() const -> const std::string&
{
    return _Description;
}

auto MementoList::Instance() -> SelfType&
{
    static SelfType memento_list{};
    return memento_list;
}

auto MementoList::Undo()  -> bool
{
    if(_CurrentIterator == _List.end())
    {
        return false;
    }

    (_CurrentIterator++)->Restore();
    return true;
}

auto MementoList::Redo()  -> bool
{
    if(_CurrentIterator == _List.begin())
    {
        return false;
    }

    (--_CurrentIterator)->Restore();
    return true;
}

auto MementoList::GetCapacity() const -> size_t
{
    return _Capacity;
}

void MementoList::SetCapacity(size_t capacity)
{
    _Capacity = capacity;
}

auto MementoList::GetSize() const -> size_t
{
    return _List.size();
}

void MementoList::Clear()
{
    _List.clear();
}

auto MementoList::IsCurrent(const ContainerType::const_iterator& iter) const -> bool
{
    return iter == _CurrentIterator;
}

auto MementoList::begin() noexcept -> ContainerType::iterator
{
    return _List.begin();
}

auto MementoList::end() noexcept -> ContainerType::iterator
{
    return _List.end();
}

auto MementoList::begin() const noexcept -> ContainerType::const_iterator
{
    return _List.begin();
}

auto MementoList::end() const noexcept -> ContainerType::const_iterator
{
    return _List.end();
}

auto MementoList::cbegin() const noexcept -> ContainerType::const_iterator
{
    return _List.cbegin();
}

auto MementoList::cend() const noexcept -> ContainerType::const_iterator
{
    return _List.cend();
}

auto iMementoOriginator::Snapshot() -> std::shared_ptr<MementoDataBase>
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

void iMementoOriginator::Restore(const std::shared_ptr<MementoDataBase>& memento_data)
{
    OnRestore(memento_data);
    _PreviousMementoData = memento_data;
}