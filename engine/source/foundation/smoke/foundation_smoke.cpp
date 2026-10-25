// Headless verification of the foundation ring.
//
// It is intentionally self-contained: it does not touch the legacy runtime /
// editor modules, so it can be built and run at any point during the migration.
//
//   foundation_smoke            -> runs every check, exit code 0 on success
//   foundation_smoke --verbose  -> also prints the reflected field table

#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "app/app.hpp"
#include "app/runtime_config.hpp"
#include "app/engine.hpp"
#include "foundation/event_bus.hpp"
#include "foundation/reflect.hpp"

namespace SmokeTest
{
    // ---- events used by the checks -------------------------------------------

    struct PingEvent: public Arcadia::Foundation::Event
    {
    public:
        explicit PingEvent(int value = 0):
            _Value(value)
        {
        }

    public:
        int _Value{ 0 };
    };

    struct LateEvent: public Arcadia::Foundation::Event
    {
    };

    // ---- a component with one single field declaration ----------------------

    struct TestComponent
    {
    public:
        bool _Visible{ true };
        std::int32_t _Priority{ 3 };
        float _Speed{ 1.5f };
        float _Gain{ 2.f };
        glm::vec2 _Viewport{ 1280.f, 720.f };
        glm::vec3 _Position{ 1.f, 2.f, 3.f };
        glm::vec4 _Color{ 0.1f, 0.2f, 0.3f, 0.4f };
        glm::quat _Rotation{ 1.f, 0.f, 0.f, 0.f };
        std::string _Name{ "smoke" };

        // Deliberately *not* reflected: proves the table is the only source of truth.
        float _NotReflected{ 42.f };
    };

    ACDA_FIELDS(TestComponent)
    {
        ACDA_FIELD_KEY(_Visible, "visible", Bool);
        ACDA_FIELD_KEY(_Priority, "priority", Int);
        ACDA_FIELD_KEY(_Speed, "speed", Float);
        ACDA_FIELD_RANGE(_Gain, Float, 0.f, 10.f);
        ACDA_FIELD_KEY(_Viewport, "viewport", Vec2);
        ACDA_FIELD_KEY(_Position, "position", Vec3);
        ACDA_FIELD_KEY(_Color, "color", Vec4);
        ACDA_FIELD_KEY(_Rotation, "rotation", Quat);
        ACDA_FIELD_KEY(_Name, "name", String);
    }
}

namespace
{
    int g_failures = 0;
    int g_checks = 0;

    void Check(bool condition, const char* expression, int line)
    {
        ++g_checks;
        if(!condition)
        {
            ++g_failures;
            std::printf("FAIL  line %d:  %s\n", line, expression);
        }
    }

    auto NearlyEqual(float lhs, float rhs, float epsilon = 1e-5f) -> bool
    {
        return std::fabs(lhs - rhs) <= epsilon;
    }

    // ---- event bus ----------------------------------------------------------

    void CheckDeferredDispatch()
    {
        Arcadia::Foundation::EventBus bus{};
        std::vector<int> calls{};

        auto subscription = bus.Subscribe<SmokeTest::PingEvent>(
            Arcadia::Foundation::Order::Player,
            [&calls](SmokeTest::PingEvent& event) -> bool
            {
                calls.push_back(event._Value);
                return false;
            }
        );

        bus.Signal<SmokeTest::PingEvent>(7);

        Check(bus.PendingCount() == 1, "queued event is pending", __LINE__);
        Check(calls.empty(), "Signal() must not dispatch immediately", __LINE__);

        bus.DispatchQueued();
        Check(calls.size() == 1 && calls.front() == 7, "DispatchQueued() delivers the event", __LINE__);
        Check(bus.PendingCount() == 0, "queue is empty after a dispatch", __LINE__);
    }

