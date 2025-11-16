#include "mainwindow.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfoList>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QProcess>
#include <QRandomGenerator>
#include <QScreen>
#include <QStackedWidget>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTimer>
#include <QUrl>
#include <QtMath>
#include <string>

#include "background_music.h"
#include "car_selection_controller.h"
#include "lobby_service.h"
#include "navigation.h"
#include "player_info.h"
#include "room_manager.h"
#include "rooms_pager.h"
#include "styles.h"
#include "ui_mainwindow.h"
#include "wait_room_controller.h"

void MainWindow::onWaitTimerTickHost() {
    try {
        if (this->lobbySvc->pollState().started) {
            game_started = true;
            this->close();
        }
    } catch (...) {}
}

void MainWindow::onWaitTimerTickJoin() {
    try {
        if (this->lobbySvc->pollState().started) {
            game_started = true;
            this->close();
        }
    } catch (...) {}
}

MainWindow::MainWindow(ClientProtocol& protocol, bool& game_started_ref, std::string& username_ref,
                       QWidget* parent):
        QMainWindow(parent),
        ui(new Ui::Lobby),
        protocol(protocol),
        lobbyApi(std::make_unique<LobbyApi>(this->protocol)),
        lobbySvc(std::make_unique<LobbyService>(*lobbyApi)),
        roomsPager(nullptr),
        waitCtrl(nullptr),
        carCtrl(nullptr),
        music(nullptr),
        game_started(game_started_ref),
        username(username_ref),
        refreshTimer(nullptr) {
    ui->setupUi(this);

    QTimer::singleShot(0, this, [this] {
        Navigation::goToPage(ui->stackedWidget->currentWidget(), ui->stackedWidget, this);
    });

    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, [this](int) {
        Navigation::goToPage(ui->stackedWidget->currentWidget(), ui->stackedWidget, this);
    });

    this->setCentralWidget(ui->centralwidget);
    this->centralWidget()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->stackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Instanciar controladores
    roomsPager = std::make_unique<RoomsPager>(*lobbySvc, ui);
    waitCtrl = std::make_unique<WaitRoomController>(*lobbySvc, ui, this);
    carCtrl = std::make_unique<CarSelectionController>(*lobbySvc, ui);

    carCtrl->initCars();
    music = std::make_unique<BackgroundMusic>(this);

    music->start();
    game_started = false;

    // Estilos y fuente global
    UIStyles::applyGlobalStyle();
    allRooms.clear();

    Navigation::goToPage(ui->page_connection, ui->stackedWidget, this);

    connect(ui->connectButton, &QPushButton::clicked, this, [this]() {
        if (this->protocol.isClientConnected()) {
            QMessageBox::critical(this, "Connection failed", "Connection is not available");
            return;
        }
        QMessageBox::information(this, "Connected", "Successfully connected to server");
        Navigation::goToPage(ui->page_username, ui->stackedWidget, this);
    });

    // Username
    connect(ui->btnConfirmUsername, &QPushButton::clicked, this, [this]() {
        QString name = ui->input_username->text().trimmed();

        if (name.isEmpty()) {
            QMessageBox::warning(this, "Username", "Please enter a valid username.");
            return;
        }

        player.username = name;

        try {
            if (!this->lobbySvc->login(player.username)) {
                QMessageBox::warning(this, "Username", "Username is already in use. Try another.");
                return;
            }
            username = name.toStdString();
            auto rooms = this->lobbySvc->listRooms();
            allRooms.clear();
            for (const auto& r: rooms) allRooms << r;
            showPage(0);
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("Server error: %1").arg(e.what()));
            return;
        }

        // Ir a la pantalla de salas dispo
        Navigation::goToPage(ui->page_menu, ui->stackedWidget, this);
    });


    // Botones del menú principal
    connect(ui->btnCreate, &QPushButton::clicked, this, &MainWindow::handleOpenMapsPage);
    connect(ui->btnJoin, &QPushButton::clicked, this, &MainWindow::handleJoinGame);
    connect(ui->btnMap, &QPushButton::clicked, this, &MainWindow::openEditorMap);

    connect(ui->btnSelectMaps, &QPushButton::clicked, this, &MainWindow::handleSelectMaps);

    // Paginación salas disponibles
    connect(ui->btnNextRooms, &QPushButton::clicked, this, [this] { roomsPager->showNext(); });
    connect(ui->btnPrevRooms, &QPushButton::clicked, this, [this] { roomsPager->showPrev(); });

    // Otros botones
    connect(ui->btnContinueRooms, &QPushButton::clicked, this,
            [this]() { Navigation::goToPage(ui->page_join, ui->stackedWidget, this); });

    connect(ui->btnContinue, &QPushButton::clicked, this, &MainWindow::handleContinueToWait);
    connect(ui->btnConfirmJoin, &QPushButton::clicked, this, &MainWindow::handleConfirmJoin);
    connect(ui->btnBackJoin, &QPushButton::clicked, this, &MainWindow::handleBackToMenu);
    connect(ui->btnRefresh, &QPushButton::clicked, this, &MainWindow::handleRefreshPlayers,
            Qt::UniqueConnection);
    connect(ui->btnStartGame, &QPushButton::clicked, this, &MainWindow::handleStartGame);

    connect(ui->listMaps, &QListWidget::itemClicked, this, [this](QListWidgetItem* item) {
        bool current = item->data(Qt::UserRole + 1).toBool();
        item->setData(Qt::UserRole + 1, !current);
        updateMapItemStyle(item);
    });

    connect(ui->btnChooseCar, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->page_car);
        carCtrl->updateImage();
    });

    // Elegir  autos con el carrousel
    connect(ui->btnNextCar, &QPushButton::clicked, this, [this]() { carCtrl->next(); });
    connect(ui->btnPrevCar, &QPushButton::clicked, this, [this]() { carCtrl->prev(); });

    connect(ui->btnBackToLobby, &QPushButton::clicked, this, [this]() {
        player.selectedCar = carCtrl->currentCarType();
        try {
            if (!carCtrl->submitSelectedCar()) {
                QMessageBox::warning(this, "Choose Car", "Failed to select car on server.");
                return;
            }
        } catch (const std::exception& e) {
            QMessageBox::warning(this, "Choose Car",
                                 QString("Failed to send car: %1").arg(e.what()));
        }
        ui->stackedWidget->setCurrentWidget(ui->page_wait);
    });

    connect(ui->btnBackToLobby, &QPushButton::clicked, this,
            [this]() { ui->stackedWidget->setCurrentWidget(ui->page_wait); });
    refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timeout, this, &MainWindow::handleRefreshPlayers);

    // Centrar ventana
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    this->move(screenGeometry.center() - this->rect().center());
}

