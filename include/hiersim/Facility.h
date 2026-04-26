#ifndef HIERSIM_FACILITY_H
#define HIERSIM_FACILITY_H

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <algorithm>

namespace hiersim {

// Forward declarations
class Organization;
class Individual;

/**
 * @brief Represents a physical or digital facility in the simulation.
 * 
 * Facilities are structures or digital spaces used to execute societal functions:
 * factories, research labs, government buildings, military bases, schools, etc.
 */
class Facility {
public:
    using ID = uint64_t;
    
    /**
     * @brief Types of facilities
     */
    enum class FacilityType {
        Factory,            // Production facility
        Makerspace,         // Innovation/crafting space
        GovernmentBuilding, // Administrative center
        ResearchLab,        // Research facility
        DigitalInfra,       // Server/network infrastructure
        MilitaryBase,       // Military installation
        School,             // Educational facility
        University,         // Higher education
        Market,             // Trading floor/marketplace
        Hospital,           // Healthcare facility
        Farm,               // Agricultural production
        Mine,               // Resource extraction
        Port,               // Shipping/logistics hub
        Airport,            // Air transport hub
        PowerPlant,         // Energy generation
        Warehouse,          // Storage facility
        Office,             // Administrative office
        Residential,        // Housing complex
        MixedUse            // Multi-purpose facility
    };
    
    /**
     * @brief Construct a new Facility object
     * @param name Name of the facility
     * @param type Type of facility
     * @param id Unique identifier (auto-generated if not provided)
     */
    explicit Facility(const std::string& name = "", 
                     FacilityType type = FacilityType::Factory,
                     ID id = 0);
    
    /**
     * @brief Destroy the Facility object
     */
    ~Facility();
    
    // Getters
    ID getId() const { return id_; }
    const std::string& getName() const { return name_; }
    FacilityType getType() const { return type_; }
    double getLocationX() const { return locationX_; }
    double getLocationY() const { return locationY_; }
    double getLocationZ() const { return locationZ_; }  // For multi-level or underground
    size_t getCapacity() const { return capacity_; }
    size_t getCurrentOccupancy() const { return currentOccupancy_; }
    double getEfficiency() const { return efficiency_; }  // 0.0 to 1.0
    ID getOwner() const { return owner_; }
    const std::vector<ID>& getWorkers() const { return workers_; }
    bool isActive() const { return active_; }
    bool isDestroyed() const { return destroyed_; }
    
    // Setters
    void setName(const std::string& name) { name_ = name; }
    void setLocation(double x, double y, double z = 0.0);
    void setCapacity(size_t capacity) { capacity_ = capacity; }
    void setEfficiency(double eff) { efficiency_ = std::clamp(eff, 0.0, 1.0); }
    void setOwner(ID orgId) { owner_ = orgId; }
    void setActive(bool active) { active_ = active; }
    void setDestroyed(bool destroyed) { destroyed_ = destroyed; }
    
    // Worker management
    void addWorker(ID individualId);
    void removeWorker(ID individualId);
    bool hasWorker(ID individualId) const;
    bool hasCapacity() const { return currentOccupancy_ < capacity_; }
    
    // Access control
    struct AccessLevel {
        enum Level {
            Public,         // Anyone can access
            MemberOnly,     // Only organization members
            StaffOnly,      // Only workers at this facility
            Restricted,     // Specific individuals only
            Secret          // Hidden from most actors
        };
        
        Level level;
        std::vector<ID> authorizedIndividuals;  // For Restricted level
    };
    
    void setAccessLevel(const AccessLevel& access);
    const AccessLevel& getAccessLevel() const { return accessLevel_; }
    bool canAccess(ID individualId, bool isMember, bool isWorker) const;
    
    // Production/Function metrics
    double getOutput() const { return output_; }
    void setOutput(double output) { output_ = output; }
    double getInputRequired() const { return inputRequired_; }
    void setInputRequired(double input) { inputRequired_ = input; }
    
    // Damage and repair
    void damage(double amount);  // 0.0 to 1.0
    void repair(double amount);
    double getIntegrity() const { return integrity_; }  // 1.0 = perfect, 0.0 = destroyed
    
    // Serialization
    std::string serialize() const;
    static Facility deserialize(const std::string& data);
    
private:
    ID id_;
    std::string name_;
    FacilityType type_;
    double locationX_, locationY_, locationZ_;
    size_t capacity_;
    size_t currentOccupancy_;
    double efficiency_;
    ID owner_;  // Owning organization ID
    std::vector<ID> workers_;  // Individual IDs working here
    AccessLevel accessLevel_;
    double output_;
    double inputRequired_;
    double integrity_;
    bool active_;
    bool destroyed_;
    
    static ID nextId_;
};

} // namespace hiersim

#endif // HIERSIM_FACILITY_H
