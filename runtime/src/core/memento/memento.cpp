#include "pch.hpp"
#include "memento.hpp"

auto arcadia::memento::restore() const -> bool
{
    if(_originator_wptr.expired())
    {
        return false;
    }
    else
    {
        _originator_dispatcher(_originator_wptr.lock());
    }
}

auto arcadia::memento_list::instance() -> self_type&
{
    static self_type memento_list{};
    return memento_list;
}

auto arcadia::memento_list::capacity() const -> std::size_t
{
    return _capacity;
}

auto arcadia::memento_list::capacity(std::size_t capacity)
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
