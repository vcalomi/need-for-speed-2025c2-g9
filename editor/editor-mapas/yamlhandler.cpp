#include "yamlhandler.h"

#include <QFileDialog>
#include <fstream>

#include <yaml-cpp/yaml.h>

QString YamlHandler::getSaveFilename() {
    return QFileDialog::getSaveFileName(nullptr, QObject::tr("Guardar recorrido"), "output/",
                                        QObject::tr("Archivos YAML (*.yaml)"));
}

void YamlHandler::save(const QString& filename, const QVector<QPointF>& checkpoints,
                       const QString& city, const QSize& canvasSize) {
    if (filename.isEmpty() || canvasSize.isEmpty())
        return;

    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "city" << YAML::Value << city.toStdString();
    out << YAML::Key << "checkpoints" << YAML::Value << YAML::BeginSeq;

    int id = 1;
    for (const auto& p: checkpoints) {
        // Normalizamos entre 0 y 1
        double normX = p.x() / canvasSize.width();
        double normY = p.y() / canvasSize.height();

        out << YAML::BeginMap;
        out << YAML::Key << "id" << YAML::Value << id++;
        out << YAML::Key << "x" << YAML::Value << normX;
        out << YAML::Key << "y" << YAML::Value << normY;
        out << YAML::EndMap;
    }

    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::ofstream fout(filename.toStdString());
    fout << out.c_str();
    fout.close();
}
