#ifndef YAMLHANDLER_H
#define YAMLHANDLER_H

#include <QGraphicsScene>
#include <QPointF>
#include <QSize>
#include <QString>
#include <QVector>

namespace YamlHandler {
QString getSaveFilename();
void saveSceneAsTrack(const QString& filename, QGraphicsScene* scene, const QSize& mapPixelSize,
                      const QString& cityId);
}  // namespace YamlHandler

#endif  // YAMLHANDLER_H
