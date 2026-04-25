#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    QMainWindow window;
    window.setWindowTitle("Hierarchy Simulator - Phase 2 Core Data Structures");
    window.setGeometry(100, 100, 800, 600);
    
    QWidget* centralWidget = new QWidget(&window);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    
    QLabel* titleLabel = new QLabel("<h1>Hierarchy Simulator</h1>");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);
    
    QLabel* statusLabel = new QLabel(R"(
        <h2>Phase 2: Core Data Structures - COMPLETE</h2>
        <p>The following core simulation objects have been implemented:</p>
        <ul>
            <li><b>Individual</b> - Distinct agents with skills, relationships, ideology, wealth, and decision-making</li>
            <li><b>Organization</b> - Hierarchical groupings with democracy/autocracy spectrum and resource distribution</li>
            <li><b>Facility</b> - Physical/digital structures with location, capacity, efficiency, and access control</li>
            <li><b>Unit</b> - Armed groupings with morale, loyalty, combat effectiveness, and deployment</li>
        </ul>
        <p>All classes include:</p>
        <ul>
            <li>Serialization/deserialization support</li>
            <li>Comprehensive getters/setters</li>
            <li>Domain-specific methods</li>
            <li>Unique ID generation</li>
        </ul>
        <p><i>Simulation is entirely tick-based (no real-time components).</i></p>
    )");
    statusLabel->setWordWrap(true);
    layout->addWidget(statusLabel);
    
    window.setCentralWidget(centralWidget);
    window.show();
    
    return app.exec();
}
