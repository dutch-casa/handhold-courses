#pragma once
#include <functional>
#include <any>
#include <typeindex>
#include <unordered_map>
#include <vector>

class EventBus {
public:
    template<typename E>
    int subscribe(std::function<void(const E&)> handler) {
        // TODO: store handler, return unique ID
        return 0;
    }

    void unsubscribe(int id) {
        // TODO: remove subscription by ID
    }

    template<typename E>
    void emit(const E& event) {
        // TODO: dispatch to all handlers for this event type
    }

private:
    struct Subscription {
        int id;
        std::type_index type;
        std::any handler;
    };

    std::unordered_map<std::type_index, std::vector<Subscription>> handlers_;
    int next_id_ = 1;
};
