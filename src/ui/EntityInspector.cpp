#include "hiersim/ui/EntityInspector.h"
#include "hiersim/Simulation.h"
#include "hiersim/Individual.h"
#include "hiersim/Organization.h"
#include "hiersim/Facility.h"
#include "hiersim/Unit.h"
#include "hiersim/WorldMap.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>

namespace hiersim::ui {

EntityInspector::EntityInspector(QWidget *parent)
    : QWidget(parent)
    , m_tabs(nullptr)
    , m_propertyTree(nullptr)
    , m_regionTab(nullptr)
    , m_regionLayout(nullptr)
    , m_simulation(nullptr)
{
    setupUi();
}

EntityInspector::~EntityInspector() = default;

void EntityInspector::initialize(Simulation* simulation) {
    m_simulation = simulation;
}

std::optional<std::string> EntityInspector::getSelectedEntityId() const {
    if (!m_currentEntityId.empty()) {
        return m_currentEntityId;
    }
    return std::nullopt;
}

void EntityInspector::refresh(const std::string& entityId) {
    if (!entityId.empty()) {
        inspectEntity(entityId);
    }
}

void EntityInspector::setupUi() {
    auto* layout = new QVBoxLayout(this);
    
    m_tabs = new QTabWidget(this);
    
    // Entity details tab
    QWidget* entityTab = new QWidget();
    auto* entityLayout = new QVBoxLayout(entityTab);
    
    m_propertyTree = new QTreeWidget();
    m_propertyTree->setHeaderLabels({"Property", "Value"});
    m_propertyTree->setAlternatingRowColors(true);
    entityLayout->addWidget(m_propertyTree);
    
    m_tabs->addTab(entityTab, "Entity");
    
    // Region details tab
    m_regionTab = new QWidget();
    m_regionLayout = new QFormLayout(m_regionTab);
    m_tabs->addTab(m_regionTab, "Region");
    
    layout->addWidget(m_tabs);
    
    clear();
}

void EntityInspector::inspectEntity(const std::string& entityId) {
    m_currentEntityId = entityId;
    m_tabs->setCurrentIndex(0);
    
    // Clear existing properties
    m_propertyTree->clear();
    
    // TODO: Look up entity in simulation and display properties
    // For now, show placeholder
    auto* idItem = new QTreeWidgetItem(m_propertyTree);
    idItem->setText(0, "ID");
    idItem->setText(1, QString::fromStdString(entityId));
    
    auto* typeItem = new QTreeWidgetItem(m_propertyTree);
    typeItem->setText(0, "Type");
    typeItem->setText(1, "Unknown (entity lookup not implemented)");
}

void EntityInspector::inspectRegion(const std::string& regionId) {
    m_currentRegionId = regionId;
    m_tabs->setCurrentIndex(1);
    
    // Clear existing fields
    QLayoutItem* item;
    while ((item = m_regionLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    // TODO: Look up region in world map and display properties
    // For now, show placeholder
    m_regionLayout->addRow(new QLabel("Region ID:"), 
                           new QLabel(QString::fromStdString(regionId)));
    m_regionLayout->addRow(new QLabel("Terrain:"), 
                           new QLabel("Unknown (region lookup not implemented)"));
    m_regionLayout->addRow(new QLabel("Climate:"), 
                           new QLabel("Unknown"));
    m_regionLayout->addRow(new QLabel("Owner:"), 
                           new QLabel("None"));
}

void EntityInspector::clear() {
    m_currentEntityId.clear();
    m_currentRegionId.clear();
    m_propertyTree->clear();
    
    // Clear region tab
    QLayoutItem* item;
    while ((item = m_regionLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    m_regionLayout->addRow(new QLabel("No region selected"), new QLabel(""));
}

void EntityInspector::displayIndividual(const std::string& id) {
    // TODO: Implement individual display
}

void EntityInspector::displayOrganization(const std::string& id) {
    // TODO: Implement organization display
}

void EntityInspector::displayFacility(const std::string& id) {
    // TODO: Implement facility display
}

void EntityInspector::displayUnit(const std::string& id) {
    // TODO: Implement unit display
}

void EntityInspector::displayRegion(const std::string& id) {
    // TODO: Implement region display
}

} // namespace hiersim::ui
