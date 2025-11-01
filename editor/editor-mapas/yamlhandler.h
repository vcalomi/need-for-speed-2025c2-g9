#ifndef YAMLHANDLER_H
#define YAMLHANDLER_H

#include <QString>
#include <QVector>
#include <QPointF>
#include <yaml-cpp/yaml.h>

class YamlHandler {
public:
    static QString getSaveFilename();

    static void save(const QString &filename,
                     const QVector<QPointF> &checkpoints,
                     const QString &city);
};

#endif // YAMLHANDLER_H
