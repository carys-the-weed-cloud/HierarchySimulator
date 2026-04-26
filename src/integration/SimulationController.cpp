#include "hiersim/SimulationController.h"
#include "hiersim/ScenarioManager.h"
#include "hiersim/ui/MainWindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

namespace hiersim {

SimulationController::SimulationController(QObject* parent)
    : QObject(parent)
    , scenarioManager_(std::make_unique<ScenarioManager>())
    , timeController_(std::make_unique<TimeController>())
{
    // Create main window after other members are initialized
    mainWindow_ = std::make_unique<ui::MainWindow>();
    
    // Connect TimeController signals to simulation slots
    connect(timeController_.get(), &TimeController::tickOccurred,
            this, &SimulationController::onTick);
    connect(timeController_.get(), &TimeController::speedChanged,
            this, &SimulationController::onSpeedChanged);
    connect(timeController_.get(), &TimeController::stateChanged,
            this, &SimulationController::onStateChanged);

    // Connect MainWindow actions to controller slots
    connect(mainWindow_.get(), &ui::MainWindow::loadScenarioRequested,
            this, &SimulationController::loadScenario);
    connect(mainWindow_.get(), &ui::MainWindow::saveScenarioRequested,
            this, &SimulationController::saveScenario);

    // Connect simulation events to UI updates
    connect(this, &SimulationController::tickCompleted,
            mainWindow_.get(), &ui::MainWindow::updateTickCount);
    connect(this, &SimulationController::stateChanged,
            mainWindow_.get(), &ui::MainWindow::updateSimulationState);
    connect(this, &SimulationController::eventOccurred,
            mainWindow_.get(), &ui::MainWindow::addLogEntry);
}

SimulationController::~SimulationController() = default;

bool SimulationController::initialize(const std::string& scenarioId)
{
    auto scenario = scenarioManager_->loadScenario(scenarioId);
    if (!scenario) {
        qCritical() << "Failed to load scenario:" << QString::fromStdString(scenarioId);
        return false;
    }

    simulation_ = std::make_unique<Simulation>(scenario);
    
    // Initialize main window with simulation data
    mainWindow_->initialize(simulation_.get());
    
    initialized_ = true;
    emit stateChanged(false); // Start paused
    
    qDebug() << "Simulation initialized with scenario:" << QString::fromStdString(scenarioId);
    return true;
}

void SimulationController::onTick(int tickNumber)
{
    if (!initialized_ || !simulation_) {
        return;
    }

    simulation_->step();
    
    int currentTick = simulation_->getCurrentTick();
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

void SimulationController::onStateChanged(bool running, bool paused)
{
    emit stateChanged(running && !paused);
    qDebug() << "Simulation state changed - running:" << running << "paused:" << paused;
}

void SimulationController::loadScenario(const QString& filePath)
{
    if (filePath.isEmpty()) {
        return;
    }

    auto scenario = Scenario::loadFromFile(filePath.toStdString());
    if (!scenario) {
        QMessageBox::critical(mainWindow_.get(), tr("Load Error"),
                              tr("Failed to load scenario from %1").arg(filePath));
        return;
    }

    simulation_ = std::make_unique<Simulation>(scenario);
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
    if (scenario && scenarioManager_->saveScenario(*scenario)) {
        emit eventOccurred(tr("Saved scenario to %1").arg(filePath));
    } else {
        QMessageBox::critical(mainWindow_.get(), tr("Save Error"),
                              tr("Failed to save scenario to %1").arg(filePath));
    }
}

} // namespace hiersim
