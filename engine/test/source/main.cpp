#include "gtest/gtest.h"

#include <vector>

#include "app/engine.hpp"
#include "foundation/event_bus.hpp"

namespace
{
    struct ProbeEvent: public Arcadia::Foundation::Event
    {
    };
}

TEST(ArcadiaEngine, EventBusDeliversInSubscriptionOrder)
{
    Arcadia::Engine engine{};

    std::vector<Arcadia::Foundation::Order> delivered{};
    std::vector<Arcadia::Foundation::Subscription> subscriptions{};

    const Arcadia::Foundation::Order orders[]{
        Arcadia::Foundation::Order::Window,
        Arcadia::Foundation::Order::Editor,
        Arcadia::Foundation::Order::Scene
    };

    // Registered out of order on purpose: the bus sorts by Order, not by registration.
    for(const Arcadia::Foundation::Order order : orders)
    {
        subscriptions.push_back(
            engine.events.SubscribeAny(
                order,
                [&delivered, order](Arcadia::Foundation::Event&) -> bool
                {
                    delivered.push_back(order);
                    return false;
                }
            )
        );
    }

    engine.events.Signal<ProbeEvent>();
    engine.events.DispatchQueued();

    const std::vector<Arcadia::Foundation::Order> expected{
        Arcadia::Foundation::Order::Editor,
        Arcadia::Foundation::Order::Scene,
        Arcadia::Foundation::Order::Window
    };
    EXPECT_EQ(delivered, expected);
}

TEST(ArcadiaEngine, DefaultEngineIsHeadless)
{
    Arcadia::Engine engine{};

    EXPECT_EQ(engine.platform, nullptr);
    EXPECT_EQ(engine.physics, nullptr);
    EXPECT_EQ(engine.renderer, nullptr);
    EXPECT_TRUE(engine.IsRunning());
}
