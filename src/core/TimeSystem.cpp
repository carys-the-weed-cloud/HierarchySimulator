#include "hiersim/TimeSystem.h"
namespace hiersim {
void TimeSystem::advance() {
    day_++;
    if (day_ > 365) { day_ = 1; year_++; }
}
void TimeSystem::reset() { year_ = 2024; day_ = 1; }
}
