#include "hiersim/Culture.h"
#include "hiersim/StringUtils.h"
#include <cmath>
#include <sstream>
#include <algorithm>

namespace hiersim {

// CulturalTraits implementation

void CulturalTraits::blend(const CulturalTraits& other, float weight) {
    float inverse_weight = 1.0f - weight;
    individualism = individualism * inverse_weight + other.individualism * weight;
    tradition = tradition * inverse_weight + other.tradition * weight;
    innovation = innovation * inverse_weight + other.innovation * weight;
    hierarchy = hierarchy * inverse_weight + other.hierarchy * weight;
    materialism = materialism * inverse_weight + other.materialism * weight;
    risk_tolerance = risk_tolerance * inverse_weight + other.risk_tolerance * weight;
    long_term_orientation = long_term_orientation * inverse_weight + other.long_term_orientation * weight;
    universalism = universalism * inverse_weight + other.universalism * weight;
}

float CulturalTraits::distance(const CulturalTraits& other) const {
    float sum_sq = 0.0f;
    sum_sq += std::pow(individualism - other.individualism, 2);
    sum_sq += std::pow(tradition - other.tradition, 2);
    sum_sq += std::pow(innovation - other.innovation, 2);
    sum_sq += std::pow(hierarchy - other.hierarchy, 2);
    sum_sq += std::pow(materialism - other.materialism, 2);
    sum_sq += std::pow(risk_tolerance - other.risk_tolerance, 2);
    sum_sq += std::pow(long_term_orientation - other.long_term_orientation, 2);
    sum_sq += std::pow(universalism - other.universalism, 2);
    return std::sqrt(sum_sq / 8.0f); // Normalize by number of traits
}

std::string CulturalTraits::toString() const {
    std::ostringstream oss;
    oss << "CulturalTraits{";
    oss << "ind=" << individualism << ", ";
    oss << "trad=" << tradition << ", ";
    oss << "inn=" << innovation << ", ";
    oss << "hier=" << hierarchy << ", ";
    oss << "mat=" << materialism << ", ";
    oss << "risk=" << risk_tolerance << ", ";
    oss << "lto=" << long_term_orientation << ", ";
    oss << "univ=" << universalism;
    oss << "}";
    return oss.str();
}

// Culture implementation

Culture::Culture(const std::string& name) : name_(name) {}

void Culture::updateFromInteractions(
    const std::vector<std::pair<std::shared_ptr<Culture>, float>>& other_cultures) {
    
    if (other_cultures.empty()) {
        return;
    }
    
    CulturalTraits new_traits = traits_;
    float total_weight = 0.0f;
    
    for (const auto& [other_culture, strength] : other_cultures) {
        if (!other_culture || strength <= 0.0f) {
            continue;
        }
        
        // Calculate cultural distance - closer cultures blend more easily
        float distance_factor = 1.0f - std::min(1.0f, traits_.distance(other_culture->getTraits()));
        float effective_strength = strength * distance_factor;
        
        new_traits.blend(other_culture->getTraits(), effective_strength);
        total_weight += effective_strength;
    }
    
    if (total_weight > 0.0f) {
        // Apply blended changes gradually (culture changes slowly)
        float adaptation_rate = 0.1f; // 10% change per tick max
        traits_.blend(new_traits, std::min(total_weight, adaptation_rate));
    }
}

void Culture::applyIdeologyPressure(
    float ideology_pressure, 
    const std::vector<std::string>& affected_traits) {
    
    if (affected_traits.empty()) {
        return;
    }
    
    float pressure_per_trait = ideology_pressure / static_cast<float>(affected_traits.size());
    float change_rate = 0.05f; // 5% change per tick max
    
    for (const auto& trait_name : affected_traits) {
        float* trait_ptr = nullptr;
        
        if (trait_name == "individualism") {
            trait_ptr = &traits_.individualism;
        } else if (trait_name == "tradition") {
            trait_ptr = &traits_.tradition;
        } else if (trait_name == "innovation") {
            trait_ptr = &traits_.innovation;
        } else if (trait_name == "hierarchy") {
            trait_ptr = &traits_.hierarchy;
        } else if (trait_name == "materialism") {
            trait_ptr = &traits_.materialism;
        } else if (trait_name == "risk_tolerance") {
            trait_ptr = &traits_.risk_tolerance;
        } else if (trait_name == "long_term_orientation") {
            trait_ptr = &traits_.long_term_orientation;
        } else if (trait_name == "universalism") {
            trait_ptr = &traits_.universalism;
        }
        
        if (trait_ptr) {
            float target_value = (ideology_pressure > 0) ? 1.0f : 0.0f;
            float delta = (target_value - *trait_ptr) * std::abs(pressure_per_trait) * change_rate;
            *trait_ptr = std::clamp(*trait_ptr + delta, 0.0f, 1.0f);
        }
    }
}

std::string Culture::serialize() const {
    std::ostringstream oss;
    oss << "Culture{\n";
    oss << "  name: " << name_ << "\n";
    oss << "  traits: {\n";
    oss << "    individualism: " << traits_.individualism << "\n";
    oss << "    tradition: " << traits_.tradition << "\n";
    oss << "    innovation: " << traits_.innovation << "\n";
    oss << "    hierarchy: " << traits_.hierarchy << "\n";
    oss << "    materialism: " << traits_.materialism << "\n";
    oss << "    risk_tolerance: " << traits_.risk_tolerance << "\n";
    oss << "    long_term_orientation: " << traits_.long_term_orientation << "\n";
    oss << "    universalism: " << traits_.universalism << "\n";
    oss << "  }\n";
    oss << "}";
    return oss.str();
}

std::shared_ptr<Culture> Culture::deserialize(const std::string& data) {
    auto culture = std::make_shared<Culture>();
    
    // Simple parsing - in production use proper parser
    culture->name_ = StringUtils::extractValue(data, "name:");
    
    culture->traits_.individualism = std::stof(StringUtils::extractValue(data, "individualism:"));
    culture->traits_.tradition = std::stof(StringUtils::extractValue(data, "tradition:"));
    culture->traits_.innovation = std::stof(StringUtils::extractValue(data, "innovation:"));
    culture->traits_.hierarchy = std::stof(StringUtils::extractValue(data, "hierarchy:"));
    culture->traits_.materialism = std::stof(StringUtils::extractValue(data, "materialism:"));
    culture->traits_.risk_tolerance = std::stof(StringUtils::extractValue(data, "risk_tolerance:"));
    culture->traits_.long_term_orientation = std::stof(StringUtils::extractValue(data, "long_term_orientation:"));
    culture->traits_.universalism = std::stof(StringUtils::extractValue(data, "universalism:"));
    
    return culture;
}

} // namespace hiersim
