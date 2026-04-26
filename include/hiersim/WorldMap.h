#ifndef HIERSIM_WORLD_MAP_H
#define HIERSIM_WORLD_MAP_H

#include "hiersim/Location.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cstdint>

namespace hiersim {

/**
 * @brief Represents a region/province on the world map.
 */
struct Region {
    uint64_t id;
    std::string name;
    double latitude;
    double longitude;
    double area;  // in km²
    
    // Resources available
    std::map<std::string, double> resources;
    
    // Terrain type
    enum class Terrain {
        Plains,
        Mountains,
        Forest,
        Desert,
        Tundra,
        Jungle,
        Hills,
        Coastal,
        Island
    };
    
    Terrain terrain;
    
    // Climate
    enum class Climate {
        Tropical,
        Arid,
        Temperate,
        Continental,
        Polar,
        Mediterranean
    };
    
    Climate climate;
    
    // Infrastructure level (0-100)
    int infrastructure;
    
    // Population (aggregate, not individual agents)
    int64_t population;
    
    // Owner organization ID (0 if unowned)
    uint64_t ownerId;
};

/**
 * @brief Represents the world map for the simulation.
 * 
 * The world map can be abstract (nodes and connections) or geographic
 * (coordinate-based with regions). This implementation uses a hybrid approach.
 */
class WorldMap {
public:
    using ID = uint64_t;
    using RegionID = uint64_t;
    
    /**
     * @brief Construct a new WorldMap object
     * @param name Map name
     * @param id Unique identifier
     */
    explicit WorldMap(const std::string& name, ID id = 0);
    ~WorldMap() = default;
    
    // Getters
    ID getId() const { return id_; }
    const std::string& getName() const { return name_; }
    const std::map<RegionID, Region>& getRegions() const { return regions_; }
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    
    /**
     * @brief Add a region to the map
     * @param region Region to add
     * @return true if added successfully
     */
    bool addRegion(const Region& region);
    
    /**
     * @brief Remove a region from the map
     * @param regionId ID of region to remove
     * @return true if removed
     */
    bool removeRegion(RegionID regionId);
    
    /**
     * @brief Get a region by ID
     * @param regionId ID of the region
     * @return Pointer to region, nullptr if not found
     */
    Region* getRegion(RegionID regionId);
    const Region* getRegion(RegionID regionId) const;
    
    /**
     * @brief Get region at coordinates
     * @param lat Latitude
     * @param lon Longitude
     * @return Pointer to region, nullptr if not found
     */
    Region* getRegionAt(double lat, double lon);
    
    /**
     * @brief Connect two regions (for abstract maps)
     * @param regionId1 First region
     * @param regionId2 Second region
     * @param connectionStrength Strength of connection (0-1)
     */
    void connectRegions(RegionID regionId1, RegionID regionId2, double connectionStrength = 1.0);
    
    /**
     * @brief Get connected regions
     * @param regionId ID of the region
     * @return Vector of connected region IDs with strength
     */
    std::vector<std::pair<RegionID, double>> getConnectedRegions(RegionID regionId) const;
    
    /**
     * @brief Calculate distance between two regions
     * @param regionId1 First region
     * @param regionId2 Second region
     * @return Distance in km (or abstract units)
     */
    double getDistance(RegionID regionId1, RegionID regionId2) const;
    
    /**
     * @brief Find path between two regions
     * @param startId Starting region
     * @param endId Destination region
     * @return Vector of region IDs representing the path
     */
    std::vector<RegionID> findPath(RegionID startId, RegionID endId) const;
    
    /**
     * @brief Get all regions owned by an organization
     * @param ownerId Organization ID
     * @return Vector of region IDs
     */
    std::vector<RegionID> getRegionsByOwner(uint64_t ownerId) const;
    
    /**
     * @brief Transfer ownership of a region
     * @param regionId Region to transfer
     * @param newOwnerId New owner organization ID
     * @return true if successful
     */
    bool transferOwnership(RegionID regionId, uint64_t newOwnerId);
    
    /**
     * @brief Serialize map data
     * @return Serialized string representation
     */
    std::string serialize() const;
    
    /**
     * @brief Deserialize map from string
     * @param data Serialized data
     * @return Deserialized WorldMap object
     */
    static WorldMap deserialize(const std::string& data);
    
private:
    ID id_;
    std::string name_;
    int width_;   // Map dimensions (for display)
    int height_;
    
    std::map<RegionID, Region> regions_;
    
    // Adjacency list for graph-based navigation
    std::map<RegionID, std::map<RegionID, double>> adjacencyList_;
    
    static ID nextId_;
};

/**
 * @brief Convert terrain type to string
 */
std::string terrainTypeToString(Region::Terrain terrain);

/**
 * @brief Convert string to terrain type
 */
Region::Terrain stringToTerrainType(const std::string& str);

/**
 * @brief Convert climate type to string
 */
std::string climateTypeToString(Region::Climate climate);

/**
 * @brief Convert string to climate type
 */
Region::Climate stringToClimateType(const std::string& str);

} // namespace hiersim

#endif // HIERSIM_WORLD_MAP_H