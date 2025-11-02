#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "yamlhandler.h"
#include <QMouseEvent>
#include <QGraphicsLineItem>
#include <QPixmap>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
        ui(new Ui::MainWindow),
        scene(new QGraphicsScene(this)) {

    ui->setupUi(this);
    ui->graphicsViewMap->setScene(scene);
    ui->comboCity->addItems({"Liberty City", "San Andreas", "Vice City"});
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_loadMapBtn_clicked() {
    QString city = ui->comboCity->currentText();
    QString mapPath;

    if (city == "Liberty City")
        mapPath = ":/images/liberty_city.png";
    else if (city == "San Andreas")
        mapPath = ":/images/san_andreas.png";
    else
        mapPath = ":/images/vice_city.png";

    // Cargar imagen desde recursos
    QPixmap pixmap(mapPath);
    if (pixmap.isNull()) {
        qDebug() << "No se pudo cargar el mapa:" << mapPath;
        return;
    }

    scene->clear();

    QGraphicsPixmapItem* background = scene->addPixmap(pixmap);
    background->setZValue(-1);

    currentMapSize = pixmap.size(); // Guardamos el tamaño del mapa cargado

    ui->graphicsViewMap->fitInView(background, Qt::KeepAspectRatio);

}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    // Verificamos si el click fue dentro del graphicsView
    if (!ui->graphicsViewMap->rect().contains(ui->graphicsViewMap->mapFromGlobal(event->globalPos())))
        return;

    // Convertir posición de click a coordenadas de escena
    QPointF scenePos = ui->graphicsViewMap->mapToScene(
        ui->graphicsViewMap->mapFromGlobal(event->globalPos())
    );

    // Normalizar (0 a 1) según el tamaño actual del mapa
    if (currentMapSize.isEmpty()) return;

    QPointF normalized(scenePos.x() / currentMapSize.width(),
                       scenePos.y() / currentMapSize.height());

    checkpoints.append(normalized);

    // Dibujar el checkpoint en coordenadas reales (desnormalizadas)
    QPointF realPos(normalized.x() * currentMapSize.width(),
                    normalized.y() * currentMapSize.height());
    drawCheckpoint(realPos, checkpoints.size());

    if (checkpoints.size() > 1) {
        QPointF prev(checkpoints[checkpoints.size() - 2].x() * currentMapSize.width(),
                     checkpoints[checkpoints.size() - 2].y() * currentMapSize.height());
        drawLine(prev, realPos);
    }
}


void MainWindow::drawCheckpoint(const QPointF &p, int id) {
    int size = 10;
    auto *circle = scene->addEllipse(p.x() - size / 2, p.y() - size / 2, size, size,
                                     QPen(Qt::red), QBrush(Qt::red));
    auto *label = scene->addText(QString::number(id));
    label->setDefaultTextColor(Qt::white);
    label->setPos(p.x() + 8, p.y() - 8);
}

void MainWindow::drawLine(const QPointF &from, const QPointF &to) {
    scene->addLine(QLineF(from, to), QPen(Qt::yellow, 2));
}

void MainWindow::on_cleanBtn_clicked() {
    scene->clear();
    checkpoints.clear();
}

void MainWindow::on_saveMapBtn_clicked() {
    if (checkpoints.isEmpty()) {
        QMessageBox::warning(this, "Error", "No hay checkpoints para guardar.");
        return;
    }

    QString filename = YamlHandler::getSaveFilename();
    if (filename.isEmpty()) return;

    YamlHandler::save(filename,
                      checkpoints,
                      ui->comboCity->currentText(),
                      currentMapSize);  
                      
    QMessageBox::information(this, "Guardado", "Recorrido guardado exitosamente.");
}



