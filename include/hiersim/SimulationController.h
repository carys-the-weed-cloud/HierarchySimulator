#pragma once

#include "ui/MainWindow.h"
#include "Simulation.h"
#include "TimeController.h"
#include "ScenarioManager.h"

#include <QObject>
#include <QTimer>
#include <memory>

namespace hiersim {

/**
 * @brief Bridges the backend simulation engine with the frontend UI.
 * 
 * This controller manages the lifecycle of the Simulation object,
 * connects TimeController signals to simulation steps, and updates
 * UI components when simulation state changes.
 */
class SimulationController : public QObject {
    Q_OBJECT

public:
    explicit SimulationController(QObject* parent = nullptr);
    ~SimulationController() override;

    /**
     * @brief Initialize the controller with a specific scenario.
     * @param scenarioId ID of the scenario to load.
     * @return true if initialization succeeded.
     */
    bool initialize(const std::string& scenarioId);

    /**
     * @brief Get the underlying simulation instance.
     */
    Simulation* getSimulation() const { return simulation_.get(); }

    /**
     * @brief Get the main window instance.
     */
    MainWindow* getMainWindow() const { return mainWindow_.get(); }

public slots:
    /**
     * @brief Triggered when time controller requests a tick.
     */
    void onTick();

    /**
     * @brief Triggered when simulation speed changes.
     * @param ticksPerSecond New speed value.
     */
    void onSpeedChanged(double ticksPerSecond);

    /**
     * @brief Triggered when simulation is paused/resumed.
     * @param paused True if paused.
     */
    void onPausedChanged(bool paused);

    /**
     * @brief Load a new scenario.
     * @param filePath Path to the scenario file.
     */
    void loadScenario(const QString& filePath);

    /**
     * @brief Save current scenario.
     * @param filePath Path to save to.
     */
    void saveScenario(const QString& filePath);

signals:
    /**
     * @brief Emitted when simulation tick completes.
     * @param tickCount Current tick count.
     */
    void tickCompleted(int tickCount);

    /**
     * @brief Emitted when simulation state changes.
     * @param running Is simulation running?
     */
    void stateChanged(bool running);

    /**
     * @brief Emitted when an important event occurs.
     * @param message Event message.
     */
    void eventOccurred(const QString& message);

private:
    std::unique_ptr<Simulation> simulation_;
    std::unique_ptr<ScenarioManager> scenarioManager_;
    std::unique_ptr<MainWindow> mainWindow_;
    std::unique_ptr<TimeController> timeController_;
    
    bool initialized_ = false;
};

} // namespace hiersim
