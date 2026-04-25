#include <iostream>
#include <cassert>
#include "hiersim/Organization.h"

using namespace hiersim;

void testOrganizationCreation() {
    Organization org("Test Corporation", Organization::StructureType::Corporation);
    assert(org.getId() > 0);
    assert(org.getName() == "Test Corporation");
    assert(org.getType() == Organization::StructureType::Corporation);
    assert(org.isActive());
    std::cout << "[PASS] testOrganizationCreation" << std::endl;
}

void testOrganizationDemocracy() {
    Organization org("Democratic Org", Organization::StructureType::Cooperative);
    
    org.setDemocracyLevel(0.9);
    assert(org.getDemocracyLevel() == 0.9);
    
    // Test clamping
    org.setDemocracyLevel(1.5);
    assert(org.getDemocracyLevel() == 1.0);
    
    org.setDemocracyLevel(-0.2);
    assert(org.getDemocracyLevel() == 0.0);
    std::cout << "[PASS] testOrganizationDemocracy" << std::endl;
}

void testOrganizationMembership() {
    Organization org("Members Test");
    
    org.addMember(101);
    org.addMember(102);
    org.addMember(103);
    
    assert(org.hasMember(101));
    assert(org.hasMember(102));
    assert(!org.hasMember(999));
    assert(org.getMemberCount() == 3);
    
    org.removeMember(102);
    assert(!org.hasMember(102));
    assert(org.getMemberCount() == 2);
    std::cout << "[PASS] testOrganizationMembership" << std::endl;
}

void testOrganizationHierarchy() {
    Organization parent("Parent Org");
    Organization child1("Child 1");
    Organization child2("Child 2");
    
    parent.addSubordinate(child1.getId());
    parent.addSubordinate(child2.getId());
    
    assert(parent.isSubordinate(child1.getId()));
    assert(parent.isSubordinate(child2.getId()));
    assert(parent.getSubordinates().size() == 2);
    
    child1.setParent(parent.getId());
    assert(child1.getParent() == parent.getId());
    
    parent.removeSubordinate(child1.getId());
    assert(!parent.isSubordinate(child1.getId()));
    std::cout << "[PASS] testOrganizationHierarchy" << std::endl;
}

void testOrganizationResources() {
    Organization org("Resource Test");
    
    org.depositResources(1000.0);
    assert(org.getTreasury() == 1000.0);
    assert(org.canAfford(500.0));
    assert(!org.canAfford(1500.0));
    
    org.withdrawResources(300.0);
    assert(org.getTreasury() == 700.0);
    
    org.withdrawResources(800.0);  // Should fail, insufficient funds
    assert(org.getTreasury() == 700.0);
    std::cout << "[PASS] testOrganizationResources" << std::endl;
}

void testOrganizationSerialization() {
    Organization org("Serialize Org", Organization::StructureType::Union, 200);
    org.setDemocracyLevel(0.75);
    org.setTreasury(50000.0);
    org.addMember(101);
    org.addMember(102);
    
    std::string serialized = org.serialize();
    Organization deserialized = Organization::deserialize(serialized);
    
    assert(deserialized.getId() == 200);
    assert(deserialized.getName() == "Serialize Org");
    assert(deserialized.getTreasury() == 50000.0);
    std::cout << "[PASS] testOrganizationSerialization" << std::endl;
}

int main() {
    std::cout << "Running Organization Unit Tests..." << std::endl;
    
    testOrganizationCreation();
    testOrganizationDemocracy();
    testOrganizationMembership();
    testOrganizationHierarchy();
    testOrganizationResources();
    testOrganizationSerialization();
    
    std::cout << "\nAll Organization tests passed!" << std::endl;
    return 0;
}
