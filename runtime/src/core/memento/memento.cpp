#include "pch.hpp"

#include "memento.hpp"

void Memento::restore() const
{
    _originator_restore_fn();
}

auto Memento::description() const -> const std::string&
{
    return _description;
}

auto MementoList::instance() -> self_type&
{
    static self_type memento_list{};
    return memento_list;
}

auto MementoList::undo()  -> bool
{
    if(_current_iter == _list.end())
    {
        return false;
    }

    (_current_iter++)->restore();
    return true;
}

auto MementoList::redo()  -> bool
{
    if(_current_iter == _list.begin())
    {
        return false;
    }

    (--_current_iter)->restore();
    return true;
}

auto MementoList::capacity() const -> size_t
{
    return _capacity;
}

void MementoList::capacity(size_t capacity)
{
    _capacity = capacity;
}

auto MementoList::size() const -> size_t
{
    return _list.size();
}

void MementoList::clear()
{
    _list.clear();
}

auto MementoList::is_current(const container_type::const_iterator& iter) const -> bool
{
    return iter == _current_iter;
}

auto MementoList::begin() noexcept -> container_type::iterator
{
    return _list.begin();
}

auto MementoList::end() noexcept -> container_type::iterator
{
    return _list.end();
}

auto MementoList::begin() const noexcept -> container_type::const_iterator
{
    return _list.begin();
}

auto MementoList::end() const noexcept -> container_type::const_iterator
{
    return _list.end();
}

auto MementoList::cbegin() const noexcept -> container_type::const_iterator
{
    return _list.cbegin();
}

auto MementoList::cend() const noexcept -> container_type::const_iterator
{
    return _list.cend();
}

auto iMementoOriginator::snapshot() -> std::shared_ptr<MementoDataBase>
{
    auto memento_data = on_snapshot();
    if(!_prev_memento_data)
    {
        _prev_memento_data = memento_data;
    }
    else if(memento_data != _prev_memento_data)
    {
        std::swap(memento_data, _prev_memento_data);
    }
    return memento_data;
}

void iMementoOriginator::restore(const std::shared_ptr<MementoDataBase>& memento_data)
{
    on_restore(memento_data);
    _prev_memento_data = memento_data;
}
