#pragma once

#include "Simulation.h"
#include "TimeControlWidget.h"
#include "EntityInspector.h"
#include "LogPanel.h"
#include "MapView.h"

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QLabel>
#include <optional>
#include <string>

namespace hiersim {

/**
 * @brief Main application window for the Hierarchy Simulator.
 * 
 * Provides the central UI framework including menu bar, toolbars,
 * dock widgets, and status bar. Integrates all UI components and
 * manages communication between them.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    /**
     * @brief Initialize the main window with simulation data.
     * @param simulation Pointer to the active simulation instance.
     */
    void initialize(Simulation* simulation);

    /**
     * @brief Get the currently selected entity ID.
     * @return Optional entity ID if one is selected.
     */
    std::optional<std::string> getSelectedEntityId() const;

    /**
     * @brief Refresh the entity inspector with current data.
     * @param entityId ID of the entity to refresh.
     */
    void refreshEntityInspector(const std::string& entityId);

    /**
     * @brief Update the map view with latest simulation state.
     */
    void updateMapView();

public slots:
    /**
     * @brief Update the tick count display in status bar.
     * @param count Current tick count.
     */
    void updateTickCount(int count);

    /**
     * @brief Update simulation running state in UI.
     * @param running Is simulation running?
     */
    void updateSimulationState(bool running);

    /**
     * @brief Add an entry to the event log.
     * @param message Log message.
     */
    void addLogEntry(const QString& message);

signals:
    /**
     * @brief Emitted when user requests to load a scenario.
     * @param filePath Path to scenario file.
     */
    void loadScenarioRequested(const QString& filePath);

    /**
     * @brief Emitted when user requests to save a scenario.
     * @param filePath Path to save scenario to.
     */
    void saveScenarioRequested(const QString& filePath);

private:
    void createMenus();
    void createToolBars();
    void createDockWidgets();
    void createStatusBar();
    void connectSignals();

    // UI Components
    MapView* mapView_;
    TimeControlWidget* timeControlWidget_;
    EntityInspector* entityInspector_;
    LogPanel* logPanel_;
    
    // Status bar widgets
    QLabel* tickCountLabel_;
    QLabel* speedLabel_;
    QLabel* stateLabel_;
    
    // Menus
    QMenu* fileMenu_;
    QMenu* simulationMenu_;
    QMenu* viewMenu_;
    QMenu* helpMenu_;
    
    // Toolbars
    QToolBar* fileToolBar_;
    QToolBar* simulationToolBar_;
    
    Simulation* simulation_ = nullptr;
};

} // namespace hiersim