#include "hiersim/Random.h"
#include <chrono>
namespace hiersim {
Random::Random() : rng_(std::chrono::steady_clock::now().time_since_epoch().count()) {}
Random& Random::instance() { static Random r; return r; }
int Random::nextInt(int min, int max) { std::uniform_int_distribution<> dist(min, max); return dist(rng_); }
double Random::nextDouble(double min, double max) { std::uniform_real_distribution<> dist(min, max); return dist(rng_); }
bool Random::nextBool(double probability) { return nextDouble(0.0, 1.0) < probability; }
}
