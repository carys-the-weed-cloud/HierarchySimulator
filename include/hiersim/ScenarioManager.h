#ifndef HIERSIM_SCENARIO_MANAGER_H
#define HIERSIM_SCENARIO_MANAGER_H

#include "hiersim/Scenario.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace hiersim {

/**
 * @brief Manages scenarios for the simulation
 * 
 * The ScenarioManager handles loading, saving, and browsing available scenarios.
 * It maintains a catalog of scenarios and provides methods to load them for gameplay.
 */
class ScenarioManager {
public:
    /**
     * @brief Construct a new Scenario Manager
     * @param scenariosDir Directory containing scenario files
     */
    explicit ScenarioManager(const std::string& scenariosDir = "assets/scenarios");
    
    ~ScenarioManager() = default;
    
    /**
     * @brief Scan the scenarios directory for available scenarios
     * @return Number of scenarios found
     */
    int scanForScenarios();
    
    /**
     * @brief Get list of all available scenarios
     * @return Vector of scenario metadata
     */
    const std::vector<ScenarioMetadata>& getAvailableScenarios() const { return catalog_; }
    
    /**
     * @brief Load a scenario by ID
     * @param scenarioId ID of the scenario to load
     * @return Loaded scenario, nullptr if not found
     */
    std::unique_ptr<Scenario> loadScenario(std::string scenarioId);
    
    /**
     * @brief Load a scenario by name
     * @param name Name of the scenario
     * @return Loaded scenario, nullptr if not found
     */
    std::unique_ptr<Scenario> loadScenarioByName(const std::string& name);
    
    /**
     * @brief Save a scenario to the scenarios directory
     * @param scenario Scenario to save
     * @return true if saved successfully
     */
    bool saveScenario(const Scenario& scenario);
    
    /**
     * @brief Delete a scenario
     * @param scenarioId ID of scenario to delete
     * @return true if deleted successfully
     */
    bool deleteScenario(uint64_t scenarioId);
    
    /**
     * @brief Get the default/base scenario
     * @return The base scenario for new games
     */
    std::unique_ptr<Scenario> getDefaultScenario();
    
    /**
     * @brief Create a new empty scenario with default metadata
     * @param name Name for the new scenario
     * @return New scenario object
     */
    Scenario createNewScenario(const std::string& name);
    
    /**
     * @brief Validate a scenario
     * @param scenario Scenario to validate
     * @return List of validation errors (empty if valid)
     */
    std::vector<std::string> validateScenario(const Scenario& scenario) const;
    
private:
    std::string scenariosDir_;
    std::vector<ScenarioMetadata> catalog_;
    std::map<std::string, std::string> scenarioFiles_;  // ID -> filename mapping
    
    /**
     * @brief Load scenario metadata from a file
     * @param filepath Path to scenario file
     * @return Metadata if successful, nullopt otherwise
     */
    std::unique_ptr<ScenarioMetadata> loadMetadataFromFile(const std::string& filepath);
};

} // namespace hiersim

#endif // HIERSIM_SCENARIO_MANAGER_H