void MainWindow::resizeEvent(QResizeEvent* e) {
    if (ui && ui->stackedWidget && ui->stackedWidget->currentWidget()) {
        Navigation::goToPage(ui->stackedWidget->currentWidget(), ui->stackedWidget, this);
    }
    QMainWindow::resizeEvent(e);
}

void MainWindow::openEditorMap() {
    // Ruta absoluta correcta al ejecutable del editor
    QString editorPath = QDir::cleanPath(
            QDir::currentPath() + "/../editor/editor-mapas/build/Desktop-Debug/editor-mapas");

    qDebug() << "Intentando abrir el editor en:" << editorPath;

    if (!QFile::exists(editorPath)) {
        QMessageBox::warning(this, "Error",
                             "No se encontró el ejecutable del editor:\n" + editorPath);
        return;
    }

    // Crear proceso
    QProcess* editorProcess = new QProcess(this);
    editorProcess->setProgram(editorPath);

    connect(editorProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            [this, editorProcess](int, QProcess::ExitStatus) {
                editorProcess->deleteLater();
                Navigation::goToPage(ui->page_menu, ui->stackedWidget, this);
            });

    editorProcess->start();

    if (!editorProcess->waitForStarted(2000)) {
        QMessageBox::warning(this, "Error", "No se pudo abrir el editor de mapas.");
        delete editorProcess;
    }
}

