#pragma once

#include<functional>
#include<list>
#include<memory>

#include"core/base.hpp"

namespace arcadia
{
    template<class MementoData>
    struct ARCADIA_API memento_originator_interface
    {
    public:
        using memento_data_type = MementoData;
        using self_type = memento_originator_interface;
    public:

        /// @brief Generate a memento data
        /// @return Memento data
        [[nodiscard]]
        virtual auto snapshot() const->memento_data_type = 0;

        /// @brief Restore self with memento data
        /// @param memento_data Memento data to restore with
        virtual void restore(const memento_data_type& memento_data) = 0;
    };

    template<class MementoOriginator, class MementoData>
    concept memento_originator_like = requires{
        std::derived_from<MementoOriginator, arcadia::memento_originator_interface<MementoData>>;
    };

    struct ARCADIA_API memento: arcadia::noncopyable
    {
    public:
        using self_type = memento;
    public:
        /// @brief Create a memento
        /// @tparam MementoData Type of memento data
        /// @tparam ...Args Types of argument to construct memento data
        /// @tparam MementoOriginator Type of memento originator
        /// @param originator_retriever Function used to return referece to the originator (in case the originator is in an opaque structure, or may be destroyed and recreated so that its address is unreliable)
        /// @param ...args Arguments to construct memento data
        template<
            class MementoData,
            arcadia::memento_originator_like<MementoData> MementoOriginator,
            class ...Args
        >
        memento(
            const std::string& description,
            arcadia::in_place_types_t<MementoData, MementoOriginator>,
            const std::function<MementoOriginator& ()>& originator_retriever,
            Args&& ...args
        ):
            _description(description),
            _originator_retriever_uptr(
                new std::function<MementoOriginator& ()>{ originator_retriever },
                [&](void* ptr)
        {
            delete static_cast<std::function<MementoOriginator& ()>*>(ptr);
        }
            ),
            _memento_data_uptr(
                new MementoData(std::forward<Args>(args)...),
                [&](void* data_ptr)
        {
            delete static_cast<MementoData*>(data_ptr);
        }
            ),
            _originator_restore_fn(
                [&]()
        {
            MementoOriginator& originator =
                (*static_cast<std::function<MementoOriginator & ()>*>(_originator_retriever_uptr.get()))();
            MementoData& memento_data = *static_cast<MementoData*>(_memento_data_uptr.get());

            originator.restore(memento_data);
        }
            )
        {}

        /// @brief Restore originator with memento data
        void restore() const;

        [[nodiscard]]
        auto get_description() const -> const std::string&;

    private:
        std::string _description{};
        std::unique_ptr<void, std::function<void(void*)>> _originator_retriever_uptr{}; // Used to store originator retriever with type erasure
        std::unique_ptr<void, std::function<void(void*)>> _memento_data_uptr; // Used to store memento data with type erasure
        std::function<void()> _originator_restore_fn; // 1. call originator retriever to get originator; 2. get memento data; 3. call restore() in originator with memento data
    };

    struct ARCADIA_API memento_list: arcadia::noncopyable
    {
    public:
        using container_type = std::list<arcadia::memento>;
        using self_type = memento_list;
    public:
        static auto instance() -> self_type&;

        /// @brief Snapshot @a MementoOriginator
        /// @tparam MementaData Type of memento data
        /// @tparam MementoOriginator Type of memento originator
        /// @param originator Originator to snapshot
        template<
            class MementoData,
            arcadia::memento_originator_like<MementoData> MementoOriginator
        >
        void snapshot(
            const std::string& description,
            const std::function<MementoOriginator& ()>& originator_retriever
        )
        {
            // Erase restored mementos since a new memento should be on a new branch from current position
            _list.erase(_list.begin(), _current_iter);

            // Emplace new memento
            _list.emplace_front(description, arcadia::in_place_types<MementoData, MementoOriginator>, originator_retriever, originator_retriever().snapshot());

            // Relocate current position
            _current_iter = _list.begin();

        }

        /// @brief Restore prev memento
        /// @return True, if succeed; False, if there is no prev memento to restore
        [[nodiscard]]
        auto undo() -> bool;

        /// @brief Restore next memento
        /// @return True, if succeed; False, if there is no next memento to restore
        [[nodiscard]]
        auto redo() -> bool;

        [[nodiscard]]
        auto get_capacity() const->std::size_t;
        void set_capacity(std::size_t capacity);

        [[nodiscard]]
        auto size() const->std::size_t;

        void clear();

        /// @brief Check whether the memento refered by @a iter is current memento
        /// @param iter Iterator referring to a memento
        /// @note @a Current @a memento is the next memento to use when undo:
        ///                                                   current
        ///     memento#1 <--- memento#2 <--- memento#3 <--- memento#4 <---   ---> memento#5
        ///               undo           undo           undo           undo   redo
        [[nodiscard]]
        auto is_current(const container_type::const_iterator& iter) const -> bool;

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
        container_type::iterator _current_iter{ _list.begin() }; // Points to the memento to be undone
    };

}
