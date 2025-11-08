#include "yamlhandler.h"

#include <QFileDialog>
#include <QGraphicsItem>
#include <fstream>

#include <yaml-cpp/yaml.h>

#include "markeritem.h"

static QPointF normalize(const QPointF& p, const QSize& canvas) {
    return QPointF(p.x() / canvas.width(), p.y() / canvas.height());
}

QString YamlHandler::getSaveFilename() {
    return QFileDialog::getSaveFileName(nullptr, QObject::tr("Save track"), "output/",
                                        QObject::tr("YAML files (*.yaml)"));
}

void YamlHandler::saveSceneAsTrack(const QString& filename, QGraphicsScene* scene,
                                   const QSize& mapPixelSize, const QString& cityId) {
    if (filename.isEmpty() || !scene || mapPixelSize.isEmpty())
        return;

    // Recolectar elementos
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
    out << YAML::Key << "track_id" << YAML::Value << "track";
    out << YAML::Key << "track_name" << YAML::Value << "track";
    out << YAML::Key << "city_id" << YAML::Value << cityId.toStdString();
    out << YAML::Key << "laps" << YAML::Value << 1;

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

    std::ofstream fout(filename.toStdString());
    fout << out.c_str();
    fout.close();
}
