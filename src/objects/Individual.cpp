#include "hiersim/Individual.h"
#include <sstream>
#include <algorithm>

namespace hiersim {

// Initialize static member
Individual::ID Individual::nextId_ = 1;

Individual::Individual(const std::string& name, ID id)
    : id_(id != 0 ? id : nextId_++)
    , name_(name)
    , age_(25)
    , wealth_(0.0)
    , ideology_("none")
    , organization_(nullptr)
    , facility_(nullptr)
    , alive_(true)
{
    if (id != 0 && id >= nextId_) {
        nextId_ = id + 1;
    }
}

Individual::~Individual() = default;

void Individual::addSkill(const std::string& skill, double value) {
    skills_[skill] = value;
}

void Individual::removeSkill(const std::string& skill) {
    skills_.erase(skill);
}

double Individual::getSkill(const std::string& skill) const {
    auto it = skills_.find(skill);
    return (it != skills_.end()) ? it->second : 0.0;
}

void Individual::addRelationship(ID individualId, double strength) {
    relationships_[individualId] = strength;
}

void Individual::removeRelationship(ID individualId) {
    relationships_.erase(individualId);
}

double Individual::getRelationshipStrength(ID individualId) const {
    auto it = relationships_.find(individualId);
    return (it != relationships_.end()) ? it->second : 0.0;
}

void Individual::learn(const std::string& knowledge, double proficiency) {
    knowledge_[knowledge] = proficiency;
}

bool Individual::hasKnowledge(const std::string& knowledge) const {
    return knowledge_.find(knowledge) != knowledge_.end();
}

double Individual::getKnowledgeProficiency(const std::string& knowledge) const {
    auto it = knowledge_.find(knowledge);
    return (it != knowledge_.end()) ? it->second : 0.0;
}

void Individual::queueDecision(const Decision& decision) {
    decisionQueue_.push_back(decision);
    // Sort by utility (highest first)
    std::sort(decisionQueue_.begin(), decisionQueue_.end(),
        [](const Decision& a, const Decision& b) {
            return a.utility > b.utility;
        });
}

Individual::Decision Individual::executeNextDecision(int64_t currentTick) {
    if (decisionQueue_.empty()) {
        return Decision{"", "", 0.0, -1};
    }
    
    Decision decision = decisionQueue_.front();
    decisionQueue_.erase(decisionQueue_.begin());
    return decision;
}

std::string Individual::serialize() const {
    std::ostringstream oss;
    oss << id_ << "|" << name_ << "|" << age_ << "|" << wealth_ << "|" 
        << ideology_ << "|" << (alive_ ? "1" : "0");
    
    // Serialize skills
    oss << "|skills:";
    for (const auto& [skill, value] : skills_) {
        oss << skill << "=" << value << ",";
    }
    
    // Serialize relationships
    oss << "|relationships:";
    for (const auto& [id, strength] : relationships_) {
        oss << id << "=" << strength << ",";
    }
    
    // Serialize knowledge
    oss << "|knowledge:";
    for (const auto& [know, prof] : knowledge_) {
        oss << know << "=" << prof << ",";
    }
    
    return oss.str();
}

Individual Individual::deserialize(const std::string& data) {
    Individual ind("placeholder", 0);
    std::istringstream iss(data);
    
    char delim;
    iss >> ind.id_ >> delim;
    std::getline(iss, ind.name_, delim);
    iss >> ind.age_ >> delim >> ind.wealth_ >> delim;
    std::getline(iss, ind.ideology_, delim);
    
    std::string aliveStr;
    std::getline(iss, aliveStr, delim);
    ind.alive_ = (aliveStr == "1");
    
    // Parse skills, relationships, knowledge (simplified)
    // Full implementation would parse the sections after |skills:, |relationships:, |knowledge:
    
    if (ind.id_ >= nextId_) {
        nextId_ = ind.id_ + 1;
    }
    
    return ind;
}

} // namespace hiersim
