#include <iostream>
#include <cassert>
#include "hiersim/Individual.h"

using namespace hiersim;

void testIndividualCreation() {
    Individual ind("Test Person");
    assert(ind.getId() > 0);
    assert(ind.getName() == "Test Person");
    assert(ind.getAge() == 25);
    assert(ind.isAlive());
    std::cout << "[PASS] testIndividualCreation" << std::endl;
}

void testIndividualSkills() {
    Individual ind("Skill Test");
    ind.addSkill("Leadership", 0.8);
    ind.addSkill("Economics", 0.6);
    
    assert(ind.getSkill("Leadership") == 0.8);
    assert(ind.getSkill("Economics") == 0.6);
    assert(ind.getSkill("NonExistent") == 0.0);
    
    ind.removeSkill("Economics");
    assert(ind.getSkill("Economics") == 0.0);
    std::cout << "[PASS] testIndividualSkills" << std::endl;
}

void testIndividualRelationships() {
    Individual ind1("Person 1");
    Individual ind2("Person 2");
    
    ind1.addRelationship(ind2.getId(), 0.9);
    assert(ind1.getRelationshipStrength(ind2.getId()) == 0.9);
    
    ind1.removeRelationship(ind2.getId());
    assert(ind1.getRelationshipStrength(ind2.getId()) == 0.0);
    std::cout << "[PASS] testIndividualRelationships" << std::endl;
}

void testIndividualKnowledge() {
    Individual ind("Knowledgeable Person");
    ind.learn("Marxist Theory", 0.75);
    ind.learn("Keynesian Economics", 0.6);
    
    assert(ind.hasKnowledge("Marxist Theory"));
    assert(!ind.hasKnowledge("NonExistent"));
    assert(ind.getKnowledgeProficiency("Marxist Theory") == 0.75);
    std::cout << "[PASS] testIndividualKnowledge" << std::endl;
}

void testIndividualDecisions() {
    Individual ind("Decision Maker");
    
    Individual::Decision d1{"join_org", "Workers Union", 0.8, 1};
    Individual::Decision d2{"protest", "City Hall", 0.6, 1};
    
    ind.queueDecision(d1);
    ind.queueDecision(d2);
    
    assert(ind.hasPendingDecisions());
    
    // Highest utility should be executed first
    Individual::Decision executed = ind.executeNextDecision(1);
    assert(executed.type == "join_org");
    assert(executed.utility == 0.8);
    std::cout << "[PASS] testIndividualDecisions" << std::endl;
}

void testIndividualSerialization() {
    Individual ind("Serialize Test", 100);
    ind.setAge(35);
    ind.setWealth(50000.0);
    ind.setIdeology("Social Democracy");
    ind.addSkill("Programming", 0.9);
    
    std::string serialized = ind.serialize();
    Individual deserialized = Individual::deserialize(serialized);
    
    assert(deserialized.getId() == 100);
    assert(deserialized.getName() == "Serialize Test");
    assert(deserialized.getAge() == 35);
    assert(deserialized.getWealth() == 50000.0);
    assert(deserialized.getIdeology() == "Social Democracy");
    std::cout << "[PASS] testIndividualSerialization" << std::endl;
}

int main() {
    std::cout << "Running Individual Unit Tests..." << std::endl;
    
    testIndividualCreation();
    testIndividualSkills();
    testIndividualRelationships();
    testIndividualKnowledge();
    testIndividualDecisions();
    testIndividualSerialization();
    
    std::cout << "\nAll Individual tests passed!" << std::endl;
    return 0;
}
