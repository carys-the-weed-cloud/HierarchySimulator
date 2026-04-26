#ifndef HIERSIM_CULTURE_H
#define HIERSIM_CULTURE_H

#include <string>
#include <map>
#include <vector>
#include <memory>

namespace hiersim {

/**
 * @brief Cultural traits that define a region or organization's culture
 * 
 * Each trait ranges from 0.0 to 1.0
 */
struct CulturalTraits {
    float individualism = 0.5f;      // Individual vs Collective orientation
    float tradition = 0.5f;          // Traditional vs Modern values
    float innovation = 0.5f;         // Resistance vs Embrace of change
    float hierarchy = 0.5f;          // Egalitarian vs Hierarchical
    float materialism = 0.5f;        // Spiritual vs Material focus
    float risk_tolerance = 0.5f;     // Risk-averse vs Risk-seeking
    float long_term_orientation = 0.5f;  // Short-term vs Long-term planning
    float universalism = 0.5f;       // Particularist vs Universalist ethics
    
    /**
     * @brief Blend this culture with another based on weight
     * @param other The other culture to blend with
     * @param weight Weight of the other culture (0.0-1.0)
     */
    void blend(const CulturalTraits& other, float weight);
    
    /**
     * @brief Calculate cultural distance between two cultures
     * @param other The other culture
     * @return Distance metric (0.0 = identical, higher = more different)
     */
    float distance(const CulturalTraits& other) const;
    
    /**
     * @brief Serialize to string
     */
    std::string toString() const;
};

/**
 * @brief Represents the culture of a region, organization, or group
 */
class Culture {
public:
    Culture() = default;
    explicit Culture(const std::string& name);
    
    const std::string& getName() const { return name_; }
    void setName(const std::string& name) { name_ = name; }
    
    const CulturalTraits& getTraits() const { return traits_; }
    CulturalTraits& getTraits() { return traits_; }
    void setTraits(const CulturalTraits& traits) { traits_ = traits; }
    
    /**
     * @brief Update culture based on interactions with other cultures
     * @param other_cultures List of interacting cultures
     * @param interaction_strength Strength of each interaction (0.0-1.0)
     */
    void updateFromInteractions(const std::vector<std::pair<std::shared_ptr<Culture>, float>>& other_cultures);
    
    /**
     * @brief Apply cultural pressure from dominant ideology
     * @param ideology_pressure Pressure value (-1.0 to 1.0)
     * @param affected_traits Which traits are affected
     */
    void applyIdeologyPressure(float ideology_pressure, const std::vector<std::string>& affected_traits);
    
    /**
     * @brief Serialize culture to string
     */
    std::string serialize() const;
    
    /**
     * @brief Deserialize culture from string
     */
    static std::shared_ptr<Culture> deserialize(const std::string& data);
    
private:
    std::string name_;
    CulturalTraits traits_;
};

} // namespace hiersim

#endif // HIERSIM_CULTURE_H
