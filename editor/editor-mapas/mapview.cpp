#include "mapview.h"

#include <QApplication>
#include <QCursor>
#include <QGraphicsEllipseItem>
#include <QScrollBar>

MapView::MapView(QWidget* parent): QGraphicsView(parent) {
    setScene(new QGraphicsScene(this));
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setBackgroundBrush(QColor("#1e1e1e"));
    setDragMode(QGraphicsView::RubberBandDrag);
}

void MapView::loadMap(const QPixmap& pm) {
    if (background) {
        scene()->removeItem(background);
        delete background;
        background = nullptr;
    }
    background = scene()->addPixmap(pm);
    background->setZValue(-100);
    scene()->setSceneRect(background->boundingRect());
    fitMap();
}

void MapView::clearAll() {
    for (auto* it: scene()->items()) {
        if (it != background)
            scene()->removeItem(it);
    }
}

QSize MapView::mapPixelSize() const { return background ? background->pixmap().size() : QSize(); }
QRectF MapView::mapRect() const { return background ? background->boundingRect() : QRectF(); }

void MapView::fitMap() {
    if (!background)
        return;
    fitInView(background, Qt::KeepAspectRatio);
}

void MapView::wheelEvent(QWheelEvent* e) {
    const double factor = 1.15;
    if (e->modifiers() & Qt::ControlModifier) {
        if (e->angleDelta().y() > 0)
            scale(factor, factor);
        else
            scale(1.0 / factor, 1.0 / factor);
        e->accept();
    } else {
        QGraphicsView::wheelEvent(e);
    }
}

void MapView::mousePressEvent(QMouseEvent* e) {
    if ((e->button() == Qt::MiddleButton) ||
        (e->button() == Qt::LeftButton && cursor().shape() == Qt::OpenHandCursor)) {
        panning = true;
        panStart = e->pos();
        setCursor(Qt::ClosedHandCursor);
        e->accept();
        return;
    }

    if (e->button() == Qt::LeftButton) {
        if (currentTool == Tool::Select) {
            QGraphicsView::mousePressEvent(e);
        } else {
            auto sp = toScene(e->pos());
            if (insideMap(sp))
                placeItemAt(sp);
            e->accept();
        }
        return;
    }

    if (e->button() == Qt::RightButton && currentTool == Tool::Erase) {
        auto sp = toScene(e->pos());
        auto itemsHere = scene()->items(sp);
        for (auto* it: itemsHere) {
            if (it == background)
                continue;
            scene()->removeItem(it);
            emit this->itemRemoved(it);
            delete it;
            updateCheckpointLines();
            break;
        }
        e->accept();
        return;
    }

    QGraphicsView::mousePressEvent(e);
}

void MapView::mouseMoveEvent(QMouseEvent* e) {
    if (panning) {
        QPointF delta = e->pos() - panStart;
        panStart = e->pos();
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        e->accept();
        return;
    }
    QGraphicsView::mouseMoveEvent(e);
}

void MapView::mouseReleaseEvent(QMouseEvent* e) {
    if (panning) {
        panning = false;
        setCursor(Qt::ArrowCursor);
        e->accept();
        return;
    }
    QGraphicsView::mouseReleaseEvent(e);
}

void MapView::keyPressEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_Space) {
        setCursor(Qt::OpenHandCursor);
        e->accept();
        return;
    }
    QGraphicsView::keyPressEvent(e);
}

void MapView::keyReleaseEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_Space) {
        setCursor(Qt::ArrowCursor);
        e->accept();
        return;
    }
    QGraphicsView::keyReleaseEvent(e);
}

bool MapView::insideMap(const QPointF& p) const {
    return background && background->boundingRect().contains(p);
}

void MapView::placeItemAt(const QPointF& scenePos) {
    if (!background)
        return;

    MarkerKind k = MarkerKind::Checkpoint;

    switch (currentTool) {
        case Tool::PlaceCheckpoint:
            k = MarkerKind::Checkpoint;
            break;
        case Tool::PlaceHint:
            k = MarkerKind::Hint;
            break;
        case Tool::PlaceSpawn:
            k = MarkerKind::Spawn;
            break;
        case Tool::PlaceStart:
            k = MarkerKind::Start;
            break;
        case Tool::PlaceFinish:
            k = MarkerKind::Finish;
            break;
        default:
            return;
    }

    auto* it = new MarkerItem(k);
    it->setPos(scenePos);
    scene()->addItem(it);
    emit itemAdded(it);
    updateCheckpointLines();
}

void MapView::updateCheckpointLines() {
    // Borrar líneas existentes
    for (auto* ln: cpLines) scene()->removeItem(ln);
    cpLines.clear();

    // Recolectar checkpoints en orden
    QList<QPointF> pts;

    for (auto* it: scene()->items(Qt::AscendingOrder)) {
        if (auto* mi = dynamic_cast<MarkerItem*>(it)) {
            if (mi->kindOf() == MarkerKind::Checkpoint) {
                pts.append(mi->sceneBoundingRect().center());
            }
        }
    }

    // Dibujar líneas entre puntos consecutivos
    for (int i = 0; i < pts.size() - 1; i++) {
        QLineF line(pts[i], pts[i + 1]);
        auto* li = scene()->addLine(line, QPen(QColor("#00ffff"), 3));
        li->setZValue(-50);
        cpLines.append(li);
    }
}
