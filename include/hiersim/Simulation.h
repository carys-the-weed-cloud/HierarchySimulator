#ifndef HIERSIM_SIMULATION_H
#define HIERSIM_SIMULATION_H

#include "Scenario.h"
#include "EconomicModel.h"
#include <QObject>
#include <memory>
#include <vector>
#include <functional>
#include <chrono>
#include <atomic>
#include <mutex>
#include <queue>
#include <thread>
#include <nlohmann/json.hpp>

namespace hiersim {

/**
 * @brief Represents a scheduled event in the simulation
 */
struct ScheduledEvent {
    int tick;                          // Tick at which event should occur
    std::string type;                  // Event type identifier
    std::string source;                // Source of the event (object ID)
    std::string target;                // Target of the event (if applicable)
    nlohmann::json data;               // Event-specific data
    
    bool operator>(const ScheduledEvent& other) const {
        return tick > other.tick;
    }
};

/**
 * @brief Statistics tracked during simulation
 */
struct SimulationStats {
    // Economic indicators
    double gdp = 0.0;
    double unemployment_rate = 0.0;
    double inflation_rate = 0.0;
    double profit_rate = 0.0;
    double wage_share = 0.0;
    
    // Population statistics
    int64_t total_population = 0;
    int64_t employed_population = 0;
    int64_t unemployed_population = 0;
    
    // Organization statistics
    int total_organizations = 0;
    int total_facilities = 0;
    int total_units = 0;
    int total_individuals = 0;
    
    // Crisis indicators
    bool overproduction_crisis = false;
    bool profit_squeeze_crisis = false;
    bool financial_instability = false;
    
    // Simulation metadata
    int current_tick = 0;
    double ticks_per_second = 1.0;
    std::chrono::system_clock::time_point last_update;
};

/**
 * @brief Callback types for simulation events
 */
using SimulationCallback = std::function<void(const SimulationStats&)>;
using EventCallback = std::function<void(const ScheduledEvent&)>;

/**
 * @brief Main simulation controller managing the tick-based game loop
 * 
 * The Simulation class orchestrates the entire game world, advancing time
 * in discrete daily ticks and coordinating all subsystems.
 * 
 * Time System:
 * - 1 tick = 1 day in-game
 * - Configurable speed: 1-6 ticks per second (real-time)
 * - Full pause/step/play control
 */
class Simulation : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Simulation speed presets
     */
    enum class Speed {
        Paused = 0,
        Slow = 1,      // 1 tick/second
        Normal = 3,    // 3 ticks/second (recommended)
        Fast = 6       // 6 ticks/second (maximum)
    };
    
    /**
     * @brief Construct a new Simulation object
     * @param scenario Initial scenario to load
     */
    explicit Simulation(std::shared_ptr<Scenario> scenario);
    
    /**
     * @brief Destructor
     */
    ~Simulation();
    
    // Prevent copying
    Simulation(const Simulation&) = delete;
    Simulation& operator=(const Simulation&) = delete;
    
    /**
     * @brief Start the simulation loop
     */
    void start();
    
    /**
     * @brief Stop the simulation loop
     */
    void stop();
    
    /**
     * @brief Pause the simulation
     */
    void pause();
    
    /**
     * @brief Resume the simulation
     */
    void resume();
    
    /**
     * @brief Advance simulation by exactly one tick
     */
    void step();
    
    /**
     * @brief Set simulation speed
     * @param ticksPerSecond Ticks per real-world second (1-6)
     */
    void setSpeed(double ticksPerSecond);
    
    /**
     * @brief Get current simulation speed
     * @return Current ticks per second
     */
    double getSpeed() const;
    
    /**
     * @brief Check if simulation is running
     * @return true if actively simulating
     */
    bool isRunning() const;
    
    /**
     * @brief Set whether the simulation is running
     * @param running true to start/resume, false to pause
     */
    void setRunning(bool running);
    
    /**
     * @brief Check if simulation is paused
     * @return true if paused
     */
    bool isPaused() const;
    
    /**
     * @brief Get current tick number
     * @return Current simulation tick
     */
    int getCurrentTick() const;
    
    /**
     * @brief Advance simulation by one tick (public for MainWindow)
     */
    void tick();
    
    /**
     * @brief Load a scenario from file
     * @param filepath Path to scenario file
     */
    void loadScenario(const std::string& filepath);
    
    /**
     * @brief Save current scenario to file
     * @param filepath Path to save scenario
     */
    void saveScenario(const std::string& filepath);
    
    /**
     * @brief Get simulation statistics
     * @return Current simulation stats
     */
    SimulationStats getStats() const;
    
    /**
     * @brief Get the current scenario
     * @return Shared pointer to scenario
     */
    std::shared_ptr<Scenario> getScenario() const;
    
    /**
     * @brief Schedule an event for a future tick
     * @param event Event to schedule
     */
    void scheduleEvent(ScheduledEvent event);
    
    /**
     * @brief Register a callback for stat updates
     * @param callback Function to call when stats update
     */
    void onStatsUpdate(SimulationCallback callback);
    
    /**
     * @brief Register a callback for specific event types
     * @param eventType Type of event to listen for
     * @param callback Function to call when event occurs
     */
    void onEvent(const std::string& eventType, EventCallback callback);
    
private:
    /**
     * @brief Main simulation loop (runs in separate thread)
     */
    void simulationLoop();
    
    /**
     * @brief Process a single simulation tick
     */
    void processTick();
    
    /**
     * @brief Process due events
     */
    void processEvents();
    
    /**
     * @brief Update economic model
     */
    void updateEconomy();
    
    /**
     * @brief Update organizations and their decisions
     */
    void updateOrganizations();
    
    /**
     * @brief Update individuals and their decisions
     */
    void updateIndividuals();
    
    /**
     * @brief Update units and military operations
     */
    void updateUnits();
    
    /**
     * @brief Calculate current simulation statistics
     */
    void calculateStats();
    
    /**
     * @brief Detect crisis conditions
     */
    void detectCrises();
    
    /**
     * @brief Notify registered callbacks
     */
    void notifyCallbacks();

signals:
    /**
     * @brief Emitted when the tick counter changes
     */
    void tickChanged(int tick);
    
    /**
     * @brief Emitted when simulation speed changes
     */
    void speedChanged(int speed);
    
    /**
     * @brief Emitted when simulation running state changes
     */
    void stateChanged(bool running);

private:
    std::shared_ptr<Scenario> m_scenario;
    std::unique_ptr<EconomicModel> m_economicModel;
    
    // Time control
    std::atomic<int> m_currentTick{0};
    std::atomic<double> m_ticksPerSecond{1.0};
    std::atomic<bool> m_running{false};
    std::atomic<bool> m_paused{false};
    
    // Event scheduling
    std::priority_queue<ScheduledEvent, std::vector<ScheduledEvent>, std::greater<ScheduledEvent>> m_eventQueue;
    mutable std::mutex m_eventMutex;
    
    // Callbacks
    std::vector<SimulationCallback> m_statsCallbacks;
    std::unordered_map<std::string, std::vector<EventCallback>> m_eventCallbacks;
    mutable std::mutex m_callbackMutex;
    
    // Statistics
    SimulationStats m_stats;
    mutable std::mutex m_statsMutex;
    
    // Simulation thread
    std::thread m_simulationThread;
};

} // namespace hiersim

#endif // HIERSIM_SIMULATION_H
