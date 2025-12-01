#include "mainwindow.h"

#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QPixmap>
#include <QShortcut>
#include <QTimer>

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

    QTimer::singleShot(0, this, &MainWindow::loadMapFromCity);
    connect(ui->comboCity, &QComboBox::currentTextChanged, this, &MainWindow::loadMapFromCity);

    // Botones
    connect(ui->checkpointToolBtn, &QPushButton::clicked, this, &MainWindow::toolCheckpoint);
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

void MainWindow::loadMapFromCity() {
    QPixmap pm = mapForCity(currentCityId());
    if (pm.isNull()) {
        QMessageBox::warning(this, "Error", "Could not load map image.");
        return;
    }

    view->clearAll();
    view->loadMap(pm);
    history.clear();
}

void MainWindow::on_loadMapBtn_clicked() {
    QString fname = YamlHandler::getOpenFilename(this);
    if (fname.isEmpty())
        return;

    QString cityId = YamlHandler::readCityId(fname);
    if (cityId.isEmpty()) {
        QMessageBox::warning(this, "Error", "Invalid YAML: missing city_id");
        return;
    }

    if (cityId == "liberty_city")
        ui->comboCity->setCurrentText("Liberty City");
    else if (cityId == "san_andreas")
        ui->comboCity->setCurrentText("San Andreas");
    else
        ui->comboCity->setCurrentText("Vice City");

    loadMapFromCity();

    if (!YamlHandler::loadSceneFromTrack(fname, view->scenePtr())) {
        QMessageBox::warning(this, "Error", "Could not load track elements.");
        return;
    }

    view->updateCheckpointLines();

    history.clear();
}

void MainWindow::on_saveMapBtn_clicked() {
    QString fname = YamlHandler::getSaveFilename(this);
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

QPixmap MainWindow::mapForCity(const QString& cityId) const {
    QString base = QDir::cleanPath(QDir::currentPath() + "/../editor/editor-mapas/images");

    QString fileName;

    if (cityId == "liberty_city")
        fileName = base + "/liberty_city.png";
    else if (cityId == "san_andreas")
        fileName = base + "/san_andreas.png";
    else if (cityId == "vice_city")
        fileName = base + "/vice_city.png";
    else
        return QPixmap();

    QPixmap pm(fileName);
    if (pm.isNull()) {
        qWarning() << "Failed to load map image:" << fileName;
    }
    return pm;
}

void MainWindow::toolCheckpoint() { view->setTool(MapView::Tool::PlaceCheckpoint); }
void MainWindow::toolSpawn() { view->setTool(MapView::Tool::PlaceSpawn); }
void MainWindow::toolStart() { view->setTool(MapView::Tool::PlaceStart); }
void MainWindow::toolFinish() { view->setTool(MapView::Tool::PlaceFinish); }
void MainWindow::toolErase() { view->setTool(MapView::Tool::Erase); }
