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

namespace hiersim::ui {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mapView_(nullptr)
    , timeControlWidget_(nullptr)
    , entityInspector_(nullptr)
    , logPanel_(nullptr)
    , tickCountLabel_(nullptr)
    , speedLabel_(nullptr)
    , stateLabel_(nullptr)
    , fileMenu_(nullptr)
    , simulationMenu_(nullptr)
    , viewMenu_(nullptr)
    , helpMenu_(nullptr)
    , fileToolBar_(nullptr)
    , simulationToolBar_(nullptr)
{
    setWindowTitle("Hierarchy Simulator");
    setMinimumSize(1280, 720);
    
    createMenus();
    createToolBars();
    createDockWidgets();
    createStatusBar();
    connectSignals();
}

MainWindow::~MainWindow() = default;

void MainWindow::initialize(Simulation* simulation) {
    simulation_ = simulation;
    
    // Initialize UI components with simulation data
    if (mapView_) {
        mapView_->initialize(simulation);
    }
    if (entityInspector_) {
        entityInspector_->initialize(simulation);
    }
    if (logPanel_) {
        logPanel_->initialize(simulation);
    }
}

std::optional<std::string> MainWindow::getSelectedEntityId() const {
    if (entityInspector_) {
        return entityInspector_->getSelectedEntityId();
    }
    return std::nullopt;
}

void MainWindow::refreshEntityInspector(const std::string& entityId) {
    if (entityInspector_) {
        entityInspector_->refresh(entityId);
    }
}

void MainWindow::updateMapView() {
    if (mapView_) {
        mapView_->update();
    }
}

void MainWindow::updateTickCount(int count) {
    if (tickCountLabel_) {
        tickCountLabel_->setText(QString("Tick: %1").arg(count));
    }
}

void MainWindow::updateSimulationState(bool running) {
    if (stateLabel_) {
        stateLabel_->setText(running ? "Running" : "Paused");
    }
}

void MainWindow::addLogEntry(const QString& message) {
    if (logPanel_) {
        logPanel_->addEntry(message);
    }
}

void MainWindow::createMenus() {
    // File menu
    fileMenu_ = menuBar()->addMenu("&File");
    
    QAction* loadAction = fileMenu_->addAction("&Load Scenario...");
    loadAction->setShortcut(QKeySequence::Open);
    connect(loadAction, &QAction::triggered, [this]() {
        emit loadScenarioRequested("");
    });
    
    QAction* saveAction = fileMenu_->addAction("&Save Scenario");
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, [this]() {
        emit saveScenarioRequested("");
    });
    
    fileMenu_->addSeparator();
    
    QAction* exitAction = fileMenu_->addAction("E&xit");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);
    
    // Simulation menu
    simulationMenu_ = menuBar()->addMenu("&Simulation");
    
    // View menu
    viewMenu_ = menuBar()->addMenu("&View");
    
    QAction* fullScreenAction = viewMenu_->addAction("&Full Screen");
    fullScreenAction->setShortcut(QKeySequence::FullScreen);
    connect(fullScreenAction, &QAction::triggered, [this]() {
        if (isFullScreen()) {
            showNormal();
        } else {
            showFullScreen();
        }
    });
    
    // Help menu
    helpMenu_ = menuBar()->addMenu("&Help");
    
    QAction* aboutAction = helpMenu_->addAction("&About");
    connect(aboutAction, &QAction::triggered, [this]() {
        QMessageBox::about(this, "About Hierarchy Simulator",
            "Hierarchy Simulator v0.1\n\n"
            "A tick-based simulation of hierarchies, economies, and societies.\n\n"
            "Built with C++23, Qt6, and Vulkan.");
    });
}

void MainWindow::createToolBars() {
    // File toolbar
    fileToolBar_ = addToolBar("File");
    fileToolBar_->setMovable(false);
    
    // Simulation toolbar with time control
    simulationToolBar_ = addToolBar("Simulation");
    simulationToolBar_->setMovable(false);
    
    timeControlWidget_ = new TimeControlWidget(this);
    simulationToolBar_->addWidget(timeControlWidget_);
}

void MainWindow::createDockWidgets() {
    // Create central map view
    mapView_ = new MapView(this);
    setCentralWidget(mapView_);
    
    // Create entity inspector dock
    entityInspector_ = new EntityInspector(this);
    QDockWidget* inspectorDock = new QDockWidget("Entity Inspector", this);
    inspectorDock->setWidget(entityInspector_);
    addDockWidget(Qt::RightDockWidgetArea, inspectorDock);
    
    // Create log panel dock
    logPanel_ = new LogPanel(this);
    QDockWidget* logDock = new QDockWidget("Event Log", this);
    logDock->setWidget(logPanel_);
    addDockWidget(Qt::BottomDockWidgetArea, logDock);
}

void MainWindow::createStatusBar() {
    QStatusBar* status = statusBar();
    
    tickCountLabel_ = new QLabel("Tick: 0");
    status->addPermanentWidget(tickCountLabel_);
    
    speedLabel_ = new QLabel("Speed: 1x");
    status->addPermanentWidget(speedLabel_);
    
    stateLabel_ = new QLabel("Paused");
    status->addPermanentWidget(stateLabel_);
}

void MainWindow::connectSignals() {
    // Connect time control signals
    if (timeControlWidget_) {
        connect(timeControlWidget_, &TimeControlWidget::speedChanged,
            [this](int ticksPerSecond) {
                if (speedLabel_) {
                    speedLabel_->setText(QString("Speed: %1x").arg(ticksPerSecond));
                }
            });
    }
    
    // Connect map view signals
    if (mapView_) {
        connect(mapView_, &MapView::regionSelected,
            [this](const std::string& regionId) {
                if (entityInspector_) {
                    entityInspector_->inspectRegion(regionId);
                }
            });
        connect(mapView_, &MapView::entitySelected,
            [this](const std::string& entityId) {
                if (entityInspector_) {
                    entityInspector_->inspectEntity(entityId);
                }
            });
    }
}

} // namespace hiersim::ui
