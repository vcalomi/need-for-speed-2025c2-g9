#include "mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QShortcut>

#include "markeritem.h"
#include "ui_mainwindow.h"
#include "yamlhandler.h"

MainWindow::MainWindow(QWidget* parent):
        QMainWindow(parent), ui(new Ui::MainWindow), view(nullptr) {

    ui->setupUi(this);
    showMaximized();

    view = new MapView(this);
    ui->horizontalLayout->replaceWidget(ui->graphicsViewMap, view);
    ui->graphicsViewMap->deleteLater();

    ui->comboCity->addItems({"Liberty City", "San Andreas", "Vice City"});

    // Botones
    connect(ui->loadMapBtn, &QPushButton::clicked, this, &MainWindow::on_loadMapBtn_clicked);
    connect(ui->saveMapBtn, &QPushButton::clicked, this, &MainWindow::on_saveMapBtn_clicked);
    connect(ui->cleanBtn, &QPushButton::clicked, this, &MainWindow::on_cleanBtn_clicked);

    connect(ui->selectToolBtn, &QPushButton::clicked, this, &MainWindow::toolSelect);
    connect(ui->checkpointToolBtn, &QPushButton::clicked, this, &MainWindow::toolCheckpoint);
    connect(ui->hintToolBtn, &QPushButton::clicked, this, &MainWindow::toolHint);
    connect(ui->spawnToolBtn, &QPushButton::clicked, this, &MainWindow::toolSpawn);
    connect(ui->startToolBtn, &QPushButton::clicked, this, &MainWindow::toolStart);
    connect(ui->finishToolBtn, &QPushButton::clicked, this, &MainWindow::toolFinish);

    connect(ui->zoomInBtn, &QPushButton::clicked, view, &MapView::zoomIn);
    connect(ui->zoomOutBtn, &QPushButton::clicked, view, &MapView::zoomOut);
    connect(ui->resetZoomBtn, &QPushButton::clicked, view, &MapView::resetZoom);

    // Atajo Ctrl+Z
    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Z), this, SLOT(onUndo()));

    // Conexión para undo/redo
    connect(view, &MapView::itemAdded, this, [this](QGraphicsItem* it) {
        history.push({Command::Add, it});
    });
    connect(view, &MapView::itemRemoved, this, [this](QGraphicsItem* it) {
        history.push({Command::Remove, it});
    });
}

MainWindow::~MainWindow() { delete ui; }

QString MainWindow::currentCityId() const {
    const QString c = ui->comboCity->currentText();
    if (c == "Liberty City")
        return "liberty_city";
    if (c == "San Andreas")
        return "san_andreas";
    return "vice_city";
}

QPixmap MainWindow::mapForCity(const QString& city) const {
    if (city == "Liberty City")
        return QPixmap(":/images/liberty_city.png");
    if (city == "San Andreas")
        return QPixmap(":/images/san_andreas.png");
    return QPixmap(":/images/vice_city.png");
}

void MainWindow::on_loadMapBtn_clicked() {
    QPixmap pm = mapForCity(ui->comboCity->currentText());
    if (pm.isNull()) {
        QMessageBox::warning(this, "Error", "Could not load the selected map image.");
        return;
    }

    view->clearAll();
    view->loadMap(pm);
    history.clear();
}

void MainWindow::on_saveMapBtn_clicked() {
    QString fname = YamlHandler::getSaveFilename();
    if (fname.isEmpty())
        return;

    YamlHandler::saveSceneAsTrack(fname, view->scenePtr(), view->mapPixelSize(), currentCityId());
    QMessageBox::information(this, "Saved", "Track saved successfully.");
}

void MainWindow::on_cleanBtn_clicked() {
    view->clearAll();
    history.clear();
}

void MainWindow::onUndo() {
    if (history.isEmpty())
        return;
    auto cmd = history.pop();
    if (cmd.type == Command::Add) {
        if (cmd.item) {
            view->scenePtr()->removeItem(cmd.item);
            delete cmd.item;
        }
    } else if (cmd.type == Command::Remove) {
        if (cmd.item)
            view->scenePtr()->addItem(cmd.item);
    }
}

void MainWindow::toolSelect() { view->setTool(MapView::Tool::Select); }
void MainWindow::toolCheckpoint() { view->setTool(MapView::Tool::PlaceCheckpoint); }
void MainWindow::toolHint() { view->setTool(MapView::Tool::PlaceHint); }
void MainWindow::toolSpawn() { view->setTool(MapView::Tool::PlaceSpawn); }
void MainWindow::toolStart() { view->setTool(MapView::Tool::PlaceStart); }
void MainWindow::toolFinish() { view->setTool(MapView::Tool::PlaceFinish); }
void MainWindow::toolErase() { view->setTool(MapView::Tool::Erase); }
