#include "hiersim/ui/MapView.h"
#include "hiersim/Simulation.h"
#include "hiersim/WorldMap.h"
#include <QOpenGLFunctions>
#include <QPainter>

namespace hiersim::ui {

MapView::MapView(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_worldMap(nullptr)
    , m_simulation(nullptr)
    , m_zoom(1.0f)
    , m_panX(0.0f)
    , m_panY(0.0f)
    , m_isDragging(false)
{
    setMinimumSize(800, 600);
    setFocusPolicy(Qt::StrongFocus);
}

MapView::~MapView() = default;

void MapView::initialize(Simulation* simulation) {
    m_simulation = simulation;
    if (simulation && simulation->getScenario()) {
        m_worldMap = simulation->getScenario()->getWorldMap();
        update();
    }
}

void MapView::setWorldMap(std::shared_ptr<WorldMap> map) {
    m_worldMap = map;
    update();
}

void MapView::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
}

void MapView::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void MapView::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (!m_worldMap) return;
    
    // TODO: Implement Vulkan-based rendering
    // For now, use OpenGL placeholder
    renderRegions();
    renderConnections();
}

void MapView::renderRegions() {
    // Placeholder: In production, this would use Vulkan to render regions
    // with terrain colors and ownership overlays
    const auto& regions = m_worldMap->getRegions();
    
    glBegin(GL_TRIANGLES);
    for (const auto& [id, region] : regions) {
        // Color based on terrain type
        switch (region.terrain) {
            case Region::Terrain::Plains:
                glColor3f(0.2f, 0.6f, 0.2f);
                break;
            case Region::Terrain::Forest:
                glColor3f(0.1f, 0.4f, 0.1f);
                break;
            case Region::Terrain::Mountains:
                glColor3f(0.5f, 0.5f, 0.5f);
                break;
            case Region::Terrain::Desert:
                glColor3f(0.8f, 0.7f, 0.4f);
                break;
            case Region::Terrain::Coastal:
                glColor3f(0.4f, 0.6f, 0.8f);
                break;
            default:
                glColor3f(0.3f, 0.3f, 0.3f);
        }
        
        // Simple triangle representation for each region
        glVertex2f(region.longitude * 10 + m_panX, region.latitude * 10 + m_panY);
        glVertex2f(region.longitude * 10 + 50 + m_panX, region.latitude * 10 + m_panY);
        glVertex2f(region.longitude * 10 + 25 + m_panX, region.latitude * 10 + 40 + m_panY);
    }
    glEnd();
}

void MapView::renderConnections() {
    glBegin(GL_LINES);
    glColor3f(0.5f, 0.5f, 0.5f);
    
    // Render connections between regions
    // TODO: Implement proper connection rendering
    glEnd();
}

void MapView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isDragging = true;
        m_lastMousePos = event->pos();
    } else if (event->button() == Qt::RightButton) {
        // Select region on right click
        // TODO: Implement proper hit detection
        if (m_worldMap && !m_worldMap->getRegions().empty()) {
            auto firstRegion = m_worldMap->getRegions().begin();
            emit regionSelected(firstRegion->first);
        }
    }
}

void MapView::mouseMoveEvent(QMouseEvent *event) {
    if (m_isDragging) {
        QPoint delta = event->pos() - m_lastMousePos;
        m_panX += delta.x() / m_zoom;
        m_panY += delta.y() / m_zoom;
        m_lastMousePos = event->pos();
        update();
    }
}

void MapView::wheelEvent(QWheelEvent *event) {
    float zoomFactor = event->angleDelta().y() > 0 ? 1.1f : 0.9f;
    m_zoom *= zoomFactor;
    m_zoom = std::max(0.1f, std::min(10.0f, m_zoom));
    update();
}

void MapView::updateCamera() {
    // Camera transformation logic
    // TODO: Implement proper camera matrix
}

} // namespace hiersim::ui