void MainWindow::handleOpenMapsPage() {
    // Ruta absoluta desde el lobby hasta los mapas
    QString mapsPath = QDir::cleanPath(QDir::currentPath() + "/../editor/editor-mapas/maps");

    qDebug() << "Buscando mapas en:" << mapsPath;

    QDir dir(mapsPath);
    if (!dir.exists()) {
        QMessageBox::warning(this, "Error", "No se encontró la carpeta de mapas:\n" + mapsPath);
        return;
    }

    ui->listMaps->clear();
    ui->listMaps->setSpacing(8);
    ui->listMaps->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->listMaps->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listMaps->setViewMode(QListView::ListMode);

    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    QFileInfoList mapFiles = dir.entryInfoList();

    if (mapFiles.isEmpty()) {
        ui->listMaps->addItem("No se encontraron recorridos disponibles.");
    } else {
        for (const QFileInfo& info: mapFiles) {
            QListWidgetItem* item = new QListWidgetItem(info.baseName());
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable |
                           Qt::ItemIsEnabled);
            item->setData(Qt::UserRole, info.absoluteFilePath());
            item->setCheckState(Qt::Unchecked);
            item->setData(Qt::UserRole + 1, false);
            ui->listMaps->addItem(item);
            updateMapItemStyle(item);
        }
    }
    Navigation::goToPage(ui->page_maps, ui->stackedWidget, this);
}

void MainWindow::onMapItemChanged(QListWidgetItem* item) { updateMapItemStyle(item); }

void MainWindow::updateMapItemStyle(QListWidgetItem* item) {
    if (!item)
        return;

    bool isSelected = item->data(Qt::UserRole + 1).toBool();

    if (isSelected) {
        item->setBackground(QColor("#ff6600"));
        item->setForeground(QColor("white"));
    } else {
        item->setBackground(QColor(255, 255, 255, 25));
        item->setForeground(QColor("white"));
    }
}

void MainWindow::handleSelectMaps() {
    QStringList selectedMaps;
    for (int i = 0; i < ui->listMaps->count(); ++i) {
        QListWidgetItem* item = ui->listMaps->item(i);
        if (item->checkState() == Qt::Checked) {
            selectedMaps << item->text();
        }
    }
    if (selectedMaps.isEmpty()) {
        QMessageBox::warning(this, "Selección vacía", "Debes seleccionar al menos un recorrido.");
        return;
    }
    qDebug() << "Mapas seleccionados:" << selectedMaps;
    handleCreateButton();
}

void MainWindow::showPage(int page) {
    ui->listRooms->clear();
    try {
        auto rooms = this->lobbySvc->listRooms();
        if (!rooms.empty()) {
            allRooms.clear();
            for (const auto& r: rooms) allRooms << r;
        }
    } catch (const std::exception& e) {}
    int totalPages = qMax(1, qCeil(allRooms.size() / static_cast<double>(PAGE_SIZE)));
    currentPage = qBound(0, page, totalPages - 1);

    int start = currentPage * PAGE_SIZE;
    int end = qMin(start + PAGE_SIZE, allRooms.size());
    for (int i = start; i < end; ++i) ui->listRooms->addItem(allRooms[i]);

    ui->labelPageInfo->setText(QString("Page %1 / %2").arg(currentPage + 1).arg(totalPages));

    ui->btnPrevRooms->setEnabled(currentPage > 0);
    ui->btnNextRooms->setEnabled(currentPage < totalPages - 1);
}

void MainWindow::showNextRoom() { showPage(currentPage + 1); }
void MainWindow::showPrevRoom() { showPage(currentPage - 1); }

void MainWindow::handleBackToMenu() {
    Navigation::goToPage(ui->page_menu, ui->stackedWidget, this);
}

void MainWindow::handleJoinGame() {
    player.isHost = false;

    try {
        // Pedir al servidor la lista de salas disponibles
        auto rooms = this->lobbySvc->listRooms();
        allRooms.clear();
        for (const auto& r: rooms) allRooms << r;
        showPage(0);

        // Navegar a la página de salas
        Navigation::goToPage(ui->page_rooms, ui->stackedWidget, this);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to load rooms: %1").arg(e.what()));
        return;
    }
}

void MainWindow::handleCreateGame() {
    player.isHost = true;
    Navigation::goToPage(ui->page_create, ui->stackedWidget, this);
}

