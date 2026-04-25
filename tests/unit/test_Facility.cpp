#include <iostream>
#include <cassert>
#include "hiersim/Facility.h"

using namespace hiersim;

void testFacilityCreation() {
    Facility fac("Test Factory", Facility::FacilityType::Factory);
    assert(fac.getId() > 0);
    assert(fac.getName() == "Test Factory");
    assert(fac.getType() == Facility::FacilityType::Factory);
    assert(fac.isActive());
    assert(!fac.isDestroyed());
    std::cout << "[PASS] testFacilityCreation" << std::endl;
}

void testFacilityLocation() {
    Facility fac("Location Test");
    
    fac.setLocation(100.5, 200.75, 50.0);
    assert(fac.getLocationX() == 100.5);
    assert(fac.getLocationY() == 200.75);
    assert(fac.getLocationZ() == 50.0);
    std::cout << "[PASS] testFacilityLocation" << std::endl;
}

void testFacilityWorkers() {
    Facility fac("Worker Test");
    fac.setCapacity(5);
    
    fac.addWorker(101);
    fac.addWorker(102);
    fac.addWorker(103);
    
    assert(fac.hasWorker(101));
    assert(fac.hasWorker(102));
    assert(!fac.hasWorker(999));
    assert(fac.getCurrentOccupancy() == 3);
    assert(fac.hasCapacity());
    
    fac.removeWorker(102);
    assert(!fac.hasWorker(102));
    assert(fac.getCurrentOccupancy() == 2);
    std::cout << "[PASS] testFacilityWorkers" << std::endl;
}

void testFacilityAccessControl() {
    Facility fac("Access Test");
    
    // Test Public access
    Facility::AccessLevel publicAccess{Facility::AccessLevel::Public, {}};
    fac.setAccessLevel(publicAccess);
    assert(fac.canAccess(1, false, false));
    
    // Test MemberOnly access
    Facility::AccessLevel memberAccess{Facility::AccessLevel::MemberOnly, {}};
    fac.setAccessLevel(memberAccess);
    assert(fac.canAccess(1, true, false));
    assert(!fac.canAccess(1, false, false));
    
    // Test StaffOnly access
    Facility::AccessLevel staffAccess{Facility::AccessLevel::StaffOnly, {}};
    fac.setAccessLevel(staffAccess);
    assert(fac.canAccess(1, false, true));
    fac.addWorker(1);
    assert(fac.canAccess(1, false, false));  // Is a worker
    std::cout << "[PASS] testFacilityAccessControl" << std::endl;
}

void testFacilityDamageAndRepair() {
    Facility fac("Durability Test");
    
    assert(fac.getIntegrity() == 1.0);
    assert(fac.getEfficiency() == 1.0);
    
    fac.damage(0.3);
    assert(fac.getIntegrity() == 0.7);
    assert(fac.getEfficiency() == 0.7);
    
    fac.repair(0.2);
    assert(fac.getIntegrity() == 0.9);
    
    fac.damage(1.0);  // Destroy it
    assert(fac.getIntegrity() == 0.0);
    assert(fac.isDestroyed());
    assert(!fac.isActive());
    std::cout << "[PASS] testFacilityDamageAndRepair" << std::endl;
}

void testFacilitySerialization() {
    Facility fac("Serialize Facility", Facility::FacilityType::ResearchLab, 300);
    fac.setLocation(50.0, 75.0);
    fac.setCapacity(200);
    fac.setEfficiency(0.85);
    fac.setOutput(1500.0);
    
    std::string serialized = fac.serialize();
    Facility deserialized = Facility::deserialize(serialized);
    
    assert(deserialized.getId() == 300);
    assert(deserialized.getName() == "Serialize Facility");
    assert(deserialized.getCapacity() == 200);
    std::cout << "[PASS] testFacilitySerialization" << std::endl;
}

int main() {
    std::cout << "Running Facility Unit Tests..." << std::endl;
    
    testFacilityCreation();
    testFacilityLocation();
    testFacilityWorkers();
    testFacilityAccessControl();
    testFacilityDamageAndRepair();
    testFacilitySerialization();
    
    std::cout << "\nAll Facility tests passed!" << std::endl;
    return 0;
}
