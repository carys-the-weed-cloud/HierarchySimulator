#include "hiersim/SimulationEngine.h"
#include "hiersim/Simulation.h"
namespace hiersim {
void SimulationEngine::run(Simulation* sim, int ticks) { for (int i = 0; i < ticks; i++) sim->step(); }
void SimulationEngine::pause() {}
void SimulationEngine::resume() {}
void SimulationEngine::stop() {}
}