    void CheckOrderAndConsumption()
    {
        Arcadia::Foundation::EventBus bus{};
        std::vector<std::string_view> order{};
        bool consume = false;

        auto first = bus.Subscribe<SmokeTest::PingEvent>(
            Arcadia::Foundation::Order::Editor,
            [&order](SmokeTest::PingEvent&) -> bool
            {
                order.emplace_back("editor");
                return false;
            }
        );
        auto second = bus.Subscribe<SmokeTest::PingEvent>(
            Arcadia::Foundation::Order::Imgui,
            [&order, &consume](SmokeTest::PingEvent&) -> bool
            {
                order.emplace_back("imgui");
                return consume;
            }
        );
        auto third = bus.Subscribe<SmokeTest::PingEvent>(
            Arcadia::Foundation::Order::Physics,
            [&order](SmokeTest::PingEvent&) -> bool
            {
                order.emplace_back("physics");
                return false;
            }
        );

        order.clear();
        consume = false;
        bus.Signal<SmokeTest::PingEvent>(1);
        bus.DispatchQueued();
        Check(
            order == std::vector<std::string_view>{ "editor","imgui","physics" },
            "subscribers run in Order",
            __LINE__
        );

        order.clear();
        consume = true;
        bus.Signal<SmokeTest::PingEvent>(2);
        bus.DispatchQueued();
        Check(
            order == std::vector<std::string_view>{ "editor","imgui" },
            "a handler returning true consumes the event",
            __LINE__
        );
    }

    void CheckRaiiUnsubscribe()
    {
        Arcadia::Foundation::EventBus bus{};
        int hits = 0;

        {
            auto subscription = bus.Subscribe<SmokeTest::PingEvent>(
                Arcadia::Foundation::Order::Scene,
                [&hits](SmokeTest::PingEvent&) -> bool
                {
                    ++hits;
                    return false;
                }
            );
            bus.Signal<SmokeTest::PingEvent>(1);
            bus.DispatchQueued();
            Check(hits == 1, "subscriber is called while alive", __LINE__);
        }

        bus.Signal<SmokeTest::PingEvent>(2);
        bus.DispatchQueued();
        Check(hits == 1, "destroying the Subscription unsubscribes", __LINE__);
    }

    void CheckSubscribeDuringDispatch()
    {
        Arcadia::Foundation::EventBus bus{};
        int late_hits = 0;
        int total_hits = 0;
        std::vector<Arcadia::Foundation::Subscription> keep_alive{};

        auto subscriber = bus.Subscribe<SmokeTest::PingEvent>(
            Arcadia::Foundation::Order::Runtime,
            [&bus, &late_hits, &total_hits, &keep_alive](SmokeTest::PingEvent&) -> bool
            {
                ++total_hits;
                if(total_hits == 1)
                {
                    keep_alive.push_back(
                        bus.Subscribe<SmokeTest::PingEvent>(
                            Arcadia::Foundation::Order::Window,
                            [&late_hits](SmokeTest::PingEvent&) -> bool
                            {
                                ++late_hits;
                                return false;
                            }
                        )
                    );
                }
                return false;
            }
        );

        bus.Signal<SmokeTest::PingEvent>(1);
        bus.DispatchQueued();
        Check(late_hits == 0, "a subscription added during dispatch does not see the current event", __LINE__);

        bus.Signal<SmokeTest::PingEvent>(2);
        bus.DispatchQueued();
        Check(late_hits == 1, "it sees the next event", __LINE__);
    }

    void CheckNestedSignal()
    {
        Arcadia::Foundation::EventBus bus{};
        int pings = 0;
        int lates = 0;

        auto ping_subscription = bus.Subscribe<SmokeTest::PingEvent>(
            Arcadia::Foundation::Order::Script,
            [&bus, &pings](SmokeTest::PingEvent&) -> bool
            {
                ++pings;
                (void) bus.Signal<SmokeTest::LateEvent>();
                return false;
            }
        );
        auto late_subscription = bus.Subscribe<SmokeTest::LateEvent>(
            Arcadia::Foundation::Order::Script,
            [&lates](SmokeTest::LateEvent&) -> bool
            {
                ++lates;
                return false;
            }
        );

        bus.Signal<SmokeTest::PingEvent>(1);
        bus.DispatchQueued();
        Check(pings == 1, "outer event was dispatched", __LINE__);
        Check(lates == 0, "an event signalled while dispatching waits for the next frame", __LINE__);

        bus.DispatchQueued();
        Check(lates == 1, "it is dispatched on the next DispatchQueued()", __LINE__);
    }

