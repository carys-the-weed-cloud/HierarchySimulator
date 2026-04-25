#include "hiersim/Organization.h"
#include <sstream>
#include <algorithm>
#include <cmath>

namespace hiersim {

// Initialize static member
Organization::ID Organization::nextId_ = 1;

Organization::Organization(const std::string& name, StructureType type, ID id)
    : id_(id != 0 ? id : nextId_++)
    , name_(name)
    , type_(type)
    , democracyLevel_(0.5)  // Default to mixed democracy/autocracy
    , parent_(0)
    , treasury_(0.0)
    , active_(true)
{
    // Set default recruitment criteria
    recruitmentCriteria_ = RecruitmentCriteria{
        .minSkills = {},
        .requiredIdeology = "",
        .minWealth = 0.0,
        .maxWealth = std::numeric_limits<double>::max(),
        .activeOnly = true
    };
    
    if (id != 0 && id >= nextId_) {
        nextId_ = id + 1;
    }
}

Organization::~Organization() = default;

void Organization::addMember(ID individualId) {
    if (!hasMember(individualId)) {
        members_.push_back(individualId);
        memberRanks_[individualId] = 0;  // Default rank
    }
}

void Organization::removeMember(ID individualId) {
    auto it = std::find(members_.begin(), members_.end(), individualId);
    if (it != members_.end()) {
        members_.erase(it);
        memberRanks_.erase(individualId);
    }
}

bool Organization::hasMember(ID individualId) const {
    return std::find(members_.begin(), members_.end(), individualId) != members_.end();
}

void Organization::addSubordinate(ID orgId) {
    if (!isSubordinate(orgId)) {
        subordinates_.push_back(orgId);
    }
}

void Organization::removeSubordinate(ID orgId) {
    auto it = std::find(subordinates_.begin(), subordinates_.end(), orgId);
    if (it != subordinates_.end()) {
        subordinates_.erase(it);
    }
}

bool Organization::isSubordinate(ID orgId) const {
    return std::find(subordinates_.begin(), subordinates_.end(), orgId) != subordinates_.end();
}

void Organization::depositResources(double amount) {
    if (amount > 0) {
        treasury_ += amount;
    }
}

void Organization::withdrawResources(double amount) {
    if (amount > 0 && treasury_ >= amount) {
        treasury_ -= amount;
    }
}

bool Organization::canAfford(double amount) const {
    return treasury_ >= amount;
}

void Organization::distributeResources() {
    if (members_.empty() || treasury_ <= 0) {
        return;
    }
    
    // Distribution logic based on democracy level
    // In more democratic orgs, resources are distributed more equally
    // In more autocratic orgs, resources flow to higher ranks
    
    double perMemberShare = treasury_ / members_.size();
    
    if (democracyLevel_ >= 0.8) {
        // Highly democratic: equal distribution
        // In a real implementation, this would transfer to Individual objects
        treasury_ = 0;
    } else if (democracyLevel_ <= 0.2) {
        // Highly autocratic: leadership keeps most
        // Top 10% get 90% of resources
        size_t leadershipCount = std::max(size_t(1), members_.size() / 10);
        double leadershipShare = treasury_ * 0.9 / leadershipCount;
        // Implementation would distribute to high-ranking members
        treasury_ *= 0.1;  // Keep 10% in treasury
    } else {
        // Mixed: partial equal distribution
        double distributableAmount = treasury_ * democracyLevel_;
        treasury_ -= distributableAmount;
        // Implementation would distribute to members
    }
}

void Organization::setRecruitmentCriteria(const RecruitmentCriteria& criteria) {
    recruitmentCriteria_ = criteria;
}

std::string Organization::serialize() const {
    std::ostringstream oss;
    oss << id_ << "|" << name_ << "|" << static_cast<int>(type_) << "|" 
        << democracyLevel_ << "|" << treasury_ << "|" << (active_ ? "1" : "0");
    
    // Serialize members
    oss << "|members:";
    for (ID memberId : members_) {
        oss << memberId << ",";
    }
    
    // Serialize subordinates
    oss << "|subordinates:";
    for (ID subId : subordinates_) {
        oss << subId << ",";
    }
    
    // Serialize facilities
    oss << "|facilities:";
    for (ID facId : facilities_) {
        oss << facId << ",";
    }
    
    return oss.str();
}

Organization Organization::deserialize(const std::string& data) {
    Organization org("placeholder", StructureType::Hierarchy, 0);
    std::istringstream iss(data);
    
    char delim;
    int typeInt;
    iss >> org.id_ >> delim;
    std::getline(iss, org.name_, delim);
    iss >> typeInt >> delim >> org.democracyLevel_ >> delim >> org.treasury_ >> delim;
    
    std::string activeStr;
    std::getline(iss, activeStr, delim);
    org.active_ = (activeStr == "1");
    
    org.type_ = static_cast<StructureType>(typeInt);
    
    // Parse members, subordinates, facilities (simplified)
    // Full implementation would parse the sections after |members:, |subordinates:, |facilities:
    
    if (org.id_ >= nextId_) {
        nextId_ = org.id_ + 1;
    }
    
    return org;
}

} // namespace hiersim
