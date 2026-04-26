#pragma once
namespace hiersim {
class TimeSystem {
public:
    int getYear() const { return year_; }
    int getDay() const { return day_; }
    void advance();
    void reset();
private:
    int year_ = 2024;
    int day_ = 1;
};
}
