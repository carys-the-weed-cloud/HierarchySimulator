#include "hiersim/Law.h"
#include <algorithm>
#include <cmath>

namespace hiersim {

// Law implementation

float Law::calculateCompliance(EnforcementLevel enforcement_level,
                               float ideology_alignment,
                               float fear_factor) {
    // Base compliance from ideology alignment (50% weight)
    float ideology_compliance = 0.5f + (ideology_alignment * 0.5f); // Maps -1..1 to 0..1
    
    // Enforcement contribution (30% weight)
    float enforcement_factor = 0.0f;
    switch (enforcement_level) {
        case EnforcementLevel::None:
            enforcement_factor = 0.0f;
            break;
        case EnforcementLevel::Low:
            enforcement_factor = 0.25f;
            break;
        case EnforcementLevel::Moderate:
            enforcement_factor = 0.5f;
            break;
        case EnforcementLevel::High:
            enforcement_factor = 0.75f;
            break;
        case EnforcementLevel::Total:
            enforcement_factor = 1.0f;
            break;
    }
    
    // Fear factor amplifies enforcement effect
    float enforcement_compliance = enforcement_factor * fear_factor;
    
    // Combine: 50% ideology, 30% enforcement, 20% base
    float compliance = (0.5f * ideology_compliance) + 
                       (0.3f * enforcement_compliance) + 
                       0.2f;
    
    return std::clamp(compliance, 0.0f, 1.0f);
}

std::string Law::categoryToString(LawCategory category) {
    switch (category) {
        case LawCategory::Economic: return "Economic";
        case LawCategory::Social: return "Social";
        case LawCategory::Political: return "Political";
        case LawCategory::Military: return "Military";
        case LawCategory::Environmental: return "Environmental";
        default: return "Unknown";
    }
}

std::string Law::enforcementToString(EnforcementLevel level) {
    switch (level) {
        case EnforcementLevel::None: return "Not Enforced";
        case EnforcementLevel::Low: return "Low Enforcement";
        case EnforcementLevel::Moderate: return "Moderate Enforcement";
        case EnforcementLevel::High: return "High Enforcement";
        case EnforcementLevel::Total: return "Total Enforcement";
        default: return "Unknown";
    }
}

// LegalSystem implementation

void LegalSystem::initializeDefaultLaws() {
    // Economic laws
    laws_["private_property"] = Law{
        "private_property",
        "Private Property Rights",
        "Legal protection of private ownership",
        LawCategory::Economic,
        {},  // required_technologies
        {},  // prohibited_ideologies
        0.2f,  // economic_impact
        0.1f,  // freedom_impact
        0.1f,  // stability_impact
        10,    // implementation_cost
        0,     // maintenance_cost
        5      // enforcement_capacity_needed
    };
    
    laws_["progressive_taxation"] = Law{
        "progressive_taxation",
        "Progressive Taxation",
        "Tax rates increase with income",
        LawCategory::Economic,
        {},
        {},
        -0.1f,
        -0.1f,
        0.2f,   // popularity_impact
        20,     // implementation_cost
        5,      // maintenance_cost
        10      // enforcement_capacity_needed
    };
    
    laws_["minimum_wage"] = Law{
        "minimum_wage",
        "Minimum Wage Law",
        "Legal minimum hourly wage",
        LawCategory::Economic,
        {},
        {},
        -0.05f,
        -0.1f,
        0.1f,
        0.15f,
        15,
        3,
        8
    };
    
    laws_["universal_basic_income"] = Law{
        "universal_basic_income",
        "Universal Basic Income",
        "Guaranteed income for all citizens",
        LawCategory::Economic,
        {"basic_computing"},
        {},
        -0.2f,
        0.2f,
        0.2f,
        0.3f,
        50,
        100,
        0
    };
    
    // Social laws
    laws_["free_education"] = Law{
        "free_education",
        "Free Public Education",
        "Government-funded education for all",
        LawCategory::Social,
        {},
        {},
        -0.1f,
        0.1f,
        0.1f,
        0.2f,
        40,
        30,
        5
    };
    
    laws_["universal_healthcare"] = Law{
        "universal_healthcare",
        "Universal Healthcare",
        "Government-provided healthcare for all",
        LawCategory::Social,
        {"vaccination"},
        {},
        -0.15f,
        0.1f,
        0.15f,
        0.25f,
        60,
        50,
        0
    };
    
    laws_["civil_rights"] = Law{
        "civil_rights",
        "Civil Rights Act",
        "Protection against discrimination",
        LawCategory::Social,
        {},
        {"fascism"},
        0.0f,
        0.2f,
        0.1f,
        0.15f,
        20,
        5,
        10
    };
    
    // Political laws
    laws_["universal_suffrage"] = Law{
        "universal_suffrage",
        "Universal Suffrage",
        "Voting rights for all adult citizens",
        LawCategory::Political,
        {},
        {},
        0.0f,
        0.2f,
        0.1f,
        0.2f,
        15,
        0,
        5
    };
    
    laws_["freedom_of_press"] = Law{
        "freedom_of_press",
        "Freedom of the Press",
        "Protection of media independence",
        LawCategory::Political,
        {},
        {"fascism", "totalitarian_communism"},
        0.0f,
        0.25f,
        -0.1f,
        0.0f,
        10,
        0,
        0
    };
    
    // Military laws
    laws_["conscription"] = Law{
        "conscription",
        "Military Conscription",
        "Mandatory military service",
        LawCategory::Military,
        {},
        {},
        -0.1f,
        -0.3f,
        0.1f,
        -0.2f,
        20,
        10,
        15
    };
    
    laws_["defense_budget_increase"] = Law{
        "defense_budget_increase",
        "Defense Budget Increase",
        "Increased military spending",
        LawCategory::Military,
        {},
        {},
        -0.15f,
        0.0f,
        0.15f,
        0.1f,
        30,
        40,
        0
    };
    
    // Environmental laws
    laws_["carbon_tax"] = Law{
        "carbon_tax",
        "Carbon Tax",
        "Tax on carbon emissions",
        LawCategory::Environmental,
        {"electricity"},
        {},
        -0.1f,
        -0.05f,
        0.0f,
        0.1f,
        25,
        5,
        10
    };
    
    laws_["renewable_energy_subsidy"] = Law{
        "renewable_energy_subsidy",
        "Renewable Energy Subsidies",
        "Government support for clean energy",
        LawCategory::Environmental,
        {"nuclear_power"},
        {},
        -0.1f,
        0.0f,
        0.0f,
        0.15f,
        40,
        20,
        0
    };
}

const Law* LegalSystem::getLaw(const std::string& id) const {
    auto it = laws_.find(id);
    return (it != laws_.end()) ? &it->second : nullptr;
}

std::vector<const Law*> LegalSystem::getLawsByCategory(LawCategory category) const {
    std::vector<const Law*> result;
    for (const auto& [id, law] : laws_) {
        if (law.category == category) {
            result.push_back(&law);
        }
    }
    return result;
}

std::vector<const Law*> LegalSystem::getEnactedLaws() const {
    std::vector<const Law*> result;
    for (const auto& [id, law] : laws_) {
        if (law.is_enacted) {
            result.push_back(&law);
        }
    }
    return result;
}

std::vector<const Law*> LegalSystem::getAvailableLaws(
    const std::set<std::string>& available_techs,
    const std::string& dominant_ideology) const {
    
    std::vector<const Law*> result;
    for (const auto& [id, law] : laws_) {
        if (law.is_enacted) {
            continue; // Already enacted
        }
        
        // Check technology requirements
        bool has_techs = true;
        for (const auto& req_tech : law.required_technologies) {
            if (available_techs.find(req_tech) == available_techs.end()) {
                has_techs = false;
                break;
            }
        }
        if (!has_techs) {
            continue;
        }
        
        // Check ideology restrictions
        if (law.prohibited_ideologies.find(dominant_ideology) != law.prohibited_ideologies.end()) {
            continue;
        }
        
        result.push_back(&law);
    }
    return result;
}

bool LegalSystem::enactLaw(const std::string& law_id, int64_t current_tick, bool has_resources) {
    auto it = laws_.find(law_id);
    if (it == laws_.end() || it->second.is_enacted) {
        return false;
    }
    
    if (!has_resources && it->second.implementation_cost > 0) {
        return false;
    }
    
    it->second.is_enacted = true;
    it->second.enactment_tick = current_tick;
    it->second.enforcement = EnforcementLevel::Moderate; // Default enforcement
    it->second.compliance = 0.5f; // Start with moderate compliance
    
    return true;
}

bool LegalSystem::repealLaw(const std::string& law_id) {
    auto it = laws_.find(law_id);
    if (it == laws_.end() || !it->second.is_enacted) {
        return false;
    }
    
    it->second.is_enacted = false;
    it->second.enactment_tick = -1;
    it->second.enforcement = EnforcementLevel::None;
    it->second.compliance = 0.0f;
    
    return true;
}

bool LegalSystem::setEnforcementLevel(const std::string& law_id, EnforcementLevel level) {
    auto it = laws_.find(law_id);
    if (it == laws_.end() || !it->second.is_enacted) {
        return false;
    }
    
    it->second.enforcement = level;
    return true;
}

void LegalSystem::updateCompliance(float ideology_alignment, float fear_factor, int enforcement_capacity) {
    int remaining_capacity = enforcement_capacity;
    
    // First pass: calculate desired enforcement levels
    std::vector<std::pair<std::string, int>> law_needs;
    for (auto& [id, law] : laws_) {
        if (law.is_enacted) {
            law_needs.push_back({id, law.enforcement_capacity_needed});
        }
    }
    
    // Sort by enforcement level priority (higher enforcement first)
    std::sort(law_needs.begin(), law_needs.end(),
        [this](const auto& a, const auto& b) {
            const Law* law_a = getLaw(a.first);
            const Law* law_b = getLaw(b.first);
            return (law_a ? static_cast<int>(law_a->enforcement) : 0) >
                   (law_b ? static_cast<int>(law_b->enforcement) : 0);
        });
    
    // Second pass: update compliance based on available capacity
    for (const auto& [id, needed] : law_needs) {
        auto it = laws_.find(id);
        if (it == laws_.end() || !it->second.is_enacted) {
            continue;
        }
        
        Law& law = it->second;
        
        // Determine effective enforcement based on capacity
        EnforcementLevel effective_enforcement = law.enforcement;
        if (remaining_capacity < needed) {
            // Reduce enforcement level if not enough capacity
            if (remaining_capacity <= 0) {
                effective_enforcement = EnforcementLevel::None;
            } else if (remaining_capacity < needed / 4) {
                effective_enforcement = EnforcementLevel::Low;
            } else if (remaining_capacity < needed / 2) {
                effective_enforcement = EnforcementLevel::Moderate;
            } else if (remaining_capacity < needed) {
                effective_enforcement = EnforcementLevel::High;
            }
        }
        
        law.compliance = Law::calculateCompliance(effective_enforcement, ideology_alignment, fear_factor);
        
        if (remaining_capacity >= needed) {
            remaining_capacity -= needed;
        } else {
            remaining_capacity = 0;
        }
    }
}

int LegalSystem::getTotalMaintenanceCost() const {
    int total = 0;
    for (const auto& [id, law] : laws_) {
        if (law.is_enacted) {
            total += law.maintenance_cost;
        }
    }
    return total;
}

int LegalSystem::getTotalEnforcementCapacityNeeded() const {
    int total = 0;
    for (const auto& [id, law] : laws_) {
        if (law.is_enacted) {
            total += law.enforcement_capacity_needed;
        }
    }
    return total;
}

float LegalSystem::getAverageCompliance() const {
    int count = 0;
    float total = 0.0f;
    for (const auto& [id, law] : laws_) {
        if (law.is_enacted) {
            total += law.compliance;
            count++;
        }
    }
    return (count > 0) ? (total / static_cast<float>(count)) : 0.0f;
}

bool LegalSystem::isLawEnacted(const std::string& law_id) const {
    auto it = laws_.find(law_id);
    return (it != laws_.end() && it->second.is_enacted);
}

size_t LegalSystem::getEnactedLawCount() const {
    size_t count = 0;
    for (const auto& [id, law] : laws_) {
        if (law.is_enacted) {
            count++;
        }
    }
    return count;
}

// LegislativeProcess implementation

std::string LegislativeProcess::proposeLaw(const std::string& law_id,
                                           const std::string& proposer,
                                           int64_t current_tick) {
    std::string proposal_id = "proposal_" + std::to_string(next_proposal_id_++);
    
    Proposal proposal;
    proposal.law_id = law_id;
    proposal.proposer = proposer;
    proposal.proposal_tick = current_tick;
    proposal.is_passed = false;
    proposal.is_rejected = false;
    
    proposals_[proposal_id] = proposal;
    return proposal_id;
}

bool LegislativeProcess::castVote(const std::string& proposal_id,
                                  const std::string& voter_id,
                                  bool support) {
    auto it = proposals_.find(proposal_id);
    if (it == proposals_.end() || it->second.is_passed || it->second.is_rejected) {
        return false;
    }
    
    it->second.votes[voter_id] = support;
    return true;
}

std::vector<std::string> LegislativeProcess::processVotes(
    LegalSystem& legal_system,
    int64_t current_tick,
    std::function<bool(int)> has_resources) {
    
    std::vector<std::string> enacted_laws;
    
    for (auto& [id, proposal] : proposals_) {
        if (proposal.is_passed || proposal.is_rejected) {
            continue;
        }
        
        // Count votes
        int support_count = 0;
        int oppose_count = 0;
        for (const auto& [voter, vote] : proposal.votes) {
            if (vote) {
                support_count++;
            } else {
                oppose_count++;
            }
        }
        
        // Simple majority rule
        int total_votes = support_count + oppose_count;
        if (total_votes == 0) {
            continue; // No votes yet
        }
        
        if (support_count > oppose_count) {
            // Law passed!
            proposal.is_passed = true;
            
            // Try to enact
            int cost = 0;
            if (const Law* law = legal_system.getLaw(proposal.law_id)) {
                cost = law->implementation_cost;
            }
            
            if (legal_system.enactLaw(proposal.law_id, current_tick, has_resources(cost))) {
                enacted_laws.push_back(proposal.law_id);
            }
        } else {
            proposal.is_rejected = true;
        }
    }
    
    return enacted_laws;
}

void LegislativeProcess::clearOldProposals(int64_t current_tick, int ticks_to_keep) {
    for (auto it = proposals_.begin(); it != proposals_.end(); ) {
        if (current_tick - it->second.proposal_tick > ticks_to_keep) {
            it = proposals_.erase(it);
        } else {
            ++it;
        }
    }
}

} // namespace hiersim
