#include "hiersim/ui/MainWindow.h"
#include "hiersim/ui/MapView.h"
#include "hiersim/ui/EntityInspector.h"
#include "hiersim/ui/TimeControlWidget.h"
#include "hiersim/ui/LogPanel.h"
#include "hiersim/Simulation.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDockWidget>
#include <QToolBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>

namespace hiersim::ui {

MainWindow::MainWindow(std::shared_ptr<Simulation> sim, QWidget *parent)
    : QMainWindow(parent)
    , m_simulation(sim)
    , m_mapView(nullptr)
    , m_centerStack(nullptr)
    , m_entityInspector(nullptr)
    , m_logPanel(nullptr)
    , m_timeControl(nullptr)
    , m_isFullScreen(false)
{
    setWindowTitle("Hierarchy Simulator");
    setMinimumSize(1280, 720);
    
    setupUi();
    setupMenuBar();
    setupStatusBar();
    connectSignals();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUi() {
    // Create central map view
    m_mapView = new MapView(this);
    
    m_centerStack = new QStackedWidget(this);
    m_centerStack->addWidget(m_mapView);
    setCentralWidget(m_centerStack);
    
    // Create time control widget
    m_timeControl = new TimeControlWidget(this);
    QToolBar* timeToolbar = addToolBar("Time Control");
    timeToolbar->addWidget(m_timeControl);
    timeToolbar->setMovable(false);
    
    // Create entity inspector dock
    m_entityInspector = new EntityInspector(this);
    QDockWidget* inspectorDock = new QDockWidget("Entity Inspector", this);
    inspectorDock->setWidget(m_entityInspector);
    addDockWidget(Qt::RightDockWidgetArea, inspectorDock);
    
    // Create log panel dock
    m_logPanel = new LogPanel(this);
    QDockWidget* logDock = new QDockWidget("Event Log", this);
    logDock->setWidget(m_logPanel);
    addDockWidget(Qt::BottomDockWidgetArea, logDock);
}

void MainWindow::setupMenuBar() {
    // File menu
    QMenu* fileMenu = menuBar()->addMenu("&File");
    
    QAction* loadAction = fileMenu->addAction("&Load Scenario...");
    loadAction->setShortcut(QKeySequence::Open);
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadScenario);
    
    QAction* saveAction = fileMenu->addAction("&Save Scenario");
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveScenario);
    
    fileMenu->addSeparator();
    
    QAction* exitAction = fileMenu->addAction("E&xit");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);
    
    // Simulation menu
    QMenu* simMenu = menuBar()->addMenu("&Simulation");
    
    QAction* playAction = simMenu->addAction("&Play");
    playAction->setShortcut(QKeySequence(Qt::Key_Space));
    connect(playAction, &QAction::triggered, [this]() {
        if (m_simulation) m_simulation->resume();
    });
    
    QAction* pauseAction = simMenu->addAction("P&ause");
    pauseAction->setShortcut(QKeySequence(Qt::Key_P));
    connect(pauseAction, &QAction::triggered, [this]() {
        if (m_simulation) m_simulation->pause();
    });
    
    simMenu->addSeparator();
    
    QAction* stepAction = simMenu->addAction("Step &Forward");
    stepAction->setShortcut(QKeySequence(Qt::Key_F));
    connect(stepAction, &QAction::triggered, [this]() {
        if (m_simulation) m_simulation->step();
    });
    
    // View menu
    QMenu* viewMenu = menuBar()->addMenu("&View");
    
    QAction* fullScreenAction = viewMenu->addAction("&Full Screen");
    fullScreenAction->setShortcut(QKeySequence::FullScreen);
    connect(fullScreenAction, &QAction::triggered, this, &MainWindow::toggleFullScreen);
    
    // Help menu
    QMenu* helpMenu = menuBar()->addMenu("&Help");
    
    QAction* aboutAction = helpMenu->addAction("&About");
    connect(aboutAction, &QAction::triggered, [this]() {
        QMessageBox::about(this, "About Hierarchy Simulator",
            "Hierarchy Simulator v0.1\n\n"
            "A tick-based simulation of hierarchies, economies, and societies.\n\n"
            "Built with C++23, Qt6, and Vulkan.");
    });
}

