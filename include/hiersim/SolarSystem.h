#pragma once
#include "Location.h"
#include <string>
#include <vector>
#include <cstdint>

namespace hiersim {

/**
 * @brief Celestial body types in the solar system
 */
enum class CelestialBodyType {
    Star,           // The Sun
    Planet,         // Major planets
    DwarfPlanet,    // Pluto, Ceres, Eris, etc.
    Moon,           // Natural satellites
    Asteroid,       // Asteroid belt objects
    Comet,          // Cometary bodies
    SpaceStation,   // Artificial stations
    OrbitalHabitat, // O'Neill cylinders, Stanford tori
    SurfaceBase     // Surface installations on moons/planets
};

/**
 * @brief Resource deposits available on celestial bodies
 */
struct SpaceResource {
    std::string id;
    std::string name;
    double quantity = 0.0;  // In metric tons
    double extractionRate = 0.0;  // Tons per tick
    bool isDepleted = false;
    
    // Resource types
    enum Type {
        WaterIce,
        Helium3,
        RareEarthElements,
        PlatinumGroupMetals,
        Iron,
        Nickel,
        Titanium,
        Aluminum,
        Silicon,
        Hydrogen,
        Deuterium,
        Uranium,
        Thorium,
        Regolith,  // For construction
        Vacuum     // Special resource for orbital manufacturing
    };
    
    Type type = WaterIce;
};

/**
 * @brief Infrastructure that can be built in space
 */
struct SpaceInfrastructure {
    std::string id;
    std::string name;
    std::string ownerId;  // Organization ID
    
    enum class Type {
        MiningOutpost,
        ProcessingFacility,
        Refinery,
        ManufacturingPlant,
        SolarPowerStation,
        NuclearPowerStation,
        FuelDepot,
        CargoHub,
        PassengerTerminal,
        Shipyard,
        ResearchStation,
        MilitaryBase,
        CommunicationArray,
        TerraformingEquipment,
        MassDriver,      // Electromagnetic launcher
        SpaceElevator,   // Planetary tether
        OrbitalRing      // Advanced orbital structure
    };
    
    Type type = Type::MiningOutpost;
    Location location;
    double capacity = 100.0;
    double efficiency = 1.0;
    int population = 0;  // Workers stationed here
    double morale = 0.5;  // Worker morale (0-1)
    double integrity = 1.0;  // Structural integrity (0-1)
    bool isOperational = true;
    std::vector<std::string> inputResources;
    std::vector<std::string> outputResources;
    int constructionTick = 0;
};

/**
 * @brief Space vessels for transport and operations
 */
struct SpaceVessel {
    std::string id;
    std::string name;
    std::string ownerId;  // Organization ID
    
    enum class Type {
        CargoShip,
        Tanker,           // Fuel/resource transport
        PassengerLiner,
        MiningShip,
        ConstructionShip,
        Warship,
        PatrolBoat,
        ScienceVessel,
        ColonyShip,
        Tender            // Support vessel
    };
    
    Type type = Type::CargoShip;
    Location currentLocation;
    Location destinationLocation;
    double cargoCapacity = 1000.0;  // Tons
    double currentCargo = 0.0;
    double fuel = 100.0;  // Percentage
    double fuelCapacity = 100.0;
    double hullIntegrity = 1.0;  // 0-1
    double shields = 0.0;  // 0-1 if equipped
    int crew = 0;
    double morale = 0.5;  // Crew morale
    bool isMoving = false;
    int arrivalTick = -1;  // ETA in ticks
    double speed = 0.0;  // km/s effective
};

/**
 * @brief Represents a celestial body or orbital zone
 */
struct CelestialBody {
    std::string id;
    std::string name;
    CelestialBodyType type = CelestialBodyType::Planet;
    
    // Physical properties
    double mass = 0.0;  // kg
    double radius = 0.0;  // km
    double gravity = 0.0;  // m/s²
    double surfaceTemperature = 288.0;  // Kelvin (average)
    bool hasAtmosphere = false;
    std::string atmosphereComposition;  // e.g., "N2:78%, O2:21%"
    
