#include "pch.hpp"
#include "memento.hpp"

void arcadia::memento::restore() const
{
    _originator_restore_fn();
}

auto arcadia::memento::get_description() const -> const std::string&
{
    return _description;
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

    (_current_iter++)->restore();
}

auto arcadia::memento_list::redo()  -> bool
{
    if(_current_iter == _list.begin())
    {
        return false;
    }

    (--_current_iter)->restore();
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

auto arcadia::memento_list::is_current(const container_type::const_iterator& iter) const -> bool
{
    return iter == _current_iter;
}

auto arcadia::memento_list::begin() noexcept -> container_type::iterator
{
    return _list.begin();
}

auto arcadia::memento_list::end() noexcept -> container_type::iterator
{
    return _list.end();
}

auto arcadia::memento_list::begin() const noexcept -> container_type::const_iterator
{
    return _list.begin();
}

auto arcadia::memento_list::end() const noexcept -> container_type::const_iterator
{
    return _list.end();
}

auto arcadia::memento_list::cbegin() const noexcept -> container_type::const_iterator
{
    return _list.cbegin();
}

auto arcadia::memento_list::cend() const noexcept -> container_type::const_iterator
{
    return _list.cend();
}
