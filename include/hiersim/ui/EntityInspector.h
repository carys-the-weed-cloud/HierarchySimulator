#pragma once

#include <QWidget>
#include <QTreeWidget>
#include <QFormLayout>
#include <string>
#include <memory>

namespace hiersim {
    class Individual;
    class Organization;
    class Facility;
    class Unit;
    class WorldMap;
}

namespace hiersim::ui {
    /**
     * @brief Panel for inspecting simulation entities
     * 
     * Displays detailed information about selected entities
     * including Individuals, Organizations, Facilities, and Units.
     */
    class EntityInspector : public QWidget {
        Q_OBJECT

    public:
        explicit EntityInspector(QWidget *parent = nullptr);
        ~EntityInspector();

        void inspectEntity(const std::string& entityId);
        void inspectRegion(const std::string& regionId);
        void clear();

    private:
        void setupUi();
        void displayIndividual(const std::string& id);
        void displayOrganization(const std::string& id);
        void displayFacility(const std::string& id);
        void displayUnit(const std::string& id);
        void displayRegion(const std::string& id);
        
        QTabWidget* m_tabs;
        QTreeWidget* m_propertyTree;
        QWidget* m_regionTab;
        QFormLayout* m_regionLayout;
        
        std::string m_currentEntityId;
        std::string m_currentRegionId;
    };
}
