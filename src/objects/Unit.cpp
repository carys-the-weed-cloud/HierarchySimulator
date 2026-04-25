#include "hiersim/Unit.h"
#include "hiersim/Facility.h"
#include <sstream>
#include <algorithm>
#include <cmath>

namespace hiersim {

// Initialize static member
Unit::ID Unit::nextId_ = 1;

Unit::Unit(const std::string& name, UnitType type, ID id)
    : id_(id != 0 ? id : nextId_++)
    , name_(name)
    , type_(type)
    , equippingOrg_(0)
    , commandingOrg_(0)
    , morale_(0.7)
    , loyalty_(0.8)
    , combatEffectiveness_(0.5)
    , strength_(1.0)
    , experience_(0.0)
    , locationX_(0.0)
    , locationY_(0.0)
    , locationZ_(0.0)
    , currentOrder_{Unit::CombatOrder::Hold, 0, 0, 0, 0, 0, -1}
    , active_(true)
    , deployed_(false)
{
    if (id != 0 && id >= nextId_) {
        nextId_ = id + 1;
    }
}

Unit::~Unit() = default;

void Unit::addPersonnel(ID individualId) {
    if (!hasPersonnel(individualId)) {
        personnel_.push_back(individualId);
    }
}

void Unit::removePersonnel(ID individualId) {
    auto it = std::find(personnel_.begin(), personnel_.end(), individualId);
    if (it != personnel_.end()) {
        personnel_.erase(it);
    }
}

bool Unit::hasPersonnel(ID individualId) const {
    return std::find(personnel_.begin(), personnel_.end(), individualId) != personnel_.end();
}

void Unit::setLocation(double x, double y, double z) {
    locationX_ = x;
    locationY_ = y;
    locationZ_ = z;
}

void Unit::deploy(double x, double y, double z) {
    setLocation(x, y, z);
    deployed_ = true;
    active_ = true;
}

void Unit::retreat() {
    deployed_ = false;
    currentOrder_ = CombatOrder{CombatOrder::Retreat, 0, 0, 0, 0, 0, -1};
}

bool Unit::isAtLocation(double x, double y, double radius) const {
    double dx = locationX_ - x;
    double dy = locationY_ - y;
    return std::sqrt(dx * dx + dy * dy) <= radius;
}

void Unit::issueOrder(const CombatOrder& order) {
    orderQueue_.push_back(order);
}

Unit::CombatOrder Unit::executeNextOrder(int64_t currentTick) {
    if (orderQueue_.empty()) {
        return currentOrder_;
    }
    
    // Check if oldest order is still valid
    if (orderQueue_[0].validUntil != -1 && orderQueue_[0].validUntil < currentTick) {
        orderQueue_.erase(orderQueue_.begin());
        if (orderQueue_.empty()) {
            return currentOrder_;
        }
    }
    
    currentOrder_ = orderQueue_.front();
    orderQueue_.erase(orderQueue_.begin());
    return currentOrder_;
}

Unit::CombatResult Unit::engage(Unit& enemy) {
    CombatResult result = {0.0, 0.0, 0.0, 0.0, false, ""};
    
    // Calculate combat effectiveness factors
    double ourPower = strength_ * combatEffectiveness_ * (1.0 + experience_) * morale_;
    double enemyPower = enemy.strength_ * enemy.combatEffectiveness_ * (1.0 + enemy.experience_) * enemy.morale_;
    
    // Add some randomness (simplified)
    double ourRoll = ourPower * (0.8 + 0.4 * (rand() % 100) / 100.0);
    double enemyRoll = enemyPower * (0.8 + 0.4 * (rand() % 100) / 100.0);
    
    if (ourRoll > enemyRoll) {
        // Victory
        result.victory = true;
        result.damageDealt = enemy.strength_ * 0.3;  // Deal 30% damage
        result.damageTaken = strength_ * 0.1;  // Take 10% damage
        result.moraleChange = 0.05;  // Morale boost
        result.loyaltyChange = 0.02;
        result.description = "Victory in engagement";
        
        enemy.takeCasualties(0.3);
        takeCasualties(0.1);
    } else {
        // Defeat
        result.victory = false;
        result.damageDealt = enemy.strength_ * 0.1;
        result.damageTaken = strength_ * 0.3;
        result.moraleChange = -0.1;  // Morale penalty
        result.loyaltyChange = -0.05;
        result.description = "Defeat in engagement";
        
        takeCasualties(0.3);
        enemy.takeCasualties(0.1);
    }
    
    // Apply morale and loyalty changes
    morale_ = std::clamp(morale_ + result.moraleChange, 0.0, 1.0);
    loyalty_ = std::clamp(loyalty_ + result.loyaltyChange, 0.0, 1.0);
    
    // Gain experience from combat
    gainExperience(0.05);
    
    return result;
}

Unit::CombatResult Unit::attackFacility(Facility& facility) {
    CombatResult result = {0.0, 0.0, 0.0, 0.0, false, ""};
    
    if (!facility.isActive() || facility.isDestroyed()) {
        result.victory = true;
        result.description = "Facility already destroyed/inactive";
        return result;
    }
    
    // Calculate attack power
    double attackPower = strength_ * combatEffectiveness_ * (1.0 + experience_) * morale_;
    double defensePower = facility.getIntegrity() * facility.getEfficiency();
    
    double attackRoll = attackPower * (0.8 + 0.4 * (rand() % 100) / 100.0);
    double defenseRoll = defensePower * (0.8 + 0.4 * (rand() % 100) / 100.0);
    
    if (attackRoll > defenseRoll) {
        result.victory = true;
        double damage = facility.getIntegrity() * 0.4;
        facility.damage(damage);
        result.damageDealt = damage;
        result.description = "Facility damaged successfully";
    } else {
        result.victory = false;
        result.description = "Attack repelled by facility defenses";
    }
    
    return result;
}

void Unit::takeCasualties(double ratio) {
    if (ratio <= 0 || ratio > 1) return;
    
    strength_ = std::max(0.0, strength_ - ratio);
    
    // Remove personnel proportionally
    size_t casualties = static_cast<size_t>(personnel_.size() * ratio);
    for (size_t i = 0; i < casualties && !personnel_.empty(); ++i) {
        personnel_.pop_back();
    }
    
    // Morale penalty from casualties
    morale_ = std::clamp(morale_ - ratio * 0.2, 0.0, 1.0);
    
    if (strength_ <= 0) {
        active_ = false;
        deployed_ = false;
    }
}

void Unit::reinforce(size_t count) {
    strength_ = std::min(1.0, strength_ + count * 0.01);
    morale_ = std::clamp(morale_ + 0.02, 0.0, 1.0);
}

void Unit::gainExperience(double amount) {
    experience_ = std::clamp(experience_ + amount, 0.0, 1.0);
    combatEffectiveness_ = std::min(1.0, 0.5 + experience_ * 0.5);
}

void Unit::train(double intensity) {
    if (intensity <= 0) return;
    
    experience_ = std::clamp(experience_ + intensity * 0.1, 0.0, 1.0);
    combatEffectiveness_ = std::min(1.0, combatEffectiveness_ + intensity * 0.05);
    morale_ = std::clamp(morale_ + intensity * 0.02, 0.0, 1.0);
}

std::string Unit::serialize() const {
    std::ostringstream oss;
    oss << id_ << "|" << name_ << "|" << static_cast<int>(type_) << "|"
        << equippingOrg_ << "|" << commandingOrg_ << "|"
        << morale_ << "|" << loyalty_ << "|" << combatEffectiveness_ << "|"
        << strength_ << "|" << experience_ << "|"
        << locationX_ << "|" << locationY_ << "|" << locationZ_ << "|"
        << (active_ ? "1" : "0") << "|" << (deployed_ ? "1" : "0");
    
    // Serialize personnel
    oss << "|personnel:";
    for (ID personId : personnel_) {
        oss << personId << ",";
    }
    
    return oss.str();
}

Unit Unit::deserialize(const std::string& data) {
    Unit unit("placeholder", UnitType::StandingArmy, 0);
    std::istringstream iss(data);
    
    char delim;
    int typeInt, activeInt, deployedInt;
    
    iss >> unit.id_ >> delim;
    std::getline(iss, unit.name_, delim);
    iss >> typeInt >> delim
        >> unit.equippingOrg_ >> delim >> unit.commandingOrg_ >> delim
        >> unit.morale_ >> delim >> unit.loyalty_ >> delim 
        >> unit.combatEffectiveness_ >> delim >> unit.strength_ >> delim
        >> unit.experience_ >> delim
        >> unit.locationX_ >> delim >> unit.locationY_ >> delim >> unit.locationZ_ >> delim
        >> activeInt >> delim >> deployedInt;
    
    unit.type_ = static_cast<UnitType>(typeInt);
    unit.active_ = (activeInt == 1);
    unit.deployed_ = (deployedInt == 1);
    
    // Parse personnel (simplified)
    // Full implementation would parse the section after |personnel:
    
    if (unit.id_ >= nextId_) {
        nextId_ = unit.id_ + 1;
    }
    
    return unit;
}

} // namespace hiersim
