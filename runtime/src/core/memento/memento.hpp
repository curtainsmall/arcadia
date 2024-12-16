#pragma once

#include<functional>
#include<list>
#include<memory>

#include"core/base.hpp"

namespace Arcadia
{
    class MementoDataBase;

    template<typename MementoData>
    concept cMementoData = requires{
        std::derived_from<MementoData, MementoDataBase>;
        std::equality_comparable<MementoData>;
    };

    class MementoDataBase
    {
    public:
        template<typename MementoData>
        auto CastTo() -> MementoData&
        {
            return static_cast<MementoData&>(*this);
        }
    };

    class iMementoOriginator
    {
    public:
        using SelfType = iMementoOriginator;
    public:
        auto Snapshot() -> std::shared_ptr<MementoDataBase>;
        void Restore(const std::shared_ptr<MementoDataBase>& sp_memento_data);

    protected:
        /// @brief Generate a memento data
        /// @return Memento data
        [[nodiscard]]
        virtual auto OnSnapshot() const->std::shared_ptr<MementoDataBase> = 0;

        /// @brief Restore self with memento data
        /// @param memento_data Memento data to restore with
        virtual void OnRestore(const std::shared_ptr<MementoDataBase>& memento_data) = 0;

    private:
        std::shared_ptr<MementoDataBase> _PreviousMementoData{};
    };

    template<typename MementoOriginator>
    concept cMementoOriginator = requires{
        std::derived_from<MementoOriginator, iMementoOriginator>;
    };

    class Memento: public Noncopyable
    {
    public:
        using SelfType = Memento;
    public:
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
        ) :
            _Description(description),
            _OriginatorRetriever(
                new std::function<MementoOriginator& ()>(originator_retriever),
                [](void* ptr)
        {
            delete static_cast<std::function<MementoOriginator& ()>*>(ptr);
        }
            ),
            _MementoData(memento_data),
            _OriginatorRestoreFunction(
                [this]()
        {
            MementoOriginator& originator = (*static_cast<std::function<MementoOriginator & ()>*>(_OriginatorRetriever.get()))();
            originator.Restore(_MementoData);
        }
            )
        {}

        /// @brief Restore originator with memento data
        void Restore() const;

        [[nodiscard]]
        auto GetDescription() const -> const std::string&;

    private:
        std::string _Description{};
        std::unique_ptr<void, std::function<void(void*)>> _OriginatorRetriever{}; // Used to store originator retriever with type erasure
        std::shared_ptr<MementoDataBase> _MementoData; // Used to store memento data with type erasure
        std::function<void()> _OriginatorRestoreFunction; // 1. call originator retriever to get originator; 2. get memento data; 3. call restore() in originator with memento data
    };

    class MementoList: public Noncopyable
    {
    public:
        using ContainerType = std::list<Memento>;
        using SelfType = MementoList;
    public:
        static auto Instance() -> SelfType&;

        /// @brief Snapshot @a MementoOriginator
        /// @tparam MementoOriginator Type of memento originator
        /// @param description Description
        /// @param originator_retriever Originator to snapshot
        template<cMementoOriginator MementoOriginator>
        void Snapshot(
            const std::string& description,
            const std::function<MementoOriginator& ()>& originator_retriever
        )
        {
            // Erase restored mementos since a new memento should be on a new branch from current position
            _List.erase(_List.begin(), _CurrentIterator);

            // Emplace new memento
            _List.emplace_front(description, std::in_place_type<MementoOriginator>, originator_retriever, originator_retriever().Snapshot());

            // Relocate current position
            _CurrentIterator = _List.begin();
        }

        /// @brief Restore prev memento
        /// @return True, if succeed; False, if there is no prev memento to restore
        auto Undo() -> bool;

        /// @brief Restore next memento
        /// @return True, if succeed; False, if there is no next memento to restore
        auto Redo() -> bool;

        [[nodiscard]]
        auto GetCapacity() const->std::size_t;
        void SetCapacity(std::size_t capacity);

        [[nodiscard]]
        auto GetSize() const->std::size_t;

        void Clear();

        /// @brief Check whether the memento refered by @a iter is current memento
        /// @param iter Iterator referring to a memento
        /// @note @a Current @a memento is the next memento to use when undo:
        ///                                                   current
        ///     memento#1 <--- memento#2 <--- memento#3 <--- memento#4 <---   ---> memento#5
        ///               undo           undo           undo           undo   redo
        [[nodiscard]]
        auto IsCurrent(const ContainerType::const_iterator& iter) const -> bool;

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
        ContainerType::iterator _CurrentIterator{ _List.begin() }; // Points to the memento to be undone
    };
}