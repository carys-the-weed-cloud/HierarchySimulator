#pragma once
#include "hiersim/Ideology.h"
#include "hiersim/Policy.h"
#include "hiersim/Coalition.h"
#include "hiersim/Organization.h"
#include <vector>
#include <unordered_map>
#include <string>

namespace hiersim {

class PoliticsModule {
public:
    // Ideology management
    void registerIdeology(const std::string& name, const Ideology& ideology);
    Ideology getIdeology(const std::string& name) const;
    std::vector<std::string> getAllIdeologyNames() const;
    
    // Coalition formation and management
    void formCoalition(const std::string& coalitionName, 
                       const std::vector<std::string>& orgIds,
                       const std::string& dominantIdeology);
    void dissolveCoalition(const std::string& coalitionId);
    Coalition* getCoalition(const std::string& coalitionId);
    std::vector<Coalition*> getAllCoalitions();
    
    // Policy system
    void proposePolicy(const std::string& policyId, 
                       const std::string& name,
                       const std::string& description,
                       double economicImpact,
                       double socialImpact,
                       const std::string& proposerCoalitionId);
    bool enactPolicy(const std::string& policyId, double supportThreshold = 0.5);
    void repealPolicy(const std::string& policyId);
    Policy* getPolicy(const std::string& policyId);
    std::vector<Policy*> getAllActivePolicies();
    std::vector<Policy*> getAllProposedPolicies();
    
    // Political dynamics
    void updateIdeologySpread();  // Spread ideologies through population
    void updateCoalitionStability();  // Calculate coalition stability based on member alignment
    void checkRevolutionConditions();  // Check if revolution/rebellion conditions are met
    
    // Tick processing
    void processTick();
    
private:
    std::unordered_map<std::string, Ideology> ideologies_;
    std::unordered_map<std::string, Coalition> coalitions_;
    std::unordered_map<std::string, Policy> policies_;
    
    // Helper methods
    double calculateCoalitionSupport(const Coalition& coalition, const Policy& policy);
    void triggerRevolution(const std::string& regionId, const std::string& rebelIdeology);
};

} // namespace hiersim
