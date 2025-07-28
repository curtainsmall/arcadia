#pragma once

#include <deque>
#include <functional>
#include <memory>
#include <string>

#include "core/noncopyable.hpp"
#include "platform/defines.hpp"

namespace Arcadia
{
    struct ACDA_API Command
    {
    public:
        using FunctionType = std::function<void()>;
        using SelfType = Command;
    public:
        Command(
            std::string_view description,
            const FunctionType& execute_fn,
            const FunctionType& unexecute_fn
        );

        Command(SelfType&&) noexcept = default;
        auto operator=(SelfType&&) noexcept -> SelfType & = default;

        void Execute() const;
        void Unexecute() const;

        [[nodiscard]]
        auto GetDescription() const -> std::string_view;

    private:
        std::string _Description{};
        FunctionType _ExecuteFunction;
        FunctionType _UnexecuteFunction;
    };

    struct ACDA_API CommandList: public Noncopyable
    {
    public:
        using FunctionType = Command::FunctionType;
        using ContainerType = std::deque<Command>;
        using SelfType = CommandList;
    public:
        [[nodiscard]]
        static auto Instance() -> SelfType&;

        // The latest command is emplaced at the begin of the deque
        void Emplace(
            std::string_view description,
            const FunctionType& execute_fn,
            const FunctionType& unexecute_fn
        );

        void EmplaceAndExecute(
            std::string_view description,
            const FunctionType& execute_fn,
            const FunctionType& unexecute_fn
        );

        auto Undo() -> bool;

        auto Redo() -> bool;

        [[nodiscard]]
        auto GetMaxSize() const -> std::size_t;
        void SetMaxSize(std::size_t capacity);

        [[nodiscard]]
        auto GetSize() const -> std::size_t;

        void Clear();

        [[nodiscard]]
        auto IsCurrent(const ContainerType::const_iterator& iter) const -> bool;

        [[nodiscard]]
        auto begin() noexcept -> ContainerType::iterator;
        [[nodiscard]]
        auto end() noexcept -> ContainerType::iterator;

        [[nodiscard]]
        auto begin() const noexcept -> ContainerType::const_iterator;
        [[nodiscard]]
        auto end() const noexcept -> ContainerType::const_iterator;

        [[nodiscard]]
        auto cbegin() const noexcept -> ContainerType::const_iterator;
        [[nodiscard]]
        auto cend() const noexcept -> ContainerType::const_iterator;

    private:
        std::size_t _MaxSize{ 40 };
        ContainerType _Container{};
        ContainerType::iterator _CurrentIterator{ _Container.end() };
    };
}