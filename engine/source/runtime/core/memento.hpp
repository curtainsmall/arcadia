#pragma once

#include <functional>
#include <list>
#include <memory>

#include "platform/defines.hpp"

namespace Arcadia
{
#if 0
    struct ACDA_API MementoDataBase;

    namespace Concepts
    {
        template<class T>
        concept MementoData =
            std::derived_from<T, MementoDataBase>
            && std::equality_comparable<T>;
    }

    struct ACDA_API MementoDataBase
    {
    public:
        template<class MementoData>
        auto CastTo() -> MementoData&
        {
            return static_cast<MementoData&>(*this);
        }
    };

    struct ACDA_API MementoOriginatorInterface
    {
    public:
        using SelfType = MementoOriginatorInterface;
    public:
        void Snapshot();
        void Restore();

    protected:
        [[nodiscard]]
        virtual auto OnSnapshot() const -> std::unique_ptr<MementoDataBase> = 0;
        virtual void OnRestore(const std::unique_ptr<MementoDataBase>& memento_data_base_sptr) = 0;

    private:
        std::unique_ptr<MementoDataBase> _upMementoData{};
    };

    namespace Concepts
    {
        template<class T>
        concept MementoOriginator = std::derived_from<T, MementoOriginatorInterface>;
    }

    struct ACDA_API Memento: public Noncopyable
    {
    public:
        using SelfType = Memento;
    public:
        template<
            Concepts::MementoOriginator MementoOriginator
        >
        Memento(
            std::string_view description,
            std::in_place_type_t<MementoOriginator> in_place_type_originator,
            const std::function<MementoOriginator& ()>& originator_retriever,
            const std::shared_ptr<MementoDataBase>& memento_data_sptr
        ):
            _Description(description),
            _upOriginatorRetriever(
                new std::function<MementoOriginator& ()>(originator_retriever),
                [](void* ptr)
                {
                    delete static_cast<std::function<MementoOriginator& ()>*>(ptr);
                }
            ),
            _spMementoData(memento_data_sptr),
            _OriginatorRestoreFunction(
                [&]()
                {
                    MementoOriginator& originator = (*static_cast<std::function<MementoOriginator & ()>*>(_upOriginatorRetriever.get()))();
                    originator.Restore(_spMementoData);
                }
            )
        {
        }

        void Restore() const;

        [[nodiscard]]
        auto GetDescription() const -> std::string_view;

    private:
        std::string _Description{};
        std::unique_ptr<void, std::function<void(void*)>> _upOriginatorRetriever{}; // Used to store originator retriever with type erasure
        std::shared_ptr<MementoDataBase> _spMementoData; // Used to store memento data with type erasure
        std::function<void()> _OriginatorRestoreFunction; // 1. call originator retriever to get originator; 2. get memento data; 3. call restore() in originator with memento data
    };

    struct ACDA_API MementoList: public Noncopyable
    {
    public:
        using ContainerType = std::list<Memento>;
        using SelfType = MementoList;
    public:
        static auto Instance() -> SelfType&;

        template<Concepts::MementoOriginator MementoOriginator>
        void Snapshot(
            std::string_view description,
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

        auto Undo() -> bool;
        auto Redo() -> bool;

        [[nodiscard]]
        auto GetCapacity() const -> std::size_t;
        void SetCapacity(std::size_t capacity);

        [[nodiscard]]
        auto GetSize() const -> std::size_t;

        void Clear();

        //                                                   current
        //     memento#1 <--- memento#2 <--- memento#3 <--- memento#4 <---   ---> memento#5
        //               undo           undo           undo           undo   redo
        [[nodiscard]]
        auto IsCurrent(const ContainerType::const_iterator& iter) const -> bool;

        [[nodiscard]]
        auto begin() noexcept -> ContainerType::iterator;
        [[nodiscard]]
        auto end() noexcept -> ContainerType::iterator;

        [[nodiscard]]
        auto begin() const noexcept -> ContainerType::const_iterator;
        [[nodiscard]]
        auto end() const noexcept -> ContainerType::const_iterator;

        [[nodiscard]]
        auto cbegin() const noexcept -> ContainerType::const_iterator;
        [[nodiscard]]
        auto cend() const noexcept -> ContainerType::const_iterator;

    private:
        std::size_t _Capacity{ 40 };
        ContainerType _List{};
        ContainerType::iterator _CurrentIterator{ _List.begin() }; // Points to the memento to be undone
    };

#endif

    template<class Memento>
    struct ACDA_API Mementoable
    {
    public:
        Mementoable() = default;
        virtual ~Mementoable() = default;

        void Snapshot()
        {
            _upMemento = OnSnapshot();
        }

        void Restore()
        {
            OnRestore(_upMemento);
        }

    protected:
        virtual auto OnSnapshot() const -> std::unique_ptr<Memento> = 0;
        virtual void OnRestore(const std::unique_ptr<Memento>& memento_uptr) = 0;

    private:
        std::unique_ptr<Memento> _upMemento{};
    };
}