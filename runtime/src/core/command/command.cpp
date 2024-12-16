#include "pch.hpp"

#include "command.hpp"

Arcadia::Command::Command(
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

void Arcadia::Command::Execute() const
{
    _ExecuteFunction();
}

void Arcadia::Command::Unexecute() const
{
    _UnexecuteFunction();
}

auto Arcadia::Command::GetDescription() const -> const std::string&
{
    return _Description;
}

auto Arcadia::CommandList::Instance() -> SelfType&
{
    static SelfType command_list{};
    return command_list;
}

void Arcadia::CommandList::emplace(
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

auto Arcadia::CommandList::Undo() -> bool
{
    if(_CurrentIterator == _List.begin())
    {
        return false;
    }

    (_CurrentIterator--)->Unexecute();
    return true;
}

auto Arcadia::CommandList::Redo() -> bool
{
    if((++_CurrentIterator)-- == _List.end())
    {
        return false;
    }

    (_CurrentIterator++)->Execute();
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

auto Arcadia::CommandList::GetSize() const -> std::size_t
{
    return _List.size();
}

void Arcadia::CommandList::Clear()
{
    _List.clear();
}

auto Arcadia::CommandList::begin() noexcept -> ContainerType::iterator
{
    return _List.begin();
}

auto Arcadia::CommandList::end() noexcept -> ContainerType::iterator
{
    return _List.end();
}

auto Arcadia::CommandList::begin() const noexcept -> ContainerType::const_iterator
{
    return _List.begin();
}

auto Arcadia::CommandList::end() const noexcept -> ContainerType::const_iterator
{
    return _List.end();
}

auto Arcadia::CommandList::cbegin() const noexcept -> ContainerType::const_iterator
{
    return _List.cbegin();
}

auto Arcadia::CommandList::cend() const noexcept -> ContainerType::const_iterator
{
    return _List.cend();
}