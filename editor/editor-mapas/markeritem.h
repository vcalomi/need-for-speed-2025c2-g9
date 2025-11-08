#ifndef MARKERITEM_H
#define MARKERITEM_H

#include <QGraphicsItem>
#include <QPainter>

enum class MarkerKind { Checkpoint, Hint, Spawn, Start, Finish };

class MarkerItem: public QGraphicsItem {
public:
    explicit MarkerItem(MarkerKind k, QGraphicsItem* parent = nullptr):
            QGraphicsItem(parent), kind(k) {
        setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
        setZValue(10);
    }

    QRectF boundingRect() const override { return QRectF(-8, -8, 16, 16); }

    void paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*) override {
        p->setRenderHint(QPainter::Antialiasing, true);
        QPen pen(Qt::black, 1);
        p->setPen(pen);

        switch (kind) {
            case MarkerKind::Checkpoint:
                p->setBrush(Qt::red);
                p->drawEllipse(-6, -6, 12, 12);
                break;
            case MarkerKind::Hint: {
                p->setBrush(Qt::yellow);
                QPolygonF arrow;
                arrow << QPointF(0, -6) << QPointF(6, 6) << QPointF(-6, 6);
                p->drawPolygon(arrow);
                break;
            }
            case MarkerKind::Spawn:
                p->setBrush(Qt::blue);
                p->drawRect(-6, -6, 12, 12);
                break;
            case MarkerKind::Start:
                p->setBrush(Qt::green);
                p->drawEllipse(-10, -10, 20, 20);
                p->drawText(QRectF(-6, -6, 16, 16), Qt::AlignCenter, "S");
                break;
            case MarkerKind::Finish:
                p->setBrush(Qt::white);
                p->drawEllipse(-10, -10, 20, 20);
                p->drawText(QRectF(-6, -6, 16, 16), Qt::AlignCenter, "F");
                break;
        }
    }

    MarkerKind kindOf() const { return kind; }

private:
    MarkerKind kind;
};

#endif  // MARKERITEM_H
