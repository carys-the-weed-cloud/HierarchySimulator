# HierarchySimulator

## A Gameified Simulation of Alternate Worlds

A Linux-based C++23, Qt6.11, Vulkan, and CMake-powered simulation game inspired by **Shadow Empire**, **Victoria 3**, **Democracy 3**, and **Terra Invicta**. Track, observe, follow, adjust, and dismantle hierarchies around the world to shape societies according to your vision.

---

## 🎮 Project Vision

From cyberpunk dystopias controlled by vertically integrated exploitative corporations to fully realized communist societies with no classes or hierarchies—simulate any economic or social system and watch it play out in a dynamic, agent-based world.

### Core Philosophy
- **Experimental Sandbox**: Test economic theories (Keynesian, Marxist, post-Keynesian, etc.) in a living simulation
- **Hierarchy-Focused Gameplay**: Manipulate power structures from unions to corporations to states
- **Emergent Complexity**: Watch complex social dynamics emerge from simple rules
- **Path to the Stars**: Guide civilizations from terrestrial politics to solar system colonization

### Time System
- **Tick-Based Simulation**: 1 tick = 1 day in-game time
- **Speed Control**: Player can adjust simulation speed from 1 tick/second (optimal) up to 6 ticks/second (maximum)
- **Pause & Step**: Full control to pause, step forward one tick at a time, or run at variable speeds

---

## 🏗️ Core Simulation Objects

### 1. **Individual**
People significant enough to be simulated as distinct agents rather than part of the aggregate population. These are key figures—leaders, innovators, organizers—who drive change.

### 2. **Organization**
Hierarchical groupings (vertical or horizontal) of people who cooperate to execute functions:
- States & Governments
- Corporations (private, state-owned, cooperative)
- Labor Unions
- Rebel Groups & Insurgencies
- Homeowners Associations
- Private Military Contractors
- Hospitals & Healthcare Systems
- Political Parties
- NGOs & Advocacy Groups

### 3. **Facility**
Physical structures or digital spaces used to execute societal functions:
- Factories & Production Facilities
- Makerspaces & Innovation Hubs
- Government Buildings (Pentagon analogs, Party Headquarters)
- Research Laboratories
- Digital Infrastructure (servers, networks)
- Military Bases
- Schools & Universities
- Markets & Trading Floors

### 4. **Unit**
Armed groupings of people that fight on behalf of others:
- Standing Armies
- Militias & Paramilitaries
- Peacekeeping Forces
- Mercenary Companies
- Police Forces
- Revolutionary Brigades

### 5. **Aggregate Population** *(Implicit)*
The masses represented statistically rather than as individual agents, providing labor, consumption, and political pressure.

---

## 🔧 Technical Stack

| Component | Technology |
|-----------|------------|
| **Language** | C++23 |
| **GUI Framework** | Qt 6.11 |
| **Graphics API** | Vulkan |
| **Build System** | CMake |
| **Target Platform** | Linux |
| **Simulation Engine** | Custom ECS-based architecture |
| **Economic Model** | Post-Keynesian with Marxist dynamics |
| **Target Hardware** | RTX 2080, 32GB RAM, i7-8700K |
| **Optimization Focus** | GPU-accelerated rendering, multi-threaded simulation, efficient memory management |

---

## 📋 Roadmap to Version 0.1

### Phase 1: Foundation (Weeks 1-4)
**Goal**: Basic project structure and build system

- [x] Initialize Git repository
- [x] Set up CMake build system with C++23 support
- [x] Configure Qt6.11 integration
- [x] Set up Vulkan initialization and window creation
- [x] Create basic project directory structure
- [x] Implement logging and configuration systems
- [x] Establish coding standards and documentation templates

**Deliverables**:
- [x] Compiling "Hello World" with Qt window and Vulkan context
- [x] Basic CI/CD pipeline
- [x] Developer onboarding documentation

**Status**: ✅ COMPLETE

### Phase 2: Core Data Structures (Weeks 5-8)
**Goal**: Implement the four core simulation objects

- [x] Design and implement `Individual` class with:
  - [x] Attributes (skills, relationships, ideology, wealth)
  - [x] Decision-making framework
  - [x] Memory and learning systems
  
- [x] Design and implement `Organization` class with:
  - [x] Hierarchical structure representation
  - [x] Internal democracy/autocracy spectrum
  - [x] Resource pooling and distribution
  - [x] Recruitment and membership systems
  
- [x] Design and implement `Facility` class with:
  - [x] Location and spatial relationships
  - [x] Function types (production, research, governance, military)
  - [x] Capacity and efficiency metrics
  - [x] Ownership and access control
  
- [x] Design and implement `Unit` class with:
  - [x] Composition (which Organizations equip them)
  - [x] Loyalty and morale systems
  - [x] Combat effectiveness modeling
  - [x] Deployment and movement

