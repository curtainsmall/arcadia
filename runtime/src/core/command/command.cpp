#include "pch.hpp"

#include "command.hpp"

Command::Command(
    const std::string& description,
    const FunctionType& execute_fn,
    const FunctionType& unexecute_fn
) :
    _Description(description),
    _ExecuteFunction(execute_fn),
    _UnexecuteFunction(unexecute_fn)
{
    Execute();
}

void Command::Execute() const
{
    _ExecuteFunction();
}

void Command::Unexecute() const
{
    _UnexecuteFunction();
}

auto Command::GetDescription() const -> const std::string&
{
    return _Description;
}

auto CommandList::Instance() -> SelfType&
{
    SelfType command_list{};
    return command_list;
}

void CommandList::emplace(
    const std::string& description,
    const FunctionType& execute_fn,
    const FunctionType& unexecute_fn
)
{
    // Erase restored command since a new command should be on a new branch from current position
    _List.erase(_List.begin(), _CurrentIterator);

    // Emplace new command
    _List.emplace_front(description, execute_fn, unexecute_fn);

    // Relocate current position
    _CurrentIterator = _List.begin();
}

auto CommandList::Undo() -> bool
{
    if(_CurrentIterator == _List.begin())
    {
        return false;
    }

    (_CurrentIterator--)->Unexecute();
    return true;
}

auto CommandList::Redo() -> bool
{
    if((++_CurrentIterator)-- == _List.end())
    {
        return false;
    }

    (_CurrentIterator++)->Execute();
    return true;
}

auto CommandList::GetCapacity() const -> size_t
{
    return _Capacity;
}

void CommandList::SetCapacity(size_t capacity)
{
    _Capacity = capacity;
}

auto CommandList::GetSize() const -> size_t
{
    return _List.size();
}

void CommandList::Clear()
{
    _List.clear();
}

auto CommandList::begin() noexcept -> ContainerType::iterator
{
    return _List.begin();
}

auto CommandList::end() noexcept -> ContainerType::iterator
{
    return _List.end();
}

auto CommandList::begin() const noexcept -> ContainerType::const_iterator
{
    return _List.begin();
}

auto CommandList::end() const noexcept -> ContainerType::const_iterator
{
    return _List.end();
}

auto CommandList::cbegin() const noexcept -> ContainerType::const_iterator
{
    return _List.cbegin();
}

auto CommandList::cend() const noexcept -> ContainerType::const_iterator
{
    return _List.cend();
}