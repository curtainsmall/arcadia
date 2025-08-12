#pragma once

#include <functional>
#include <memory>
#include <queue>
#include <tuple>
#include <typeindex>
#include <typeinfo>
#include <unordered_set>
#include <vector>

#include "core/enum.hpp"
#include "core/log.hpp"
#include "platform/defines.hpp"

namespace Arcadia
{
    enum struct ACDA_API EventHandleState: std::uint8_t
    {
        NotHandled = 0,
        Handled = 1 << 0,
        OnceAgain = 1 << 1,

        _EnumBitfield
    };

    struct ACDA_API EventBase
    {
    public:
        using SelfType = EventBase;
    public:
        EventBase() = default;
        // Virtual destructor that make event type virtual
        virtual ~EventBase() = default;

        EventBase(const SelfType&) = delete;
        auto operator=(const SelfType&) -> SelfType & = delete;

        void MarkHandled();
        void MarkOnceAgain();

        auto GetHandleState() const -> EventHandleState;
        void ClearMark();
    private:
        EventHandleState _HandleState{ EventHandleState::NotHandled };
    };

    namespace Concepts
    {
        template<class T>
        concept Event = std::derived_from<T, EventBase>;
    }

    template<Concepts::Event Event>
    using EventHandler = std::function<void(Event&)>;

    struct ACDA_API EventDispatcher
    {
    public:
        using SelfType = EventDispatcher;
    public:
        EventDispatcher(EventBase& event):
            _pEvent(&event)
        {
        }
        ~EventDispatcher() = default;

        EventDispatcher(const SelfType&) = delete;
        auto operator=(const SelfType&) -> SelfType & = delete;

        template<Concepts::Event Event>
        auto Dispatch(const EventHandler<Event>& handler) -> SelfType&
        {
            if(typeid(*_pEvent) == typeid(Event))
            {
                handler(static_cast<Event&>(*_pEvent));
                _Dispatched = true;
            }
            return *this;
        }

        auto IsDispatched() const -> bool;

    private:
        EventBase* _pEvent;
        bool _Dispatched{ false };
    };

    struct ACDA_API EventQueue
    {
    public:
        using DebugExcludedEventTypeSetType = std::unordered_set<std::type_index>;
        using SelfType = EventQueue;
    private:
        using _EventQueueType = std::queue<std::unique_ptr<EventBase>>;

    public:
        static auto Instance() -> SelfType&;

        EventQueue() = default;
        ~EventQueue() = default;

        EventQueue(const SelfType&) = delete;
        auto operator=(const SelfType&) -> SelfType & = delete;

        template<Concepts::Event Event, class ...Args>
        auto Signal(Args&& ...args) -> SelfType&
        {
            _pCollectingQueue->emplace(std::make_unique<Event>(std::forward<Args>(args)...));

            return *this;
        }

        void SwapQueue();
        auto HasEvent() const -> bool;
        auto ProcessEvent(const EventHandler<EventBase>& handler) -> bool;
        void EventProcessFinished();
    private:
        _EventQueueType _QueueA{};
        _EventQueueType _QueueB{};
        _EventQueueType* _pProcessingQueue{ &_QueueA };
        _EventQueueType* _pCollectingQueue{ &_QueueB };
    };
}