    void CheckSignalNow()
    {
        Arcadia::Foundation::EventBus bus{};
        int hits = 0;

        auto subscription = bus.Subscribe<SmokeTest::PingEvent>(
            Arcadia::Foundation::Order::Window,
            [&hits](SmokeTest::PingEvent&) -> bool
            {
                ++hits;
                return false;
            }
        );

        bus.SignalNow<SmokeTest::PingEvent>(5);
        Check(hits == 1, "SignalNow() dispatches immediately", __LINE__);
        Check(bus.PendingCount() == 0, "SignalNow() does not queue", __LINE__);
    }

    void CheckIntrospection()
    {
        Arcadia::Foundation::EventBus bus{};
        auto first = bus.Subscribe<SmokeTest::PingEvent>(
            Arcadia::Foundation::Order::Editor,
            [](SmokeTest::PingEvent&) -> bool
            {
                return false;
            }
        );
        auto second = bus.Subscribe<SmokeTest::LateEvent>(
            Arcadia::Foundation::Order::Renderer,
            [](SmokeTest::LateEvent&) -> bool
            {
                return false;
            }
        );

        int registrations = 0;
        bus.ForEachRegistration(
            [&registrations](const Arcadia::Foundation::RegistrationInfo& info) -> void
            {
                ++registrations;
                Check(!info.TypeName.empty(), "registration carries a type name", __LINE__);
            }
        );
        Check(registrations == 2, "both registrations are visible for dumping", __LINE__);
    }

    // ---- reflection ---------------------------------------------------------

    void CheckReflectionRoundTrip()
    {
        SmokeTest::TestComponent component{};
        component._Speed = 2.25f;
        component._Gain = 7.5f;
        component._Position = { .5f, -1.5f, 4.f };
        component._Rotation = glm::quat{ 0.5f, 0.5f, 0.5f, 0.5f };
        component._Name = "world";
        component._Visible = false;

        const nlohmann::json json = Arcadia::Foundation::ToJson(component);

        Check(json.contains("speed"), "declared field is serialized", __LINE__);
        Check(json.contains("position"), "explicit json key is used", __LINE__);
        Check(!json.contains("_Speed"), "member name is not used as key", __LINE__);
        Check(!json.contains("NotReflected"), "undeclared members stay out of the table", __LINE__);
        Check(json.at("position").is_array() && json.at("position").size() == 3, "vec3 -> 3 components", __LINE__);
        Check(json.at("rotation").size() == 4, "quat -> 4 components", __LINE__);
        Check(NearlyEqual(json.at("rotation").at(0).get<float>(), 0.5f), "quat keeps (w,x,y,z) order", __LINE__);

        SmokeTest::TestComponent loaded{};
        Arcadia::Foundation::FromJson(loaded, json);

        Check(NearlyEqual(loaded._Speed, 2.25f), "float round-trips", __LINE__);
        Check(NearlyEqual(loaded._Gain, 7.5f), "ranged float round-trips", __LINE__);
        Check(loaded._Visible == false, "bool round-trips", __LINE__);
        Check(loaded._Name == "world", "string round-trips", __LINE__);
        Check(NearlyEqual(loaded._Position.z, 4.f), "vec3 round-trips", __LINE__);
        Check(
            NearlyEqual(loaded._Rotation.w, 0.5f) && NearlyEqual(loaded._Rotation.z, 0.5f),
            "quat round-trips",
            __LINE__
        );
        Check(NearlyEqual(loaded._NotReflected, 42.f), "undeclared members are untouched", __LINE__);
    }

