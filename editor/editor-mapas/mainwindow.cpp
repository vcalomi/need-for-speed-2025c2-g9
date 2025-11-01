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

    // Limpiar escena anterior si había algo
    scene->clear();

    // Crear un QGraphicsPixmapItem y agregarlo a la escena
    QGraphicsPixmapItem* background = scene->addPixmap(pixmap);
    background->setZValue(-1);  // para que quede detrás de los checkpoints

    // Ajustar tamaño de la vista
    ui->graphicsViewMap->fitInView(background, Qt::KeepAspectRatio);
}


void MainWindow::mousePressEvent(QMouseEvent *event) {
    QPointF scenePos = ui->graphicsViewMap->mapToScene(event->pos());
    checkpoints.append(scenePos);

    drawCheckpoint(scenePos, checkpoints.size());
    if (checkpoints.size() > 1)
        drawLine(checkpoints[checkpoints.size() - 2], checkpoints.last());
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

    YamlHandler::save(filename, checkpoints, ui->comboCity->currentText());
    QMessageBox::information(this, "Guardado", "Recorrido guardado exitosamente.");
}
