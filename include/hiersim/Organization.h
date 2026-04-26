#ifndef HIERSIM_ORGANIZATION_H
#define HIERSIM_ORGANIZATION_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cstdint>
#include <algorithm>

namespace hiersim {

// Forward declarations
class Individual;
class Facility;
class Unit;

/**
 * @brief Represents a hierarchical organization in the simulation.
 * 
 * Organizations are hierarchical groupings (vertical or horizontal) of people
 * who cooperate to execute functions. Examples include states, corporations,
 * unions, rebel groups, political parties, NGOs, etc.
 */
class Organization {
public:
    using ID = uint64_t;
    
    /**
     * @brief Types of organizational structures
     */
    enum class StructureType {
        Hierarchy,      // Top-down command structure
        Network,        // Decentralized network
        Collective,     // Horizontal collective
        Cooperative,    // Worker cooperative
        Corporation,    // Traditional corporation
        State,          // Government entity
        Union,          // Labor union
        Party,          // Political party
        NGO,            // Non-governmental organization
        Military,       // Military organization
        Rebel           // Insurgency/rebel group
    };
    
    /**
     * @brief Construct a new Organization object
     * @param name Name of the organization
     * @param type Type of organization
     * @param id Unique identifier (auto-generated if not provided)
     */
    explicit Organization(const std::string& name, 
                         StructureType type = StructureType::Hierarchy,
                         ID id = 0);
    
    /**
     * @brief Destroy the Organization object
     */
    ~Organization();
    
    // Getters
    ID getId() const { return id_; }
    const std::string& getName() const { return name_; }
    StructureType getType() const { return type_; }
    double getDemocracyLevel() const { return democracyLevel_; }  // 0.0 (autocracy) to 1.0 (full democracy)
    size_t getMemberCount() const { return members_.size(); }
    const std::vector<ID>& getMembers() const { return members_; }
    ID getParent() const { return parent_; }
    const std::vector<ID>& getSubordinates() const { return subordinates_; }
    const std::vector<ID>& getFacilities() const { return facilities_; }
    double getTreasury() const { return treasury_; }
    bool isActive() const { return active_; }
    
    // Setters
    void setName(const std::string& name) { name_ = name; }
    void setDemocracyLevel(double level) { democracyLevel_ = std::clamp(level, 0.0, 1.0); }
    void setParent(ID parentId) { parent_ = parentId; }
    void setTreasury(double amount) { treasury_ = amount; }
    void setActive(bool active) { active_ = active; }
    
    // Membership management
    void addMember(ID individualId);
    void removeMember(ID individualId);
    bool hasMember(ID individualId) const;
    
    // Hierarchy management
    void addSubordinate(ID orgId);
    void removeSubordinate(ID orgId);
    bool isSubordinate(ID orgId) const;
    
    // Resource management
    void depositResources(double amount);
    void withdrawResources(double amount);
    bool canAfford(double amount) const;
    
    // Distribution (based on democracy level and internal rules)
    void distributeResources();
    
    // Recruitment
    struct RecruitmentCriteria {
        std::map<std::string, double> minSkills;
        std::string requiredIdeology;
        double minWealth;
        double maxWealth;
        bool activeOnly;
    };
    
    void setRecruitmentCriteria(const RecruitmentCriteria& criteria);
    const RecruitmentCriteria& getRecruitmentCriteria() const { return recruitmentCriteria_; }
    
    // Serialization
    std::string serialize() const;
    static Organization deserialize(const std::string& data);
    
private:
    ID id_;
    std::string name_;
    StructureType type_;
    double democracyLevel_;  // 0.0 = pure autocracy, 1.0 = pure democracy
    std::vector<ID> members_;  // Individual IDs
    std::map<ID, int> memberRanks_;  // Individual ID -> rank/level in hierarchy
    std::vector<ID> subordinates_;  // Subordinate organization IDs
    ID parent_;  // Parent organization ID (0 if none)
    std::vector<ID> facilities_;  // Facility IDs owned/controlled
    double treasury_;
    RecruitmentCriteria recruitmentCriteria_;
    bool active_;
    
    static ID nextId_;
};

} // namespace hiersim

#endif // HIERSIM_ORGANIZATION_H
