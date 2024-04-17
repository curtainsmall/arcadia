#include "pch.hpp"

#include "command.hpp"

Arcadia::Command::Command(
    const std::string& description,
    const function_type& execute_fn,
    const function_type& unexecute_fn
):
    _Description(description),
    _ExecuteFn(execute_fn),
    _UnexecuteFn(unexecute_fn)
{
    Execute();
}

void Arcadia::Command::Execute() const
{
    _ExecuteFn();
}

void Arcadia::Command::Unexecute() const
{
    _UnexecuteFn();
}

auto Arcadia::Command::GetDescription() const -> const std::string&
{
    return _Description;
}

auto Arcadia::CommandList::Instance() -> self_type&
{
    self_type command_list{};
    return command_list;
}

void Arcadia::CommandList::emplace(
    const std::string& description,
    const function_type& execute_fn,
    const function_type& unexecute_fn
)
{
    // Erase restored command since a new command should be on a new branch from current position
    _List.erase(_List.begin(), _CurrentIter);

    // Emplace new command
    _List.emplace_front(description, execute_fn, unexecute_fn);

    // Relocate current position
    _CurrentIter = _List.begin();
}

auto Arcadia::CommandList::Undo() -> bool
{
    if(_CurrentIter == _List.begin())
    {
        return false;
    }

    (_CurrentIter--)->Unexecute();
    return true;
}

auto Arcadia::CommandList::Redo() -> bool
{
    if((++_CurrentIter)-- == _List.end())
    {
        return false;
    }

    (_CurrentIter++)->Execute();
    return true;
}

auto Arcadia::CommandList::GetCapacity() const -> std::size_t
{
    return _Capacity;
}

void Arcadia::CommandList::SetCapacity(std::size_t capacity)
{
    _Capacity = capacity;
}

auto Arcadia::CommandList::Size() const -> std::size_t
{
    return _List.size();
}

void Arcadia::CommandList::Clear()
{
    _List.clear();
}

auto Arcadia::CommandList::begin() noexcept -> container_type::iterator
{
    return _List.begin();
}

auto Arcadia::CommandList::end() noexcept -> container_type::iterator
{
    return _List.end();
}

auto Arcadia::CommandList::begin() const noexcept -> container_type::const_iterator
{
    return _List.begin();
}

auto Arcadia::CommandList::end() const noexcept -> container_type::const_iterator
{
    return _List.end();
}

auto Arcadia::CommandList::cbegin() const noexcept -> container_type::const_iterator
{
    return _List.cbegin();
}

auto Arcadia::CommandList::cend() const noexcept -> container_type::const_iterator
{
    return _List.cend();
}


