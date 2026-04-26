#include "hiersim/WorldMap.h"
#include <cmath>
#include <algorithm>
#include <queue>
#include <sstream>

namespace hiersim {

// Static member initialization
WorldMap::ID WorldMap::nextId_ = 1;

WorldMap::WorldMap(const std::string& name, ID id)
    : id_(id == 0 ? nextId_++ : id)
    , name_(name)
    , width_(1000)
    , height_(1000)
{
}

bool WorldMap::addRegion(const Region& region) {
    if (regions_.find(region.id) != regions_.end()) {
        return false;
    }
    regions_[region.id] = region;
    return true;
}

bool WorldMap::removeRegion(RegionID regionId) {
    auto it = regions_.find(regionId);
    if (it == regions_.end()) {
        return false;
    }
    
    adjacencyList_.erase(regionId);
    for (auto& [key, connections] : adjacencyList_) {
        connections.erase(regionId);
    }
    
    regions_.erase(it);
    return true;
}

Region* WorldMap::getRegion(RegionID regionId) {
    auto it = regions_.find(regionId);
    return it != regions_.end() ? &it->second : nullptr;
}

const Region* WorldMap::getRegion(RegionID regionId) const {
    auto it = regions_.find(regionId);
    return it != regions_.end() ? &it->second : nullptr;
}

Region* WorldMap::getRegionAt(double lat, double lon) {
    Region* closest = nullptr;
    double minDistance = std::numeric_limits<double>::max();
    
    for (auto& [id, region] : regions_) {
        double dLat = region.latitude - lat;
        double dLon = region.longitude - lon;
        double distance = std::sqrt(dLat * dLat + dLon * dLon);
        
        if (distance < minDistance) {
            minDistance = distance;
            closest = &region;
        }
    }
    
    return closest;
}

void WorldMap::connectRegions(RegionID regionId1, RegionID regionId2, double connectionStrength) {
    if (regions_.find(regionId1) == regions_.end() || 
        regions_.find(regionId2) == regions_.end()) {
        return;
    }
    
    connectionStrength = std::clamp(connectionStrength, 0.0, 1.0);
    adjacencyList_[regionId1][regionId2] = connectionStrength;
    adjacencyList_[regionId2][regionId1] = connectionStrength;
}

std::vector<std::pair<WorldMap::RegionID, double>> WorldMap::getConnectedRegions(RegionID regionId) const {
    std::vector<std::pair<RegionID, double>> result;
    
    auto it = adjacencyList_.find(regionId);
    if (it != adjacencyList_.end()) {
        for (const auto& [connectedId, strength] : it->second) {
            result.emplace_back(connectedId, strength);
        }
    }
    
    return result;
}

double WorldMap::getDistance(RegionID regionId1, RegionID regionId2) const {
    auto r1 = getRegion(regionId1);
    auto r2 = getRegion(regionId2);
    
    if (!r1 || !r2) {
        return std::numeric_limits<double>::max();
    }
    
    const double R = 6371.0;
    double dLat = (r2->latitude - r1->latitude) * M_PI / 180.0;
    double dLon = (r2->longitude - r1->longitude) * M_PI / 180.0;
    
    double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
               std::cos(r1->latitude * M_PI / 180.0) * std::cos(r2->latitude * M_PI / 180.0) *
               std::sin(dLon / 2) * std::sin(dLon / 2);
    
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    
    return R * c;
}

std::vector<WorldMap::RegionID> WorldMap::findPath(RegionID startId, RegionID endId) const {
    if (regions_.find(startId) == regions_.end() || 
        regions_.find(endId) == regions_.end()) {
        return {};
    }
    
    if (startId == endId) {
        return {startId};
    }
    
    std::map<RegionID, double> distances;
    std::map<RegionID, RegionID> previous;
    std::priority_queue<std::pair<double, RegionID>, 
                        std::vector<std::pair<double, RegionID>>, 
                        std::greater<>> pq;
    
    for (const auto& [id, region] : regions_) {
        distances[id] = std::numeric_limits<double>::max();
    }
    distances[startId] = 0;
    pq.push({0, startId});
    
    while (!pq.empty()) {
        auto [dist, current] = pq.top();
        pq.pop();
        
        if (dist > distances[current]) {
            continue;
        }
        
        if (current == endId) {
            break;
        }
        
        auto it = adjacencyList_.find(current);
        if (it == adjacencyList_.end()) {
            continue;
        }
        
        for (const auto& [neighbor, strength] : it->second) {
            double weight = (strength > 0) ? (1.0 / strength) * getDistance(current, neighbor) 
                                           : getDistance(current, neighbor);
            double alt = distances[current] + weight;
            
            if (alt < distances[neighbor]) {
                distances[neighbor] = alt;
                previous[neighbor] = current;
                pq.push({alt, neighbor});
            }
        }
    }
    
    std::vector<RegionID> path;
    RegionID current = endId;
    
    while (previous.find(current) != previous.end()) {
        path.insert(path.begin(), current);
        current = previous[current];
    }
    
    if (path.empty() || path.front() != startId) {
        return {};
    }
    
    path.insert(path.begin(), startId);
    return path;
}

std::vector<WorldMap::RegionID> WorldMap::getRegionsByOwner(uint64_t ownerId) const {
    std::vector<RegionID> result;
    
    for (const auto& [id, region] : regions_) {
        if (region.ownerId == ownerId) {
            result.push_back(id);
        }
    }
    
    return result;
}

bool WorldMap::transferOwnership(RegionID regionId, uint64_t newOwnerId) {
    auto region = getRegion(regionId);
    if (!region) {
        return false;
    }
    
    region->ownerId = newOwnerId;
    return true;
}

std::string WorldMap::serialize() const {
    std::ostringstream oss;
    oss << "WorldMap:" << id_ << ":" << name_ << ":" << width_ << ":" << height_ << "\n";
    
    for (const auto& [id, region] : regions_) {
        oss << "Region:" << region.id << ":" << region.name << ":"
            << region.latitude << ":" << region.longitude << ":"
            << region.area << ":" << static_cast<int>(region.terrain) << ":"
            << static_cast<int>(region.climate) << ":" << region.infrastructure << ":"
            << region.population << ":" << region.ownerId << "\n";
    }
    
    for (const auto& [regionId, connections] : adjacencyList_) {
        for (const auto& [connectedId, strength] : connections) {
            oss << "Connection:" << regionId << ":" << connectedId << ":" << strength << "\n";
        }
    }
    
    return oss.str();
}

WorldMap WorldMap::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string line;
    
