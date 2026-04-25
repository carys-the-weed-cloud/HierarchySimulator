#include "hiersim/Facility.h"
#include <sstream>
#include <algorithm>
#include <cmath>

namespace hiersim {

// Initialize static member
Facility::ID Facility::nextId_ = 1;

Facility::Facility(const std::string& name, FacilityType type, ID id)
    : id_(id != 0 ? id : nextId_++)
    , name_(name)
    , type_(type)
    , locationX_(0.0)
    , locationY_(0.0)
    , locationZ_(0.0)
    , capacity_(100)
    , currentOccupancy_(0)
    , efficiency_(1.0)
    , owner_(0)
    , output_(0.0)
    , inputRequired_(0.0)
    , integrity_(1.0)
    , active_(true)
    , destroyed_(false)
{
    // Set default access level to MemberOnly
    accessLevel_ = AccessLevel{AccessLevel::MemberOnly, {}};
    
    if (id != 0 && id >= nextId_) {
        nextId_ = id + 1;
    }
}

Facility::~Facility() = default;

void Facility::setLocation(double x, double y, double z) {
    locationX_ = x;
    locationY_ = y;
    locationZ_ = z;
}

void Facility::addWorker(ID individualId) {
    if (!hasWorker(individualId) && hasCapacity()) {
        workers_.push_back(individualId);
        currentOccupancy_++;
    }
}

void Facility::removeWorker(ID individualId) {
    auto it = std::find(workers_.begin(), workers_.end(), individualId);
    if (it != workers_.end()) {
        workers_.erase(it);
        currentOccupancy_--;
    }
}

bool Facility::hasWorker(ID individualId) const {
    return std::find(workers_.begin(), workers_.end(), individualId) != workers_.end();
}

void Facility::setAccessLevel(const AccessLevel& access) {
    accessLevel_ = access;
}

bool Facility::canAccess(ID individualId, bool isMember, bool isWorker) const {
    switch (accessLevel_.level) {
        case AccessLevel::Public:
            return true;
            
        case AccessLevel::MemberOnly:
            return isMember;
            
        case AccessLevel::StaffOnly:
            return isWorker || hasWorker(individualId);
            
        case AccessLevel::Restricted:
            return std::find(accessLevel_.authorizedIndividuals.begin(),
                           accessLevel_.authorizedIndividuals.end(),
                           individualId) != accessLevel_.authorizedIndividuals.end();
            
        case AccessLevel::Secret:
            return false;  // Hidden from most actors
            
        default:
            return false;
    }
}

void Facility::damage(double amount) {
    if (amount > 0) {
        integrity_ = std::max(0.0, integrity_ - amount);
        efficiency_ = integrity_;  // Efficiency drops with integrity
        
        if (integrity_ <= 0.0) {
            destroyed_ = true;
            active_ = false;
        }
    }
}

void Facility::repair(double amount) {
    if (amount > 0 && !destroyed_) {
        integrity_ = std::min(1.0, integrity_ + amount);
        efficiency_ = integrity_;  // Efficiency restored with repairs
        
        if (integrity_ > 0.5) {
            active_ = true;  // Can operate if mostly repaired
        }
    }
}

std::string Facility::serialize() const {
    std::ostringstream oss;
    oss << id_ << "|" << name_ << "|" << static_cast<int>(type_) << "|"
        << locationX_ << "|" << locationY_ << "|" << locationZ_ << "|"
        << capacity_ << "|" << efficiency_ << "|" << owner_ << "|"
        << output_ << "|" << inputRequired_ << "|" << integrity_ << "|"
        << (active_ ? "1" : "0") << "|" << (destroyed_ ? "1" : "0");
    
    // Serialize workers
    oss << "|workers:";
    for (ID workerId : workers_) {
        oss << workerId << ",";
    }
    
    // Serialize authorized individuals for restricted access
    oss << "|authorized:";
    for (ID authId : accessLevel_.authorizedIndividuals) {
        oss << authId << ",";
    }
    
    return oss.str();
}

Facility Facility::deserialize(const std::string& data) {
    Facility fac("placeholder", FacilityType::Factory, 0);
    std::istringstream iss(data);
    
    char delim;
    int typeInt, activeInt, destroyedInt;
    
    iss >> fac.id_ >> delim;
    std::getline(iss, fac.name_, delim);
    iss >> typeInt >> delim 
        >> fac.locationX_ >> delim >> fac.locationY_ >> delim >> fac.locationZ_ >> delim
        >> fac.capacity_ >> delim >> fac.efficiency_ >> delim >> fac.owner_ >> delim
        >> fac.output_ >> delim >> fac.inputRequired_ >> delim >> fac.integrity_ >> delim
        >> activeInt >> delim >> destroyedInt;
    
    fac.type_ = static_cast<FacilityType>(typeInt);
    fac.active_ = (activeInt == 1);
    fac.destroyed_ = (destroyedInt == 1);
    fac.currentOccupancy_ = 0;  // Will be recalculated from workers
    
    fac.accessLevel_ = AccessLevel{AccessLevel::MemberOnly, {}};
    
    // Parse workers and authorized individuals (simplified)
    // Full implementation would parse the sections after |workers:, |authorized:
    
    if (fac.id_ >= nextId_) {
        nextId_ = fac.id_ + 1;
    }
    
    return fac;
}

} // namespace hiersim