**Deliverables**:
- [x] All four core classes with serialization support
- [x] Unit tests for each class
- [x] Editor tools for manual object creation

**Status**: ✅ COMPLETE

### Phase 3: Economic Simulation Engine (Weeks 9-14)
**Goal**: Build the Keynesian-Marxist economic model

- [x] Implement commodity production and value theory
- [x] Create money and credit systems
- [x] Model wage labor and exploitation rates
- [x] Build supply chain networks between Facilities
- [x] Implement aggregate demand modeling (Keynesian)
- [x] Create class dynamics and surplus value extraction (Marxist)
- [x] Design unemployment and underemployment mechanics
- [x] Build investment and accumulation systems
- [x] Implement crisis tendencies (overproduction, financial instability)

**Deliverables**:
- [x] Running economic simulation with basic feedback loops
- [x] Visualization of economic flows
- [x] Scenario testing framework

**Status**: ✅ COMPLETE

### Phase 4: World Generation & Initialization (Weeks 15-18)
**Goal**: Create an alternate pre-built world

- [x] Design world map system (abstract or geographic)
- [x] Implement Region struct with terrain, climate, resources, infrastructure
- [x] Create WorldMap class with region management
- [x] Implement pathfinding (Dijkstra's algorithm)
- [x] Add distance calculations (Haversine formula)
- [x] Implement region ownership and transfer mechanics
- [x] Add serialization/deserialization support
- [ ] Generate initial distribution of Individuals
- [ ] Populate world with starting Organizations
- [ ] Place initial Facilities
- [ ] Create starting Units and military balances
- [ ] Establish initial economic conditions
- [ ] Build historical backstory generation

**Deliverables**:
- [x] WorldMap and Region classes with full implementation
- [x] Pathfinding and distance calculation systems
- [x] Region ownership mechanics
- [ ] Procedurally generated starting worlds
- [ ] Save/load system for world states
- [ ] At least one hand-crafted scenario

**Status**: 🔄 IN PROGRESS (Core map system complete, generation pending)

### Phase 5: Simulation Loop & Time System (Weeks 19-22)
**Goal**: Make the world run and evolve

- [ ] Implement discrete time step system (1 tick = 1 day)
- [ ] Create event scheduling and resolution
- [ ] Build decision queues for Individuals and Organizations
- [ ] Implement conflict resolution systems
- [ ] Create feedback loops between economy and politics
- [ ] Design crisis and tipping point detection
- [ ] Build statistical tracking and aggregation
- [ ] Implement player speed control (1-6 ticks/second)
- [ ] Add pause, play, and step-forward controls
- [ ] Optimize for target hardware (RTX 2080, i7-8700K, 32GB RAM)

**Deliverables**:
- Running simulation that evolves over time
- Basic AI for autonomous actors
- Replay and observation tools
- Variable speed control UI
- Performance benchmarks for target hardware

### Phase 6: User Interface & Interaction (Weeks 23-28)
**Goal**: Allow players to interact with the simulation

- [ ] Build main game view with Qt
- [ ] Create object inspection panels
- [ ] Implement hierarchy visualization (organization charts, network graphs)
- [ ] Design economic data dashboards
- [ ] Build intervention tools (create/modify/destroy objects)
- [ ] Create scenario editor
- [ ] Implement save/load UI
- [ ] Add camera controls and navigation (Vulkan rendering)

**Deliverables**:
- Playable alpha with full UI
- Interactive manipulation of all simulation objects
- Tick-based visualization of simulation state

### Phase 7: Politics & Power Systems (Weeks 29-34)
**Goal**: Model political dynamics and hierarchy manipulation

- [ ] Implement ideology formation and spread
- [ ] Create policy proposal and enactment systems
- [ ] Model legitimacy and authority
- [ ] Build coalition formation mechanics
- [ ] Design revolution and rebellion systems
- [ ] Implement reform vs. rupture dynamics
- [ ] Create hierarchy creation/dismantling tools
- [ ] Model state capacity and bureaucracy

**Deliverables**:
- Full political simulation layer
- Multiple paths to power (electoral, revolutionary, coup)
- Ideology editor and customization

### Phase 8: Technology & Progression (Weeks 35-38)
**Goal**: Enable technological development and colonization prep

- [ ] Build technology tree system
- [ ] Create research Facilities and processes
- [ ] Model knowledge diffusion
- [ ] Implement automation and its effects on labor
- [ ] Design spaceflight technology prerequisites
- [ ] Create tech level gating for game features

**Deliverables**:
- Working R&D system
- Technology-driven gameplay progression
- Unlockable late-game features

### Phase 9: Solar System Expansion (Weeks 39-44)
**Goal**: Add space colonization for late-game

- [ ] Design solar system map
- [ ] Create space-capable Units
- [ ] Model orbital infrastructure (stations, habitats)
- [ ] Implement resource extraction from celestial bodies
- [ ] Build colonization mechanics
- [ ] Design interplanetary logistics
- [ ] Create space-based Organizations

**Deliverables**:
- Functional space layer
- Multi-planet economies
- Late-game victory conditions

### Phase 10: Polish & 0.1 Release (Weeks 45-48)
**Goal**: Prepare for public release

- [ ] Comprehensive testing and bug fixing
- [ ] Performance optimization
- [ ] Tutorial and onboarding
- [ ] Documentation completion
- [ ] Example scenarios showcasing different economic systems
- [ ] Community feedback integration
- [ ] Version 0.1 release candidate
- [ ] Public release

**Deliverables**:
- **Version 0.1**: A playable, stable simulation with all core features
- Documentation for players and modders
- At least 5 pre-built scenarios (Cyberpunk, Social Democracy, State Socialism, Anarchist Federation, Libertarian Paradise)

---

## 🎯 Example Scenarios

### 1. Cyberpunk Dystopia
A world of strictly vertically controlled, exploitative commodity-producing workplaces. Mega-corporations dominate, unions are crushed, and the aggregate population lives in precarity. Can you organize resistance and build counter-power?

### 2. Social Democracy
A mixed economy with strong welfare states, regulated capitalism, and powerful labor unions. Experiment with balancing growth, equality, and sustainability.

### 3. State Socialism
A centrally planned economy with state ownership of major Facilities. Can you manage coordination problems and maintain innovation while eliminating exploitation?

### 4. Anarchist Federation
A society organized through horizontal Organizations, voluntary associations, and direct resource distribution. No monopoly on violence—can it survive against external threats?

### 5. Libertarian Paradise
Minimal state, maximum private property and free markets. Does it converge to oligarchy or sustain liberty?

### 6. Fully Realized Communism
No classes, no hierarchies, no state. Direct resource distribution based on need. People live freely without institutions that monopolize violence. Is it stable? Does it thrive?

---

## 📁 Project Structure (Planned)

```
HierarchySimulator/
├── CMakeLists.txt
├── README.md
├── LICENSE
├── docs/
│   ├── design/
│   ├── api/
│   └── tutorials/
├── src/
│   ├── core/           # Engine core, ECS, simulation loop
│   ├── objects/        # Individual, Organization, Facility, Unit
│   ├── economy/        # Economic simulation models
│   ├── politics/       # Political systems and ideologies
│   ├── world/          # Map, generation, geography
│   ├── space/          # Solar system mechanics
│   ├── ui/             # Qt-based user interface
│   ├── graphics/       # Vulkan rendering
│   └── utils/          # Logging, serialization, helpers
├── include/
│   └── hiersim/
├── tests/
│   ├── unit/
│   ├── integration/
│   └── scenarios/
├── assets/
│   ├── shaders/
│   ├── textures/
│   └── scenarios/
└── tools/
    ├── editor/
    └── scenario-builder/
```

---

## 🤝 Contributing

This is an open project welcoming contributors interested in:
- Economic simulation and modeling
- Political science and sociology
- Game development and UX
- C++ and systems programming
- Vulkan graphics programming
- Qt application development
- Scenario design and world-building

---

## 📄 License

[License details to be added]

---

## 🚀 Getting Started (Development)

### Prerequisites
- Linux (tested on latest Ubuntu/Fedora)
- C++23 compatible compiler (GCC 13+ or Clang 16+)
- CMake 3.28+
- Qt 6.11+
- Vulkan SDK
- Git

### Building
```bash
git clone https://github.com/[username]/HierarchySimulator.git
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

## 🎮 Version 0.1 Feature Checklist

**Core Simulation**:
- [x] Project initialized
- [ ] Individual agents with decision-making
- [ ] Organizations with hierarchical structures
- [ ] Facilities with production/services
- [ ] Units with combat capabilities
- [ ] Aggregate population modeling

**Economy**:
- [ ] Commodity production and exchange
- [ ] Money, wages, and profits
- [ ] Investment and accumulation
- [ ] Crisis dynamics

**Politics**:
- [ ] Ideology system
- [ ] Policy enactment
- [ ] Revolution/reform mechanics

**Interface**:
- [ ] Qt-based main window
- [ ] Object inspectors
- [ ] Economic dashboards
- [ ] Hierarchy visualizations

**Content**:
- [ ] 5+ pre-built scenarios
- [ ] Tutorial
- [ ] Documentation

**Technical**:
- [ ] Save/load system
- [ ] Performance targets met (10k+ Individuals, 1k+ Organizations)
- [ ] Cross-distro compatibility

---

*"The philosophers have only interpreted the world, in various ways. The point, however, is to change it."* — Karl Marx

*"In the long run we are all dead."* — John Maynard Keynes

Let's build a world where you can experiment with changing it.
