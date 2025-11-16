#include "yamlhandler.h"

#include <QFileDialog>
#include <QGraphicsItem>
#include <fstream>
#include <string>

#include <yaml-cpp/yaml.h>

#include "markeritem.h"

static QPointF normalize(const QPointF& p, const QSize& canvas) {
    return QPointF(p.x() / canvas.width(), p.y() / canvas.height());
}

QString YamlHandler::getOpenFilename(QWidget* parent) {
    QFileDialog dialog(parent, "Open track", "output/", "YAML files (*.yaml *.yml)");
    dialog.setOption(QFileDialog::DontUseNativeDialog, true);

    if (dialog.exec() == QDialog::Accepted)
        return dialog.selectedFiles().first();

    return "";
}


QString YamlHandler::readCityId(const QString& filename) {
    YAML::Node file = YAML::LoadFile(filename.toStdString());
    if (!file["city_id"])
        return "";
    return QString::fromStdString(file["city_id"].as<std::string>());
}

static QPointF denormalize(const QPointF& n, const QSize& size) {
    return QPointF(n.x() * size.width(), n.y() * size.height());
}

QString YamlHandler::getSaveFilename(QWidget* parent) {
    QFileDialog dialog(nullptr, "Save track", QDir::currentPath() + "/editor/editor-mapas/maps/",
                       "YAML files (*.yaml *.yml)");

    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setOption(QFileDialog::DontUseNativeDialog, true);

    if (dialog.exec() != QDialog::Accepted)
        return "";

    QString fname = dialog.selectedFiles().first();

    // Asegurar extensión .yaml
    if (!fname.endsWith(".yaml") && !fname.endsWith(".yml"))
        fname += ".yaml";

    return fname;
}

void YamlHandler::saveSceneAsTrack(const QString& filename, QGraphicsScene* scene,
                                   const QSize& mapPixelSize, const QString& cityId) {
    if (filename.isEmpty() || !scene || mapPixelSize.isEmpty())
        return;

    struct Checkpoint {
        int id;
        QPointF pos;
        QVector<QPair<QPointF, double>> hints;
    };
    QVector<Checkpoint> cps;
    QVector<QPointF> spawns;
    QPointF start(-1, -1), finish(-1, -1);

    // Asignar IDs a checkpoints en orden de creación (orden de items en escena)
    int nextId = 1;

    for (auto* gi: scene->items(Qt::AscendingOrder)) {
        auto* mi = dynamic_cast<MarkerItem*>(gi);
        if (!mi)
            continue;

        QPointF center = mi->sceneBoundingRect().center();
        QPointF n = normalize(center, mapPixelSize);

        switch (mi->kindOf()) {
            case MarkerKind::Checkpoint: {
                Checkpoint c;
                c.id = nextId++;
                c.pos = n;
                cps.push_back(c);
                break;
            }
            case MarkerKind::Hint: {
                if (cps.isEmpty())
                    break;
                int idx = cps.size() - 1;
                cps[idx].hints.push_back({n, 0.0});
                break;
            }
            case MarkerKind::Spawn:
                spawns.push_back(n);
                break;
            case MarkerKind::Start:
                start = n;
                break;
            case MarkerKind::Finish:
                finish = n;
                break;
        }
    }

    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "city_id" << YAML::Value << cityId.toStdString();
    out << YAML::Key << "checkpoints" << YAML::Value << YAML::BeginSeq;
    for (const auto& c: cps) {
        out << YAML::BeginMap;
        out << YAML::Key << "id" << YAML::Value << c.id;
        out << YAML::Key << "position" << YAML::Value << YAML::Flow << YAML::BeginSeq << c.pos.x()
            << c.pos.y() << YAML::EndSeq;
        out << YAML::Key << "hints" << YAML::Value << YAML::BeginSeq;
        for (const auto& h: c.hints) {
            out << YAML::BeginMap;
            out << YAML::Key << "position" << YAML::Value << YAML::Flow << YAML::BeginSeq
                << h.first.x() << h.first.y() << YAML::EndSeq;
            out << YAML::EndMap;
        }
        out << YAML::EndSeq;  // hints
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;  // checkpoints

    out << YAML::Key << "player_spawns" << YAML::Value << YAML::BeginSeq;
    int pid = 1;
    for (const auto& s: spawns) {
        out << YAML::BeginMap;
        out << YAML::Key << "id" << YAML::Value << ("p" + std::to_string(pid++));
        out << YAML::Key << "position" << YAML::Value << YAML::Flow << YAML::BeginSeq << s.x()
            << s.y() << YAML::EndSeq;
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;

    if (start.x() >= 0) {
        out << YAML::Key << "start" << YAML::Value << YAML::BeginMap << YAML::Key << "position"
            << YAML::Value << YAML::Flow << YAML::BeginSeq << start.x() << start.y() << YAML::EndSeq
            << YAML::EndMap;
    }
    if (finish.x() >= 0) {
        out << YAML::Key << "finish" << YAML::Value << YAML::BeginMap << YAML::Key << "position"
            << YAML::Value << YAML::Flow << YAML::BeginSeq << finish.x() << finish.y()
            << YAML::EndSeq << YAML::EndMap;
    }

    out << YAML::EndMap;

    std::ofstream fout;
    fout.open(filename.toStdString(), std::ios::out | std::ios::trunc);
    fout << out.c_str();
    fout.close();
}

bool YamlHandler::loadSceneFromTrack(const QString& filename, QGraphicsScene* scene,
                                     const QSize& mapSize) {
    if (!scene || mapSize.isEmpty())
        return false;

    YAML::Node file = YAML::LoadFile(filename.toStdString());

    // Checkpoints
    if (file["checkpoints"]) {
        for (const auto& cp: file["checkpoints"]) {
            auto pos = cp["position"];
            QPointF p(pos[0].as<double>(), pos[1].as<double>());
            QPointF d = denormalize(p, mapSize);

            auto* it = new MarkerItem(MarkerKind::Checkpoint);
            it->setPos(d);
            scene->addItem(it);

            // Hints
            if (cp["hints"]) {
                for (const auto& h: cp["hints"]) {
                    auto pos2 = h["position"];
                    QPointF hp(pos2[0].as<double>(), pos2[1].as<double>());
                    QPointF hd = denormalize(hp, mapSize);

                    auto* hint = new MarkerItem(MarkerKind::Hint);
                    hint->setPos(hd);
                    scene->addItem(hint);
                }
            }
        }
    }

    if (file["start"]) {
        auto pos = file["start"]["position"];
        QPointF p(pos[0].as<double>(), pos[1].as<double>());
        auto* it = new MarkerItem(MarkerKind::Start);
        it->setPos(denormalize(p, mapSize));
        scene->addItem(it);
    }

    if (file["finish"]) {
        auto pos = file["finish"]["position"];
        QPointF p(pos[0].as<double>(), pos[1].as<double>());
        auto* it = new MarkerItem(MarkerKind::Finish);
        it->setPos(denormalize(p, mapSize));
        scene->addItem(it);
    }

    if (file["player_spawns"]) {
        for (const auto& sp: file["player_spawns"]) {
            auto pos = sp["position"];
            QPointF p(pos[0].as<double>(), pos[1].as<double>());

            auto* it = new MarkerItem(MarkerKind::Spawn);
            it->setPos(denormalize(p, mapSize));
            scene->addItem(it);
        }
    }
    return true;
}
