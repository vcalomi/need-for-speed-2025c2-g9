#include "yamlhandler.h"

#include <QFileDialog>
#include <QGraphicsItem>
#include <QMessageBox>
#include <fstream>
#include <string>

#include <yaml-cpp/yaml.h>

#include "markeritem.h"

static QPointF normalize(const QPointF& p, const QSize& canvas) {
    if (canvas.width() == 0 || canvas.height() == 0)
        return QPointF(0, 0);
    return QPointF(p.x() / canvas.width(), p.y() / canvas.height());
}

QString YamlHandler::getOpenFilename(QWidget* parent) {
    QString baseDir = QDir::cleanPath(QDir::currentPath() + "/../server/maps");

    QFileDialog dialog(parent, "Open track", baseDir, "YAML files (*.yaml *.yml)");
    dialog.setOption(QFileDialog::DontUseNativeDialog, true);

    if (dialog.exec() == QDialog::Accepted)
        return dialog.selectedFiles().first();

    return "";
}

QString YamlHandler::readCityId(const QString& filename) {
    try {
        YAML::Node file = YAML::LoadFile(filename.toStdString());
        if (file["city_id"])
            return QString::fromStdString(file["city_id"].as<std::string>());
    } catch (const std::exception&) {
        return "";
    }
    return "";
}

static QPointF denormalize(const QPointF& n, const QSize& size) {
    return QPointF(n.x() * size.width(), n.y() * size.height());
}

QString YamlHandler::getSaveFilename(QWidget* parent) {
    QString baseDir = QDir::cleanPath(QDir::currentPath() + "/../server/maps");

    // Crear carpeta si no existe
    QDir().mkpath(baseDir);

    QFileDialog dialog(parent, "Save track", baseDir, "YAML files (*.yaml *.yml)");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setOption(QFileDialog::DontUseNativeDialog, true);

    if (dialog.exec() != QDialog::Accepted)
        return "";

    QString fname = dialog.selectedFiles().first();

    // Asegurar extensión
    if (!fname.endsWith(".yaml") && !fname.endsWith(".yml"))
        fname += ".yaml";

    return fname;
}

