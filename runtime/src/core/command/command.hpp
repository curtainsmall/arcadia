#pragma once

#include<functional>
#include<list>
#include<memory>
#include<string>

#include"core/base.hpp"

namespace Arcadia
{

    struct Command: Arcadia::Noncopyable
    {
    public:
        using function_type = std::function<void()>;
        using self_type = Command;
    public:
        Command(
            const std::string& description,
            const function_type& execute_fn,
            const function_type& unexecute_fn
        );

        Command(self_type&&) noexcept = default;
        auto operator=(self_type&&) noexcept -> self_type & = default;

        void Execute() const;
        void Unexecute() const;

        [[nodiscard]]
        auto GetDescription() const -> const std::string&;

    private:
        std::string _Description{};
        function_type _ExecuteFn;
        function_type _UnexecuteFn;
    };

    struct CommandList: Arcadia::Noncopyable
    {
    public:
        using function_type = Arcadia::Command::function_type;
        using container_type = std::list<Arcadia::Command>;
        using self_type = CommandList;
    public:
        [[nodiscard]]
        static auto Instance() -> self_type&;

        /// @brief Create a command
        /// @param execute_fn Function to execute
        /// @param unexecute_fn Function to unexecute
        /// @note The command will be executed once when it is contructed
        void emplace(
            const std::string& description,
            const function_type& execute_fn,
            const function_type& unexecute_fn
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
        auto Size() const->std::size_t;

        /// @brief Clear command list (when you saved the project and no longer needs previous commands)
        void Clear();

        [[nodiscard]]
        auto begin() noexcept -> container_type::iterator;
        [[nodiscard]]
        auto end() noexcept -> container_type::iterator;

        [[nodiscard]]
        auto begin() const noexcept->container_type::const_iterator;
        [[nodiscard]]
        auto end() const noexcept->container_type::const_iterator;

        [[nodiscard]]
        auto cbegin() const noexcept->container_type::const_iterator;
        [[nodiscard]]
        auto cend() const noexcept->container_type::const_iterator;

    private:
        std::size_t _Capacity{ 40 };
        container_type _List{};
        container_type::const_iterator _CurrentIter{};
    };
}
