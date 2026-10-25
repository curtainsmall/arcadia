#include "foundation/event_Bus.hpp"

#include <algorithm>

namespace Arcadia::Foundation
{
    Subscription::Subscription(EventBus* bus, std::type_index type, std::size_t id):
        _Bus(bus),
        _Type(type),
        _Id(id)
    {
    }

    Subscription::~Subscription()
    {
        Reset();
    }

    Subscription::Subscription(Subscription&& other) noexcept:
        _Bus(other._Bus),
        _Type(other._Type),
        _Id(other._Id)
    {
        other._Bus = nullptr;
    }

    auto Subscription::operator=(Subscription&& other) noexcept -> Subscription&
    {
        if(this != &other)
        {
            Reset();

            _Bus = other._Bus;
            _Type = other._Type;
            _Id = other._Id;

            other._Bus = nullptr;
        }
        return *this;
    }

    void Subscription::Reset()
    {
        if(_Bus != nullptr)
        {
            _Bus->Unsubscribe(_Type, _Id);
            _Bus = nullptr;
        }
    }

    namespace
    {
        // Key used by the catch-all registrations, so they never collide with a
        // concrete event type.
        auto AnyKey() -> std::type_index
        {
            return std::type_index{ typeid(Event) };
        }
    }

    auto EventBus::_Bucket(std::type_index type) -> BucketType&
    {
        std::unique_ptr<BucketType>& bucket_uptr = _Subscribers[type];
        if(!bucket_uptr)
        {
            bucket_uptr = std::make_unique<BucketType>();
        }
        return *bucket_uptr;
    }

    auto EventBus::_FindBucket(std::type_index type) const -> const BucketType*
    {
        auto iter = _Subscribers.find(type);
        if(iter == _Subscribers.end() || !iter->second)
        {
            return nullptr;
        }
        return iter->second.get();
    }

    auto EventBus::SubscribeAny(Order order, std::function<bool(Event&)> handler) -> Subscription
    {
        const std::size_t id = _NextId++;

        Entry entry{};
        entry.Priority = order;
        entry._Id = id;
        entry.Active = true;
        entry.TypeName = "{any}";
        entry.Function = std::move(handler);

        auto pos = _AnyBucket.begin();
        while(pos != _AnyBucket.end() && (pos->Priority < order || (pos->Priority == order && pos->_Id < id)))
        {
            ++pos;
        }
        _AnyBucket.insert(pos, std::move(entry));

        return Subscription{ this, AnyKey(), id };
    }

    void EventBus::Unsubscribe(std::type_index type, std::size_t id)
    {
        BucketType* bucket_ptr = nullptr;
        if(type == AnyKey())
        {
            bucket_ptr = &_AnyBucket;
        }
        else
        {
            auto bucket_iter = _Subscribers.find(type);
            if(bucket_iter == _Subscribers.end() || !bucket_iter->second)
            {
                return;
            }
            bucket_ptr = bucket_iter->second.get();
        }

        auto iter = std::find_if(
            bucket_ptr->begin(),
            bucket_ptr->end(),
            [id](const Entry& entry) -> bool
            {
                return entry._Id == id;
            }
        );
        if(iter != bucket_ptr->end())
        {
            // Deactivated, not erased: the entry may be referenced by a walk that
            // is currently running inside _DispatchOne(). _Compact() reclaims it.
            iter->Active = false;
        }
    }

    void EventBus::DispatchQueued()
    {
        _Dispatch.swap(_Collect);

        for(const std::unique_ptr<Event>& event_uptr : _Dispatch)
        {
            if(event_uptr)
            {
                _DispatchOne(*event_uptr);
            }
        }

        _Dispatch.clear();
        _Compact();
    }

    void EventBus::_DispatchOne(Event& event)
    {
        const BucketType* typed = _FindBucket(std::type_index{ typeid(event) });

        // Merge walk of the two Priority-sorted buckets: catch-all first at equal
        // Priority, so a service can consume an event before the type subscribers.
        std::size_t any_index = 0;
        std::size_t typed_index = 0;
        const std::size_t any_count = _AnyBucket.size();
        const std::size_t typed_count = (typed != nullptr) ? typed->size() : 0;

        while(any_index < any_count || typed_index < typed_count)
        {
            const bool take_any =
                typed_index >= typed_count ||
                (any_index < any_count &&
                 (_AnyBucket[any_index].Priority <= (*typed)[typed_index].Priority));

            const Entry& entry = take_any ? _AnyBucket[any_index++] : (*typed)[typed_index++];
            if(!entry.Active)
            {
                continue;
            }
            if(entry.Function(event) || event.Handled())
            {
                return;
            }
        }
    }

    void EventBus::_Compact()
    {
        const auto reclaim = [](BucketType& bucket) -> void
        {
            std::erase_if(
                bucket,
                [](const Entry& entry) -> bool
                {
                    return !entry.Active;
                }
            );
        };

        reclaim(_AnyBucket);
        for(auto& [type, bucket_uptr] : _Subscribers)
        {
            (void) type;
            if(bucket_uptr)
            {
                reclaim(*bucket_uptr);
            }
        }
    }

    void EventBus::ForEachRegistration(const std::function<void(const RegistrationInfo&)>& visit) const
    {
        const auto emit = [&visit](const BucketType& bucket) -> void
        {
            for(const Entry& entry : bucket)
            {
                RegistrationInfo info{};
                info.TypeName = entry.TypeName;
                info.Priority = entry.Priority;
                info.Active = entry.Active;
                visit(info);
            }
        };

        emit(_AnyBucket);
        for(const auto& [type, bucket_uptr] : _Subscribers)
        {
            (void) type;
            if(bucket_uptr)
            {
                emit(*bucket_uptr);
            }
        }
    }
}