void YamlHandler::saveSceneAsTrack(const QString& filename, QGraphicsScene* scene,
                                   const QSize& mapPixelSize, const QString& cityId) {
    if (filename.isEmpty() || !scene || mapPixelSize.isEmpty())
        return;

    QString mapImage;

    if (cityId == "liberty_city")
        mapImage = "liberty_city.png";
    else if (cityId == "san_andreas")
        mapImage = "san_andreas.png";
    else
        mapImage = "vice_city.png";

    QList<MarkerItem*> checkpoints;
    QList<MarkerItem*> spawns;

    MarkerItem* start = nullptr;
    MarkerItem* finish = nullptr;

    // Recolectar items
    for (auto* gi: scene->items(Qt::AscendingOrder)) {
        auto* mi = dynamic_cast<MarkerItem*>(gi);
        if (!mi)
            continue;

        switch (mi->kindOf()) {
            case MarkerKind::Checkpoint:
                checkpoints.append(mi);
                break;
            case MarkerKind::Spawn:
                spawns.append(mi);
                break;
            case MarkerKind::Start:
                start = mi;
                break;
            case MarkerKind::Finish:
                finish = mi;
                break;
        }
    }

    YAML::Emitter out;
    out << YAML::BeginMap;

    out << YAML::Key << "city_id" << YAML::Value << cityId.toStdString();
    out << YAML::Key << "map_image" << YAML::Value << mapImage.toStdString();

    out << YAML::Key << "checkpoints" << YAML::Value << YAML::BeginSeq;

    for (auto* cp: checkpoints) {
        QPointF n = normalize(cp->sceneBoundingRect().center(), mapPixelSize);

        out << YAML::BeginMap;
        out << YAML::Key << "position" << YAML::Value << YAML::Flow << YAML::BeginSeq << n.x()
            << n.y() << YAML::EndSeq;

        out << YAML::EndSeq;
        out << YAML::EndMap;
    }

    out << YAML::EndSeq;

    out << YAML::Key << "player_spawns" << YAML::Value << YAML::BeginSeq;
    for (auto* s: spawns) {
        QPointF n = normalize(s->pos(), mapPixelSize);
        out << YAML::BeginMap;
        out << YAML::Key << "position" << YAML::Value << YAML::Flow << YAML::BeginSeq << n.x()
            << n.y() << YAML::EndSeq;
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;

    if (start) {
        QPointF n = normalize(start->pos(), mapPixelSize);
        out << YAML::Key << "start" << YAML::Value << YAML::BeginMap;
        out << YAML::Key << "position" << YAML::Value << YAML::Flow << YAML::BeginSeq << n.x()
            << n.y() << YAML::EndSeq;
        out << YAML::EndMap;
    }

    if (finish) {
        QPointF n = normalize(finish->pos(), mapPixelSize);
        out << YAML::Key << "finish" << YAML::Value << YAML::BeginMap;
        out << YAML::Key << "position" << YAML::Value << YAML::Flow << YAML::BeginSeq << n.x()
            << n.y() << YAML::EndSeq;
        out << YAML::EndMap;
    }

    out << YAML::EndMap;

    std::ofstream fout(filename.toStdString(), std::ios::out | std::ios::trunc);
    fout << out.c_str();
    fout.close();
}

bool YamlHandler::loadSceneFromTrack(const QString& filename, QGraphicsScene* scene) {
    if (!scene)
        return false;

    YAML::Node root = YAML::LoadFile(filename.toStdString());

    if (!root["city_id"])
        return false;

    QString cityId = QString::fromStdString(root["city_id"].as<std::string>());
    QString mapImage;

    if (root["map_image"]) {
        mapImage = QString::fromStdString(root["map_image"].as<std::string>());
    } else {
        if (cityId == "liberty_city")
            mapImage = "liberty_city.png";
        else if (cityId == "san_andreas")
            mapImage = "san_andreas.png";
        else
            mapImage = "vice_city.png";
    }

    QString imagesBase = QDir::cleanPath(QDir::currentPath() + "/../editor/editor-mapas/images");
    QPixmap pm(imagesBase + "/" + mapImage);
    if (pm.isNull()) {
        QString fallback;
        if (cityId == "liberty_city")
            fallback = "liberty_city.png";
        else if (cityId == "san_andreas")
            fallback = "san_andreas.png";
        else
            fallback = "vice_city.png";

        pm = QPixmap(imagesBase + "/" + fallback);
    }

    for (auto* it: scene->items()) {
        if (dynamic_cast<QGraphicsPixmapItem*>(it) == nullptr) {
            scene->removeItem(it);
            delete it;
        }
    }

    QSize actualMapSize;
    if (!pm.isNull()) {
        actualMapSize = pm.size();
    } else {
        for (auto* it: scene->items()) {
            if (auto* pmi = dynamic_cast<QGraphicsPixmapItem*>(it)) {
                actualMapSize = pmi->pixmap().size();
                break;
            }
        }
    }

    if (actualMapSize.isEmpty())
        return false;

    if (root["checkpoints"]) {
        for (const auto& cp: root["checkpoints"]) {
            auto pos = cp["position"];
            QPointF p(pos[0].as<double>(), pos[1].as<double>());
            QPointF d = denormalize(p, actualMapSize);

            auto* it = new MarkerItem(MarkerKind::Checkpoint);
            it->setPos(d);
            scene->addItem(it);
        }
    }

    if (root["player_spawns"]) {
        for (const auto& sp: root["player_spawns"]) {
            auto pos = sp["position"];
            QPointF p(pos[0].as<double>(), pos[1].as<double>());
            QPointF d = denormalize(p, actualMapSize);

            auto* it = new MarkerItem(MarkerKind::Spawn);
            it->setPos(d);
            scene->addItem(it);
        }
    }

    if (root["start"]) {
        auto pos = root["start"]["position"];
        QPointF p(pos[0].as<double>(), pos[1].as<double>());
        QPointF d = denormalize(p, actualMapSize);

        auto* it = new MarkerItem(MarkerKind::Start);
        it->setPos(d);
        scene->addItem(it);
    }

    if (root["finish"]) {
        auto pos = root["finish"]["position"];
        QPointF p(pos[0].as<double>(), pos[1].as<double>());
        QPointF d = denormalize(p, actualMapSize);

        auto* it = new MarkerItem(MarkerKind::Finish);
        it->setPos(d);
        scene->addItem(it);
    }

    return true;
}
