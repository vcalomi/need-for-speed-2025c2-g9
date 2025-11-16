#ifndef YAMLHANDLER_H
#define YAMLHANDLER_H

#include <QGraphicsScene>
#include <QSize>
#include <QString>

class YamlHandler {
public:
    static QString getSaveFilename(QWidget* parent = nullptr);
    static QString getOpenFilename(QWidget* parent = nullptr);

    static void saveSceneAsTrack(const QString& filename, QGraphicsScene* scene,
                                 const QSize& mapPixelSize, const QString& cityId);

    static QString readCityId(const QString& filename);

    static bool loadSceneFromTrack(const QString& filename, QGraphicsScene* scene,
                                   const QSize& mapPixelSize);
};

#endif  // YAMLHANDLER_H
