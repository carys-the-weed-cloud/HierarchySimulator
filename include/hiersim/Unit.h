#ifndef HIERSIM_UNIT_H
#define HIERSIM_UNIT_H

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <algorithm>

namespace hiersim {

// Forward declarations
class Organization;
class Individual;
class Facility;

/**
 * @brief Represents an armed unit in the simulation.
 * 
 * Units are armed groupings of people that fight on behalf of others:
 * standing armies, militias, peacekeeping forces, mercenaries, police, etc.
 */
class Unit {
public:
    using ID = uint64_t;
    
    /**
     * @brief Types of military units
     */
    enum class UnitType {
        StandingArmy,       // Professional military
        Militia,            // Citizen soldiers
        Paramilitary,       // Semi-military organization
        Peacekeeping,       // UN-style peacekeepers
        Mercenary,          // For-hire military
        Police,             // Law enforcement
        Revolutionary,      // Revolutionary brigade
        Guerrilla,          // Irregular warfare
        Naval,              // Navy unit
        AirForce,           // Air force unit
        SpecialForces,      // Elite special operations
        BorderGuard,        // Border security
        Intelligence,       // Intelligence operatives
        PrivateSecurity     // Private military contractor
    };
    
    /**
     * @brief Construct a new Unit object
     * @param name Name of the unit
     * @param type Type of unit
     * @param id Unique identifier (auto-generated if not provided)
     */
    explicit Unit(const std::string& name,
                 UnitType type = UnitType::StandingArmy,
                 ID id = 0);
    
    /**
     * @brief Destroy the Unit object
     */
    ~Unit();
    
    // Getters
    ID getId() const { return id_; }
    const std::string& getName() const { return name_; }
    UnitType getType() const { return type_; }
    ID getEquippingOrg() const { return equippingOrg_; }
    ID getCommandingOrg() const { return commandingOrg_; }
    size_t getPersonnelCount() const { return personnel_.size(); }
    const std::vector<ID>& getPersonnel() const { return personnel_; }
    double getMorale() const { return morale_; }  // 0.0 to 1.0
    double getLoyalty() const { return loyalty_; }  // 0.0 to 1.0 (to commanding org)
    double getCombatEffectiveness() const { return combatEffectiveness_; }  // 0.0 to 1.0
    double getStrength() const { return strength_; }  // Current strength (0.0 to 1.0)
    bool isActive() const { return active_; }
    bool isDeployed() const { return deployed_; }
    
    // Setters
    void setName(const std::string& name) { name_ = name; }
    void setEquippingOrg(ID orgId) { equippingOrg_ = orgId; }
    void setCommandingOrg(ID orgId) { commandingOrg_ = orgId; }
    void setMorale(double morale) { morale_ = std::clamp(morale, 0.0, 1.0); }
    void setLoyalty(double loyalty) { loyalty_ = std::clamp(loyalty, 0.0, 1.0); }
    void setActive(bool active) { active_ = active; }
    void setDeployed(bool deployed) { deployed_ = deployed; }
    
    // Personnel management
    void addPersonnel(ID individualId);
    void removePersonnel(ID individualId);
    bool hasPersonnel(ID individualId) const;
    
    // Location and deployment
    double getLocationX() const { return locationX_; }
    double getLocationY() const { return locationY_; }
    double getLocationZ() const { return locationZ_; }
    void setLocation(double x, double y, double z = 0.0);
    void deploy(double x, double y, double z = 0.0);
    void retreat();
    bool isAtLocation(double x, double y, double radius = 1.0) const;
    
    // Combat operations
    struct CombatOrder {
        enum OrderType {
            Attack,
            Defend,
            Hold,
            Patrol,
            Retreat,
            Reinforce,
            Siege,
            Ambush
        };
        
        OrderType order;
        ID targetUnit;      // Target unit ID (if applicable)
        ID targetFacility;  // Target facility ID (if applicable)
        double targetX;     // Target coordinates
        double targetY;
        double targetZ;
        int64_t validUntil; // Tick until which order is valid
    };
    
    void issueOrder(const CombatOrder& order);
    CombatOrder getCurrentOrder() const { return currentOrder_; }
    bool hasOrders() const { return !orderQueue_.empty(); }
    CombatOrder executeNextOrder(int64_t currentTick);
    
    // Combat resolution
    struct CombatResult {
        double damageDealt;
        double damageTaken;
        double moraleChange;
        double loyaltyChange;
        bool victory;
        std::string description;
    };
    
    CombatResult engage(Unit& enemy);
    CombatResult attackFacility(Facility& facility);
    
    // Casualties and reinforcement
    void takeCasualties(double ratio);  // 0.0 to 1.0
    void reinforce(size_t count);
    
    // Experience and training
    double getExperience() const { return experience_; }  // 0.0 to 1.0
    void gainExperience(double amount);
    void train(double intensity);  // Improves effectiveness
    
    // Serialization
    std::string serialize() const;
    static Unit deserialize(const std::string& data);
    
private:
    ID id_;
    std::string name_;
    UnitType type_;
    ID equippingOrg_;   // Organization that provides equipment/supplies
    ID commandingOrg_;  // Organization that commands this unit
    std::vector<ID> personnel_;  // Individual IDs in this unit
    double morale_;
    double loyalty_;
    double combatEffectiveness_;
    double strength_;
    double experience_;
    double locationX_, locationY_, locationZ_;
    CombatOrder currentOrder_;
    std::vector<CombatOrder> orderQueue_;
    bool active_;
    bool deployed_;
    
    static ID nextId_;
};

} // namespace hiersim

#endif // HIERSIM_UNIT_H
