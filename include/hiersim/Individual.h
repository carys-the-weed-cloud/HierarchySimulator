#ifndef HIERSIM_INDIVIDUAL_H
#define HIERSIM_INDIVIDUAL_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cstdint>

namespace hiersim {

// Forward declarations
class Organization;
class Facility;

/**
 * @brief Represents an individual agent in the simulation.
 * 
 * Individuals are people significant enough to be simulated as distinct agents
 * rather than part of the aggregate population. These are key figures—leaders,
 * innovators, organizers—who drive change.
 */
class Individual {
public:
    // Unique identifier for this individual
    using ID = uint64_t;
    
    /**
     * @brief Construct a new Individual object
     * @param name The name of the individual
     * @param id Unique identifier (auto-generated if not provided)
     */
    explicit Individual(const std::string& name = "", ID id = 0);
    
    /**
     * @brief Destroy the Individual object
     */
    ~Individual();
    
    // Getters
    ID getId() const { return id_; }
    const std::string& getName() const { return name_; }
    int getAge() const { return age_; }
    double getWealth() const { return wealth_; }
    const std::string& getIdeology() const { return ideology_; }
    const std::map<std::string, double>& getSkills() const { return skills_; }
    const std::map<ID, double>& getRelationships() const { return relationships_; }
    Organization* getOrganization() const { return organization_; }
    Facility* getFacility() const { return facility_; }
    bool isAlive() const { return alive_; }
    
    // Setters
    void setName(const std::string& name) { name_ = name; }
    void setAge(int age) { age_ = age; }
    void setWealth(double wealth) { wealth_ = wealth; }
    void setIdeology(const std::string& ideology) { ideology_ = ideology; }
    void setOrganization(Organization* org) { organization_ = org; }
    void setFacility(Facility* facility) { facility_ = facility; }
    void setAlive(bool alive) { alive_ = alive; }
    
    // Skill management
    void addSkill(const std::string& skill, double value);
    void removeSkill(const std::string& skill);
    double getSkill(const std::string& skill) const;
    
    // Relationship management
    void addRelationship(ID individualId, double strength = 1.0);
    void removeRelationship(ID individualId);
    double getRelationshipStrength(ID individualId) const;
    
    // Memory and learning
    void learn(const std::string& knowledge, double proficiency);
    bool hasKnowledge(const std::string& knowledge) const;
    double getKnowledgeProficiency(const std::string& knowledge) const;
    
    // Decision making
    struct Decision {
        std::string type;
        std::string target;
        double utility;
        int64_t tickScheduled;
    };
    
    void queueDecision(const Decision& decision);
    Decision executeNextDecision(int64_t currentTick);
    bool hasPendingDecisions() const { return !decisionQueue_.empty(); }
    
    // Serialization
    std::string serialize() const;
    static Individual deserialize(const std::string& data);
    
private:
    ID id_;
    std::string name_;
    int age_;
    double wealth_;
    std::string ideology_;
    std::map<std::string, double> skills_;
    std::map<ID, double> relationships_;  // individual ID -> relationship strength
    std::map<std::string, double> knowledge_;  // knowledge -> proficiency
    std::vector<Decision> decisionQueue_;
    Organization* organization_;
    Facility* facility_;
    bool alive_;
    
    static ID nextId_;
};

} // namespace hiersim

#endif // HIERSIM_INDIVIDUAL_H
