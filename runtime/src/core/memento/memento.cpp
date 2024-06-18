#include "pch.hpp"

#include "memento.hpp"

void Memento::Restore() const
{
    _OriginatorRestoreFn();
}

auto Memento::GetDescription() const -> const std::string&
{
    return _Description;
}

auto MementoList::Instance() -> self_type&
{
    static self_type memento_list{};
    return memento_list;
}

auto MementoList::Undo()  -> bool
{
    if(_CurrentIter == _List.end())
    {
        return false;
    }

    (_CurrentIter++)->Restore();
    return true;
}

auto MementoList::Redo()  -> bool
{
    if(_CurrentIter == _List.begin())
    {
        return false;
    }

    (--_CurrentIter)->Restore();
    return true;
}

auto MementoList::GetCapacity() const -> std::size_t
{
    return _Capacity;
}

void MementoList::SetCapacity(std::size_t capacity)
{
    _Capacity = capacity;
}

auto MementoList::Size() const -> std::size_t
{
    return _List.size();
}

void MementoList::Clear()
{
    _List.clear();
}

auto MementoList::IsCurrent(const container_type::const_iterator& iter) const -> bool
{
    return iter == _CurrentIter;
}

auto MementoList::begin() noexcept -> container_type::iterator
{
    return _List.begin();
}

auto MementoList::end() noexcept -> container_type::iterator
{
    return _List.end();
}

auto MementoList::begin() const noexcept -> container_type::const_iterator
{
    return _List.begin();
}

auto MementoList::end() const noexcept -> container_type::const_iterator
{
    return _List.end();
}

auto MementoList::cbegin() const noexcept -> container_type::const_iterator
{
    return _List.cbegin();
}

auto MementoList::cend() const noexcept -> container_type::const_iterator
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
