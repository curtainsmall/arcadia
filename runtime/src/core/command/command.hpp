#pragma once

#include<functional>
#include<list>
#include<memory>

#include"core/base.hpp"

namespace arcadia
{
    struct ARCADIA_API command_interface: arcadia::noncopyable
    {
    public:
        using self_type = command_interface;
    public:
        virtual void execute() const = 0;
        virtual void unexecute() const = 0;
    };

    template<class Command>
    concept command_like = requires{
        std::derived_from<Command, arcadia::command_interface>;
    };

    struct ARCADIA_API command: arcadia::command_interface
    {
    public:
        using function_type = std::function<void()>;
        using self_type = command;
    public:
        inline command(
            const function_type& execute_fn,
            const function_type& unexecute_fn ={
            []()
            {}
            }
        ):
            _execute_fn(execute_fn),
            _unexecute_fn(unexecute_fn)
        {}

        inline void execute() const override
        {
            _execute_fn();
        }
        inline void unexecute() const override
        {
            _unexecute_fn();
        }

    private:
        function_type _execute_fn;
        function_type _unexecute_fn;
    };

    struct ARCADIA_API command_list: arcadia::noncopyable
    {
    public:
        using container_type = std::list<std::unique_ptr<arcadia::command_interface>>;
        using self_type = command_list;
    public:
        [[nodiscard]]
        static auto instance() -> self_type&;

        /// @brief Execute a new command and emplace it to the list
        /// @tparam ...Args Types of argument to construct the command
        /// @tparam Command Type of command to construct
        /// @param ...args Arguments to construct the command
        /// @note If the current command is not the last command (some commands have been undone), the remaining commands will be erased 
        template<arcadia::command_like Command, class ...Args>
        void execute(Args&& ...args)
        {
            std::unique_ptr<arcadia::command_interface> command_uptr = std::make_unique<Command>(std::forward<Args>(args)...);
            command_uptr->execute();

            if(capacity > 0)
            {
                ARCADIA_ASSERT(_current_iter != _list.end());
                _list.erase(++_current_iter, _list.end());
                _list.emplace_back(std::move(command_uptr));
                _current_iter = --_list.end();
                while(_list.size() > capacity)
                {
                    if(_current_iter == _list.cbegin())
                    {
                        ++_current_iter;
                    }
                    _list.pop_front();
                }
            }

        }

        /// @brief Call unexecute() and move to the previous command
        /// @return Whether succeed
        auto undo() -> bool;

        /// @brief Call execute() and move to the next command
        /// @return Whether succeed
        auto redo() -> bool;

        /// @brief Get size of command list
        /// @return Size
        [[nodiscard]]
        auto size() const->std::size_t;

        /// @brief Clear command list (when you saved the project and no longer needs previous commands)
        void clear();

    public:
        std::size_t capacity{ 40 };
    private:
        container_type _list{};
        container_type::const_iterator _current_iter{};
    };
}
