#ifndef HIERSIM_TIMECONTROLLER_H
#define HIERSIM_TIMECONTROLLER_H

#include <atomic>
#include <functional>
#include <QObject>
#include <QTimer>

namespace hiersim {

/**
 * @brief Qt-based time controller for UI integration
 * 
 * Provides a Qt-friendly interface for controlling simulation speed
 * and integrating with the UI. Uses QTimer for precise tick timing.
 * 
 * Time System:
 * - 1 tick = 1 day in-game
 * - Speed range: 1-6 ticks per second (optimal)
 * - Supports pause, play, step, and variable speed
 */
class TimeController : public QObject {
    Q_OBJECT
    
public:
    /**
     * @brief Simulation speed presets
     */
    enum class SpeedPreset {
        Paused = 0,
        Slow = 1,      // 1 tick/second
        Normal = 3,    // 3 ticks/second (recommended)
        Fast = 6       // 6 ticks/second (maximum)
    };
    
    /**
     * @brief Construct a new Time Controller
     * @param parent Parent QObject
     */
    explicit TimeController(QObject* parent = nullptr);
    
    /**
     * @brief Destructor
     */
    ~TimeController();
    
    /**
     * @brief Start the simulation timer
     */
    void start();
    
    /**
     * @brief Stop the simulation timer
     */
    void stop();
    
    /**
     * @brief Pause the simulation
     */
    void pause();
    
    /**
     * @brief Resume the simulation
     */
    void resume();
    
    /**
     * @brief Advance by exactly one tick
     */
    void step();
    
    /**
     * @brief Set simulation speed using preset
     * @param preset Speed preset
     */
    void setSpeedPreset(SpeedPreset preset);
    
    /**
     * @brief Set simulation speed in ticks per second
     * @param ticksPerSecond Ticks per real-world second (1.0-6.0)
     */
    void setSpeed(double ticksPerSecond);
    
    /**
     * @brief Get current speed in ticks per second
     * @return Current ticks per second
     */
    double getSpeed() const;
    
    /**
     * @brief Get current speed preset
     * @return Current speed preset
     */
    SpeedPreset getSpeedPreset() const;
    
    /**
     * @brief Check if simulation is running
     * @return true if actively simulating
     */
    bool isRunning() const;
    
    /**
     * @brief Check if simulation is paused
     * @return true if paused
     */
    bool isPaused() const;
    
    /**
     * @brief Get current tick number
     * @return Current simulation tick
     */
    int getCurrentTick() const;
    
    /**
     * @brief Set the tick callback function
     * @param callback Function to call on each tick
     */
    void setTickCallback(std::function<void(int)> callback);
    
signals:
    /**
     * @brief Emitted when a tick occurs
     * @param tickNumber Current tick number
     */
    void tickOccurred(int tickNumber);
    
    /**
     * @brief Emitted when speed changes
     * @param ticksPerSecond New speed in ticks per second
     */
    void speedChanged(double ticksPerSecond);
    
    /**
     * @brief Emitted when simulation state changes
     * @param running Is simulation running
     * @param paused Is simulation paused
     */
    void stateChanged(bool running, bool paused);

private slots:
    /**
     * @brief Internal slot called by timer on each tick
     */
    void onTimerTick();

private:
    /**
     * @brief Update timer interval based on current speed
     */
    void updateTimerInterval();
    
    /**
     * @brief Clamp speed to valid range
     * @param speed Speed to clamp
     * @return Clamped speed
     */
    double clampSpeed(double speed) const;

private:
    QTimer* m_timer;
    std::atomic<int> m_currentTick{0};
    std::atomic<double> m_ticksPerSecond{1.0};
    std::atomic<bool> m_running{false};
    std::atomic<bool> m_paused{false};
    SpeedPreset m_currentPreset{SpeedPreset::Normal};
    
    std::function<void(int)> m_tickCallback;
};

} // namespace hiersim

#endif // HIERSIM_TIMECONTROLLER_H
