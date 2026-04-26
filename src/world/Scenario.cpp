#include "hiersim/Scenario.h"
#include "hiersim/Serializer.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <stdexcept>

namespace hiersim {

Scenario::ID Scenario::nextId_ = 1;

Scenario::Scenario() 
    : id_(nextId_++)
    , worldMap_(std::make_unique<WorldMap>("Default Map")) {
    metadata_.startingYear = 2024;
    metadata_.startingTick = 0;
    metadata_.version = "1.0";
    metadata_.createdDate = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    metadata_.modifiedDate = metadata_.createdDate;
}

Scenario::Scenario(const ScenarioMetadata& metadata)
    : id_(nextId_++)
    , metadata_(metadata)
    , worldMap_(std::make_unique<WorldMap>("Scenario Map")) {
    metadata_.modifiedDate = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

Organization* Scenario::getOrganization(uint64_t id) {
    auto it = organizations_.find(id);
    return it != organizations_.end() ? &it->second : nullptr;
}

const Organization* Scenario::getOrganization(uint64_t id) const {
    auto it = organizations_.find(id);
    return it != organizations_.end() ? &it->second : nullptr;
}

bool Scenario::addOrganization(const Organization& org) {
    if (organizations_.find(org.getId()) != organizations_.end()) {
        return false;
    }
    organizations_[org.getId()] = org;
    return true;
}

bool Scenario::removeOrganization(uint64_t id) {
    return organizations_.erase(id) > 0;
}

Facility* Scenario::getFacility(uint64_t id) {
    auto it = facilities_.find(id);
    return it != facilities_.end() ? &it->second : nullptr;
}

const Facility* Scenario::getFacility(uint64_t id) const {
    auto it = facilities_.find(id);
    return it != facilities_.end() ? &it->second : nullptr;
}

bool Scenario::addFacility(const Facility& facility) {
    if (facilities_.find(facility.getId()) != facilities_.end()) {
        return false;
    }
    facilities_[facility.getId()] = facility;
    return true;
}

bool Scenario::removeFacility(uint64_t id) {
    return facilities_.erase(id) > 0;
}

Unit* Scenario::getUnit(uint64_t id) {
    auto it = units_.find(id);
    return it != units_.end() ? &it->second : nullptr;
}

const Unit* Scenario::getUnit(uint64_t id) const {
    auto it = units_.find(id);
    return it != units_.end() ? &it->second : nullptr;
}

bool Scenario::addUnit(const Unit& unit) {
    if (units_.find(unit.getId()) != units_.end()) {
        return false;
    }
    units_[unit.getId()] = unit;
    return true;
}

bool Scenario::removeUnit(uint64_t id) {
    return units_.erase(id) > 0;
}

Individual* Scenario::getIndividual(uint64_t id) {
    auto it = individuals_.find(id);
    return it != individuals_.end() ? &it->second : nullptr;
}

const Individual* Scenario::getIndividual(uint64_t id) const {
    auto it = individuals_.find(id);
    return it != individuals_.end() ? &it->second : nullptr;
}

bool Scenario::addIndividual(const Individual& individual) {
    if (individuals_.find(individual.getId()) != individuals_.end()) {
        return false;
    }
    individuals_[individual.getId()] = individual;
    return true;
}

bool Scenario::removeIndividual(uint64_t id) {
    return individuals_.erase(id) > 0;
}

void Scenario::setInitialEconomicConditions(const std::map<std::string, double>& conditions) {
    economicConditions_ = conditions;
}

std::string Scenario::serialize() const {
    std::ostringstream oss;
    
    // Serialize metadata
    oss << "SCENARIO_VERSION:1\n";
    oss << "ID:" << id_ << "\n";
    oss << "NAME:" << metadata_.name << "\n";
    oss << "DESCRIPTION:" << metadata_.description << "\n";
    oss << "AUTHOR:" << metadata_.author << "\n";
    oss << "VERSION:" << metadata_.version << "\n";
    oss << "DIFFICULTY:" << metadata_.difficulty << "\n";
    oss << "STARTING_YEAR:" << metadata_.startingYear << "\n";
    oss << "STARTING_TICK:" << metadata_.startingTick << "\n";
    oss << "ECONOMIC_SYSTEM:" << metadata_.economicSystem << "\n";
    oss << "POLITICAL_CONTEXT:" << metadata_.politicalContext << "\n";
    
    // Serialize tags
    oss << "TAGS:";
    for (size_t i = 0; i < metadata_.tags.size(); ++i) {
        if (i > 0) oss << ",";
        oss << metadata_.tags[i];
    }
    oss << "\n";
    
    // Serialize victory conditions
    oss << "VICTORY_CONDITIONS:";
    for (size_t i = 0; i < metadata_.victoryConditions.size(); ++i) {
        if (i > 0) oss << ",";
        oss << metadata_.victoryConditions[i];
    }
    oss << "\n";
    
    // Serialize failure conditions
    oss << "FAILURE_CONDITIONS:";
    for (size_t i = 0; i < metadata_.failureConditions.size(); ++i) {
        if (i > 0) oss << ",";
        oss << metadata_.failureConditions[i];
    }
    oss << "\n";
    
    // Serialize world map
    if (worldMap_) {
        oss << "WORLD_MAP:\n" << worldMap_->serialize() << "\n";
    }
    
    // Serialize organizations
    oss << "ORGANIZATIONS_COUNT:" << organizations_.size() << "\n";
    for (const auto& [id, org] : organizations_) {
        oss << "ORG:" << org.serialize() << "\n";
    }
    
    // Serialize facilities
    oss << "FACILITIES_COUNT:" << facilities_.size() << "\n";
    for (const auto& [id, fac] : facilities_) {
        oss << "FAC:" << fac.serialize() << "\n";
    }
    
    // Serialize units
    oss << "UNITS_COUNT:" << units_.size() << "\n";
    for (const auto& [id, unit] : units_) {
        oss << "UNIT:" << unit.serialize() << "\n";
    }
    
    // Serialize individuals
    oss << "INDIVIDUALS_COUNT:" << individuals_.size() << "\n";
    for (const auto& [id, ind] : individuals_) {
        oss << "IND:" << ind.serialize() << "\n";
    }
    
    // Serialize economic conditions
    oss << "ECONOMIC_CONDITIONS_COUNT:" << economicConditions_.size() << "\n";
    for (const auto& [key, value] : economicConditions_) {
        oss << "ECO:" << key << ":" << value << "\n";
    }
    
    return oss.str();
}

Scenario Scenario::deserialize(const std::string& data) {
    Scenario scenario;
    std::istringstream iss(data);
    std::string line;
    
    while (std::getline(iss, line)) {
        if (line.empty()) continue;
        
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) continue;
        
        std::string key = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 1);
        
        if (key == "NAME") {
            scenario.metadata_.name = value;
        } else if (key == "DESCRIPTION") {
            scenario.metadata_.description = value;
        } else if (key == "AUTHOR") {
            scenario.metadata_.author = value;
        } else if (key == "VERSION") {
            scenario.metadata_.version = value;
        } else if (key == "DIFFICULTY") {
            scenario.metadata_.difficulty = value;
        } else if (key == "STARTING_YEAR") {
            scenario.metadata_.startingYear = std::stoi(value);
        } else if (key == "STARTING_TICK") {
            scenario.metadata_.startingTick = std::stoi(value);
        } else if (key == "ECONOMIC_SYSTEM") {
            scenario.metadata_.economicSystem = value;
        } else if (key == "POLITICAL_CONTEXT") {
            scenario.metadata_.politicalContext = value;
        } else if (key == "TAGS") {
            // Parse comma-separated tags
            std::istringstream tagStream(value);
            std::string tag;
            while (std::getline(tagStream, tag, ',')) {
                if (!tag.empty()) {
                    scenario.metadata_.tags.push_back(tag);
                }
            }
        } else if (key == "VICTORY_CONDITIONS") {
            std::istringstream condStream(value);
            std::string cond;
            while (std::getline(condStream, cond, ',')) {
                if (!cond.empty()) {
                    scenario.metadata_.victoryConditions.push_back(cond);
                }
            }
        } else if (key == "FAILURE_CONDITIONS") {
            std::istringstream condStream(value);
            std::string cond;
            while (std::getline(condStream, cond, ',')) {
                if (!cond.empty()) {
                    scenario.metadata_.failureConditions.push_back(cond);
                }
            }
        }
        // Note: Full deserialization of objects would require parsing their formats
        // This is a simplified version for the skeleton implementation
    }
    
    return std::move(scenario);
}

bool Scenario::saveToFile(const std::string& filepath) const {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        return false;
    }
    
    file << serialize();
    file.close();
    
    return file.good();
}

Scenario Scenario::loadFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open scenario file: " + filepath);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string data = buffer.str();
    
    return deserialize(data);
}

} // namespace hiersim
