#include "hiersim/EventManager.h"
namespace hiersim {
void EventManager::schedule(const Event& event) { events_.push_back(event); }
std::vector<Event> EventManager::getEventsForTick(int tick) const {
    std::vector<Event> result;
    for (const auto& e : events_) if (e.tick == tick) result.push_back(e);
    return result;
}
void EventManager::clear() { events_.clear(); }
}
