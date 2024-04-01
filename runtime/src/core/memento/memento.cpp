#include "pch.hpp"
#include "memento.hpp"

void arcadia::memento::restore() const
{
    _originator_restore_fn();
}

auto arcadia::memento_list::instance() -> self_type&
{
    static self_type memento_list{};
    return memento_list;
}

auto arcadia::memento_list::undo()  -> bool
{
    if(_current_iter == _list.end())
    {
        return false;
    }

    (*(_current_iter++))->restore();
}

auto arcadia::memento_list::redo()  -> bool
{
    if(_current_iter == _list.begin())
    {
        return false;
    }

    (*(--_current_iter))->restore();
}

auto arcadia::memento_list::get_capacity() const -> std::size_t
{
    return _capacity;
}

void arcadia::memento_list::set_capacity(std::size_t capacity)
{
    _capacity = capacity;
}

auto arcadia::memento_list::size() const -> std::size_t
{
    return _list.size();
}


void arcadia::memento_list::clear()
{
    _list.clear();
}
