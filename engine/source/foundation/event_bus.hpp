#pragma once

#include <concepts>
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

// Foundation ring: no third party, no OS, no graphics.
//
// A/B double buffered broadcast: Signal() queues, DispatchQueued() delivers once
// per frame to every subscriber of the event type, in Order. A handler returning
// true consumes the event. Subscriptions are RAII handles.

namespace Arcadia::Foundation
{
        // Base of every event. Events are copied into the queue, so they must not
    // hold references to shorter lived objects. The base is polymorphic, so an
    // event is not an aggregate: give it a constructor when Signal() takes
    // arguments.
    struct Event
    {
    public:
        Event() = default;
        Event(const Event&) = default;
        auto operator=(const Event&) -> Event& = default;
        virtual ~Event() = default;

        void MarkHandled() { _Handled = true; }
        [[nodiscard]] auto Handled() const -> bool { return _Handled; }
        void ClearHandled() { _Handled = false; }

    private:
        bool _Handled{ false };
    };

    template<class T>
    concept EventType = std::derived_from<T, Event> && (not std::is_abstract_v<T>);

        // Delivery order. The values keep the legacy layer order, so the observable
    // behaviour does not change while the engine is migrated.
    enum class Order : int
    {
        Editor = 0,
        Imgui,
        Project,
        Runtime,
        Player,
        Script,
        Renderer,
        Scene,
        Physics,
        Window,
    };

    class EventBus;

    // RAII handle. Destroying or Reset()-ing it unsubscribes; doing so from
    // inside a handler is safe (the entry is deactivated and compacted later).
    class Subscription
    {
    public:
        Subscription() = default;
        ~Subscription();

        Subscription(Subscription&& other) noexcept;
        auto operator=(Subscription&& other) noexcept -> Subscription&;

        Subscription(const Subscription&) = delete;
        auto operator=(const Subscription&) -> Subscription& = delete;

        void Reset();
        [[nodiscard]] auto Valid() const -> bool { return _Bus != nullptr; }

    private:
        friend class EventBus;

        Subscription(EventBus* bus, std::type_index type, std::size_t id);

        EventBus* _Bus{ nullptr };
        std::type_index _Type{ typeid(void) };
        std::size_t _Id{ 0 };
    };

    struct RegistrationInfo
    {
        std::string_view TypeName{};
        Order Priority{ Order::Editor };
        bool Active{ false };
    };

    class EventBus
    {
    public:
        using HandlerType = std::function<bool(Event&)>;

    public:
        EventBus() = default;
        ~EventBus() = default;

        EventBus(const EventBus&) = delete;
        auto operator=(const EventBus&) -> EventBus& = delete;

        // Registers `handler` for the events of type `E`. Handlers are invoked in
        // `order`; equal orders keep the registration order. Returning true from a
        // handler consumes the event, so no later subscriber sees it.
        template<EventType E>
        auto Subscribe(Order order, std::function<bool(E&)> handler) -> Subscription;

        // Catch-all registration: the handler sees every event. It exists so the
        // legacy OnEvent(EventBase&) bodies of the engine can be moved onto the
        // new bus mechanically, and so the editor can fan one event out to all of
        // its windows. Returning true (or calling Event::MarkHandled()) consumes.
        auto SubscribeAny(Order order, std::function<bool(Event&)> handler) -> Subscription;

        // Queues an event. It is dispatched by the next DispatchQueued(), never
        // before: this keeps the legacy "events are handled at the top of a frame"
        // timing and makes signalled-during-dispatch events wait for the next frame.
        template<EventType E, class ...Args>
        void Signal(Args&& ...args);

        // Dispatches right away, without queueing (construction-time use only).
        template<EventType E, class ...Args>
        void SignalNow(Args&& ...args);

        // Swaps the queues and dispatches everything queued since the last call.
        void DispatchQueued();

        [[nodiscard]] auto PendingCount() const -> std::size_t { return _Collect.size(); }

        void Unsubscribe(std::type_index type, std::size_t id);

        // Introspection for debugging / "--dump-events": visits every registration.
        void ForEachRegistration(const std::function<void(const RegistrationInfo&)>& visit) const;

    private:
        struct Entry
        {
            Order Priority{ Order::Editor };
            std::size_t _Id{ 0 };
            bool Active{ true };
            std::string TypeName{};
            HandlerType Function{};
        };

        using BucketType = std::vector<Entry>;
        using BucketStorageType = std::unordered_map<std::type_index, std::unique_ptr<BucketType>>;

        auto _Bucket(std::type_index type) -> BucketType&;
        auto _FindBucket(std::type_index type) const -> const BucketType*;

        void _DispatchOne(Event& event);
        void _Compact();

    private:
        // unique_ptr: a bucket keeps its address even when _Subscribers rehashes, which
        // makes dispatch safe while a handler subscribes to another event type.
        BucketStorageType _Subscribers{};
        BucketType _AnyBucket{};
        std::vector<std::unique_ptr<Event>> _Collect{};
        std::vector<std::unique_ptr<Event>> _Dispatch{};
        std::size_t _NextId{ 0 };
    };

    template<EventType E>
    auto EventBus::Subscribe(Order order, std::function<bool(E&)> handler) -> Subscription
    {
        BucketType& bucket = _Bucket(std::type_index{ typeid(E) });
        const std::size_t id = _NextId;

        Entry entry{};
        entry.Priority = order;
        entry._Id = id;
        entry.Active = true;
        entry.TypeName = typeid(E).name();
        entry.Function = [fn = std::move(handler)](Event& event) -> bool
        {
            return fn(static_cast<E&>(event));
        };

        // Keeps the bucket sorted by (Priority, Id) so dispatch is a straight walk.
        auto pos = bucket.begin();
        while (pos != bucket.end() && (pos->Priority < order || (pos->Priority == order && pos->_Id < id)))
        {
            ++pos;
        }
        bucket.insert(pos, std::move(entry));

        _NextId++;
        return Subscription{ this, std::type_index{ typeid(E) }, id };
    }

    template<EventType E, class ...Args>
    void EventBus::Signal(Args&& ...args)
    {
        _Collect.push_back(std::make_unique<E>(std::forward<Args>(args)...));
    }

    template<EventType E, class ...Args>
    void EventBus::SignalNow(Args&& ...args)
    {
        E event(std::forward<Args>(args)...);
        _DispatchOne(event);
    }
}

