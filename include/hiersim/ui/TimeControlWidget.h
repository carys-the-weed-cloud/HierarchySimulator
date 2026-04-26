#pragma once

#include <QWidget>
#include <QSlider>
#include <QPushButton>
#include <QLCDNumber>
#include <QLabel>

namespace hiersim::ui {
    /**
     * @brief Widget for controlling simulation time
     * 
     * Provides play/pause, step forward, and speed control (1-6 ticks/second).
     */
    class TimeControlWidget : public QWidget {
        Q_OBJECT

    public:
        explicit TimeControlWidget(QWidget *parent = nullptr);
        ~TimeControlWidget();

        int currentSpeed() const { return m_speed; }
        bool isPlaying() const { return m_isPlaying; }

    signals:
        void speedChanged(int ticksPerSecond);
        void playToggled(bool playing);
        void stepRequested();

    private slots:
        void onPlayPause();
        void onStep();
        void onSpeedSliderChanged(int value);

    private:
        void setupUi();
        void updateDisplay();

        QPushButton* m_playPauseButton;
        QPushButton* m_stepButton;
        QSlider* m_speedSlider;
        QLCDNumber* m_speedDisplay;
        QLabel* m_speedLabel;
        
        int m_speed;
        bool m_isPlaying;
    };
}
