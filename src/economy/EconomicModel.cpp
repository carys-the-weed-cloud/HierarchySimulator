#include "hiersim/EconomicModel.h"
#include "hiersim/Scenario.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace hiersim {

// Initialize static member
EconomicModel::ID EconomicModel::nextId_ = 1;

EconomicModel::EconomicModel(const std::string& name, SystemType systemType)
    : id_(nextId_++)
    , name_(name)
    , systemType_(systemType)
    , totalSurplusValue_(0.0)
    , totalVariableCapital_(0.0)
    , totalConstantCapital_(0.0)
    , totalOutput_(0.0)
{
    // Initialize indicators with default values
    indicators_.gdp = 0.0;
    indicators_.unemploymentRate = 0.0;
    indicators_.inflationRate = 0.0;
    indicators_.profitRate = 0.0;
    indicators_.wageShare = 0.0;
    indicators_.exploitationRate = 0.0;
    indicators_.capacityUtilization = 1.0;
    indicators_.investmentRate = 0.0;
    indicators_.consumptionRate = 0.0;
    indicators_.governmentSpending = 0.0;
    indicators_.moneySupply = 0.0;
    indicators_.creditVolume = 0.0;
    indicators_.lastUpdateTick = 0;
    
    // Initialize class structure with default distribution
    classStructure_.capitalistClassSize = 0.05;
    classStructure_.workingClassSize = 0.70;
    classStructure_.pettyBourgeoisieSize = 0.10;
    classStructure_.lumpenSize = 0.05;
    classStructure_.managerialClassSize = 0.10;
}

void EconomicModel::addMarket(std::shared_ptr<Market> market) {
    if (!market) return;
    markets_.push_back(market);
}

bool EconomicModel::removeMarket(ID marketId) {
    auto it = std::find_if(markets_.begin(), markets_.end(),
        [marketId](const std::shared_ptr<Market>& m) {
            return m->getId() == marketId;
        });
    
    if (it != markets_.end()) {
        markets_.erase(it);
        return true;
    }
    return false;
}

Market* EconomicModel::getOrCreateMarket(Commodity::Type commodityType) {
    auto mapIt = typeToMarketMap_.find(commodityType);
    if (mapIt != typeToMarketMap_.end()) {
        // Find existing market
        for (auto& market : markets_) {
            if (market->getId() == mapIt->second) {
                return market.get();
            }
        }
    }
    
    // Create new market
    auto market = std::make_shared<Market>(commodityTypeToString(commodityType) + " Market");
    typeToMarketMap_[commodityType] = market->getId();
    markets_.push_back(market);
    return market.get();
}

void EconomicModel::setScenario(std::shared_ptr<Scenario> scenario) {
    scenario_ = scenario;
    // Initialize markets based on scenario data if available
    if (scenario_) {
        // TODO: Initialize economic data from scenario
    }
}

EconomicModel::CrisisFlags EconomicModel::detectCrises() const {
    CrisisFlags flags;
    
    // Detect overproduction crisis (high capacity utilization but low consumption)
    if (indicators_.capacityUtilization > 0.9 && indicators_.consumptionRate < 0.5) {
        flags.overproduction = true;
    }
    
    // Detect profit squeeze (falling profit rate with rising wage share)
    if (indicators_.profitRate < 0.05 && indicators_.wageShare > 0.7) {
        flags.profitSqueeze = true;
    }
    
    // Detect financial instability (high credit volume relative to money supply)
    if (indicators_.moneySupply > 0 && indicators_.creditVolume / indicators_.moneySupply > 5.0) {
        flags.financialInstability = true;
    }
    
    return flags;
}

void EconomicModel::processTick(int64_t currentTick) {
    // Clear all markets (match buy/sell orders)
    for (auto& market : markets_) {
        market->clearMarket(currentTick);
    }
    
    // Recalculate macroeconomic indicators
    calculateIndicators(currentTick);
    
    // Update class structure based on economic conditions
    updateClassStructure();
}

void EconomicModel::calculateIndicators(int64_t currentTick) {
    // This is a simplified implementation
    // Full implementation would aggregate data from all Organizations, Facilities, and Markets
    
    // Calculate GDP as total output
    indicators_.gdp = totalOutput_;
    
    // Calculate wage share (labor's share of income)
    if (indicators_.gdp > 0) {
        indicators_.wageShare = totalVariableCapital_ / indicators_.gdp;
    }
    
    // Calculate exploitation rate (surplus value / variable capital)
    if (totalVariableCapital_ > 0) {
        indicators_.exploitationRate = totalSurplusValue_ / totalVariableCapital_;
    }
    
    // Calculate profit rate (surplus value / total capital)
    double totalCapital = totalConstantCapital_ + totalVariableCapital_;
    if (totalCapital > 0) {
        indicators_.profitRate = totalSurplusValue_ / totalCapital;
    }
    
    // Capacity utilization (simplified)
    indicators_.capacityUtilization = std::min(1.0, indicators_.capacityUtilization);
    
    // Investment and consumption rates (placeholders)
    indicators_.investmentRate = 0.2;  // 20% of GDP
    indicators_.consumptionRate = 0.7; // 70% of GDP
    
    indicators_.lastUpdateTick = currentTick;
}

