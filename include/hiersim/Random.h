#pragma once
#include <random>

namespace hiersim {
class Random {
public:
    static Random& instance();
    int nextInt(int min, int max);
    double nextDouble(double min, double max);
    bool nextBool(double probability = 0.5);
private:
    Random();
    std::mt19937 rng_;
};
}
