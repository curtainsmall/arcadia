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

        [[nodiscard]]
        virtual auto snapshot() const->memento_data_type = 0;
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
        /// @brief Create a memento for give originator with give data
        /// @tparam MementoData Type of memento data
        /// @tparam MementoOriginator Type of memento originator
        /// @tparam ...Args Types of argument to construct memento data
        /// @param originator_wptr Weak pointer a the originator
        /// @param ...args Arguments to cosntruct memento data
        template<
            class MementoData,
            arcadia::memento_originator_like<MementoData> MementoOriginator,
            class ...Args
        >
        memento(
            const std::weak_ptr<arcadia::memento_originator_interface<MementoData>>& originator_wptr,
            Args&& ...args
        ):
            _originator_wptr(originator_wptr),
            _data_uptr(
                new MementoData(std::forward<Args>(args)...),
                [&](void* data_ptr)
        {
            delete static_cast<MementoData*>(data_ptr);
        }
            ),
            _originator_dispatcher(
                [&](const std::shared_ptr<void>& _originator_sptr)
        {
            std::static_pointer_cast<MementoOriginator*>(_originator_sptr)->restore(*static_cast<MementoData*>(_data_uptr.get()));
        }
            )
        {}

        /// @brief Restore the originator with stored data
        /// @return True, if succeed; False, if the originator no longer exists (the weak_ptr is expired)
        auto restore() const -> bool;

    private:
        std::weak_ptr<void> _originator_wptr;
        std::unique_ptr<void, std::function<void(void*)>> _data_uptr;
        std::function<void(const std::shared_ptr<void>&)> _originator_dispatcher;
    };

    struct ARCADIA_API memento_list: arcadia::noncopyable
    {
    public:
        using container_type = std::list<memento>;
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
            const arcadia::memento_originator_interface<MementoData>& originator
        )
        {
            // Erase restored mementos since a new memento should be on a new branch from current position
            _list.erase(_list.begin(), _current_iter);

            // Emplace new memento
            _list.emplace_front(std::make_unique<MementoData>(originator.snapshot()));

            // Relocate current position
            _current_iter = _list.begin();

            //Resize list
            _list.resize(_capacity);

        }

        [[nodiscard]]
        auto capacity() const->std::size_t;
        auto capacity(std::size_t capacity);

        [[nodiscard]]
        auto size() const->std::size_t;

        void clear();

    private:
        std::size_t _capacity{ 40 };
        container_type _list{};
        container_type::iterator _current_iter{ _list.begin() };
    };

}
