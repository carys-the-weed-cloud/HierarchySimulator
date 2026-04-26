#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <QTimer>
#include <memory>
#include <string>

namespace hiersim {
    class Simulation;
}

namespace hiersim::ui {
    class MapView;
    class EntityInspector;
    class TimeControlWidget;
    class LogPanel;

    /**
     * @brief Main application window for Hierarchy Simulator
     * 
     * Central hub integrating simulation loop, map view, and control panels.
     * Provides menu bar, toolbars, dock widgets, and status bar.
     */
    class MainWindow : public QMainWindow {
        Q_OBJECT

    public:
        explicit MainWindow(std::shared_ptr<Simulation> sim, QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void onTick();
        void onSpeedChanged(int ticksPerSecond);
        void onSimulationToggled(bool running);
        void onRegionSelected(const std::string& regionId);
        void onEntitySelected(const std::string& entityId);
        void loadScenario();
        void saveScenario();
        void toggleFullScreen();

    private:
        void setupUi();
        void setupMenuBar();
        void setupStatusBar();
        void connectSignals();

        std::shared_ptr<Simulation> m_simulation;
        
        // Central Widgets
        MapView* m_mapView;
        QStackedWidget* m_centerStack;
        
        // Dock Widgets
        EntityInspector* m_entityInspector;
        LogPanel* m_logPanel;
        
        // Toolbars
        TimeControlWidget* m_timeControl;
        
        QTimer m_tickTimer;
        bool m_isFullScreen;
    };
}
