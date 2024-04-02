#pragma once

#include<functional>
#include<list>
#include<memory>
#include<string>

#include"core/base.hpp"

namespace arcadia
{

    struct ARCADIA_API command: arcadia::noncopyable
    {
    public:
        using function_type = std::function<void()>;
        using self_type = command;
    public:
        command(
            const std::string& description,
            const function_type& execute_fn,
            const function_type& unexecute_fn
        );

        command(self_type&&) noexcept = default;
        auto operator=(self_type&&) noexcept -> self_type & = default;

        void execute() const;
        void unexecute() const;

        [[nodiscard]]
        auto get_description() const -> const std::string&;

    private:
        std::string _description{};
        function_type _execute_fn;
        function_type _unexecute_fn;
    };

    struct ARCADIA_API command_list: arcadia::noncopyable
    {
    public:
        using function_type = arcadia::command::function_type;
        using container_type = std::list<arcadia::command>;
        using self_type = command_list;
    public:
        [[nodiscard]]
        static auto instance() -> self_type&;

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
        auto undo() -> bool;

        /// @brief Call execute() and move to the next command
        /// @return Whether succeed
        auto redo() -> bool;

        [[nodiscard]]
        auto get_capacity() const->std::size_t;
        void set_capacity(std::size_t capacity);

        /// @brief Get size of command list
        /// @return Size
        [[nodiscard]]
        auto size() const->std::size_t;

        /// @brief Clear command list (when you saved the project and no longer needs previous commands)
        void clear();

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
        std::size_t _capacity{ 40 };
        container_type _list{};
        container_type::const_iterator _current_iter{};
    };
}
