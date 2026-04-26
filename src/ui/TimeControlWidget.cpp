#include "TimeControlWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

namespace hiersim::ui {

TimeControlWidget::TimeControlWidget(QWidget *parent)
    : QWidget(parent)
    , m_playPauseButton(nullptr)
    , m_stepButton(nullptr)
    , m_speedSlider(nullptr)
    , m_speedDisplay(nullptr)
    , m_speedLabel(nullptr)
    , m_speed(1)
    , m_isPlaying(false)
{
    setupUi();
}

TimeControlWidget::~TimeControlWidget() = default;

void TimeControlWidget::setupUi() {
    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);
    
    // Play/Pause button
    m_playPauseButton = new QPushButton("▶ Play");
    m_playPauseButton->setCheckable(true);
    m_playPauseButton->setChecked(false);
    connect(m_playPauseButton, &QPushButton::clicked, 
            this, &TimeControlWidget::onPlayPause);
    layout->addWidget(m_playPauseButton);
    
    // Step button
    m_stepButton = new QPushButton("⏭ Step");
    connect(m_stepButton, &QPushButton::clicked, 
            this, &TimeControlWidget::onStep);
    layout->addWidget(m_stepButton);
    
    // Speed slider (1-6 ticks/second)
    m_speedLabel = new QLabel("Speed:");
    layout->addWidget(m_speedLabel);
    
    m_speedSlider = new QSlider(Qt::Horizontal);
    m_speedSlider->setMinimum(1);
    m_speedSlider->setMaximum(6);
    m_speedSlider->setValue(1);
    m_speedSlider->setTickPosition(QSlider::TicksBelow);
    m_speedSlider->setTickInterval(1);
    m_speedSlider->setMinimumWidth(150);
    connect(m_speedSlider, &QSlider::valueChanged, 
            this, &TimeControlWidget::onSpeedSliderChanged);
    layout->addWidget(m_speedSlider);
    
    // Speed display
    m_speedDisplay = new QLCDNumber(1);
    m_speedDisplay->setSegmentStyle(QLCDNumber::Filled);
    m_speedDisplay->display(1);
    m_speedDisplay->setFixedWidth(40);
    layout->addWidget(m_speedDisplay);
    
    updateDisplay();
}

void TimeControlWidget::onPlayPause() {
    m_isPlaying = !m_isPlaying;
    
    if (m_isPlaying) {
        m_playPauseButton->setText("⏸ Pause");
        m_playPauseButton->setChecked(true);
    } else {
        m_playPauseButton->setText("▶ Play");
        m_playPauseButton->setChecked(false);
    }
    
    emit playToggled(m_isPlaying);
}

void TimeControlWidget::onStep() {
    // If playing, pause first
    if (m_isPlaying) {
        m_isPlaying = false;
        m_playPauseButton->setText("▶ Play");
        m_playPauseButton->setChecked(false);
        emit playToggled(false);
    }
    
    emit stepRequested();
}

void TimeControlWidget::onSpeedSliderChanged(int value) {
    m_speed = value;
    m_speedDisplay->display(value);
    emit speedChanged(value);
}

void TimeControlWidget::updateDisplay() {
    m_speedDisplay->display(m_speed);
}

} // namespace hiersim::ui
