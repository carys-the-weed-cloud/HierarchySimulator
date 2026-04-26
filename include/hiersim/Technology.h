#ifndef HIERSIM_TECHNOLOGY_H
#define HIERSIM_TECHNOLOGY_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <cstdint>

namespace hiersim {

/**
 * @brief Technology domain categories
 */
enum class TechDomain {
    Industrial,       // Manufacturing, automation
    Agricultural,     // Farming, food production
    Military,         // Weapons, defense systems
    Medical,          // Healthcare, biotechnology
    Energy,           // Power generation, storage
    Computing,        // Software, AI, robotics
    Transportation,   // Vehicles, logistics
    Materials,        // New materials, nanotechnology
    Space,            // Aerospace, colonization
    Social,           // Organization, governance
};

/**
 * @brief Technology tier/era
 */
enum class TechTier {
    Primitive,    // Pre-industrial
    Early,        // Early industrial
    Modern,       // Contemporary
    Advanced,     // Near-future
    Futuristic,   // Far-future
};

/**
 * @brief Represents a technology that can be researched
 */
struct Technology {
    std::string id;
    std::string name;
    std::string description;
    TechDomain domain;
    TechTier tier;
    
    std::vector<std::string> prerequisites;  // IDs of required technologies
    std::set<std::string> unlocks;           // What this unlocks (facilities, units, policies)
    
    // Research costs
    int research_cost = 100;                 // Base research points needed
    int maintenance_cost = 0;                // Ongoing cost per tick
    
    // Effects
    float productivity_bonus = 0.0f;         // Bonus to production efficiency
    float military_bonus = 0.0f;             // Bonus to combat effectiveness
    float quality_of_life_bonus = 0.0f;      // Bonus to population wellbeing
    
    bool is_discovered = false;              // Has any organization discovered this?
    int64_t discovery_tick = -1;             // Tick when first discovered
    
    /**
     * @brief Check if this technology can be researched
     * @param discovered_techs Set of already discovered technology IDs
     * @return True if all prerequisites are met
     */
    bool canResearch(const std::set<std::string>& discovered_techs) const;
    
    /**
     * @brief Get string representation of domain
     */
    static std::string domainToString(TechDomain domain);
    
    /**
     * @brief Get string representation of tier
     */
    static std::string tierToString(TechTier tier);
};

/**
 * @brief Manages technology research for an organization
 */
class TechTree {
public:
    TechTree() = default;
    
    /**
     * @brief Initialize with default technology tree
     */
    void initializeDefaultTree();
    
    /**
     * @brief Get technology by ID
     */
    const Technology* getTechnology(const std::string& id) const;
    
    /**
     * @brief Get all technologies in a domain
     */
    std::vector<const Technology*> getTechnologiesByDomain(TechDomain domain) const;
    
    /**
     * @brief Get technologies available to research (prerequisites met)
     * @param discovered_techs Set of already discovered tech IDs
     */
    std::vector<const Technology*> getAvailableTechnologies(const std::set<std::string>& discovered_techs) const;
    
    /**
     * @brief Check if a technology is discovered
     */
    bool isDiscovered(const std::string& tech_id) const;
    
    /**
     * @brief Mark a technology as discovered
     * @param tech_id Technology ID
     * @param tick Current simulation tick
     */
    void discoverTechnology(const std::string& tech_id, int64_t tick);
    
    /**
     * @brief Get all discovered technologies
     */
    const std::set<std::string>& getDiscoveredTechnologies() const { return discovered_techs_; }
    
    /**
     * @brief Get total number of technologies
     */
    size_t getTotalTechnologyCount() const { return technologies_.size(); }
    
    /**
     * @brief Get number of discovered technologies
     */
    size_t getDiscoveredCount() const { return discovered_techs_.size(); }
    
    /**
     * @brief Calculate research progress percentage
     */
    float getResearchProgress() const;
    
private:
    std::map<std::string, Technology> technologies_;
    std::set<std::string> discovered_techs_;
};

/**
 * @brief Research queue item
 */
struct ResearchQueueItem {
    std::string technology_id;
    int progress = 0;                      // Current research progress
    bool is_active = false;                // Is currently being researched
    
    const Technology* getTechnology() const;
};

/**
 * @brief Manages active research for an organization
 */
class ResearchManager {
public:
    explicit ResearchManager(const TechTree& tech_tree);
    
    /**
     * @brief Add technology to research queue
     * @param tech_id Technology to research
     * @return True if successfully added
     */
    bool queueResearch(const std::string& tech_id);
    
    /**
     * @brief Remove technology from research queue
     */
    bool cancelResearch(const std::string& tech_id);
    
    /**
     * @brief Set active research slot
     * @param tech_id Technology to research actively
     * @param slot Slot number (0-based)
     */
    bool setActiveResearch(const std::string& tech_id, int slot);
    
    /**
     * @brief Process one tick of research
     * @param research_points Points available this tick
     * @param discovered_techs Set of already discovered techs (for completion check)
     * @return List of newly discovered technology IDs
     */
    std::vector<std::string> processTick(int research_points, std::set<std::string>& discovered_techs);
    
    /**
     * @brief Get current research queue
     */
    const std::vector<ResearchQueueItem>& getQueue() const { return queue_; }
    
    /**
     * @brief Get number of research slots
     */
    int getResearchSlots() const { return research_slots_; }
    
    /**
     * @brief Set number of research slots (based on facilities)
     */
    void setResearchSlots(int slots) { research_slots_ = slots; }
    
private:
    const TechTree& tech_tree_;
    std::vector<ResearchQueueItem> queue_;
    int research_slots_ = 1;
};

} // namespace hiersim

#endif // HIERSIM_TECHNOLOGY_H
