#pragma once
#include <functional>
#include <any>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <algorithm>

class EventBus {
public:
    template<typename E>
    int subscribe(std::function<void(const E&)> handler) {
        int id = next_id_++;
        auto key = std::type_index(typeid(E));
        handlers_[key].push_back({id, key, std::any(std::move(handler))});
        return id;
    }

    void unsubscribe(int id) {
        for (auto& [type, subs] : handlers_) {
            std::erase_if(subs, [id](const Subscription& s) { return s.id == id; });
        }
    }

    template<typename E>
    void emit(const E& event) {
        auto key = std::type_index(typeid(E));
        auto it = handlers_.find(key);
        if (it == handlers_.end()) return;
        for (const auto& sub : it->second) {
            auto& fn = std::any_cast<const std::function<void(const E&)>&>(sub.handler);
            fn(event);
        }
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