    void CheckFieldTable(bool verbose)
    {
        SmokeTest::TestComponent component{};
        int field_count = 0;
        int ranged_count = 0;
        bool label_ok = true;

        Arcadia::Foundation::ForEachField(
            component,
            [&field_count, &ranged_count, &label_ok, verbose](Arcadia::Foundation::Field& field) -> void
            {
                ++field_count;
                if(field.Range.Clamps())
                {
                    ++ranged_count;
                }
                if(!field.Label().empty() && field.Label().front() == '_')
                {
                    label_ok = false;
                }
                if(verbose)
                {
                    std::printf(
                        "  field  %-10.*s  kind %d  range %s\n",
                        int(field.Label().size()),
                        field.Label().data(),
                        static_cast<int>(field.Kind),
                        field.Range.Clamps() ? "closed" : "unbounded"
                    );
                }
            }
        );

        Check(field_count == 9, "every declared field is enumerated", __LINE__);
        Check(ranged_count == 1, "exactly one field carries a closed range", __LINE__);
        Check(label_ok, "labels are stripped of their leading underscore", __LINE__);
        Check(
            Arcadia::Foundation::KeyOf("_Position") == "Position",
            "KeyOf() strips underscores",
            __LINE__
        );
    }

    void CheckUnboundedIsNotZeroClamp()
    {
        // The bug that motivated the typed range: "min == max == 0" must never
        // be produced by the unbounded constructor.
        const Arcadia::Foundation::FieldRange unbounded = Arcadia::Foundation::FieldRange::Unbounded();
        const Arcadia::Foundation::FieldRange closed = Arcadia::Foundation::FieldRange::Closed(0.f, 10.f);
        const Arcadia::Foundation::FieldRange degenerate = Arcadia::Foundation::FieldRange::Closed(0.f, 0.f);

        Check(!unbounded.Clamps(), "Unbounded() never clamps", __LINE__);
        Check(closed.Clamps(), "Closed(0,10) clamps", __LINE__);
        Check(!degenerate.Clamps(), "Closed(0,0) is not treated as a zero-range clamp", __LINE__);
    }

    void CheckPatch()
    {
        SmokeTest::TestComponent component{};
        Arcadia::Foundation::EditScope<SmokeTest::TestComponent> scope{ component };

        Check(!scope.MakePatch().Changed(), "a patch without edits is unchanged", __LINE__);

        component._Speed = 9.f;
        const Arcadia::Foundation::Patch patch = scope.MakePatch();
        Check(patch.Changed(), "an edited component produces a changed patch", __LINE__);
        Check(patch.Before.at("speed").get<float>() != patch.After.at("speed").get<float>(), "before/after differ", __LINE__);
    }
}

// ---- engine host (headless) ----------------------------------------------------

namespace
{
    struct FrameLog
    {
    public:
        std::vector<std::string> _entries{};

    public:
        void Add(std::string_view entry)
        {
            _entries.emplace_back(entry);
        }
    };

    struct StubPlatform: public Arcadia::Platform
    {
    public:
        FrameLog* _log{ nullptr };

    public:
        void PollEvents() override { _log->Add("poll"); }
        void SwapBuffers() override { _log->Add("swap"); }
    };

    struct StubPhysics: public Arcadia::Physics
    {
    public:
        FrameLog* _log{ nullptr };

    public:
        void Step(std::chrono::nanoseconds) override { _log->Add("physics.step"); }
        void ApplyToWorld() override { _log->Add("physics.apply"); }
    };

    struct StubRenderer: public Arcadia::Renderer
    {
    public:
        FrameLog* _log{ nullptr };

    public:
        void Draw() override { _log->Add("render"); }
    };

    struct StubApp: public Arcadia::App
    {
    public:
        FrameLog* _log{ nullptr };
        int frameLimit{ 1 };
        int frames{ 0 };

    public:
        void OnFrame(Arcadia::Engine& engine) override
        {
            _log->Add("app.frame");
            ++frames;
            if(frames >= frameLimit)
            {
                engine.Stop();
            }
        }
    };

