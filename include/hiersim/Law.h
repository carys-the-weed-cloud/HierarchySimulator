#ifndef HIERSIM_LAW_H
#define HIERSIM_LAW_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <cstdint>

namespace hiersim {

/**
 * @brief Law categories
 */
enum class LawCategory {
    Economic,     // Tax, trade, labor laws
    Social,       // Civil rights, education, healthcare
    Political,    // Voting, governance, civil liberties
    Military,     // Conscription, defense policy
    Environmental // Conservation, pollution, resource use
};

/**
 * @brief Law enforcement level
 */
enum class EnforcementLevel {
    None,         // Law exists but not enforced
    Low,          // Minimal enforcement
    Moderate,     // Standard enforcement
    High,         // Strict enforcement
    Total         // Maximum enforcement (police state)
};

/**
 * @brief Represents a law in the simulation
 */
struct Law {
    std::string id;
    std::string name;
    std::string description;
    LawCategory category;
    
    // Legal requirements
    std::set<std::string> required_technologies;  // Techs needed to enact
    std::set<std::string> prohibited_ideologies;  // Ideologies that oppose this
    
    // Effects
    float economic_impact = 0.0f;      // Impact on economy (-1.0 to 1.0)
    float freedom_impact = 0.0f;       // Impact on civil liberties (-1.0 to 1.0)
    float stability_impact = 0.0f;     // Impact on political stability (-1.0 to 1.0)
    float popularity_impact = 0.0f;    // Impact on government popularity (-1.0 to 1.0)
    
    // Implementation
    int implementation_cost = 0;       // One-time cost to implement
    int maintenance_cost = 0;          // Ongoing cost per tick
    int enforcement_capacity_needed = 0; // Police/judicial capacity required
    
    // Status
    bool is_enacted = false;
    int64_t enactment_tick = -1;
    EnforcementLevel enforcement = EnforcementLevel::None;
    float compliance = 0.0f;           // Current compliance level (0.0-1.0)
    
    /**
     * @brief Calculate compliance based on enforcement and ideology alignment
     * @param enforcement_level Current enforcement level
     * @param ideology_alignment How well law aligns with dominant ideology (-1.0 to 1.0)
     * @param fear_factor Population fear of punishment (0.0-1.0)
     * @return Compliance level (0.0-1.0)
     */
    static float calculateCompliance(EnforcementLevel enforcement_level, 
                                     float ideology_alignment, 
                                     float fear_factor);
    
    /**
     * @brief Get string representation of category
     */
    static std::string categoryToString(LawCategory category);
    
    /**
     * @brief Get string representation of enforcement level
     */
    static std::string enforcementToString(EnforcementLevel level);
};

/**
 * @brief Manages laws and legal system for a region or organization
 */
class LegalSystem {
public:
    LegalSystem() = default;
    
    /**
     * @brief Initialize with default laws
     */
    void initializeDefaultLaws();
    
    /**
     * @brief Get law by ID
     */
    const Law* getLaw(const std::string& id) const;
    
    /**
     * @brief Get all laws in a category
     */
    std::vector<const Law*> getLawsByCategory(LawCategory category) const;
    
    /**
     * @brief Get enacted laws
     */
    std::vector<const Law*> getEnactedLaws() const;
    
    /**
     * @brief Get available laws (can be enacted)
     * @param available_techs Set of available technology IDs
     * @param dominant_ideology Current dominant ideology
     */
    std::vector<const Law*> getAvailableLaws(
        const std::set<std::string>& available_techs,
        const std::string& dominant_ideology) const;
    
    /**
     * @brief Attempt to enact a law
     * @param law_id Law to enact
     * @param current_tick Current simulation tick
     * @param has_resources Whether entity has resources to implement
     * @return True if successfully enacted
     */
    bool enactLaw(const std::string& law_id, int64_t current_tick, bool has_resources);
    
    /**
     * @brief Repeal a law
     * @param law_id Law to repeal
     * @return True if successfully repealed
     */
    bool repealLaw(const std::string& law_id);
    
    /**
     * @brief Set enforcement level for a law
     * @param law_id Law to modify
     * @param level New enforcement level
     * @return True if successfully updated
     */
    bool setEnforcementLevel(const std::string& law_id, EnforcementLevel level);
    
    /**
     * @brief Update compliance for all enacted laws
     * @param ideology_alignment Overall ideology alignment (-1.0 to 1.0)
     * @param fear_factor Population fear factor (0.0-1.0)
     * @param enforcement_capacity Available enforcement capacity
     */
    void updateCompliance(float ideology_alignment, float fear_factor, int enforcement_capacity);
    
    /**
     * @brief Get total maintenance cost of all enacted laws
     */
    int getTotalMaintenanceCost() const;
    
    /**
     * @brief Get total enforcement capacity needed
     */
    int getTotalEnforcementCapacityNeeded() const;
    
    /**
     * @brief Get average compliance across all enacted laws
     */
    float getAverageCompliance() const;
    
    /**
     * @brief Check if a law is enacted
     */
    bool isLawEnacted(const std::string& law_id) const;
    
    /**
     * @brief Get number of enacted laws
     */
    size_t getEnactedLawCount() const;
    
private:
    std::map<std::string, Law> laws_;
};

/**
 * @brief Legislative process for proposing and voting on laws
 */
class LegislativeProcess {
public:
    struct Proposal {
        std::string law_id;
        std::string proposer;              // Organization or coalition ID
        int64_t proposal_tick;
        std::map<std::string, bool> votes; // Voter ID -> vote (true=support, false=oppose)
        bool is_passed = false;
        bool is_rejected = false;
    };
    
    /**
     * @brief Propose a new law
     * @param law_id Law to propose
     * @param proposer Entity proposing the law
     * @param current_tick Current tick
     * @return Proposal ID or empty if failed
     */
    std::string proposeLaw(const std::string& law_id, 
                          const std::string& proposer,
                          int64_t current_tick);
    
    /**
     * @brief Cast a vote on a proposal
     * @param proposal_id Proposal to vote on
     * @param voter_id Entity voting
     * @param support True for support, false for oppose
     * @return True if vote recorded
     */
    bool castVote(const std::string& proposal_id, 
                  const std::string& voter_id, 
                  bool support);
    
    /**
     * @brief Process voting results and enact/reject proposals
     * @param legal_system Legal system to enact laws in
     * @param current_tick Current tick
     * @param has_resources Function to check if entity has resources
     * @return List of newly enacted law IDs
     */
    std::vector<std::string> processVotes(
        LegalSystem& legal_system,
        int64_t current_tick,
        std::function<bool(int)> has_resources);
    
    /**
     * @brief Get active proposals
     */
    const std::map<std::string, Proposal>& getProposals() const { return proposals_; }
    
    /**
     * @brief Clear old processed proposals
     * @param ticks_to_keep Number of ticks to keep history for
     */
    void clearOldProposals(int64_t current_tick, int ticks_to_keep = 10);
    
private:
    std::map<std::string, Proposal> proposals_;
    int next_proposal_id_ = 1;
};

} // namespace hiersim

#endif // HIERSIM_LAW_H
