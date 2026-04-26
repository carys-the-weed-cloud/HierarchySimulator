#ifndef HIERSIM_ECONOMIC_MODEL_H
#define HIERSIM_ECONOMIC_MODEL_H

#include "hiersim/Commodity.h"
#include "hiersim/Market.h"
#include "hiersim/Scenario.h"
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <cstdint>
#include <cmath>

namespace hiersim {

// Forward declarations
class Organization;
class Facility;
class Individual;

/**
 * @brief Represents the economic simulation model combining Keynesian and Marxist dynamics.
 * 
 * This model simulates:
 * - Commodity production and value theory (Marxist)
 * - Money, credit, and aggregate demand (Keynesian)
 * - Class dynamics and surplus value extraction
 * - Investment, accumulation, and crisis tendencies
 */
class EconomicModel {
public:
    using ID = uint64_t;
    
    /**
     * @brief Economic system types
     */
    enum class SystemType {
        Capitalism,           // Private ownership, market allocation
        StateSocialism,       // State ownership, central planning
        MarketSocialism,      // Social ownership, market allocation
        Anarchism,            // No state, voluntary associations
        MixedEconomy,         // Combination of public and private
        Communism             // No classes, no state, direct distribution
    };
    
    /**
     * @brief Macroeconomic indicators
     */
    struct MacroIndicators {
        double gdp;                    // Total output
        double unemploymentRate;       // Percentage of labor force unemployed
        double inflationRate;          // Price level change rate
        double profitRate;             // Average rate of profit
        double wageShare;              // Labor's share of income
        double exploitationRate;       // Surplus value / variable capital
        double capacityUtilization;    // Production capacity in use
        double investmentRate;         // Investment as % of GDP
        double consumptionRate;        // Consumption as % of GDP
        double governmentSpending;     // Government expenditure
        double moneySupply;            // Total money in circulation
        double creditVolume;           // Outstanding credit
        int64_t lastUpdateTick;
    };
    
    /**
     * @brief Class structure in the economy
     */
    struct ClassStructure {
        double capitalistClassSize;    // Owners of means of production
        double workingClassSize;       // Wage laborers
        double pettyBourgeoisieSize;   // Small proprietors
        double lumpenSize;             // Marginalized populations
        double managerialClassSize;    // Managers and supervisors
    };
    
    /**
     * @brief Construct a new EconomicModel object
     * @param name Model name
     * @param systemType Type of economic system
     */
    explicit EconomicModel(const std::string& name, SystemType systemType = SystemType::MixedEconomy);
    ~EconomicModel() = default;
    
    // Getters
    ID getId() const { return id_; }
    const std::string& getName() const { return name_; }
    SystemType getSystemType() const { return systemType_; }
    const MacroIndicators& getIndicators() const { return indicators_; }
    const ClassStructure& getClassStructure() const { return classStructure_; }
    const std::vector<std::shared_ptr<Market>>& getMarkets() const { return markets_; }
    
    // Setters
    void setSystemType(SystemType type) { systemType_ = type; }
    
    /**
     * @brief Add a market to the economic model
     * @param market Shared pointer to market
     */
    void addMarket(std::shared_ptr<Market> market);
    
    /**
     * @brief Remove a market from the model
     * @param marketId ID of market to remove
     * @return true if removed
     */
    bool removeMarket(ID marketId);
    
    /**
     * @brief Get or create a market for a commodity type
     * @param commodityType Type of commodity
     * @return Pointer to market
     */
    Market* getOrCreateMarket(Commodity::Type commodityType);
    
    /**
     * @brief Set the scenario for this economic model
     * @param scenario Shared pointer to scenario
     */
    void setScenario(std::shared_ptr<Scenario> scenario);
    
    /**
     * @brief Get macroeconomic indicators
     * @return Current macroeconomic indicators
     */
    MacroIndicators getMacroeconomicIndicators() const { return indicators_; }
    
    /**
     * @brief Detect economic crises
     * @return CrisisFlags indicating which crises are active
     */
    struct CrisisFlags {
        bool overproduction = false;
        bool profitSqueeze = false;
        bool financialInstability = false;
    };
    CrisisFlags detectCrises() const;
    
    /**
     * @brief Process one tick of the economic simulation
     * @param currentTick Current simulation tick
     */
    void processTick(int64_t currentTick);
    
    /**
     * @brief Calculate macroeconomic indicators
     * @param currentTick Current simulation tick
     */
    void calculateIndicators(int64_t currentTick);
    
    /**
     * @brief Update class structure based on current conditions
     */
    void updateClassStructure();
    
    /**
     * @brief Check for economic crisis conditions
     * @return true if crisis detected
     */
    bool checkCrisisConditions() const;
    
    /**
     * @brief Get the current exploitation rate
     * @return Exploitation rate (surplus value / necessary labor)
     */
    double getExploitationRate() const { return indicators_.exploitationRate; }
    
    /**
     * @brief Get the average profit rate
     * @return Profit rate
     */
    double getProfitRate() const { return indicators_.profitRate; }
    
    /**
     * @brief Get total surplus value produced
     * @return Surplus value
     */
    double getTotalSurplusValue() const { return totalSurplusValue_; }
    
    /**
     * @brief Serialize economic model data
     * @return Serialized string representation
     */
    std::string serialize() const;
    
    /**
     * @brief Deserialize economic model from string
     * @param data Serialized data
     * @return Deserialized EconomicModel object
     */
    static EconomicModel deserialize(const std::string& data);
    
private:
    ID id_;
    std::string name_;
    SystemType systemType_;
    MacroIndicators indicators_;
    ClassStructure classStructure_;
    
    std::vector<std::shared_ptr<Market>> markets_;
    std::map<Commodity::Type, ID> typeToMarketMap_;
    
    // Aggregate values
    double totalSurplusValue_;
    double totalVariableCapital_;  // Wages
    double totalConstantCapital_;  // Means of production
    double totalOutput_;
    
    // Scenario reference
    std::shared_ptr<Scenario> scenario_;
    
    static ID nextId_;
};

/**
 * @brief Convert system type to string
 */
std::string systemTypeToString(EconomicModel::SystemType type);

/**
 * @brief Convert string to system type
 */
EconomicModel::SystemType stringToSystemType(const std::string& str);

} // namespace hiersim

#endif // HIERSIM_ECONOMIC_MODEL_H