    void CheckFrameOrder()
    {
        FrameLog log{};
        StubPlatform platform{};
        platform._log = &log;
        StubPhysics physics{};
        physics._log = &log;
        StubRenderer renderer{};
        renderer._log = &log;
        StubApp app{};
        app._log = &log;

        Arcadia::Engine engine{};
        engine.platform = &platform;
        engine.physics = &physics;
        engine.renderer = &renderer;

        bool event_handled = false;
        auto subscription = engine.events.Subscribe<SmokeTest::PingEvent>(
            Arcadia::Foundation::Order::Editor,
            [&event_handled, &log](SmokeTest::PingEvent&) -> bool
            {
                event_handled = true;
                log.Add("event");
                return false;
            }
        );
        engine.events.Signal<SmokeTest::PingEvent>(1);

        engine.Frame(app);

        const std::vector<std::string> expected{
            "poll","event","physics.step","physics.apply","render","app.frame","swap"
        };
        Check(log._entries == expected, "frame steps run in the documented order", __LINE__);
        Check(event_handled, "queued events are dispatched inside the frame", __LINE__);
        Check(engine.frame.FrameIndex == 1, "the frame counter advances", __LINE__);
    }

    void CheckRunLoop()
    {
        FrameLog log{};
        StubPlatform platform{};
        platform._log = &log;
        StubApp app{};
        app._log = &log;
        app.frameLimit = 3;

        Arcadia::Engine engine{};
        engine.platform = &platform;

        engine.Run(app);

        Check(app.frames == 3, "Run() loops until the app stops it", __LINE__);
        Check(!engine.IsRunning(), "the engine stops when asked", __LINE__);
        const auto count_entries = [&log](std::string_view name) -> std::size_t
        {
            return std::size_t(std::count(log._entries.begin(), log._entries.end(), std::string{ name }));
        };
        Check(count_entries("poll") == 3, "every frame polls the platform", __LINE__);
        Check(count_entries("swap") == 3, "every frame presents", __LINE__);
        Check(count_entries("app.frame") == 3, "every frame reaches the app", __LINE__);
        Check(
            engine.GetStepProfile().contains("events.dispatch"),
            "the step profile records the frame pipeline",
            __LINE__
        );
    }

    void CheckConfigRoundTrip()
    {
        Arcadia::RuntimeConfig config{};
        config.WorkingDirectory = std::filesystem::path{ "assets" };
        config.WindowWidth = 1920;
        config.WindowHeight = 1080;
        config.UiScale = 1.25f;
        config.WindowTitle = "Round Trip";
        config.OpenedWindowIds = { "Outliner","Viewport" };

        Arcadia::RuntimeConfig loaded{};
        loaded.FromJson(config.ToJson());

        Check(loaded.WindowWidth == 1920 && loaded.WindowHeight == 1080, "window size round-trips", __LINE__);
        Check(NearlyEqual(loaded.UiScale, 1.25f), "ui scale round-trips", __LINE__);
        Check(loaded.WindowTitle == "Round Trip", "window title round-trips", __LINE__);
        Check(loaded.OpenedWindowIds.size() == 2, "open window ids round-trip", __LINE__);
        Check(loaded.WorkingDirectory == std::filesystem::path{ "assets" }, "working directory round-trips", __LINE__);

        // A file that is not there must leave the defaults alone instead of throwing.
        Arcadia::RuntimeConfig untouched{};
        untouched.Load(std::filesystem::path{ "definitely-not-here.acdacfg" });
        Check(untouched.WindowWidth == 1280, "a missing config file keeps the defaults", __LINE__);
    }
}

int main(int argc, char** argv)
{
    bool verbose = false;
    for(int index = 1; index < argc; ++index)
    {
        if(std::string_view{ argv[index] } == "--verbose")
        {
            verbose = true;
        }
    }

    std::printf("foundation smoke\n");

    CheckDeferredDispatch();
    CheckOrderAndConsumption();
    CheckRaiiUnsubscribe();
    CheckSubscribeDuringDispatch();
    CheckNestedSignal();
    CheckSignalNow();
    CheckIntrospection();
    CheckReflectionRoundTrip();
    CheckFieldTable(verbose);
    CheckUnboundedIsNotZeroClamp();
    CheckPatch();
    CheckFrameOrder();
    CheckRunLoop();
    CheckConfigRoundTrip();

    std::printf("%d checks, %d failures\n", g_checks, g_failures);
    return g_failures == 0 ? 0 : 1;
}