void MainWindow::handleCreateButton() {
    QString code = RoomManager::generateRoomCode();
    player.roomCode = code;

    Navigation::goToPage(ui->page_create, ui->stackedWidget, this);

    ui->labelCode->setText("ROOM CODE: " + code.toUpper());
    ui->labelCode->setStyleSheet(R"(
        QLabel {
            color: white;
            font-family: 'Tektur';
            font-size: 22px;
            font-weight: bold;
            border: 2px solid #ff6600;
            border-radius: 6px;
            padding: 6px;
            qproperty-alignment: 'AlignCenter';
        }
    )");

    ui->labelCreateInfo->setText("Share this code with your friends so they can join your game.");
    ui->labelCreateInfo->setStyleSheet(R"(
        QLabel {
            color: white;
            font-family: 'Tektur';
            font-size: 18px;
            font-style: italic;
            qproperty-alignment: 'AlignCenter';
        }
    )");

    ui->btnContinue->setText("Continue");
    ui->btnContinue->show();
    player.isHost = true;

    ui->comboMaxPlayers->clear();

    // Placeholder
    ui->comboMaxPlayers->addItem("Select players...");
    auto combo = ui->comboMaxPlayers;
    combo->setView(new QListView(combo));

    auto* model = qobject_cast<QStandardItemModel*>(combo->model());
    if (combo) {
        QStandardItem* first = model->item(0);
        first->setEnabled(false);
        first->setForeground(QBrush(Qt::gray));
        first->setFont(QFont("Tektur", 12, QFont::StyleItalic));
    }

    for (int i = 2; i <= 8; i++) ui->comboMaxPlayers->addItem(QString::number(i));
    ui->comboMaxPlayers->setCurrentIndex(0);
}

void MainWindow::handleContinueToWait() {
    if (ui->comboMaxPlayers->currentIndex() == 0) {
        QMessageBox::warning(this, "Missing Selection",
                             "Please select the number of players before continuing.");
        return;
    }
    player.maxPlayers = ui->comboMaxPlayers->currentText().toUInt();
    try {
        if (!this->lobbySvc->createRoom(player.roomCode, player.maxPlayers)) {
            QMessageBox::critical(this, "Create Room", "Failed to create room on server.");
            return;
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Create Room", QString("Error: %1").arg(e.what()));
        return;
    }

    Navigation::goToPage(ui->page_wait, ui->stackedWidget, this);
    ui->labelRoomCode->setText("ROOM CODE: " + player.roomCode.toUpper());
    ui->listPlayers->clear();

    player.currentPlayers = 1;
    updateLobbyStatus();
    ui->btnStartGame->setVisible(true);

    QObject::disconnect(waitCtrl.get(), nullptr, this, nullptr);
    connect(waitCtrl.get(), &WaitRoomController::gameStarted, this, [this] {
        game_started = true;
        this->close();
    });
    waitCtrl->startHost();
}

void MainWindow::handleConfirmJoin() {
    QString code = ui->inputCode->text().trimmed();
    if (code.isEmpty()) {
        QMessageBox::warning(this, "Join Game", "Please enter a room code.");
        return;
    }
    try {
        if (!this->lobbySvc->joinRoom(code)) {
            QMessageBox::critical(this, "Join Game", "Failed to join room on server.");
            return;
        }
        handleRefreshPlayers();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Join Game", QString("Error: %1").arg(e.what()));
        return;
    }

    player.roomCode = code;
    ui->labelRoomCode->setText("ROOM CODE: " + code);
    Navigation::goToPage(ui->page_wait, ui->stackedWidget, this);

    // Iniciar controlador de espera
    QObject::disconnect(waitCtrl.get(), nullptr, this, nullptr);
    connect(waitCtrl.get(), &WaitRoomController::gameStarted, this, [this] {
        game_started = true;
        this->close();
    });
    waitCtrl->startGuest();
    ui->listPlayers->clear();
    ui->btnStartGame->setVisible(false);
    ui->btnRefresh->setVisible(true);
}

void MainWindow::updateLobbyStatus() {
    QString text = QString("Players: %1 / %2").arg(player.currentPlayers).arg(player.maxPlayers);
    ui->labelLobbyStatus->setText(text);
}

void MainWindow::handleRefreshPlayers() { waitCtrl->refreshOnce(); }

void MainWindow::updateCarImage() { carCtrl->updateImage(); }

void MainWindow::handleStartGame() {
    game_started = true;
    try {
        this->protocol.sendStartGame();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Start Game", QString("Error: %1").arg(e.what()));
    }
    this->close();
}

void MainWindow::setupBackgroundMusic() {}

MainWindow::~MainWindow() {
    if (music)
        music->stop();
    delete ui;
}