void EconomicModel::updateClassStructure() {
    // Adjust class sizes based on economic conditions
    // This is simplified - full implementation would track individual class transitions
    
    switch (systemType_) {
        case SystemType::Capitalism:
            // Tendency toward polarization
            if (indicators_.exploitationRate > 1.0) {
                classStructure_.workingClassSize += 0.001;
                classStructure_.capitalistClassSize -= 0.0001;
            }
            break;
            
        case SystemType::StateSocialism:
            // Managerial class may grow
            classStructure_.managerialClassSize = std::min(0.25, classStructure_.managerialClassSize + 0.0001);
            break;
            
        case SystemType::Communism:
            // Tend toward classlessness
            classStructure_.capitalistClassSize *= 0.99;
            classStructure_.workingClassSize *= 0.99;
            break;
            
        default:
            break;
    }
    
    // Normalize to sum to 1.0
    double total = classStructure_.capitalistClassSize + 
                   classStructure_.workingClassSize + 
                   classStructure_.pettyBourgeoisieSize + 
                   classStructure_.lumpenSize + 
                   classStructure_.managerialClassSize;
    
    if (total > 0) {
        classStructure_.capitalistClassSize /= total;
        classStructure_.workingClassSize /= total;
        classStructure_.pettyBourgeoisieSize /= total;
        classStructure_.lumpenSize /= total;
        classStructure_.managerialClassSize /= total;
    }
}

bool EconomicModel::checkCrisisConditions() const {
    // Check for various crisis tendencies
    
    // Overproduction crisis: high capacity utilization but low consumption
    if (indicators_.capacityUtilization > 0.9 && indicators_.consumptionRate < 0.5) {
        return true;
    }
    
    // Profit squeeze: falling profit rate
    if (indicators_.profitRate < 0.05) {
        return true;
    }
    
    // Financial crisis: high credit relative to money supply
    if (indicators_.moneySupply > 0 && 
        indicators_.creditVolume / indicators_.moneySupply > 5.0) {
        return true;
    }
    
    return false;
}

std::string EconomicModel::serialize() const {
    std::ostringstream oss;
    oss << id_ << "|" << name_ << "|" 
        << systemTypeToString(systemType_) << "|"
        << std::fixed << std::setprecision(4)
        << indicators_.gdp << "|" << indicators_.exploitationRate << "|"
        << indicators_.profitRate << "|" << totalSurplusValue_;
    return oss.str();
}

EconomicModel EconomicModel::deserialize(const std::string& data) {
    EconomicModel model("placeholder", SystemType::MixedEconomy);
    std::istringstream iss(data);
    
    char delim;
    std::string typeStr;
    
    iss >> model.id_ >> delim;
    std::getline(iss, model.name_, delim);
    std::getline(iss, typeStr, delim);
    model.systemType_ = stringToSystemType(typeStr);
    
    iss >> model.indicators_.gdp >> delim 
        >> model.indicators_.exploitationRate >> delim
        >> model.indicators_.profitRate >> delim
        >> model.totalSurplusValue_;
    
    if (model.id_ >= nextId_) {
        nextId_ = model.id_ + 1;
    }
    
    return model;
}

std::string systemTypeToString(EconomicModel::SystemType type) {
    switch (type) {
        case EconomicModel::SystemType::Capitalism: return "Capitalism";
        case EconomicModel::SystemType::StateSocialism: return "StateSocialism";
        case EconomicModel::SystemType::MarketSocialism: return "MarketSocialism";
        case EconomicModel::SystemType::Anarchism: return "Anarchism";
        case EconomicModel::SystemType::MixedEconomy: return "MixedEconomy";
        case EconomicModel::SystemType::Communism: return "Communism";
        default: return "Unknown";
    }
}

EconomicModel::SystemType stringToSystemType(const std::string& str) {
    if (str == "Capitalism") return EconomicModel::SystemType::Capitalism;
    if (str == "StateSocialism") return EconomicModel::SystemType::StateSocialism;
    if (str == "MarketSocialism") return EconomicModel::SystemType::MarketSocialism;
    if (str == "Anarchism") return EconomicModel::SystemType::Anarchism;
    if (str == "MixedEconomy") return EconomicModel::SystemType::MixedEconomy;
    if (str == "Communism") return EconomicModel::SystemType::Communism;
    return EconomicModel::SystemType::MixedEconomy;
}

} // namespace hiersim