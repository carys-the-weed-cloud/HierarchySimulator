#include <iostream>
#include <cassert>
#include "hiersim/Unit.h"

using namespace hiersim;

void testUnitCreation() {
    Unit unit("Alpha Company", Unit::UnitType::StandingArmy);
    assert(unit.getId() > 0);
    assert(unit.getName() == "Alpha Company");
    assert(unit.getType() == Unit::UnitType::StandingArmy);
    assert(unit.isActive());
    assert(!unit.isDeployed());
    std::cout << "[PASS] testUnitCreation" << std::endl;
}

void testUnitMoraleAndLoyalty() {
    Unit unit("Morale Test");
    
    unit.setMorale(0.9);
    assert(unit.getMorale() == 0.9);
    
    unit.setLoyalty(0.85);
    assert(unit.getLoyalty() == 0.85);
    
    // Test clamping
    unit.setMorale(1.5);
    assert(unit.getMorale() == 1.0);
    
    unit.setLoyalty(-0.1);
    assert(unit.getLoyalty() == 0.0);
    std::cout << "[PASS] testUnitMoraleAndLoyalty" << std::endl;
}

void testUnitPersonnel() {
    Unit unit("Personnel Test");
    
    unit.addPersonnel(101);
    unit.addPersonnel(102);
    unit.addPersonnel(103);
    
    assert(unit.hasPersonnel(101));
    assert(unit.hasPersonnel(102));
    assert(!unit.hasPersonnel(999));
    assert(unit.getPersonnelCount() == 3);
    
    unit.removePersonnel(102);
    assert(!unit.hasPersonnel(102));
    assert(unit.getPersonnelCount() == 2);
    std::cout << "[PASS] testUnitPersonnel" << std::endl;
}

void testUnitDeployment() {
    Unit unit("Deploy Test");
    
    assert(!unit.isDeployed());
    
    unit.deploy(100.0, 200.0, 0.0);
    assert(unit.isDeployed());
    assert(unit.getLocationX() == 100.0);
    assert(unit.getLocationY() == 200.0);
    assert(unit.isAtLocation(100.0, 200.0));
    assert(unit.isAtLocation(100.5, 200.5, 1.0));  // Within radius
    
    unit.retreat();
    assert(!unit.isDeployed());
    std::cout << "[PASS] testUnitDeployment" << std::endl;
}

void testUnitOrders() {
    Unit unit("Order Test");
    
    Unit::CombatOrder order{
        Unit::CombatOrder::Attack,
        999,  // target unit
        0,    // no target facility
        150.0, 200.0, 0.0,  // target coordinates
        10    // valid until tick 10
    };
    
    unit.issueOrder(order);
    assert(unit.hasOrders());
    
    Unit::CombatOrder executed = unit.executeNextOrder(5);
    assert(executed.order == Unit::CombatOrder::Attack);
    assert(executed.targetUnit == 999);
    std::cout << "[PASS] testUnitOrders" << std::endl;
}

void testUnitExperienceAndTraining() {
    Unit unit("Training Test");
    
    assert(unit.getExperience() == 0.0);
    
    unit.gainExperience(0.3);
    assert(unit.getExperience() == 0.3);
    
    unit.train(0.5);
    assert(unit.getExperience() > 0.3);
    assert(unit.getCombatEffectiveness() > 0.5);
    std::cout << "[PASS] testUnitExperienceAndTraining" << std::endl;
}

void testUnitCasualties() {
    Unit unit("Casualty Test");
    
    unit.addPersonnel(101);
    unit.addPersonnel(102);
    unit.addPersonnel(103);
    unit.addPersonnel(104);
    unit.addPersonnel(105);
    
    assert(unit.getPersonnelCount() == 5);
    assert(unit.getStrength() == 1.0);
    
    unit.takeCasualties(0.4);  // 40% casualties
    assert(unit.getStrength() == 0.6);
    assert(unit.getPersonnelCount() < 5);
    std::cout << "[PASS] testUnitCasualties" << std::endl;
}

void testUnitSerialization() {
    Unit unit("Serialize Unit", Unit::UnitType::Marine, 400);
    unit.setMorale(0.85);
    unit.setLoyalty(0.9);
    unit.deploy(75.0, 125.0);
    
    std::string serialized = unit.serialize();
    Unit deserialized = Unit::deserialize(serialized);
    
    assert(deserialized.getId() == 400);
    assert(deserialized.getName() == "Serialize Unit");
    assert(deserialized.isDeployed());
    std::cout << "[PASS] testUnitSerialization" << std::endl;
}

int main() {
    std::cout << "Running Unit Tests..." << std::endl;
    
    testUnitCreation();
    testUnitMoraleAndLoyalty();
    testUnitPersonnel();
    testUnitDeployment();
    testUnitOrders();
    testUnitExperienceAndTraining();
    testUnitCasualties();
    testUnitSerialization();
    
    std::cout << "\nAll Unit tests passed!" << std::endl;
    return 0;
}
