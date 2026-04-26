# Hierarchy Simulator

## A Tick-Based Simulation of Alternate Worlds

![CMake Build Checker](https://github.com/carys-the-weed-cloud/HierarchySimulator/actions/workflows/cmake-build.yml/badge.svg)

A Linux-based C++23, Qt6, Vulkan, and CMake-powered simulation game inspired by **Shadow Empire**, **Victoria 3**, **Democracy 3**, and **Terra Invicta**. Track, observe, follow, adjust, and dismantle hierarchies around the world to shape societies according to your vision.

---

## 🎮 Project Vision

From cyberpunk dystopias controlled by exploitative corporations to fully realized communist societies with no classes or hierarchies—simulate any economic or social system and watch it play out in a dynamic, agent-based world.

### Core Philosophy
- **Experimental Sandbox**: Test economic theories (Keynesian, Marxist, post-Keynesian, etc.) in a living simulation
- **Hierarchy-Focused Gameplay**: Manipulate power structures from unions to corporations to states
- **Emergent Complexity**: Watch complex social dynamics emerge from simple rules
- **Path to the Stars**: Guide civilizations from terrestrial politics to solar system colonization

### Time System
- **Tick-Based Simulation**: 1 tick = 1 day in-game time
- **Speed Control**: Adjustable from 1 tick/second (optimal) up to 6 ticks/second (maximum)
- **Full Control**: Pause, step forward one tick at a time, or run at variable speeds

### Target Hardware
- **GPU**: NVIDIA RTX 2080
- **CPU**: Intel i7-8700K
- **RAM**: 32GB
- **Optimization Focus**: GPU-accelerated rendering, multi-threaded simulation, efficient memory management

---

## ✅ Completed Features

### Phase 1: Foundation ✅
- Git repository initialized
- CMake build system with C++23, Qt6, and Vulkan support
- Project directory structure
- Logging and configuration systems

### Phase 2: Core Data Structures ✅
- **Individual**: Attributes, decision-making, memory/learning systems
- **Organization**: Hierarchical structures, democracy/autocracy spectrum, resource management
- **Facility**: 18 facility types, capacity/efficiency, ownership/access control
- **Unit**: Composition tracking, morale/loyalty, combat effectiveness, deployment

### Phase 3: Economic Simulation Engine ✅
- **Commodity**: 8 commodity types, labor theory of value, depreciation
- **Market**: Order book, market clearing, transaction history, price statistics
- **EconomicModel**: 6 economic systems, macroeconomic indicators, crisis detection

### Phase 4: World & Scenarios ✅
- **WorldMap**: Region management, terrain/climate types, pathfinding (Dijkstra)
- **Scenario**: Complete game state representation, text-based save/load
- **ScenarioManager**: Scenario discovery, validation, default scenario included
- **Design**: Single hand-crafted starting scenario + player scenario editor tools

### Phase 5: Simulation Loop & Time System ✅
- Discrete time step system (1 tick = 1 day)
- Event scheduling and resolution
- Multi-threaded simulation support
- Variable speed control (1-6 ticks/second)
- Simulation and TimeController classes

### Phase 6: User Interface & Interaction ✅
- **MainWindow**: Central hub with menu bar, toolbars, dock widgets, status bar
- **MapView**: Interactive map visualization with pan/zoom, region selection
- **EntityInspector**: Dynamic property display for all entity types
- **TimeControlWidget**: Play/pause, step forward, speed slider (1-6 TPS)
- **LogPanel**: Event logging with timestamps and importance levels

---

## 📋 Remaining Roadmap

### Phase 7: Integration & Testing ✅
- Connected UI to simulation engine via signals/slots
- Implemented full entity lookup and dynamic display in EntityInspector
- Integrated Vulkan rendering pipeline for MapView with region visualization
- End-to-end testing completed with default scenario
- All core systems integrated and functional

### Phase 8: Politics & Power Systems ✅
- **Ideology System**: 7 base ideologies (Capitalism, Socialism, Communism, Fascism, Anarchism, Liberalism, Conservatism) with economic left/right, authoritarian/libertarian, and social progressive axes
- **Coalition Formation**: Organizations can form coalitions with stability mechanics based on ideological coherence
- **Policy System**: Policy proposal, enactment, and repeal mechanics with coalition support calculation
- **Political Dynamics**: Ideology spread, coalition stability updates, revolution/rebellion condition checking
- **PoliticsModule**: Central politics manager integrating all political systems

### Phase 9: Advanced Simulation Mechanics ✅
- **Culture System**: 8 cultural traits (individualism, tradition, innovation, hierarchy, materialism, risk tolerance, long-term orientation, universalism), cultural diffusion between regions, ideology pressure effects, assimilation/resistance mechanics
- **Technology System**: 10 technology domains (Industrial, Agricultural, Military, Medical, Energy, Computing, Transportation, Materials, Space, Social), node-based tech tree with prerequisites, research queues, discovery events, 15+ default technologies
- **Legal System**: 5 law categories (Economic, Social, Political, Military, Environmental), enforcement levels (None to Total), compliance modeling based on ideology alignment and fear factor, legislative process with voting
- **Integration**: All systems interact - Tech boosts Economy/Military, Culture affects Politics/Law compliance, Law restricts/enables Economy/Politics

### Phase 10: Solar System Expansion ✅
- **Celestial Bodies**: Complete solar system model with Sun, 8 planets, dwarf planets, moons, asteroid belt - each with physical/orbital properties, resource deposits, and strategic value
- **Space Resources**: 15 resource types including Water Ice, Helium-3, Rare Earth Elements, Platinum Group Metals, Deuterium, Hydrogen
- **Space Infrastructure**: 17 infrastructure types (Mining Outposts, Refineries, Shipyards, Solar/Nuclear Power Stations, Fuel Depots, Mass Drivers, Space Elevators, Orbital Rings)
- **Space Vessels**: 10 vessel types (Cargo Ships, Tankers, Warships, Colony Ships, Mining Ships, Science Vessels) with movement, fuel, cargo, and combat mechanics
- **Colonization**: Habitability index, life support capacity, population dynamics, terraforming mechanics
- **Space Routes**: Travel time calculation, route difficulty, waypoint system
- **Space Combat**: Vessel-based combat resolution with base defense integration
- **Default Solar System**: Pre-configured Sol system with accurate planetary data, resource distributions, and initial Earth-Moon/Mars routes

### Phase 11: Polish & 0.1 Release ✅ COMPLETE
- ✅ Comprehensive testing framework with unit tests for all core systems
- ✅ Tutorial scenario with guided onboarding
- ✅ Default Earth 2024 scenario fully implemented
- ✅ CI/CD pipeline with GitHub Actions build verification
- ✅ Documentation complete with building instructions
- 🔄 Performance optimization for target hardware (in progress)
- 🔄 UI polish and accessibility improvements (in progress)
- 🎯 Public alpha release preparation

---

## 🔧 Technical Stack

| Component | Technology |
|-----------|------------|
| **Language** | C++23 |
| **GUI Framework** | Qt 6 |
| **Graphics API** | Vulkan |
| **Build System** | CMake |
| **Target Platform** | Linux |

---

## 📁 Project Structure

```
HierarchySimulator/
├── CMakeLists.txt
├── README.md
├── LICENSE
├── include/hiersim/
│   ├── ui/              # Qt UI components
│   ├── *.h              # Core simulation headers
│   └── ...
├── src/
│   ├── ui/              # UI implementations
│   ├── core/            # Engine core
│   ├── objects/         # Individual, Organization, Facility, Unit
│   ├── economy/         # Economic simulation
│   ├── politics/        # Political systems
│   ├── world/           # Map and scenarios
│   ├── graphics/        # Vulkan rendering
│   └── utils/           # Utilities
├── tests/
├── assets/
│   └── scenarios/       # Scenario files
└── tools/
```

---

## 🚀 Getting Started

### Prerequisites
- Linux (Ubuntu/Fedora)
- C++23 compiler (GCC 13+ or Clang 16+)
- CMake 3.28+
- Qt 6+
- Vulkan SDK
- Git

### Installing Dependencies (Ubuntu)
```bash
sudo apt-get update
sudo apt-get install -y \
  ninja-build \
  libqt6core6 \
  libqt6widgets6 \
  libqt6gui6 \
  libvulkan-dev \
  vulkan-validationlayers \
  cmake \
  g++ \
  git
```

### Building
```bash
git clone https://github.com/carys-the-weed-cloud/HierarchySimulator.git
cd HierarchySimulator
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
./hiersim
```

### Running Tests
```bash
cd build
ctest --output-on-failure
```

---

*Let's build a world where you can experiment with changing it.*
