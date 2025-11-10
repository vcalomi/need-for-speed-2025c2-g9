#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

class Event {
public:
    virtual ~Event() = default;
    virtual std::string GetType() const = 0;
};

class EventBus {
public:
    template <typename EventT>
    void Subscribe(std::function<void(const EventT&)> listener) {
        auto& vec = listeners_[EventT::Type()];
        vec.push_back([listener](const Event& e) { listener(static_cast<const EventT&>(e)); });
    }


    void Publish(const Event& event) {
        auto it = listeners_.find(event.GetType());
        if (it == listeners_.end())
            return;
        for (const auto& listener: it->second) {
            listener(event);
        }
    }

private:
    using ListenerFunc = std::function<void(const Event&)>;
    std::unordered_map<std::string, std::vector<ListenerFunc>> listeners_;
};
