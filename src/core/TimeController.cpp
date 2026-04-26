#include "hiersim/TimeController.h"

namespace hiersim {

TimeController::TimeController(QObject* parent)
    : QObject(parent)
    , m_timer(new QTimer(this))
{
    // Configure timer for precise timing
    m_timer->setTimerType(Qt::PreciseTimer);
    m_timer->setSingleShot(false);
    
    // Connect timer to tick handler
    connect(m_timer, &QTimer::timeout, this, &TimeController::onTimerTick);
    
    // Set initial speed
    updateTimerInterval();
}

TimeController::~TimeController() {
    stop();
}

void TimeController::start() {
    if (m_running) {
        return;
    }
    
    m_running = true;
    m_paused = false;
    m_timer->start();
    
    emit stateChanged(true, false);
}

void TimeController::stop() {
    if (!m_running) {
        return;
    }
    
    m_running = false;
    m_paused = false;
    m_timer->stop();
    
    emit stateChanged(false, false);
}

void TimeController::pause() {
    if (!m_running || m_paused) {
        return;
    }
    
    m_paused = true;
    m_timer->stop();
    
    emit stateChanged(true, true);
}

void TimeController::resume() {
    if (!m_running || !m_paused) {
        return;
    }
    
    m_paused = false;
    m_timer->start();
    
    emit stateChanged(true, false);
}

void TimeController::step() {
    // If running and not paused, don't allow manual stepping
    if (m_running && !m_paused) {
        return;
    }
    
    // Execute one tick manually
    ++m_currentTick;
    
    if (m_tickCallback) {
        m_tickCallback(m_currentTick.load());
    }
    
    emit tickOccurred(m_currentTick.load());
}

void TimeController::setSpeedPreset(SpeedPreset preset) {
    m_currentPreset = preset;
    
    double speed = static_cast<double>(preset);
    setSpeed(speed);
}

void TimeController::setSpeed(double ticksPerSecond) {
    double clamped = clampSpeed(ticksPerSecond);
    
    if (m_ticksPerSecond != clamped) {
        m_ticksPerSecond = clamped;
        
        // Update preset to match if it's a standard value
        if (clamped == 0.0) {
            m_currentPreset = SpeedPreset::Paused;
        } else if (clamped == 1.0) {
            m_currentPreset = SpeedPreset::Slow;
        } else if (clamped == 3.0) {
            m_currentPreset = SpeedPreset::Normal;
        } else if (clamped == 6.0) {
            m_currentPreset = SpeedPreset::Fast;
        }
        
        updateTimerInterval();
        
        emit speedChanged(clamped);
    }
}

double TimeController::getSpeed() const {
    return m_ticksPerSecond.load();
}

TimeController::SpeedPreset TimeController::getSpeedPreset() const {
    return m_currentPreset;
}

bool TimeController::isRunning() const {
    return m_running.load();
}

bool TimeController::isPaused() const {
    return m_paused.load();
}

int TimeController::getCurrentTick() const {
    return m_currentTick.load();
}

void TimeController::setTickCallback(std::function<void(int)> callback) {
    m_tickCallback = std::move(callback);
}

void TimeController::onTimerTick() {
    if (m_paused) {
        return;
    }
    
    ++m_currentTick;
    
    if (m_tickCallback) {
        m_tickCallback(m_currentTick.load());
    }
    
    emit tickOccurred(m_currentTick.load());
}

void TimeController::updateTimerInterval() {
    if (!m_running || m_paused) {
        m_timer->stop();
        return;
    }
    
    double ticksPerSecond = m_ticksPerSecond.load();
    
    if (ticksPerSecond <= 0.0) {
        m_timer->stop();
        return;
    }
    
    // Calculate interval in milliseconds
    int intervalMs = static_cast<int>(1000.0 / ticksPerSecond);
    m_timer->setInterval(intervalMs);
    
    // Restart timer with new interval if running
    if (m_running && !m_paused) {
        m_timer->start();
    }
}

double TimeController::clampSpeed(double speed) const {
    // Clamp to valid range [0.0, 6.0]
    // 0.0 means paused
    if (speed < 0.0) {
        speed = 0.0;
    } else if (speed > 6.0) {
        speed = 6.0;
    }
    
    return speed;
}

} // namespace hiersim
