#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>

namespace hiersim::ui {
    /**
     * @brief Panel for displaying simulation event log
     */
    class LogPanel : public QWidget {
        Q_OBJECT

    public:
        explicit LogPanel(QWidget *parent = nullptr);
        ~LogPanel();

        void addLogEntry(const QString& message, bool isImportant = false);
        void clear();

    private:
        void setupUi();

        QTextEdit* m_logText;
        QVBoxLayout* m_layout;
    };
}
