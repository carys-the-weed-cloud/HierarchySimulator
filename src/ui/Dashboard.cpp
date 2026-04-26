#include "hiersim/Dashboard.h"
#include "hiersim/Simulation.h"
#include <QVBoxLayout>
#include <QGridLayout>

namespace hiersim {
Dashboard::Dashboard(QWidget* parent) : QWidget(parent) {
    auto* layout = new QGridLayout(this);
    gdpLabel_ = new QLabel("GDP: N/A", this);
    unemploymentLabel_ = new QLabel("Unemployment: N/A", this);
    inflationLabel_ = new QLabel("Inflation: N/A", this);
    tickLabel_ = new QLabel("Tick: 0", this);
    layout->addWidget(gdpLabel_, 0, 0);
    layout->addWidget(unemploymentLabel_, 0, 1);
    layout->addWidget(inflationLabel_, 1, 0);
    layout->addWidget(tickLabel_, 1, 1);
}
void Dashboard::setSimulation(Simulation* sim) { simulation_ = sim; }
void Dashboard::update() {
    if (!simulation_) return;
    tickLabel_->setText(QString("Tick: %1").arg(simulation_->getCurrentTick()));
}
}
