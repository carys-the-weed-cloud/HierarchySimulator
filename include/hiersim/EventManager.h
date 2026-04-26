#pragma once
#include <string>
#include <vector>
#include <functional>

namespace hiersim {
enum class EventType { Economic, Political, Social, Military, Natural };
struct Event {
    std::string id;
    std::string description;
    EventType type;
    int tick;
    int importance;
};
class EventManager {
public:
    void schedule(const Event& event);
    std::vector<Event> getEventsForTick(int tick) const;
    void clear();
private:
    std::vector<Event> events_;
};
}
