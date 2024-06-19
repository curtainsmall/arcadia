#include "pch.hpp"

#include "command.hpp"

Command::Command(
    const std::string& description,
    const function_type& execute_fn,
    const function_type& unexecute_fn
):
    _description(description),
    execute_fn(execute_fn),
    _unexecute_fn(unexecute_fn)
{
    execute();
}

void Command::execute() const
{
    execute_fn();
}

void Command::unexecute() const
{
    _unexecute_fn();
}

auto Command::description() const -> const std::string&
{
    return _description;
}

auto CommandList::instance() -> self_type&
{
    self_type command_list{};
    return command_list;
}

void CommandList::emplace(
    const std::string& description,
    const function_type& execute_fn,
    const function_type& unexecute_fn
)
{
    // Erase restored command since a new command should be on a new branch from current position
    _list.erase(_list.begin(), _current_iter);

    // Emplace new command
    _list.emplace_front(description, execute_fn, unexecute_fn);

    // Relocate current position
    _current_iter = _list.begin();
}

auto CommandList::undo() -> bool
{
    if(_current_iter == _list.begin())
    {
        return false;
    }

    (_current_iter--)->unexecute();
    return true;
}

auto CommandList::redo() -> bool
{
    if((++_current_iter)-- == _list.end())
    {
        return false;
    }

    (_current_iter++)->execute();
    return true;
}

auto CommandList::capacity() const -> std::size_t
{
    return _capacity;
}

void CommandList::capacity(std::size_t capacity)
{
    _capacity = capacity;
}

auto CommandList::size() const -> std::size_t
{
    return _list.size();
}

void CommandList::clear()
{
    _list.clear();
}

auto CommandList::begin() noexcept -> container_type::iterator
{
    return _list.begin();
}

auto CommandList::end() noexcept -> container_type::iterator
{
    return _list.end();
}

auto CommandList::begin() const noexcept -> container_type::const_iterator
{
    return _list.begin();
}

auto CommandList::end() const noexcept -> container_type::const_iterator
{
    return _list.end();
}

auto CommandList::cbegin() const noexcept -> container_type::const_iterator
{
    return _list.cbegin();
}

auto CommandList::cend() const noexcept -> container_type::const_iterator
{
    return _list.cend();
}


