# Hierarchy Simulator

## A Tick-Based Simulation of Alternate Worlds

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

### Phase 7: Integration & Testing (Next)
- [ ] Connect UI to simulation engine
- [ ] Implement full entity lookup and display
- [ ] Integrate Vulkan rendering for MapView
- [ ] End-to-end testing with default scenario

### Phase 8: Politics & Power Systems
- Ideology formation and spread
- Policy proposal and enactment
- Coalition formation
- Revolution and rebellion systems

### Phase 9: Technology & Progression
- Technology tree system
- Research Facilities and processes
- Automation effects on labor

### Phase 10: Solar System Expansion
- Space colonization mechanics
- Orbital infrastructure
- Interplanetary logistics

### Phase 11: Polish & 0.1 Release
- Comprehensive testing
- Tutorial and onboarding
- Multiple pre-built scenarios
- Public release

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
