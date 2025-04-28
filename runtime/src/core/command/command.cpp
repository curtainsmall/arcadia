#include "pch.hpp"

#include "command.hpp"

#include "core/assert.hpp"

Arcadia::Command::Command(
    const std::string& description,
    const FunctionType& execute_fn,
    const FunctionType& unexecute_fn
):
    _Description(description),
    _ExecuteFunction(execute_fn),
    _UnexecuteFunction(unexecute_fn)
{}

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

void Arcadia::CommandList::Emplace(
    const std::string& description,
    const FunctionType& execute_fn,
    const FunctionType& unexecute_fn
)
{
    // Erase restored command since a new command should be on a new branch from current position
    _Container.erase(_Container.begin(), _CurrentIterator);

    // Emplace new command
    _Container.emplace_front(description, execute_fn, unexecute_fn);

    if(_Container.size() > _MaxSize)
    {
        _Container.pop_back();
    }

    // Relocate current position
    _CurrentIterator = _Container.begin();
}

void Arcadia::CommandList::EmplaceAndExecute(
    const std::string& description,
    const FunctionType& execute_fn,
    const FunctionType& unexecute_fn
)
{
    Emplace(description, execute_fn, unexecute_fn);
    begin()->Execute();
}

auto Arcadia::CommandList::Undo() -> bool
{
    if(_CurrentIterator == _Container.end())
    {
        return false;
    }

    _CurrentIterator->Unexecute();
    _CurrentIterator++;
    return true;
}

auto Arcadia::CommandList::Redo() -> bool
{
    if(_CurrentIterator == _Container.begin())
    {
        return false;
    }

    _CurrentIterator--;
    _CurrentIterator->Execute();
    return true;
}

auto Arcadia::CommandList::GetMaxSize() const -> std::size_t
{
    return _MaxSize;
}

void Arcadia::CommandList::SetMaxSize(std::size_t max_size)
{
    ACDA_ASSERT(max_size > 0);

    _MaxSize = max_size;

    std::ptrdiff_t redundant = _Container.size() - _MaxSize;
    if(redundant <= 0)
    {
        return;
    }

    std::ptrdiff_t offset_to_begin = _CurrentIterator - _Container.begin();
    std::ptrdiff_t available_after = _Container.end() - _CurrentIterator - 1;
    if(available_after < redundant)
    {
        std::ptrdiff_t removed_before = redundant - available_after;
        _Container.erase(
            _Container.begin(),
            _Container.begin() + removed_before
        );
        redundant = available_after;
        offset_to_begin -= removed_before;
    }

    _Container.erase(
        _Container.end() - redundant,
        _Container.end()
    );
    _CurrentIterator = _Container.begin() + offset_to_begin;
}

auto Arcadia::CommandList::GetSize() const -> std::size_t
{
    return _Container.size();
}

void Arcadia::CommandList::Clear()
{
    _Container.clear();
    _CurrentIterator = _Container.begin();
}

auto Arcadia::CommandList::IsCurrent(const ContainerType::const_iterator& iter) const -> bool
{
    return iter == _CurrentIterator;
}

auto Arcadia::CommandList::begin() noexcept -> ContainerType::iterator
{
    return _Container.begin();
}

auto Arcadia::CommandList::end() noexcept -> ContainerType::iterator
{
    return _Container.end();
}

auto Arcadia::CommandList::begin() const noexcept -> ContainerType::const_iterator
{
    return _Container.begin();
}

auto Arcadia::CommandList::end() const noexcept -> ContainerType::const_iterator
{
    return _Container.end();
}

auto Arcadia::CommandList::cbegin() const noexcept -> ContainerType::const_iterator
{
    return _Container.cbegin();
}

auto Arcadia::CommandList::cend() const noexcept -> ContainerType::const_iterator
{
    return _Container.cend();
}