#include "LogPanel.h"
#include <QDateTime>

namespace hiersim::ui {

LogPanel::LogPanel(QWidget *parent)
    : QWidget(parent)
    , m_logText(nullptr)
    , m_layout(nullptr)
{
    setupUi();
}

LogPanel::~LogPanel() = default;

void LogPanel::setupUi() {
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(4, 4, 4, 4);
    
    m_logText = new QTextEdit();
    m_logText->setReadOnly(true);
    m_logText->setLineWrapMode(QTextEdit::WidgetWidth);
    m_logText->setMaximumBlockCount(1000); // Keep last 1000 lines
    m_layout->addWidget(m_logText);
    
    addLogEntry("Simulation log initialized.", false);
}

void LogPanel::addLogEntry(const QString& message, bool isImportant) {
    QString timestamp = QDateTime::currentDateTime().toString("[hh:mm:ss] ");
    
    if (isImportant) {
        m_logText->append(QString("<b><font color=\"red\">%1%2</font></b>").arg(timestamp, message));
    } else {
        m_logText->append(timestamp + message);
    }
}

void LogPanel::clear() {
    m_logText->clear();
}

} // namespace hiersim::ui
