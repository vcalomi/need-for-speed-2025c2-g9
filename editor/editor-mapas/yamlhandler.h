#ifndef YAMLHANDLER_H
#define YAMLHANDLER_H

#include <QString>
#include <QVector>
#include <QPointF>
#include <yaml-cpp/yaml.h>
#include <QSize>

class YamlHandler {
public:
    static QString getSaveFilename();

    static void save(const QString &filename,
                     const QVector<QPointF> &checkpoints,
                     const QString &city,
                     const QSize &canvasSize);
};

#endif // YAMLHANDLER_H
