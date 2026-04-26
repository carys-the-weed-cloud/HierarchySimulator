#pragma once
namespace hiersim {
class Simulation;
class SimulationEngine {
public:
    void run(Simulation* sim, int ticks);
    void pause();
    void resume();
    void stop();
};
}