    // Orbital properties (for natural bodies)
    std::string parentBodyId;  // Empty for Sun, planet ID for moons
    double orbitalPeriod = 0.0;  // Days
    double rotationalPeriod = 24.0;  // Hours
    double axialTilt = 0.0;  // Degrees
    double distanceFromSun = 0.0;  // AU
    
    // Strategic properties
    std::vector<SpaceResource> resources;
    std::vector<SpaceInfrastructure> infrastructure;
    std::vector<SpaceVessel> orbitingVessels;
    std::string controllingFaction;  // Organization ID
    double strategicValue = 1.0;  // 0-1 scale
    
    // Habitability (for colonization)
    double habitabilityIndex = 0.0;  // 0-1, affected by terraforming
    int population = 0;
    double lifeSupportCapacity = 0.0;  // Max population supportable
    
    // Terraforming progress (if applicable)
    double terraformingProgress = 0.0;  // 0-1
    int terraformingStartTick = -1;
};

/**
 * @brief Travel route between two locations in space
 */
struct SpaceRoute {
    std::string id;
    std::string originId;  // CelestialBody or Station ID
    std::string destinationId;
    double distance = 0.0;  // km or AU
    int travelTimeTicks = 0;  // Base travel time
    double difficulty = 1.0;  // Navigation difficulty multiplier
    bool isEstablished = false;  // Regular route with infrastructure
    std::vector<std::string> waypoints;  // Intermediate stops
};

/**
 * @brief Manages the solar system simulation
 */
class SolarSystem {
public:
    SolarSystem() = default;
    ~SolarSystem() = default;
    
    // Initialization
    void initializeDefaultSolarSystem();
    
    // Celestial body management
    bool addCelestialBody(const CelestialBody& body);
    bool removeCelestialBody(const std::string& id);
    CelestialBody* getCelestialBody(const std::string& id);
    const CelestialBody* getCelestialBody(const std::string& id) const;
    std::vector<CelestialBody*> getBodiesByType(CelestialBodyType type);
    std::vector<CelestialBody*> getAllBodies();
    
    // Infrastructure management
    bool addInfrastructure(const std::string& bodyId, const SpaceInfrastructure& infra);
    bool removeInfrastructure(const std::string& bodyId, const std::string& infraId);
    SpaceInfrastructure* getInfrastructure(const std::string& bodyId, const std::string& infraId);
    std::vector<SpaceInfrastructure*> getAllInfrastructure();
    
    // Vessel management
    bool addVessel(const std::string& bodyId, SpaceVessel vessel);
    bool removeVessel(const std::string& bodyId, const std::string& vesselId);
    SpaceVessel* getVessel(const std::string& vesselId);
    std::vector<SpaceVessel*> getAllVessels();
    
    // Route management
    bool addRoute(const SpaceRoute& route);
    bool removeRoute(const std::string& routeId);
    SpaceRoute* getRoute(const std::string& routeId);
    std::vector<SpaceRoute*> calculateRoute(const std::string& origin, const std::string& destination);
    
    // Simulation tick processing
    void processTick(int currentTick);
    void updateVesselMovement(int currentTick);
    void updateResourceExtraction();
    void updateInfrastructureOperations();
    void updatePopulationDynamics();
    
    // Colonization and terraforming
    bool attemptColonization(const std::string& bodyId, const std::string& orgId, int colonists);
    bool advanceTerraforming(const std::string& bodyId, double progress);
    
    // Combat and conflict
    bool initiateSpaceCombat(const std::string& bodyId, 
                            const std::vector<std::string>& attackerIds,
                            const std::vector<std::string>& defenderIds);
    
    // Serialization
    std::string serialize() const;
    bool deserialize(const std::string& data);
    
private:
    std::vector<CelestialBody> bodies_;
    std::vector<SpaceRoute> routes_;
    int currentTick_ = 0;
    
    // Helper functions
    double calculateTravelTime(const std::string& origin, const std::string& destination, 
                               SpaceVessel::Type vesselType, double techLevel);
    double calculateCombatOutcome(const std::vector<SpaceVessel*>& attackers,
                                  const std::vector<SpaceVessel*>& defenders);
};

} // namespace hiersim
