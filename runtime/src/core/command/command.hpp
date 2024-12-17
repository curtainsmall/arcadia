#pragma once

#include<functional>
#include<list>
#include<memory>
#include<string>

#include"core/noncopyable.hpp"
#include"platform/api_def.hpp"

namespace Arcadia
{
    class Command: public Noncopyable
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
        using ContainerType = std::list<Command>;
        using SelfType = CommandList;
    public:
        [[nodiscard]]
        static auto Instance() -> SelfType&;

        /// @brief Create a command
        /// @param execute_fn Function to execute
        /// @param unexecute_fn Function to unexecute
        /// @note The command will be executed once when it is contructed
        void emplace(
            const std::string& description,
            const FunctionType& execute_fn,
            const FunctionType& unexecute_fn
        );

        /// @brief Call unexecute() and move to the previous command
        /// @return Whether succeed
        auto Undo() -> bool;

        /// @brief Call execute() and move to the next command
        /// @return Whether succeed
        auto Redo() -> bool;

        [[nodiscard]]
        auto GetCapacity() const->std::size_t;
        void SetCapacity(std::size_t capacity);

        /// @brief Get size of command list
        /// @return Size
        [[nodiscard]]
        auto GetSize() const->std::size_t;

        /// @brief Clear command list (when you saved the project and no longer needs previous commands)
        void Clear();

        [[nodiscard]]
        auto begin() noexcept -> ContainerType::iterator;
        [[nodiscard]]
        auto end() noexcept -> ContainerType::iterator;

        [[nodiscard]]
        auto begin() const noexcept->ContainerType::const_iterator;
        [[nodiscard]]
        auto end() const noexcept->ContainerType::const_iterator;

        [[nodiscard]]
        auto cbegin() const noexcept->ContainerType::const_iterator;
        [[nodiscard]]
        auto cend() const noexcept->ContainerType::const_iterator;

    private:
        std::size_t _Capacity{ 40 };
        ContainerType _List{};
        ContainerType::const_iterator _CurrentIterator{};
    };
}