void MainWindow::setupStatusBar() {
    QStatusBar* status = statusBar();
    
    m_tickLabel = new QLabel("Tick: 0");
    status->addPermanentWidget(m_tickLabel);
    
    m_speedLabel = new QLabel("Speed: 1x");
    status->addPermanentWidget(m_speedLabel);
    
    m_stateLabel = new QLabel("Paused");
    status->addPermanentWidget(m_stateLabel);
    
    // Connect to simulation updates via stats callback
    if (m_simulation) {
        m_simulation->onStatsUpdate([this](const SimulationStats& stats) {
            QMetaObject::invokeMethod(this, [this, stats]() {
                updateStatusBar(stats);
            }, Qt::QueuedConnection);
        });
    }
}

void MainWindow::connectSignals() {
    // Time control signals
    connect(m_timeControl, &TimeControlWidget::speedChanged,
        this, &MainWindow::onSpeedChanged);
    connect(m_timeControl, &TimeControlWidget::playToggled,
        this, &MainWindow::onSimulationToggled);
    
    // Map view signals
    connect(m_mapView, &MapView::regionSelected,
        this, &MainWindow::onRegionSelected);
    connect(m_mapView, &MapView::entitySelected,
        this, &MainWindow::onEntitySelected);
    
    // Tick timer
    connect(&m_tickTimer, &QTimer::timeout, this, &MainWindow::onTick);
    m_tickTimer.setInterval(1000); // Default 1 tick per second
    m_tickTimer.start();
}

void MainWindow::onTick() {
    if (m_simulation && m_simulation->isRunning()) {
        m_simulation->step();
    }
}

void MainWindow::onSpeedChanged(int ticksPerSecond) {
    if (m_simulation) {
        m_simulation->setSpeed(ticksPerSecond);
        m_tickTimer.setInterval(1000 / ticksPerSecond);
    }
}

void MainWindow::onSimulationToggled(bool running) {
    if (m_simulation) {
        if (running) {
            m_simulation->resume();
        } else {
            m_simulation->pause();
        }
    }
}

void MainWindow::onRegionSelected(const std::string& regionId) {
    if (m_entityInspector) {
        m_entityInspector->inspectRegion(regionId);
    }
}

void MainWindow::onEntitySelected(const std::string& entityId) {
    if (m_entityInspector) {
        m_entityInspector->inspectEntity(entityId);
    }
}

void MainWindow::loadScenario() {
    QString fileName = QFileDialog::getOpenFileName(
        this, "Load Scenario", "", "Scenario Files (*.scenario);;All Files (*)");
    
    if (!fileName.isEmpty() && m_simulation) {
        // Note: Scenario loading is handled by SimulationController
        // This would need to be implemented based on your architecture
        emit scenarioLoaded(fileName.toStdString());
    }
}

void MainWindow::saveScenario() {
    QString fileName = QFileDialog::getSaveFileName(
        this, "Save Scenario", "", "Scenario Files (*.scenario);;All Files (*)");
    
    if (!fileName.isEmpty() && m_simulation) {
        // Note: Scenario saving is handled by SimulationController
        // This would need to be implemented based on your architecture
        emit scenarioSaved(fileName.toStdString());
    }
}

void MainWindow::toggleFullScreen() {
    m_isFullScreen = !m_isFullScreen;
    if (m_isFullScreen) {
        showFullScreen();
    } else {
        showNormal();
    }
}

void MainWindow::updateStatusBar(const hiersim::SimulationStats& stats) {
    if (m_tickLabel) {
        m_tickLabel->setText(QString("Tick: %1").arg(stats.current_tick));
    }
    if (m_speedLabel) {
        m_speedLabel->setText(QString("Speed: %1x").arg(static_cast<int>(stats.ticks_per_second)));
    }
    if (m_stateLabel) {
        m_stateLabel->setText(m_simulation && m_simulation->isRunning() ? "Running" : "Paused");
    }
}

} // namespace hiersim::ui