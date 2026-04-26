#include "SimulationController.h"
#include "Scenario.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

namespace hiersim {

SimulationController::SimulationController(QObject* parent)
    : QObject(parent)
    , scenarioManager_(std::make_unique<ScenarioManager>())
    , mainWindow_(std::make_unique<MainWindow>())
    , timeController_(std::make_unique<TimeController>())
{
    // Connect TimeController signals to simulation slots
    connect(timeController_.get(), &TimeController::tickRequested,
            this, &SimulationController::onTick);
    connect(timeController_.get(), &TimeController::speedChanged,
            this, &SimulationController::onSpeedChanged);
    connect(timeController_.get(), &TimeController::pausedChanged,
            this, &SimulationController::onPausedChanged);

    // Connect MainWindow actions to controller slots
    connect(mainWindow_.get(), &MainWindow::loadScenarioRequested,
            this, &SimulationController::loadScenario);
    connect(mainWindow_.get(), &MainWindow::saveScenarioRequested,
            this, &SimulationController::saveScenario);

    // Connect simulation events to UI updates
    connect(this, &SimulationController::tickCompleted,
            mainWindow_.get(), &MainWindow::updateTickCount);
    connect(this, &SimulationController::stateChanged,
            mainWindow_.get(), &MainWindow::updateSimulationState);
    connect(this, &SimulationController::eventOccurred,
            mainWindow_.get(), &MainWindow::addLogEntry);
}

SimulationController::~SimulationController() = default;

bool SimulationController::initialize(const std::string& scenarioId)
{
    auto scenario = scenarioManager_->loadById(scenarioId);
    if (!scenario) {
        qCritical() << "Failed to load scenario:" << QString::fromStdString(scenarioId);
        return false;
    }

    simulation_ = std::make_unique<Simulation>(std::move(*scenario));
    
    // Initialize main window with simulation data
    mainWindow_->initialize(simulation_.get());
    
    initialized_ = true;
    emit stateChanged(false); // Start paused
    
    qDebug() << "Simulation initialized with scenario:" << QString::fromStdString(scenarioId);
    return true;
}

void SimulationController::onTick()
{
    if (!initialized_ || !simulation_) {
        return;
    }

    simulation_->step();
    
    int currentTick = simulation_->getTickCount();
    emit tickCompleted(currentTick);
    
    // Update entity inspector if an entity is selected
    auto selectedId = mainWindow_->getSelectedEntityId();
    if (selectedId.has_value()) {
        // Refresh entity data in inspector
        mainWindow_->refreshEntityInspector(selectedId.value());
    }
    
    // Update map view with any changes
    mainWindow_->updateMapView();
}

void SimulationController::onSpeedChanged(double ticksPerSecond)
{
    if (simulation_) {
        // Could adjust simulation internal timing if needed
        qDebug() << "Simulation speed changed to:" << ticksPerSecond << "ticks/sec";
    }
}

void SimulationController::onPausedChanged(bool paused)
{
    emit stateChanged(!paused);
    qDebug() << "Simulation" << (paused ? "paused" : "resumed");
}

void SimulationController::loadScenario(const QString& filePath)
{
    if (filePath.isEmpty()) {
        return;
    }

    auto scenario = scenarioManager_->load(filePath.toStdString());
    if (!scenario) {
        QMessageBox::critical(mainWindow_.get(), tr("Load Error"),
                              tr("Failed to load scenario from %1").arg(filePath));
        return;
    }

    simulation_ = std::make_unique<Simulation>(std::move(*scenario));
    mainWindow_->initialize(simulation_.get());
    initialized_ = true;
    
    emit stateChanged(false);
    emit eventOccurred(tr("Loaded scenario: %1").arg(filePath));
}

void SimulationController::saveScenario(const QString& filePath)
{
    if (!simulation_ || filePath.isEmpty()) {
        return;
    }

    const auto& scenario = simulation_->getScenario();
    if (scenarioManager_->save(scenario, filePath.toStdString())) {
        emit eventOccurred(tr("Saved scenario to %1").arg(filePath));
    } else {
        QMessageBox::critical(mainWindow_.get(), tr("Save Error"),
                              tr("Failed to save scenario to %1").arg(filePath));
    }
}

} // namespace hiersim
