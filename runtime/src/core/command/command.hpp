#pragma once

#include<functional>
#include<deque>
#include<memory>
#include<string>

#include"core/noncopyable.hpp"
#include"platform/api_def.hpp"

namespace Arcadia
{
    class Command
    {
    public:
        using FunctionType = std::function<void()>;
        using SelfType = Command;
    public:
        Command(
            const std::string& description,
            const FunctionType& execute_fn,
            const FunctionType& unexecute_fn
        );

        Command(SelfType&&) noexcept = default;
        auto operator=(SelfType&&) noexcept -> SelfType & = default;

        void Execute() const;
        void Unexecute() const;

        [[nodiscard]]
        auto GetDescription() const -> const std::string&;

    private:
        std::string _Description{};
        FunctionType _ExecuteFunction;
        FunctionType _UnexecuteFunction;
    };

    class CommandList: public Noncopyable
    {
    public:
        using FunctionType = Command::FunctionType;
        using ContainerType = std::deque<Command>;
        using SelfType = CommandList;
    public:
        [[nodiscard]]
        static auto Instance() -> SelfType&;

        void Emplace(
            const std::string& description,
            const FunctionType& execute_fn,
            const FunctionType& unexecute_fn
        );

        auto Undo() -> bool;

        auto Redo() -> bool;

        [[nodiscard]]
        auto GetMaxSize() const->std::size_t;
        void SetMaxSize(std::size_t capacity);

        [[nodiscard]]
        auto GetSize() const->std::size_t;

        void Clear();

    private:
        std::size_t _MaxSize{ 40 };
        ContainerType _Container{};
        ContainerType::iterator _CurrentIterator{};
    };
}