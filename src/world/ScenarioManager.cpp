#include "hiersim/ScenarioManager.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

namespace fs = std::filesystem;

namespace hiersim {

ScenarioManager::ScenarioManager(const std::string& scenariosDir)
    : scenariosDir_(scenariosDir) {
    // Ensure the scenarios directory exists
    if (!fs::exists(scenariosDir_)) {
        fs::create_directories(scenariosDir_);
    }
}

int ScenarioManager::scanForScenarios() {
    catalog_.clear();
    scenarioFiles_.clear();
    
    int count = 0;
    
    try {
        for (const auto& entry : fs::directory_iterator(scenariosDir_)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                
                // Only process .scenario files
                if (filename.size() > 9 && 
                    filename.substr(filename.size() - 9) == ".scenario") {
                    
                    std::string filepath = entry.path().string();
                    auto metadata = loadMetadataFromFile(filepath);
                    
                    if (metadata) {
                        catalog_.push_back(*metadata);
                        scenarioFiles_[catalog_.back().id] = filepath;
                        count++;
                    }
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        // Directory doesn't exist or can't be accessed
        return 0;
    }
    
    // Sort by name
    std::sort(catalog_.begin(), catalog_.end(), 
        [](const ScenarioMetadata& a, const ScenarioMetadata& b) {
            return a.name < b.name;
        });
    
    return count;
}

std::unique_ptr<Scenario> ScenarioManager::loadScenario(std::string scenarioId) {
    auto it = scenarioFiles_.find(scenarioId);
    if (it == scenarioFiles_.end()) {
        return nullptr;
    }
    
    try {
        auto scenario = std::make_unique<Scenario>(Scenario::loadFromFile(it->second));
        return scenario;
    } catch (const std::exception& e) {
        return nullptr;
    }
}

std::unique_ptr<Scenario> ScenarioManager::loadScenarioByName(const std::string& name) {
    for (const auto& metadata : catalog_) {
        if (metadata.name == name) {
            return loadScenario(metadata.id);
        }
    }
    return nullptr;
}

bool ScenarioManager::saveScenario(const Scenario& scenario) {
    // Generate filename from scenario name
    std::string safeName = scenario.getMetadata().name;
    // Replace invalid characters
    for (char& c : safeName) {
        if (c == ' ' || c == '/' || c == '\\') {
            c = '_';
        }
    }
    
    std::string filename = safeName + ".scenario";
    std::string filepath = scenariosDir_ + "/" + filename;
    
    if (scenario.saveToFile(filepath)) {
        // Update catalog
        scanForScenarios();
        return true;
    }
    
    return false;
}

bool ScenarioManager::deleteScenario(uint64_t scenarioId) {
    auto it = scenarioFiles_.find(scenarioId);
    if (it == scenarioFiles_.end()) {
        return false;
    }
    
    try {
        fs::remove(it->second);
        scanForScenarios();
        return true;
    } catch (const fs::filesystem_error&) {
        return false;
    }
}

std::unique_ptr<Scenario> ScenarioManager::getDefaultScenario() {
    // Try to load a scenario named "Default" or "Base Scenario"
    auto scenario = loadScenarioByName("Default");
    if (!scenario) {
        scenario = loadScenarioByName("Base Scenario");
    }
    
    // If no default exists, create one
    if (!scenario) {
        scenario = std::make_unique<Scenario>();
        scenario->getMetadata().name = "Default";
        scenario->getMetadata().description = "Default starting scenario";
        scenario->getMetadata().author = "System";
        scenario->getMetadata().difficulty = "Medium";
        scenario->getMetadata().startingYear = 2024;
    }
    
    return scenario;
}

Scenario ScenarioManager::createNewScenario(const std::string& name) {
    ScenarioMetadata metadata;
    metadata.id = 0;  // Will be assigned by Scenario constructor
    metadata.name = name;
    metadata.description = "Custom scenario";
    metadata.author = "Player";
    metadata.version = "1.0";
    metadata.difficulty = "Medium";
    metadata.startingYear = 2024;
    metadata.startingTick = 0;
    
    return Scenario(metadata);
}

std::vector<std::string> ScenarioManager::validateScenario(const Scenario& scenario) const {
    std::vector<std::string> errors;
    
    const auto& metadata = scenario.getMetadata();
    
    // Check required metadata fields
    if (metadata.name.empty()) {
        errors.push_back("Scenario name is required");
    }
    
    if (metadata.description.empty()) {
        errors.push_back("Scenario description is required");
    }
    
    // Check world map
    const auto* worldMap = scenario.getWorldMap();
    if (!worldMap || worldMap->getRegions().empty()) {
        errors.push_back("Scenario must have at least one region");
    }
    
    // Check for at least one organization
    if (scenario.getOrganizations().empty()) {
        errors.push_back("Scenario must have at least one organization");
    }
    
    // Validate economic system
    if (metadata.economicSystem.empty()) {
        errors.push_back("Economic system must be specified");
    }
    
    // Validate starting year
    if (metadata.startingYear < 0 || metadata.startingYear > 10000) {
        errors.push_back("Invalid starting year");
    }
    
    return errors;
}

std::unique_ptr<ScenarioMetadata> ScenarioManager::loadMetadataFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return nullptr;
    }
    
    auto metadata = std::make_unique<ScenarioMetadata>();
    std::string line;
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) continue;
        
        std::string key = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 1);
        
        if (key == "ID") {
            metadata->id = std::stoull(value);
        } else if (key == "NAME") {
            metadata->name = value;
        } else if (key == "DESCRIPTION") {
            metadata->description = value;
        } else if (key == "AUTHOR") {
            metadata->author = value;
        } else if (key == "VERSION") {
            metadata->version = value;
        } else if (key == "DIFFICULTY") {
            metadata->difficulty = value;
        } else if (key == "STARTING_YEAR") {
            metadata->startingYear = std::stoi(value);
        } else if (key == "STARTING_TICK") {
            metadata->startingTick = std::stoi(value);
        } else if (key == "ECONOMIC_SYSTEM") {
            metadata->economicSystem = value;
        } else if (key == "POLITICAL_CONTEXT") {
            metadata->politicalContext = value;
        } else if (key == "TAGS") {
            std::istringstream tagStream(value);
            std::string tag;
            while (std::getline(tagStream, tag, ',')) {
                if (!tag.empty()) {
                    metadata->tags.push_back(tag);
                }
            }
        } else if (key == "CREATED_DATE") {
            metadata->createdDate = std::stoll(value);
        } else if (key == "MODIFIED_DATE") {
            metadata->modifiedDate = std::stoll(value);
        }
    }
    
    // Generate ID if not present
    if (metadata->id == 0) {
        metadata->id = std::hash<std::string>{}(filepath);
    }
    
    return metadata;
}

} // namespace hiersim
