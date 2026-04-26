#ifndef HIERSIM_SCENARIO_H
#define HIERSIM_SCENARIO_H

#include "hiersim/WorldMap.h"
#include "hiersim/Organization.h"
#include "hiersim/Facility.h"
#include "hiersim/Unit.h"
#include "hiersim/Individual.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cstdint>

namespace hiersim {

/**
 * @brief Metadata for a scenario
 */
struct ScenarioMetadata {
    std::string id;
    std::string name;
    std::string description;
    std::string author;
    std::string version;
    std::string difficulty;  // Easy, Medium, Hard, Extreme
    std::vector<std::string> tags;
    int64_t createdDate;
    int64_t modifiedDate;
    std::string thumbnailPath;
    
    // Starting conditions
    int startingYear;
    int startingTick;
    std::string economicSystem;
    std::string politicalContext;
    
    // Victory conditions
    std::vector<std::string> victoryConditions;
    std::vector<std::string> failureConditions;
};

/**
 * @brief Represents a complete scenario for the simulation
 * 
 * A scenario contains all initial state data needed to start a game:
 * - World map with regions
 * - Initial organizations
 * - Initial facilities
 * - Initial units
 * - Key individuals
 * - Economic starting conditions
 * - Historical context
 */
class Scenario {
public:
    using ID = uint64_t;
    
    /**
     * @brief Construct an empty scenario
     */
    Scenario();
    
    /**
     * @brief Construct a scenario with metadata
     * @param metadata Scenario metadata
     */
    explicit Scenario(const ScenarioMetadata& metadata);
    
    ~Scenario() = default;
    
    // Getters
    ID getId() const { return id_; }
    const ScenarioMetadata& getMetadata() const { return metadata_; }
    const WorldMap* getWorldMap() const { return worldMap_.get(); }
    WorldMap* getWorldMap() { return worldMap_.get(); }
    
    // Organizations
    const std::map<uint64_t, Organization>& getOrganizations() const { return organizations_; }
    Organization* getOrganization(uint64_t id);
    const Organization* getOrganization(uint64_t id) const;
    bool addOrganization(const Organization& org);
    bool removeOrganization(uint64_t id);
    
    // Facilities
    const std::map<uint64_t, Facility>& getFacilities() const { return facilities_; }
    Facility* getFacility(uint64_t id);
    const Facility* getFacility(uint64_t id) const;
    bool addFacility(const Facility& facility);
    bool removeFacility(uint64_t id);
    
    // Units
    const std::map<uint64_t, Unit>& getUnits() const { return units_; }
    Unit* getUnit(uint64_t id);
    const Unit* getUnit(uint64_t id) const;
    bool addUnit(const Unit& unit);
    bool removeUnit(uint64_t id);
    
    // Individuals
    const std::map<uint64_t, Individual>& getIndividuals() const { return individuals_; }
    Individual* getIndividual(uint64_t id);
    const Individual* getIndividual(uint64_t id) const;
    bool addIndividual(const Individual& individual);
    bool removeIndividual(uint64_t id);
    
    // Economic conditions
    void setInitialEconomicConditions(const std::map<std::string, double>& conditions);
    const std::map<std::string, double>& getInitialEconomicConditions() const { return economicConditions_; }
    
    // Serialization
    std::string serialize() const;
    static Scenario deserialize(const std::string& data);
    
    // Save/load to file
    bool saveToFile(const std::string& filepath) const;
    static Scenario loadFromFile(const std::string& filepath);
    
private:
    ID id_;
    ScenarioMetadata metadata_;
    std::unique_ptr<WorldMap> worldMap_;
    
    std::map<uint64_t, Organization> organizations_;
    std::map<uint64_t, Facility> facilities_;
    std::map<uint64_t, Unit> units_;
    std::map<uint64_t, Individual> individuals_;
    
    std::map<std::string, double> economicConditions_;
    
    static ID nextId_;
};

} // namespace hiersim

#endif // HIERSIM_SCENARIO_H
