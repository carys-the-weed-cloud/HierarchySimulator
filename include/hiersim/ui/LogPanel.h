#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <string>

namespace hiersim {
    class Simulation;
}

namespace hiersim::ui {
    /**
     * @brief Panel for displaying simulation event log
     */
    class LogPanel : public QWidget {
        Q_OBJECT

    public:
        explicit LogPanel(QWidget *parent = nullptr);
        ~LogPanel();

        void initialize(Simulation* simulation);
        void addEntry(const QString& message);
        void addLogEntry(const QString& message, bool isImportant = false);
        void clear();

    private:
        void setupUi();

        QTextEdit* m_logText;
        QVBoxLayout* m_layout;
        Simulation* m_simulation = nullptr;
    };
}
