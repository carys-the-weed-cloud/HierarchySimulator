#pragma once
#include <QWidget>
#include <QLabel>

namespace hiersim {
class Simulation;
class Dashboard : public QWidget {
    Q_OBJECT
public:
    explicit Dashboard(QWidget* parent = nullptr);
    void setSimulation(Simulation* sim);
    void update();
private:
    QLabel* gdpLabel_;
    QLabel* unemploymentLabel_;
    QLabel* inflationLabel_;
    QLabel* tickLabel_;
    Simulation* simulation_ = nullptr;
};
}