    WorldMap map("Deserialized");
    
    while (std::getline(iss, line)) {
        if (line.empty()) continue;
        
        std::istringstream lineStream(line);
        std::string type;
        std::getline(lineStream, type, ':');
        
        if (type == "WorldMap") {
            std::string idStr, name, widthStr, heightStr;
            std::getline(lineStream, idStr, ':');
            std::getline(lineStream, name, ':');
            std::getline(lineStream, widthStr, ':');
            std::getline(lineStream, heightStr, ':');
            
            map.id_ = std::stoull(idStr);
            map.name_ = name;
            map.width_ = std::stoi(widthStr);
            map.height_ = std::stoi(heightStr);
        }
        else if (type == "Region") {
            Region region;
            std::string idStr, name, latStr, lonStr, areaStr, terrainStr, 
                       climateStr, infraStr, popStr, ownerStr;
            
            std::getline(lineStream, idStr, ':');
            std::getline(lineStream, name, ':');
            std::getline(lineStream, latStr, ':');
            std::getline(lineStream, lonStr, ':');
            std::getline(lineStream, areaStr, ':');
            std::getline(lineStream, terrainStr, ':');
            std::getline(lineStream, climateStr, ':');
            std::getline(lineStream, infraStr, ':');
            std::getline(lineStream, popStr, ':');
            std::getline(lineStream, ownerStr, ':');
            
            region.id = std::stoull(idStr);
            region.name = name;
            region.latitude = std::stod(latStr);
            region.longitude = std::stod(lonStr);
            region.area = std::stod(areaStr);
            region.terrain = static_cast<Region::Terrain>(std::stoi(terrainStr));
            region.climate = static_cast<Region::Climate>(std::stoi(climateStr));
            region.infrastructure = std::stoi(infraStr);
            region.population = std::stoll(popStr);
            region.ownerId = std::stoull(ownerStr);
            
            map.regions_[region.id] = region;
        }
        else if (type == "Connection") {
            std::string r1Str, r2Str, strengthStr;
            std::getline(lineStream, r1Str, ':');
            std::getline(lineStream, r2Str, ':');
            std::getline(lineStream, strengthStr, ':');
            
            RegionID r1 = std::stoull(r1Str);
            RegionID r2 = std::stoull(r2Str);
            double strength = std::stod(strengthStr);
            
            map.adjacencyList_[r1][r2] = strength;
        }
    }
    
    return map;
}

std::string terrainTypeToString(Region::Terrain terrain) {
    switch (terrain) {
        case Region::Terrain::Plains: return "Plains";
        case Region::Terrain::Mountains: return "Mountains";
        case Region::Terrain::Forest: return "Forest";
        case Region::Terrain::Desert: return "Desert";
        case Region::Terrain::Tundra: return "Tundra";
        case Region::Terrain::Jungle: return "Jungle";
        case Region::Terrain::Hills: return "Hills";
        case Region::Terrain::Coastal: return "Coastal";
        case Region::Terrain::Island: return "Island";
        default: return "Unknown";
    }
}

Region::Terrain stringToTerrainType(const std::string& str) {
    if (str == "Plains") return Region::Terrain::Plains;
    if (str == "Mountains") return Region::Terrain::Mountains;
    if (str == "Forest") return Region::Terrain::Forest;
    if (str == "Desert") return Region::Terrain::Desert;
    if (str == "Tundra") return Region::Terrain::Tundra;
    if (str == "Jungle") return Region::Terrain::Jungle;
    if (str == "Hills") return Region::Terrain::Hills;
    if (str == "Coastal") return Region::Terrain::Coastal;
    if (str == "Island") return Region::Terrain::Island;
    return Region::Terrain::Plains;
}

std::string climateTypeToString(Region::Climate climate) {
    switch (climate) {
        case Region::Climate::Tropical: return "Tropical";
        case Region::Climate::Arid: return "Arid";
        case Region::Climate::Temperate: return "Temperate";
        case Region::Climate::Continental: return "Continental";
        case Region::Climate::Polar: return "Polar";
        case Region::Climate::Mediterranean: return "Mediterranean";
        default: return "Unknown";
    }
}

Region::Climate stringToClimateType(const std::string& str) {
    if (str == "Tropical") return Region::Climate::Tropical;
    if (str == "Arid") return Region::Climate::Arid;
    if (str == "Temperate") return Region::Climate::Temperate;
    if (str == "Continental") return Region::Climate::Continental;
    if (str == "Polar") return Region::Climate::Polar;
    if (str == "Mediterranean") return Region::Climate::Mediterranean;
    return Region::Climate::Temperate;
}

} // namespace hiersim
