#include "hiersim/Technology.h"
#include <algorithm>

namespace hiersim {

// Technology implementation

bool Technology::canResearch(const std::set<std::string>& discovered_techs) const {
    for (const auto& prereq : prerequisites) {
        if (discovered_techs.find(prereq) == discovered_techs.end()) {
            return false;
        }
    }
    return true;
}

std::string Technology::domainToString(TechDomain domain) {
    switch (domain) {
        case TechDomain::Industrial: return "Industrial";
        case TechDomain::Agricultural: return "Agricultural";
        case TechDomain::Military: return "Military";
        case TechDomain::Medical: return "Medical";
        case TechDomain::Energy: return "Energy";
        case TechDomain::Computing: return "Computing";
        case TechDomain::Transportation: return "Transportation";
        case TechDomain::Materials: return "Materials";
        case TechDomain::Space: return "Space";
        case TechDomain::Social: return "Social";
        default: return "Unknown";
    }
}

std::string Technology::tierToString(TechTier tier) {
    switch (tier) {
        case TechTier::Primitive: return "Primitive";
        case TechTier::Early: return "Early Industrial";
        case TechTier::Modern: return "Modern";
        case TechTier::Advanced: return "Advanced";
        case TechTier::Futuristic: return "Futuristic";
        default: return "Unknown";
    }
}

const Technology* ResearchQueueItem::getTechnology() const {
    // This would need access to TechTree - simplified for now
    return nullptr;
}

// TechTree implementation

void TechTree::initializeDefaultTree() {
    // Industrial technologies
    technologies_["steam_engine"] = Technology{
        .id = "steam_engine",
        .name = "Steam Engine",
        .description = "Basic steam power for industrial machinery",
        .domain = TechDomain::Industrial,
        .tier = TechTier::Early,
        .prerequisites = {},
        .research_cost = 50,
        .productivity_bonus = 0.1f
    };
    
    technologies_["assembly_line"] = Technology{
        .id = "assembly_line",
        .name = "Assembly Line",
        .description = "Mass production techniques",
        .domain = TechDomain::Industrial,
        .tier = TechTier::Modern,
        .prerequisites = {"steam_engine"},
        .research_cost = 100,
        .productivity_bonus = 0.25f
    };
    
    technologies_["automation"] = Technology{
        .id = "automation",
        .name = "Industrial Automation",
        .description = "Robotic assembly and automated factories",
        .domain = TechDomain::Industrial,
        .tier = TechTier::Advanced,
        .prerequisites = {"assembly_line", "basic_computing"},
        .research_cost = 200,
        .productivity_bonus = 0.5f
    };
    
    // Agricultural technologies
    technologies_["crop_rotation"] = Technology{
        .id = "crop_rotation",
        .name = "Crop Rotation",
        .description = "Systematic crop rotation for better yields",
        .domain = TechDomain::Agricultural,
        .tier = TechTier::Early,
        .prerequisites = {},
        .research_cost = 40,
        .productivity_bonus = 0.15f
    };
    
    technologies_["green_revolution"] = Technology{
        .id = "green_revolution",
        .name = "Green Revolution",
        .description = "High-yield crop varieties and fertilizers",
        .domain = TechDomain::Agricultural,
        .tier = TechTier::Modern,
        .prerequisites = {"crop_rotation"},
        .research_cost = 120,
        .productivity_bonus = 0.4f
    };
    
    // Military technologies
    technologies_["gunpowder"] = Technology{
        .id = "gunpowder",
        .name = "Gunpowder Weapons",
        .description = "Basic firearms and artillery",
        .domain = TechDomain::Military,
        .tier = TechTier::Early,
        .prerequisites = {},
        .research_cost = 60,
        .military_bonus = 0.2f
    };
    
    technologies_["combined_arms"] = Technology{
        .id = "combined_arms",
        .name = "Combined Arms Warfare",
        .description = "Integrated use of infantry, armor, and air support",
        .domain = TechDomain::Military,
        .tier = TechTier::Modern,
        .prerequisites = {"gunpowder"},
        .research_cost = 150,
        .military_bonus = 0.35f
    };
    
    // Computing technologies
    technologies_["basic_computing"] = Technology{
        .id = "basic_computing",
        .name = "Basic Computing",
        .description = "Electronic computers and programming",
        .domain = TechDomain::Computing,
        .tier = TechTier::Modern,
        .prerequisites = {},
        .research_cost = 100,
        .productivity_bonus = 0.1f
    };
    
    technologies_["artificial_intelligence"] = Technology{
        .id = "artificial_intelligence",
        .name = "Artificial Intelligence",
        .description = "Advanced AI systems and machine learning",
        .domain = TechDomain::Computing,
        .tier = TechTier::Advanced,
        .prerequisites = {"basic_computing"},
        .research_cost = 300,
        .productivity_bonus = 0.3f,
        .quality_of_life_bonus = 0.2f
    };
    
    // Energy technologies
    technologies_["electricity"] = Technology{
        .id = "electricity",
        .name = "Electric Power",
        .description = "Generation and distribution of electric power",
        .domain = TechDomain::Energy,
        .tier = TechTier::Early,
        .prerequisites = {"steam_engine"},
        .research_cost = 80,
        .productivity_bonus = 0.15f
    };
    
    technologies_["nuclear_power"] = Technology{
        .id = "nuclear_power",
        .name = "Nuclear Power",
        .description = "Nuclear fission for energy generation",
        .domain = TechDomain::Energy,
        .tier = TechTier::Modern,
        .prerequisites = {"electricity"},
        .research_cost = 200,
        .productivity_bonus = 0.3f
    };
    
    // Space technologies
    technologies_["rocketry"] = Technology{
        .id = "rocketry",
        .name = "Rocketry",
        .description = "Basic rocket technology for space access",
        .domain = TechDomain::Space,
        .tier = TechTier::Modern,
        .prerequisites = {"electricity", "advanced_materials"},
        .research_cost = 250,
        .military_bonus = 0.2f
    };
    
    technologies_["space_colonization"] = Technology{
        .id = "space_colonization",
        .name = "Space Colonization",
        .description = "Technology for permanent space settlements",
        .domain = TechDomain::Space,
        .tier = TechTier::Advanced,
        .prerequisites = {"rocketry", "life_support"},
        .research_cost = 500,
        .quality_of_life_bonus = 0.3f
    };
    
    // Materials technologies
    technologies_["advanced_materials"] = Technology{
        .id = "advanced_materials",
        .name = "Advanced Materials",
        .description = "Composites and advanced alloys",
        .domain = TechDomain::Materials,
        .tier = TechTier::Modern,
        .prerequisites = {},
        .research_cost = 90,
        .productivity_bonus = 0.1f
    };
    
    // Medical technologies
    technologies_["vaccination"] = Technology{
        .id = "vaccination",
        .name = "Vaccination",
        .description = "Immunization against diseases",
        .domain = TechDomain::Medical,
        .tier = TechTier::Early,
        .prerequisites = {},
        .research_cost = 50,
        .quality_of_life_bonus = 0.2f
    };
    
    technologies_["life_support"] = Technology{
        .id = "life_support",
        .name = "Advanced Life Support",
        .description = "Medical technology for extreme environments",
        .domain = TechDomain::Medical,
        .tier = TechTier::Advanced,
        .prerequisites = {"vaccination"},
        .research_cost = 180,
        .quality_of_life_bonus = 0.25f
    };
}

const Technology* TechTree::getTechnology(const std::string& id) const {
    auto it = technologies_.find(id);
    return (it != technologies_.end()) ? &it->second : nullptr;
}

std::vector<const Technology*> TechTree::getTechnologiesByDomain(TechDomain domain) const {
    std::vector<const Technology*> result;
    for (const auto& [id, tech] : technologies_) {
        if (tech.domain == domain) {
            result.push_back(&tech);
        }
    }
    return result;
}

std::vector<const Technology*> TechTree::getAvailableTechnologies(
    const std::set<std::string>& discovered_techs) const {
    
    std::vector<const Technology*> result;
    for (const auto& [id, tech] : technologies_) {
        if (!tech.is_discovered && tech.canResearch(discovered_techs)) {
            result.push_back(&tech);
        }
    }
    return result;
}

bool TechTree::isDiscovered(const std::string& tech_id) const {
    return discovered_techs_.find(tech_id) != discovered_techs_.end();
}

void TechTree::discoverTechnology(const std::string& tech_id, int64_t tick) {
    if (technologies_.find(tech_id) != technologies_.end()) {
        discovered_techs_.insert(tech_id);
        technologies_[tech_id].is_discovered = true;
        technologies_[tech_id].discovery_tick = tick;
    }
}

float TechTree::getResearchProgress() const {
    if (technologies_.empty()) {
        return 0.0f;
    }
    return static_cast<float>(discovered_techs_.size()) / static_cast<float>(technologies_.size());
}

// ResearchManager implementation

ResearchManager::ResearchManager(const TechTree& tech_tree) : tech_tree_(tech_tree) {}

bool ResearchManager::queueResearch(const std::string& tech_id) {
    // Check if already in queue
    for (const auto& item : queue_) {
        if (item.technology_id == tech_id) {
            return false;
        }
    }
    
    // Check if already discovered
    if (tech_tree_.isDiscovered(tech_id)) {
        return false;
    }
    
    ResearchQueueItem item;
    item.technology_id = tech_id;
    item.progress = 0;
    item.is_active = false;
    queue_.push_back(item);
    
    return true;
}

bool ResearchManager::cancelResearch(const std::string& tech_id) {
    auto it = std::find_if(queue_.begin(), queue_.end(),
        [&tech_id](const ResearchQueueItem& item) {
            return item.technology_id == tech_id;
        });
    
    if (it != queue_.end()) {
        queue_.erase(it);
        return true;
    }
    return false;
}

bool ResearchManager::setActiveResearch(const std::string& tech_id, int slot) {
    if (slot < 0 || slot >= research_slots_) {
        return false;
    }
    
    // Deactivate all in this slot range
    int active_count = 0;
    for (auto& item : queue_) {
        if (item.is_active) {
            if (active_count == slot) {
                item.is_active = false;
            }
            active_count++;
        }
    }
    
    // Activate the requested one
    for (auto& item : queue_) {
        if (item.technology_id == tech_id) {
            item.is_active = true;
            return true;
        }
    }
    
    return false;
}

std::vector<std::string> ResearchManager::processTick(
    int research_points, 
    std::set<std::string>& discovered_techs) {
    
    std::vector<std::string> newly_discovered;
    
    // Count active research slots
    int active_count = 0;
    for (const auto& item : queue_) {
        if (item.is_active) {
            active_count++;
        }
    }
    
    if (active_count == 0 || research_points <= 0) {
        return newly_discovered;
    }
    
    // Distribute research points among active projects
    int points_per_project = research_points / active_count;
    
    for (auto& item : queue_) {
        if (!item.is_active) {
            continue;
        }
        
        const Technology* tech = tech_tree_.getTechnology(item.technology_id);
        if (!tech || tech_tree_.isDiscovered(item.technology_id)) {
            item.is_active = false;
            continue;
        }
        
        item.progress += points_per_project;
        
        if (item.progress >= tech->research_cost) {
            // Technology completed!
            discovered_techs.insert(item.technology_id);
            newly_discovered.push_back(item.technology_id);
            item.is_active = false;
            
            // Remove from queue or mark as complete
            item.progress = tech->research_cost;
        }
    }
    
    return newly_discovered;
}

} // namespace hiersim
