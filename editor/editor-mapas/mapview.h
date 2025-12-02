#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

#include "markeritem.h"

class MapView: public QGraphicsView {
    Q_OBJECT
public:
    enum class Tool { Select, PlaceCheckpoint, PlaceSpawn, PlaceStart, PlaceFinish, Erase };

    explicit MapView(QWidget* parent = nullptr);

    void setTool(Tool t) { currentTool = t; }
    Tool tool() const { return currentTool; }
    void setCurrentSpawnAngle(float a) { currentSpawnAngle = a; }
    float currentSpawnAngleValue() const { return currentSpawnAngle; }

    void loadMap(const QPixmap& pm);
    void clearAll();
    QSize mapPixelSize() const;
    QRectF mapRect() const;
    QGraphicsPixmapItem* mapItem() const { return background; }
    QGraphicsScene* scenePtr() const { return scene(); }
    void zoomIn() { scale(1.2, 1.2); }
    void zoomOut() { scale(1.0 / 1.2, 1.0 / 1.2); }
    void resetZoom() { fitMap(); }
    void updateCheckpointLines();


signals:
    void itemAdded(QGraphicsItem* it);
    void itemRemoved(QGraphicsItem* it);

protected:
    void wheelEvent(QWheelEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void keyPressEvent(QKeyEvent* e) override;
    void keyReleaseEvent(QKeyEvent* e) override;

private:
    Tool currentTool = Tool::Select;
    QGraphicsPixmapItem* background = nullptr;
    bool panning = false;
    QPoint panStart;

    QPointF toScene(const QPoint& viewportPos) const { return mapToScene(viewportPos); }
    bool insideMap(const QPointF& scenePos) const;

    void fitMap();
    void placeItemAt(const QPointF& scenePos);
    QList<QGraphicsLineItem*> cpLines;
    float currentSpawnAngle = 0.0f;
};

#endif  // MAPVIEW_H
