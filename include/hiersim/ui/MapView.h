#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <string>
#include <memory>

namespace hiersim {
    class WorldMap;
    class Simulation;
}

namespace hiersim::ui {
    /**
     * @brief Vulkan-based map visualization widget
     * 
     * Renders world regions with terrain colors, ownership overlays,
     * and interactive selection capabilities.
     */
    class MapView : public QOpenGLWidget, protected QOpenGLFunctions {
        Q_OBJECT

    public:
        explicit MapView(QWidget *parent = nullptr);
        ~MapView();

        void initialize(Simulation* simulation);
        void setWorldMap(std::shared_ptr<WorldMap> map);

    signals:
        void regionSelected(const std::string& regionId);
        void entitySelected(const std::string& entityId);

    protected:
        void initializeGL() override;
        void resizeGL(int w, int h) override;
        void paintGL() override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void wheelEvent(QWheelEvent *event) override;

    private:
        void updateCamera();
        void renderRegions();
        void renderConnections();
        
        std::shared_ptr<WorldMap> m_worldMap;  // When we own the map
        const WorldMap* m_worldMapPtr = nullptr;  // Raw pointer when borrowed from Scenario
        Simulation* m_simulation = nullptr;
        
        // Camera state
        float m_zoom;
        float m_panX;
        float m_panY;
        
        // Interaction state
        std::string m_hoveredRegion;
        bool m_isDragging;
        QPoint m_lastMousePos;
    };
}
