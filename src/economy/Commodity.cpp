#include "hiersim/Commodity.h"
#include <sstream>
#include <iomanip>
#include <cmath>

namespace hiersim {

// Initialize static member
Commodity::ID Commodity::nextId_ = 1;

Commodity::Commodity(const std::string& name, Type type, ID id)
    : id_(id != 0 ? id : nextId_++)
    , name_(name)
    , type_(type)
    , value_(0.0)
    , price_(0.0)
    , laborContent_(0.0)
    , quantity_(0)
    , depreciationRate_(0.0)
{
    if (id != 0 && id >= nextId_) {
        nextId_ = id + 1;
    }
}

double Commodity::calculateValue(double directLabor, double indirectLabor, double skillMultiplier) {
    // Labor theory of value: value = (direct + indirect) * skill multiplier
    return (directLabor + indirectLabor) * skillMultiplier;
}

void Commodity::depreciate(int64_t ticks) {
    if (depreciationRate_ > 0.0 && quantity_ > 0) {
        // Apply exponential decay based on depreciation rate
        double remaining = static_cast<double>(quantity_) * std::pow(1.0 - depreciationRate_, ticks);
        quantity_ = static_cast<int64_t>(std::max(0.0, remaining));
        
        // Adjust value based on depreciation
        value_ *= std::pow(1.0 - depreciationRate_, ticks);
    }
}

std::string Commodity::serialize() const {
    std::ostringstream oss;
    oss << id_ << "|" << name_ << "|" 
        << commodityTypeToString(type_) << "|"
        << std::fixed << std::setprecision(4)
        << value_ << "|" << price_ << "|" 
        << laborContent_ << "|" << quantity_ << "|" 
        << depreciationRate_;
    return oss.str();
}

Commodity Commodity::deserialize(const std::string& data) {
    Commodity commodity("placeholder", Type::ConsumerGood, 0);
    std::istringstream iss(data);
    
    char delim;
    std::string typeStr;
    
    iss >> commodity.id_ >> delim;
    std::getline(iss, commodity.name_, delim);
    std::getline(iss, typeStr, delim);
    commodity.type_ = stringToCommodityType(typeStr);
    
    iss >> commodity.value_ >> delim >> commodity.price_ >> delim 
        >> commodity.laborContent_ >> delim >> commodity.quantity_ >> delim 
        >> commodity.depreciationRate_;
    
    if (commodity.id_ >= nextId_) {
        nextId_ = commodity.id_ + 1;
    }
    
    return commodity;
}

std::string commodityTypeToString(Commodity::Type type) {
    switch (type) {
        case Commodity::Type::RawMaterial: return "RawMaterial";
        case Commodity::Type::Intermediate: return "Intermediate";
        case Commodity::Type::ConsumerGood: return "ConsumerGood";
        case Commodity::Type::CapitalGood: return "CapitalGood";
        case Commodity::Type::Service: return "Service";
        case Commodity::Type::LuxuryGood: return "LuxuryGood";
        case Commodity::Type::Weaponry: return "Weaponry";
        case Commodity::Type::Technology: return "Technology";
        default: return "Unknown";
    }
}

Commodity::Type stringToCommodityType(const std::string& str) {
    if (str == "RawMaterial") return Commodity::Type::RawMaterial;
    if (str == "Intermediate") return Commodity::Type::Intermediate;
    if (str == "ConsumerGood") return Commodity::Type::ConsumerGood;
    if (str == "CapitalGood") return Commodity::Type::CapitalGood;
    if (str == "Service") return Commodity::Type::Service;
    if (str == "LuxuryGood") return Commodity::Type::LuxuryGood;
    if (str == "Weaponry") return Commodity::Type::Weaponry;
    if (str == "Technology") return Commodity::Type::Technology;
    return Commodity::Type::ConsumerGood;
}

} // namespace hiersim