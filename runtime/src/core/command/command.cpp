#include "pch.hpp"
#include "command.hpp"

arcadia::command::command(
    const std::string& description,
    const function_type& execute_fn,
    const function_type& unexecute_fn
):
    _description(description),
    _execute_fn(execute_fn),
    _unexecute_fn(unexecute_fn)
{
    execute();
}

void arcadia::command::execute() const
{
    _execute_fn();
}

void arcadia::command::unexecute() const
{
    _unexecute_fn();
}

auto arcadia::command::get_description() const -> const std::string&
{
    return _description;
}

auto arcadia::command_list::instance() -> self_type&
{
    self_type command_list{};
    return command_list;
}

void arcadia::command_list::emplace(
    const std::string& description,
    const function_type& execute_fn,
    const function_type& unexecute_fn
)
{
    // Erase restored command since a new command should be on a new branch from current position
    _list.erase(_list.begin(), _current_iter);

    // Emplace new command
    _list.emplace_front(std::make_unique<arcadia::command>(description, execute_fn, unexecute_fn));

    // Relocate current position
    _current_iter = _list.begin();

    //Resize list
    _list.resize(_capacity);
}


auto arcadia::command_list::undo() -> bool
{
    if(_current_iter == _list.begin())
    {
        return false;
    }

    (*(_current_iter--))->unexecute();
    return true;
}

auto arcadia::command_list::redo() -> bool
{
    if((++_current_iter)-- == _list.end())
    {
        return false;
    }

    (*(_current_iter++))->execute();
    return true;
}

auto arcadia::command_list::get_capacity() const -> std::size_t
{
    return _capacity;
}

void arcadia::command_list::set_capacity(std::size_t capacity)
{
    _capacity = capacity;
}

auto arcadia::command_list::size() const -> std::size_t
{
    return _list.size();
}

void arcadia::command_list::clear()
{
    _list.clear();
}

auto arcadia::command_list::begin() noexcept -> container_type::iterator
{
    return _list.begin();
}

auto arcadia::command_list::end() noexcept -> container_type::iterator
{
    return _list.end();
}

auto arcadia::command_list::begin() const noexcept -> container_type::const_iterator
{
    return _list.begin();
}

auto arcadia::command_list::end() const noexcept -> container_type::const_iterator
{
    return _list.end();
}

auto arcadia::command_list::cbegin() const noexcept -> container_type::const_iterator
{
    return _list.cbegin();
}

auto arcadia::command_list::cend() const noexcept -> container_type::const_iterator
{
    return _list.cend();
}


