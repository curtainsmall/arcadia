#include "pch.hpp"
#include "command.hpp"

auto arcadia::command_list::instance() -> self_type&
{
    self_type command_list{};
    return command_list;
}

auto arcadia::command_list::undo() -> bool
{
    if(_current_iter == _list.cbegin())
    {
        return false;
    }

    (*_current_iter--)->unexecute();
    return true;
}

auto arcadia::command_list::redo() -> bool
{
    if(++_current_iter == _list.cend())
    {
        return false;
    }

    (*_current_iter++)->execute();
    return true;
}

auto arcadia::command_list::size() const -> std::size_t
{
    return _list.size();
}

void arcadia::command_list::clear()
{
    _list.clear();
}

