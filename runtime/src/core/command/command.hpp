#pragma once

#include<functional>
#include<list>
#include<memory>
#include<string>

#include"core/base.hpp"

struct Command: Noncopyable
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

    void execute() const;
    void unexecute() const;

    [[nodiscard]]
    auto description() const -> const std::string&;

private:
    std::string _description{};
    function_type execute_fn;
    function_type _unexecute_fn;
};

struct CommandList: Noncopyable
{
public:
    using function_type = Command::function_type;
    using container_type = std::list<Command>;
    using self_type = CommandList;
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
    auto capacity() const->size_t;
    void capacity(size_t capacity);

    /// @brief Get size of command list
    /// @return Size
    [[nodiscard]]
    auto size() const->size_t;

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
    size_t _capacity{ 40 };
    container_type _list{};
    container_type::const_iterator _current_iter{};
};
