cmake_minimum_required(VERSION 3.28)
project(HierarchySimulator VERSION 0.1.0 LANGUAGES CXX)

# Set C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Find required packages
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Vulkan REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Source files for core library
set(CORE_SOURCES
    src/core/Simulation.cpp
    src/core/TimeController.cpp
    src/core/EventManager.cpp
    src/core/Logger.cpp
    src/core/Config.cpp
)

# Source files for simulation objects
set(OBJECTS_SOURCES
    src/objects/Individual.cpp
    src/objects/Organization.cpp
    src/objects/Facility.cpp
    src/objects/Unit.cpp
)

# Source files for economy
set(ECONOMY_SOURCES
    src/economy/EconomicModel.cpp
    src/economy/Commodity.cpp
    src/economy/Market.cpp
)

# Source files for politics
set(POLITICS_SOURCES
    src/politics/Ideology.cpp
    src/politics/Policy.cpp
    src/politics/Coalition.cpp
    src/politics/PoliticsModule.cpp
)

# Source files for culture
set(CULTURE_SOURCES
    src/culture/Culture.cpp
)

# Source files for technology
set(TECHNOLOGY_SOURCES
    src/technology/Technology.cpp
)

# Source files for law
set(LAW_SOURCES
    src/law/Law.cpp
)

# Source files for world
set(WORLD_SOURCES
    src/world/WorldMap.cpp
    src/world/Location.cpp
    src/world/Scenario.cpp
    src/world/ScenarioManager.cpp
)

# Source files for UI
set(UI_SOURCES
    src/ui/MainWindow.cpp
    src/ui/MapView.cpp
    src/ui/EntityInspector.cpp
    src/ui/TimeControlWidget.cpp
    src/ui/LogPanel.cpp
    src/ui/ObjectInspector.cpp
    src/ui/Dashboard.cpp
    src/ui/HierarchyView.cpp
)

# Source files for graphics
set(GRAPHICS_SOURCES
    src/graphics/VulkanRenderer.cpp
    src/graphics/VulkanContext.cpp
    src/graphics/Shaders.cpp
)

# Source files for utils
set(UTILS_SOURCES
    src/utils/Serializer.cpp
    src/utils/Random.cpp
    src/utils/StringUtils.cpp
)

# Header files
set(HEADERS
    include/hiersim/Simulation.h
    include/hiersim/TimeController.h
    include/hiersim/EventManager.h
    include/hiersim/Logger.h
    include/hiersim/Config.h
    include/hiersim/Individual.h
    include/hiersim/Organization.h
    include/hiersim/Facility.h
    include/hiersim/Unit.h
    include/hiersim/EconomicModel.h
    include/hiersim/Commodity.h
    include/hiersim/Market.h
    include/hiersim/Ideology.h
    include/hiersim/Policy.h
    include/hiersim/Coalition.h
    include/hiersim/PoliticsModule.h
    include/hiersim/Culture.h
    include/hiersim/Technology.h
    include/hiersim/Law.h
    include/hiersim/WorldMap.h
    include/hiersim/Location.h
    include/hiersim/Scenario.h
    include/hiersim/ScenarioManager.h
    include/hiersim/ui/MainWindow.h
    include/hiersim/ui/MapView.h
    include/hiersim/ui/EntityInspector.h
    include/hiersim/ui/TimeControlWidget.h
    include/hiersim/ui/LogPanel.h
    include/hiersim/ObjectInspector.h
    include/hiersim/Dashboard.h
    include/hiersim/HierarchyView.h
    include/hiersim/VulkanRenderer.h
    include/hiersim/VulkanContext.h
    include/hiersim/Shaders.h
    include/hiersim/Serializer.h
    include/hiersim/Random.h
    include/hiersim/StringUtils.h
)

# Create main executable
add_executable(hiersim
    src/main.cpp
    ${CORE_SOURCES}
    ${OBJECTS_SOURCES}
    ${ECONOMY_SOURCES}
    ${POLITICS_SOURCES}
    ${CULTURE_SOURCES}
    ${TECHNOLOGY_SOURCES}
    ${LAW_SOURCES}
    ${WORLD_SOURCES}
    ${UI_SOURCES}
    ${GRAPHICS_SOURCES}
    ${UTILS_SOURCES}
    ${HEADERS}
)

# Link libraries
target_link_libraries(hiersim PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Vulkan::Vulkan
)

# Enable testing
enable_testing()

# Add unit tests
add_executable(unit_tests
    tests/unit/test_Individual.cpp
    tests/unit/test_Organization.cpp
    tests/unit/test_Facility.cpp
    tests/unit/test_Unit.cpp
)

target_link_libraries(unit_tests PRIVATE
    Qt6::Core
)

add_test(NAME UnitTests COMMAND unit_tests)

# Installation
install(TARGETS hiersim DESTINATION bin)
install(DIRECTORY assets/ DESTINATION share/hiersim/assets)

# CPack configuration for packaging
set(CPACK_PACKAGE_NAME "HierarchySimulator")
set(CPACK_PACKAGE_VERSION "0.1.0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A gameified simulation of alternate worlds")
include(CPack)
