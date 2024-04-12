#pragma once

#include<functional>
#include<list>
#include<memory>

#include"core/base.hpp"

namespace Arcadia
{
    struct ARCADIA_API MementoDataBase;

    template<class MementoData>
    concept cMementoData = requires{
        std::derived_from<MementoData, MementoDataBase>;
        std::equality_comparable<MementoData>;
    };

    struct ARCADIA_API MementoDataBase
    {
    public:
        template<class MementoData>
        auto As() -> MementoData&
        {
            return static_cast<MementoData&>(*this);
        }
    };

    struct ARCADIA_API iMementoOriginator
    {
    public:
        using self_type = iMementoOriginator;
    public:
        auto Snapshot() -> std::shared_ptr<Arcadia::MementoDataBase>;
        void Restore(const std::shared_ptr<Arcadia::MementoDataBase>& sp_memento_data);

    protected:
        /// @brief Generate a memento data
        /// @return Memento data
        [[nodiscard]]
        virtual auto OnSnapshot() const->std::shared_ptr<Arcadia::MementoDataBase> = 0;

        /// @brief Restore self with memento data
        /// @param memento_data Memento data to restore with
        virtual void OnRestore(const std::shared_ptr<Arcadia::MementoDataBase>& memento_data) = 0;

    private:
        std::shared_ptr<MementoDataBase> _spPreviousMementoData{};
    };

    template<class MementoOriginator>
    concept cMementoOriginator = requires{
        std::derived_from<MementoOriginator, Arcadia::iMementoOriginator>;
    };

    struct ARCADIA_API Memento: Arcadia::Noncopyable
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
        /// @param sp_memento_data Memento data
        template<
            Arcadia::cMementoOriginator MementoOriginator
        >
        Memento(
            const std::string& description,
            std::in_place_type_t<MementoOriginator> in_place_type_originator,
            const std::function<MementoOriginator& ()>& originator_retriever,
            const std::shared_ptr<Arcadia::MementoDataBase>& sp_memento_data
        ):
            _Description(description),
            _upOriginatorRetriever(
                new std::function<MementoOriginator& ()>{ originator_retriever },
                [&](void* ptr)
        {
            delete static_cast<std::function<MementoOriginator& ()>*>(ptr);
        }
            ),
            _spMementoData(sp_memento_data),
            _OriginatorRestoreFn(
                [&]()
        {
            MementoOriginator& originator = (*static_cast<std::function<MementoOriginator & ()>*>(_upOriginatorRetriever.get()))();
            originator.Restore(_spMementoData);
        }
            )
        {}

        /// @brief Restore originator with memento data
        void Restore() const;

        [[nodiscard]]
        auto GetDescription() const -> const std::string&;

    private:
        std::string _Description{};
        std::unique_ptr<void, std::function<void(void*)>> _upOriginatorRetriever{}; // Used to store originator retriever with type erasure
        std::shared_ptr<Arcadia::MementoDataBase> _spMementoData; // Used to store memento data with type erasure
        std::function<void()> _OriginatorRestoreFn; // 1. call originator retriever to get originator; 2. get memento data; 3. call restore() in originator with memento data
    };

    struct ARCADIA_API MementoList: Arcadia::Noncopyable
    {
    public:
        using container_type = std::list<Arcadia::Memento>;
        using self_type = MementoList;
    public:
        static auto Instance() -> self_type&;

        /// @brief Snapshot @a MementoOriginator
        /// @tparam MementoOriginator Type of memento originator
        /// @param description Description
        /// @param originator_retriever Originator to snapshot
        template<Arcadia::cMementoOriginator MementoOriginator>
        void Snapshot(
            const std::string& description,
            const std::function<MementoOriginator& ()>& originator_retriever
        )
        {
            // Erase restored mementos since a new memento should be on a new branch from current position
            _List.erase(_List.begin(), _CurrentIter);

            // Emplace new memento
            _List.emplace_front(description, std::in_place_type<MementoOriginator>, originator_retriever, originator_retriever().Snapshot());

            // Relocate current position
            _CurrentIter = _List.begin();

        }

        /// @brief Restore prev memento
        /// @return True, if succeed; False, if there is no prev memento to restore
        [[nodiscard]]
        auto Undo() -> bool;

        /// @brief Restore next memento
        /// @return True, if succeed; False, if there is no next memento to restore
        [[nodiscard]]
        auto Redo() -> bool;

        [[nodiscard]]
        auto GetCapacity() const->std::size_t;
        void SetCapacity(std::size_t capacity);

        [[nodiscard]]
        auto Size() const->std::size_t;

        void Clear();

        /// @brief Check whether the memento refered by @a iter is current memento
        /// @param iter Iterator referring to a memento
        /// @note @a Current @a memento is the next memento to use when undo:
        ///                                                   current
        ///     memento#1 <--- memento#2 <--- memento#3 <--- memento#4 <---   ---> memento#5
        ///               undo           undo           undo           undo   redo
        [[nodiscard]]
        auto IsCurrent(const container_type::const_iterator& iter) const -> bool;

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
        container_type::iterator _CurrentIter{ _List.begin() }; // Points to the memento to be undone
    };


}
