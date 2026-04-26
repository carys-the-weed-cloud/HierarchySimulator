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
        .id = "private_property",
        .name = "Private Property Rights",
        .description = "Legal protection of private ownership",
        .category = LawCategory::Economic,
        .economic_impact = 0.2f,
        .freedom_impact = 0.1f,
        .stability_impact = 0.1f,
        .implementation_cost = 10,
        .enforcement_capacity_needed = 5
    };
    
    laws_["progressive_taxation"] = Law{
        .id = "progressive_taxation",
        .name = "Progressive Taxation",
        .description = "Tax rates increase with income",
        .category = LawCategory::Economic,
        .economic_impact = -0.1f,
        .freedom_impact = -0.1f,
        .popularity_impact = 0.2f,
        .implementation_cost = 20,
        .maintenance_cost = 5,
        .enforcement_capacity_needed = 10
    };
    
    laws_["minimum_wage"] = Law{
        .id = "minimum_wage",
        .name = "Minimum Wage Law",
        .description = "Legal minimum hourly wage",
        .category = LawCategory::Economic,
        .economic_impact = -0.05f,
        .freedom_impact = -0.1f,
        .popularity_impact = 0.15f,
        .stability_impact = 0.1f,
        .implementation_cost = 15,
        .maintenance_cost = 3,
        .enforcement_capacity_needed = 8
    };
    
    laws_["universal_basic_income"] = Law{
        .id = "universal_basic_income",
        .name = "Universal Basic Income",
        .description = "Guaranteed income for all citizens",
        .category = LawCategory::Economic,
        .economic_impact = -0.2f,
        .freedom_impact = 0.2f,
        .popularity_impact = 0.3f,
        .stability_impact = 0.2f,
        .implementation_cost = 50,
        .maintenance_cost = 100,
        .required_technologies = {"basic_computing"}
    };
    
    // Social laws
    laws_["free_education"] = Law{
        .id = "free_education",
        .name = "Free Public Education",
        .description = "Government-funded education for all",
        .category = LawCategory::Social,
        .economic_impact = -0.1f,
        .freedom_impact = 0.1f,
        .popularity_impact = 0.2f,
        .stability_impact = 0.1f,
        .implementation_cost = 40,
        .maintenance_cost = 30,
        .enforcement_capacity_needed = 5
    };
    
    laws_["universal_healthcare"] = Law{
        .id = "universal_healthcare",
        .name = "Universal Healthcare",
        .description = "Government-provided healthcare for all",
        .category = LawCategory::Social,
        .economic_impact = -0.15f,
        .freedom_impact = 0.1f,
        .popularity_impact = 0.25f,
        .stability_impact = 0.15f,
        .implementation_cost = 60,
        .maintenance_cost = 50,
        .required_technologies = {"vaccination"}
    };
    
    laws_["civil_rights"] = Law{
        .id = "civil_rights",
        .name = "Civil Rights Act",
        .description = "Protection against discrimination",
        .category = LawCategory::Social,
        .freedom_impact = 0.2f,
        .popularity_impact = 0.15f,
        .stability_impact = 0.1f,
        .implementation_cost = 20,
        .maintenance_cost = 5,
        .enforcement_capacity_needed = 10,
        .prohibited_ideologies = {"fascism"}
    };
    
    // Political laws
    laws_["universal_suffrage"] = Law{
        .id = "universal_suffrage",
        .name = "Universal Suffrage",
        .description = "Voting rights for all adult citizens",
        .category = LawCategory::Political,
        .freedom_impact = 0.2f,
        .popularity_impact = 0.2f,
        .stability_impact = 0.1f,
        .implementation_cost = 15,
        .enforcement_capacity_needed = 5
    };
    
    laws_["freedom_of_press"] = Law{
        .id = "freedom_of_press",
        .name = "Freedom of the Press",
        .description = "Protection of media independence",
        .category = LawCategory::Political,
        .freedom_impact = 0.25f,
        .stability_impact = -0.1f,
        .implementation_cost = 10,
        .prohibited_ideologies = {"fascism", "totalitarian_communism"}
    };
    
    // Military laws
    laws_["conscription"] = Law{
        .id = "conscription",
        .name = "Military Conscription",
        .description = "Mandatory military service",
        .category = LawCategory::Military,
        .economic_impact = -0.1f,
        .freedom_impact = -0.3f,
        .stability_impact = 0.1f,
        .popularity_impact = -0.2f,
        .implementation_cost = 20,
        .maintenance_cost = 10,
        .enforcement_capacity_needed = 15
    };
    
    laws_["defense_budget_increase"] = Law{
        .id = "defense_budget_increase",
        .name = "Defense Budget Increase",
        .description = "Increased military spending",
        .category = LawCategory::Military,
        .economic_impact = -0.15f,
        .stability_impact = 0.15f,
        .popularity_impact = 0.1f,
        .maintenance_cost = 40,
        .implementation_cost = 30
    };
    
    // Environmental laws
    laws_["carbon_tax"] = Law{
        .id = "carbon_tax",
        .name = "Carbon Tax",
        .description = "Tax on carbon emissions",
        .category = LawCategory::Environmental,
        .economic_impact = -0.1f,
        .freedom_impact = -0.05f,
        .popularity_impact = 0.1f,
        .implementation_cost = 25,
        .maintenance_cost = 5,
        .enforcement_capacity_needed = 10,
        .required_technologies = {"electricity"}
    };
    
    laws_["renewable_energy_subsidy"] = Law{
        .id = "renewable_energy_subsidy",
        .name = "Renewable Energy Subsidies",
        .description = "Government support for clean energy",
        .category = LawCategory::Environmental,
        .economic_impact = -0.1f,
        .popularity_impact = 0.15f,
        .implementation_cost = 40,
        .maintenance_cost = 20,
        .required_technologies = {"nuclear_power"}
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
