#pragma once

#include<functional>
#include<list>
#include<memory>

#include"core/base.hpp"

struct MementoDataBase;

template<class MementoData>
concept cMementoData = requires{
    std::derived_from<MementoData, MementoDataBase>;
    std::equality_comparable<MementoData>;
};

struct MementoDataBase
{
public:
    template<class MementoData>
    auto as() -> MementoData&
    {
        return static_cast<MementoData&>(*this);
    }
};

struct iMementoOriginator
{
public:
    using self_type = iMementoOriginator;
public:
    auto snapshot() -> std::shared_ptr<MementoDataBase>;
    void restore(const std::shared_ptr<MementoDataBase>& sp_memento_data);

protected:
    /// @brief Generate a memento data
    /// @return Memento data
    [[nodiscard]]
    virtual auto on_snapshot() const->std::shared_ptr<MementoDataBase> = 0;

    /// @brief Restore self with memento data
    /// @param memento_data Memento data to restore with
    virtual void on_restore(const std::shared_ptr<MementoDataBase>& memento_data) = 0;

private:
    std::shared_ptr<MementoDataBase> _prev_memento_data{};
};

template<class MementoOriginator>
concept cMementoOriginator = requires{
    std::derived_from<MementoOriginator, iMementoOriginator>;
};

struct Memento: Noncopyable
{
public:
    using self_type = Memento;
public:
    /// @brief Create a memento
    /// @tparam MementoOriginator Type of memento originator
    /// @param originator_retriever Function used to return referece to the originator (in case the originator is in an opaque structure, or may be destroyed and recreated so that its address is unreliable)
    /// @param ...args Arguments to construct memento data

    /// @brief Create a memento
    /// @tparam MementoOriginator Type of memento originator
    /// @param description Description
    /// @param in_place_type_originator Type deduction helper for @a MementoOriginator
    /// @param originator_retriever Originator retriever
    /// @param memento_data Memento data
    template<
        cMementoOriginator MementoOriginator
    >
    Memento(
        const std::string& description,
        std::in_place_type_t<MementoOriginator> in_place_type_originator,
        const std::function<MementoOriginator& ()>& originator_retriever,
        const std::shared_ptr<MementoDataBase>& memento_data
    ):
        _description(description),
        _originator_retriever(
            new std::function<MementoOriginator& ()>{ originator_retriever },
            [&](void* ptr)
    {
        delete static_cast<std::function<MementoOriginator& ()>*>(ptr);
    }
        ),
        _memento_data(memento_data),
        _originator_restore_fn(
            [&]()
    {
        MementoOriginator& originator = (*static_cast<std::function<MementoOriginator & ()>*>(_originator_retriever.get()))();
        originator.restore(_memento_data);
    }
        )
    {}

    /// @brief Restore originator with memento data
    void restore() const;

    [[nodiscard]]
    auto description() const -> const std::string&;

private:
    std::string _description{};
    std::unique_ptr<void, std::function<void(void*)>> _originator_retriever{}; // Used to store originator retriever with type erasure
    std::shared_ptr<MementoDataBase> _memento_data; // Used to store memento data with type erasure
    std::function<void()> _originator_restore_fn; // 1. call originator retriever to get originator; 2. get memento data; 3. call restore() in originator with memento data
};

struct MementoList: Noncopyable
{
public:
    using container_type = std::list<Memento>;
    using self_type = MementoList;
public:
    static auto instance() -> self_type&;

    /// @brief Snapshot @a MementoOriginator
    /// @tparam MementoOriginator Type of memento originator
    /// @param description Description
    /// @param originator_retriever Originator to snapshot
    template<cMementoOriginator MementoOriginator>
    void snapshot(
        const std::string& description,
        const std::function<MementoOriginator& ()>& originator_retriever
    )
    {
        // Erase restored mementos since a new memento should be on a new branch from current position
        _list.erase(_list.begin(), _current_iter);

        // Emplace new memento
        _list.emplace_front(description, std::in_place_type<MementoOriginator>, originator_retriever, originator_retriever().snapshot());

        // Relocate current position
        _current_iter = _list.begin();

    }

    /// @brief Restore prev memento
    /// @return True, if succeed; False, if there is no prev memento to restore
    auto undo() -> bool;

    /// @brief Restore next memento
    /// @return True, if succeed; False, if there is no next memento to restore
    auto redo() -> bool;

    [[nodiscard]]
    auto capacity() const->size_t;
    void capacity(size_t capacity);

    [[nodiscard]]
    auto size() const->size_t;

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
    size_t _capacity{ 40 };
    container_type _list{};
    container_type::iterator _current_iter{ _list.begin() }; // Points to the memento to be undone
};
