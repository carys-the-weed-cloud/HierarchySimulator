#ifndef HIERSIM_COMMODITY_H
#define HIERSIM_COMMODITY_H

#include <string>
#include <cstdint>
#include <map>

namespace hiersim {

/**
 * @brief Represents a commodity in the economic simulation.
 * 
 * Commodities are goods or services produced for exchange in the market.
 * Each commodity has a value based on socially necessary labor time.
 */
class Commodity {
public:
    using ID = uint64_t;
    
    /**
     * @brief Types of commodities
     */
    enum class Type {
        RawMaterial,      // Unprocessed resources (ore, timber, oil)
        Intermediate,     // Partially processed goods (steel, chemicals)
        ConsumerGood,     // Final goods for consumption (food, clothing)
        CapitalGood,      // Means of production (machinery, tools)
        Service,          // Intangible services (healthcare, education)
        LuxuryGood,       // High-end consumer goods
        Weaponry,         // Military equipment
        Technology        // Advanced tech products
    };
    
    /**
     * @brief Construct a new Commodity object
     * @param name Name of the commodity
     * @param type Type classification
     * @param id Unique identifier
     */
    explicit Commodity(const std::string& name, Type type = Type::ConsumerGood, ID id = 0);
    ~Commodity() = default;
    
    // Getters
    ID getId() const { return id_; }
    const std::string& getName() const { return name_; }
    Type getType() const { return type_; }
    double getValue() const { return value_; }
    double getPrice() const { return price_; }
    double getLaborContent() const { return laborContent_; }
    int64_t getQuantity() const { return quantity_; }
    double getDepreciationRate() const { return depreciationRate_; }
    
    // Setters
    void setName(const std::string& name) { name_ = name; }
    void setType(Type type) { type_ = type; }
    void setValue(double value) { value_ = value; }
    void setPrice(double price) { price_ = price; }
    void setLaborContent(double labor) { laborContent_ = labor; }
    void setQuantity(int64_t qty) { quantity_ = qty; }
    void setDepreciationRate(double rate) { depreciationRate_ = rate; }
    
    /**
     * @brief Calculate the value based on labor theory of value
     * @param directLabor Direct labor time required
     * @param indirectLabor Indirect labor (means of production)
     * @param skillMultiplier Skill complexity multiplier
     * @return Total value in labor-time units
     */
    static double calculateValue(double directLabor, double indirectLabor, double skillMultiplier = 1.0);
    
    /**
     * @brief Apply depreciation to capital goods
     * @param ticks Number of ticks elapsed
     */
    void depreciate(int64_t ticks);
    
    /**
     * @brief Serialize commodity data
     * @return Serialized string representation
     */
    std::string serialize() const;
    
    /**
     * @brief Deserialize commodity from string
     * @param data Serialized data
     * @return Deserialized Commodity object
     */
    static Commodity deserialize(const std::string& data);
    
private:
    ID id_;
    std::string name_;
    Type type_;
    double value_;           // Value in labor-time units
    double price_;           // Market price (may diverge from value)
    double laborContent_;    // Total labor embodied
    int64_t quantity_;       // Current quantity
    double depreciationRate_; // Per-tick depreciation for capital goods
    
    static ID nextId_;
};

/**
 * @brief Convert commodity type to string
 */
std::string commodityTypeToString(Commodity::Type type);

/**
 * @brief Convert string to commodity type
 */
Commodity::Type stringToCommodityType(const std::string& str);

} // namespace hiersim

#endif // HIERSIM_COMMODITY_H