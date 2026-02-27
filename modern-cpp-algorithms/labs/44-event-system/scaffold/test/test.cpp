#include <gtest/gtest.h>
#include "../src/solution.h"
#include <string>

struct ClickEvent { int x, y; };
struct KeyEvent { char key; };

TEST(EventBus, SubscribeAndEmit) {
    EventBus bus;
    int received = 0;
    bus.subscribe<ClickEvent>([&](const ClickEvent& e) {
        received = e.x + e.y;
    });
    bus.emit(ClickEvent{3, 4});
    EXPECT_EQ(received, 7);
}

TEST(EventBus, MultipleHandlers) {
    EventBus bus;
    int count = 0;
    bus.subscribe<ClickEvent>([&](const ClickEvent&) { count++; });
    bus.subscribe<ClickEvent>([&](const ClickEvent&) { count++; });
    bus.emit(ClickEvent{0, 0});
    EXPECT_EQ(count, 2);
}

TEST(EventBus, DifferentEventTypes) {
    EventBus bus;
    int clicks = 0;
    int keys = 0;
    bus.subscribe<ClickEvent>([&](const ClickEvent&) { clicks++; });
    bus.subscribe<KeyEvent>([&](const KeyEvent&) { keys++; });
    bus.emit(ClickEvent{0, 0});
    EXPECT_EQ(clicks, 1);
    EXPECT_EQ(keys, 0);
    bus.emit(KeyEvent{'a'});
    EXPECT_EQ(keys, 1);
}

TEST(EventBus, Unsubscribe) {
    EventBus bus;
    int count = 0;
    int id = bus.subscribe<ClickEvent>([&](const ClickEvent&) { count++; });
    bus.emit(ClickEvent{0, 0});
    EXPECT_EQ(count, 1);
    bus.unsubscribe(id);
    bus.emit(ClickEvent{0, 0});
    EXPECT_EQ(count, 1);
}

TEST(EventBus, EmitWithNoHandlers) {
    EventBus bus;
    bus.emit(ClickEvent{0, 0});  // should not crash
}

TEST(EventBus, UniqueIds) {
    EventBus bus;
    int id1 = bus.subscribe<ClickEvent>([](const ClickEvent&) {});
    int id2 = bus.subscribe<ClickEvent>([](const ClickEvent&) {});
    EXPECT_NE(id1, id2);
}

TEST(EventBus, UnsubscribeOnlyTarget) {
    EventBus bus;
    int a = 0, b = 0;
    int id1 = bus.subscribe<ClickEvent>([&](const ClickEvent&) { a++; });
    bus.subscribe<ClickEvent>([&](const ClickEvent&) { b++; });
    bus.unsubscribe(id1);
    bus.emit(ClickEvent{0, 0});
    EXPECT_EQ(a, 0);
    EXPECT_EQ(b, 1);
}
