#include "Simulation.h"
#include <thread>
#include <chrono>

namespace hiersim {

Simulation::Simulation(std::shared_ptr<Scenario> scenario)
    : m_scenario(scenario)
    , m_economicModel(std::make_unique<EconomicModel>())
{
    // Initialize economic model with scenario data
    if (m_scenario) {
        m_economicModel->setScenario(m_scenario);
    }
    
    // Initialize statistics
    m_stats.last_update = std::chrono::system_clock::now();
}

Simulation::~Simulation() {
    stop();
}

void Simulation::start() {
    if (m_running) {
        return;
    }
    
    m_running = true;
    m_paused = false;
    m_simulationThread = std::thread(&Simulation::simulationLoop, this);
}

void Simulation::stop() {
    if (!m_running) {
        return;
    }
    
    m_running = false;
    m_paused = false;
    
    if (m_simulationThread.joinable()) {
        m_simulationThread.join();
    }
}

void Simulation::pause() {
    m_paused = true;
}

void Simulation::resume() {
    if (m_running) {
        m_paused = false;
    }
}

void Simulation::step() {
    if (m_running && !m_paused) {
        // If already running and not paused, just return
        return;
    }
    
    // Execute one tick
    processTick();
}

void Simulation::setSpeed(double ticksPerSecond) {
    // Clamp to valid range [1.0, 6.0]
    if (ticksPerSecond < 1.0) {
        ticksPerSecond = 1.0;
    } else if (ticksPerSecond > 6.0) {
        ticksPerSecond = 6.0;
    }
    
    m_ticksPerSecond = ticksPerSecond;
}

double Simulation::getSpeed() const {
    return m_ticksPerSecond.load();
}

bool Simulation::isRunning() const {
    return m_running.load();
}

bool Simulation::isPaused() const {
    return m_paused.load();
}

int Simulation::getCurrentTick() const {
    return m_currentTick.load();
}

SimulationStats Simulation::getStats() const {
    std::lock_guard<std::mutex> lock(m_statsMutex);
    return m_stats;
}

std::shared_ptr<Scenario> Simulation::getScenario() const {
    return m_scenario;
}

void Simulation::scheduleEvent(ScheduledEvent event) {
    std::lock_guard<std::mutex> lock(m_eventMutex);
    m_eventQueue.push(std::move(event));
}

void Simulation::onStatsUpdate(SimulationCallback callback) {
    std::lock_guard<std::mutex> lock(m_callbackMutex);
    m_statsCallbacks.push_back(std::move(callback));
}

void Simulation::onEvent(const std::string& eventType, EventCallback callback) {
    std::lock_guard<std::mutex> lock(m_callbackMutex);
    m_eventCallbacks[eventType].push_back(std::move(callback));
}

void Simulation::simulationLoop() {
    auto lastTickTime = std::chrono::steady_clock::now();
    
    while (m_running) {
        if (!m_paused) {
            auto currentTime = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration<double>(currentTime - lastTickTime).count();
            
            double tickInterval = 1.0 / m_ticksPerSecond.load();
            
            if (elapsed >= tickInterval) {
                processTick();
                lastTickTime = currentTime;
            }
            
            // Small sleep to prevent busy-waiting
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        } else {
            // When paused, still update occasionally for responsiveness
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

void Simulation::processTick() {
    // Increment tick counter
    ++m_currentTick;
    
    // Process scheduled events
    processEvents();
    
    // Update subsystems
    updateEconomy();
    updateOrganizations();
    updateIndividuals();
    updateUnits();
    
    // Calculate statistics
    calculateStats();
    
    // Detect crises
    detectCrises();
    
    // Notify callbacks
    notifyCallbacks();
}

void Simulation::processEvents() {
    std::lock_guard<std::mutex> lock(m_eventMutex);
    
    int currentTick = m_currentTick.load();
    
    while (!m_eventQueue.empty() && m_eventQueue.top().tick <= currentTick) {
        ScheduledEvent event = m_eventQueue.top();
        m_eventQueue.pop();
        
        // Notify event callbacks
        {
            std::lock_guard<std::mutex> cbLock(m_callbackMutex);
            auto it = m_eventCallbacks.find(event.type);
            if (it != m_eventCallbacks.end()) {
                for (const auto& callback : it->second) {
                    callback(event);
                }
            }
        }
    }
}

void Simulation::updateEconomy() {
    if (m_economicModel && m_scenario) {
        m_economicModel->processTick();
    }
}

void Simulation::updateOrganizations() {
    if (!m_scenario) {
        return;
    }
    
    // TODO: Implement organization decision-making
    // For now, iterate through organizations and update their state
    auto& orgs = m_scenario->organizations;
    for (auto& [id, org] : orgs) {
        // Placeholder for organization updates
        // - Process resource allocation
        // - Handle membership changes
        // - Execute organizational decisions
    }
}

void Simulation::updateIndividuals() {
    if (!m_scenario) {
        return;
    }
    
    // TODO: Implement individual decision-making
    // For now, iterate through individuals and update their state
    auto& individuals = m_scenario->individuals;
    for (auto& [id, individual] : individuals) {
        // Placeholder for individual updates
        // - Process needs and desires
        // - Update relationships
        // - Make decisions based on ideology and circumstances
    }
}

void Simulation::updateUnits() {
    if (!m_scenario) {
        return;
    }
    
    // TODO: Implement unit operations
    // For now, iterate through units and update their state
    auto& units = m_scenario->units;
    for (auto& [id, unit] : units) {
        // Placeholder for unit updates
        // - Process movement orders
        // - Handle combat resolution
        // - Update morale and loyalty
    }
}

void Simulation::calculateStats() {
    std::lock_guard<std::mutex> lock(m_statsMutex);
    
    m_stats.current_tick = m_currentTick.load();
    m_stats.ticks_per_second = m_ticksPerSecond.load();
    m_stats.last_update = std::chrono::system_clock::now();
    
    if (!m_scenario) {
        return;
    }
    
    // Count objects
    m_stats.total_organizations = static_cast<int>(m_scenario->organizations.size());
    m_stats.total_facilities = static_cast<int>(m_scenario->facilities.size());
    m_stats.total_units = static_cast<int>(m_scenario->units.size());
    m_stats.total_individuals = static_cast<int>(m_scenario->individuals.size());
    
    // Get economic indicators from economic model
    if (m_economicModel) {
        auto indicators = m_economicModel->getMacroeconomicIndicators();
        m_stats.gdp = indicators.gdp;
        m_stats.unemployment_rate = indicators.unemploymentRate;
        m_stats.inflation_rate = indicators.inflationRate;
        m_stats.profit_rate = indicators.profitRate;
        m_stats.wage_share = indicators.wageShare;
    }
    
    // TODO: Calculate population statistics from scenario data
    m_stats.total_population = 0;
    m_stats.employed_population = 0;
    m_stats.unemployed_population = 0;
}

void Simulation::detectCrises() {
    std::lock_guard<std::mutex> lock(m_statsMutex);
    
    if (!m_economicModel) {
        return;
    }
    
    auto crises = m_economicModel->detectCrises();
    
    m_stats.overproduction_crisis = crises.overproduction;
    m_stats.profit_squeeze_crisis = crises.profitSqueeze;
    m_stats.financial_instability = crises.financialInstability;
}

void Simulation::notifyCallbacks() {
    std::lock_guard<std::mutex> lock(m_callbackMutex);
    
    SimulationStats statsCopy;
    {
        std::lock_guard<std::mutex> statsLock(m_statsMutex);
        statsCopy = m_stats;
    }
    
    for (const auto& callback : m_statsCallbacks) {
        callback(statsCopy);
    }
}

} // namespace hiersim
