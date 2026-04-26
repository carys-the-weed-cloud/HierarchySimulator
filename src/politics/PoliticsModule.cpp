#include "hiersim/PoliticsModule.h"
#include "hiersim/Organization.h"
#include "hiersim/Random.h"
#include <algorithm>
#include <iostream>

namespace hiersim {

void PoliticsModule::registerIdeology(const std::string& name, const Ideology& ideology) {
    ideologies_[name] = ideology;
}

Ideology PoliticsModule::getIdeology(const std::string& name) const {
    auto it = ideologies_.find(name);
    if (it != ideologies_.end()) {
        return it->second;
    }
    // Return default centrism ideology if not found
    Ideology defaultIdeology;
    defaultIdeology.name = "Centrism";
    return defaultIdeology;
}

std::vector<std::string> PoliticsModule::getAllIdeologyNames() const {
    std::vector<std::string> names;
    for (const auto& [name, _] : ideologies_) {
        names.push_back(name);
    }
    return names;
}

void PoliticsModule::formCoalition(const std::string& coalitionName,
                                   const std::vector<std::string>& orgIds,
                                   const std::string& dominantIdeology) {
    Coalition coalition;
    coalition.id = "coalition_" + std::to_string(coalitions_.size());
    coalition.name = coalitionName;
    coalition.memberOrgIds = orgIds;
    coalition.dominantIdeology = dominantIdeology;
    coalition.stability = 1.0;
    
    coalitions_[coalition.id] = coalition;
}

void PoliticsModule::dissolveCoalition(const std::string& coalitionId) {
    coalitions_.erase(coalitionId);
}

Coalition* PoliticsModule::getCoalition(const std::string& coalitionId) {
    auto it = coalitions_.find(coalitionId);
    if (it != coalitions_.end()) {
        return &it->second;
    }
    return nullptr;
}

std::vector<Coalition*> PoliticsModule::getAllCoalitions() {
    std::vector<Coalition*> result;
    for (auto& [id, coalition] : coalitions_) {
        result.push_back(&coalition);
    }
    return result;
}

void PoliticsModule::proposePolicy(const std::string& policyId,
                                   const std::string& name,
                                   const std::string& description,
                                   double economicImpact,
                                   double socialImpact,
                                   const std::string& proposerCoalitionId) {
    Policy policy;
    policy.id = policyId;
    policy.name = name;
    policy.description = description;
    policy.economicImpact = economicImpact;
    policy.socialImpact = socialImpact;
    policy.active = false;
    
    policies_[policyId] = policy;
}

bool PoliticsModule::enactPolicy(const std::string& policyId, double supportThreshold) {
    auto it = policies_.find(policyId);
    if (it == policies_.end()) {
        return false;
    }
    
    Policy& policy = it->second;
    if (policy.active) {
        return false;  // Already active
    }
    
    // Calculate total support from all coalitions
    double totalSupport = 0.0;
    int coalitionCount = 0;
    
    for (auto& [id, coalition] : coalitions_) {
        double support = calculateCoalitionSupport(coalition, policy);
        totalSupport += support;
        coalitionCount++;
    }
    
    double averageSupport = coalitionCount > 0 ? totalSupport / coalitionCount : 0.0;
    
    if (averageSupport >= supportThreshold) {
        policy.active = true;
        return true;
    }
    
    return false;
}

void PoliticsModule::repealPolicy(const std::string& policyId) {
    auto it = policies_.find(policyId);
    if (it != policies_.end()) {
        it->second.active = false;
    }
}

Policy* PoliticsModule::getPolicy(const std::string& policyId) {
    auto it = policies_.find(policyId);
    if (it != policies_.end()) {
        return &it->second;
    }
    return nullptr;
}

std::vector<Policy*> PoliticsModule::getAllActivePolicies() {
    std::vector<Policy*> result;
    for (auto& [id, policy] : policies_) {
        if (policy.active) {
            result.push_back(&policy);
        }
    }
    return result;
}

std::vector<Policy*> PoliticsModule::getAllProposedPolicies() {
    std::vector<Policy*> result;
    for (auto& [id, policy] : policies_) {
        if (!policy.active) {
            result.push_back(&policy);
        }
    }
    return result;
}

double PoliticsModule::calculateCoalitionSupport(const Coalition& coalition, const Policy& policy) {
    // Simplified support calculation based on ideological alignment
    // In a full implementation, this would consider member organizations' interests
    
    Ideology coalitionIdeology = getIdeology(coalition.dominantIdeology);
    
    // Base support modified by policy impacts
    double baseSupport = 0.5;
    
    // Economic left/right alignment
    if (policy.economicImpact > 0) {
        // Left-leaning policy
        baseSupport += (coalitionIdeology.economicLeft - 0.5) * 0.5;
    } else {
        // Right-leaning policy
        baseSupport += (0.5 - coalitionIdeology.economicLeft) * 0.5;
    }
    
    // Authoritarian/libertarian alignment with social impact
    if (policy.socialImpact > 0) {
        baseSupport += (coalitionIdeology.authoritarian - 0.5) * 0.3;
    } else {
        baseSupport += (0.5 - coalitionIdeology.authoritarian) * 0.3;
    }
    
    // Add some randomness to represent political maneuvering
    baseSupport += (Random::getInstance().nextDouble(0.0, 1.0) - 0.5) * 0.2;
    
    return std::clamp(baseSupport, 0.0, 1.0);
}

void PoliticsModule::updateIdeologySpread() {
    // Simplified ideology spread mechanism
    // In full implementation, this would interact with Individuals and Organizations
    
    for (auto& [name, ideology] : ideologies_) {
        // Ideologies spread based on their appeal and current prevalence
        // This is a placeholder for more complex social dynamics
    }
}

void PoliticsModule::updateCoalitionStability() {
    for (auto& [id, coalition] : coalitions_) {
        // Calculate stability based on ideological coherence of members
        // Members with similar ideologies increase stability
        // Divergent ideologies decrease stability
        
        double coherence = 1.0;  // Placeholder
        coalition.stability = std::clamp(coherence, 0.0, 1.0);
        
        // Coalitions with very low stability may dissolve
        if (coalition.stability < 0.2) {
            // Could trigger dissolution or member defection
        }
    }
}

void PoliticsModule::checkRevolutionConditions() {
    // Check for revolution conditions:
    // - High inequality + radical ideology spread
    // - Low government legitimacy
    // - Economic crisis
    // - Military disloyalty
    
    // Placeholder: In full implementation, this would scan regions
    // and organizations for revolutionary potential
}

void PoliticsModule::triggerRevolution(const std::string& regionId, const std::string& rebelIdeology) {
    // Create rebel organization
    // Transfer control of facilities/units in region
    // Initiate conflict mechanics
    
    std::cout << "Revolution triggered in region " << regionId 
              << " under ideology " << rebelIdeology << std::endl;
}

void PoliticsModule::processTick() {
    updateIdeologySpread();
    updateCoalitionStability();
    checkRevolutionConditions();
}

} // namespace hiersim
