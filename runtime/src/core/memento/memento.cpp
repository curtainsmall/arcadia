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
    return true;
}

auto Arcadia::MementoList::Redo()  -> bool
{
    if(_CurrentIter == _List.begin())
    {
        return false;
    }

    (--_CurrentIter)->Restore();
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

auto Arcadia::iMementoOriginator::Snapshot() -> std::shared_ptr<Arcadia::MementoDataBase>
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

void Arcadia::iMementoOriginator::Restore(const std::shared_ptr<Arcadia::MementoDataBase>& memento_data)
{
    OnRestore(memento_data);
    _PreviousMementoData = memento_data;
}
