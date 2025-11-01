#include "yamlhandler.h"
#include <QFileDialog>
#include <fstream>

QString YamlHandler::getSaveFilename() {
    return QFileDialog::getSaveFileName(nullptr,
                                        QObject::tr("Guardar recorrido"),
                                        "output/",
                                        QObject::tr("Archivos YAML (*.yaml)"));
}

void YamlHandler::save(const QString &filename,
                       const QVector<QPointF> &checkpoints,
                       const QString &city) {
    if (filename.isEmpty()) return;

    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "city" << YAML::Value << city.toStdString();
    out << YAML::Key << "checkpoints" << YAML::Value << YAML::BeginSeq;

    int id = 1;
    for (const auto &p : checkpoints) {
        out << YAML::BeginMap;
        out << YAML::Key << "id" << YAML::Value << id++;
        out << YAML::Key << "x" << YAML::Value << p.x();
        out << YAML::Key << "y" << YAML::Value << p.y();
        out << YAML::EndMap;
    }

    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::ofstream fout(filename.toStdString());
    fout << out.c_str();
    fout.close();
}
