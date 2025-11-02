#include "mainwindow.h"

#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QScreen>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTimer>
#include <QtMath>

#include "navigation.h"
#include "player_info.h"
#include "room_manager.h"
#include "styles.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(const QString& host, const QString& port, bool& game_started_ref,
                       QWidget* parent):
        QMainWindow(parent),
        ui(new Ui::Lobby),
        defaultHost(host),
        defaultPort(port),
        game_started(game_started_ref),
        waitTimer(nullptr),
        refreshTimer(nullptr) {
    ui->setupUi(this);
    game_started = false;
    cars = {Car(CarType::FIAT_600, "Fiat 600", ":/fiat_600.png"),
            Car(CarType::FERRARI_F40, "Ferrari F40", ":/ferrari.png"),
            Car(CarType::PORSCHE_911, "Porsche 911", ":/porsche.png"),
            Car(CarType::SEDAN, "Sedan", ":/sedan.png"),
            Car(CarType::SHEEP_4X4, "Sheep 4x4", ":/jeep_4x4.png"),
            Car(CarType::FORD_F100, "Ford F100", ":/ford_f100.png"),
            Car(CarType::TRUCK, "Truck", ":/truck.png")};

    // Estilos y fuente global
    UIStyles::applyGlobalStyle();
    allRooms.clear();

    Navigation::goToPage(ui->page_connection, ui->stackedWidget, this);

    connect(ui->connectButton, &QPushButton::clicked, this, [this]() {
        const QString hostname = defaultHost.isEmpty() ? QString("127.0.0.1") : defaultHost;
        const QString port = defaultPort.isEmpty() ? QString("8080") : defaultPort;

        try {
            protocol = std::make_unique<ClientProtocol>(hostname.toStdString(), port.toStdString());
            QMessageBox::information(this, "Connected",
                                     QString("Connected to %1:%2").arg(hostname, port));
            Navigation::goToPage(ui->page_username, ui->stackedWidget, this);
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Connection failed",
                                  QString("Unable to connect: %1").arg(e.what()));
        }
    });

    // Username
    connect(ui->btnConfirmUsername, &QPushButton::clicked, this, [this]() {
        QString name = ui->input_username->text().trimmed();

        if (name.isEmpty()) {
            QMessageBox::warning(this, "Username", "Please enter a valid username.");
            return;
        }

        player.username = name;
        if (protocol) {
            try {
                protocol->sendUsername(player.username.toStdString());
                protocol->sendListRooms();
                auto rooms = protocol->receiveRoomList();
                allRooms.clear();
                for (const auto& r: rooms) allRooms << QString::fromStdString(r);
                showPage(0);
            } catch (const std::exception& e) {
                QMessageBox::critical(this, "Error", QString("Server error: %1").arg(e.what()));
                return;
            }
        }

        // Ir a la pantalla de salas dispo
        Navigation::goToPage(ui->page_rooms, ui->stackedWidget, this);
    });


    // Botones del menú principal
    connect(ui->btnCreate, &QPushButton::clicked, this, &MainWindow::handleCreateButton);
    connect(ui->btnJoin, &QPushButton::clicked, this, &MainWindow::handleJoinGame);

    // Paginación salas disponibles
    connect(ui->btnNextRooms, &QPushButton::clicked, this, &MainWindow::showNextRoom);
    connect(ui->btnPrevRooms, &QPushButton::clicked, this, &MainWindow::showPrevRoom);

    // Otros botones
    connect(ui->btnContinueRooms, &QPushButton::clicked, this,
            [this]() { Navigation::goToPage(ui->page_menu, ui->stackedWidget, this); });

    connect(ui->btnContinue, &QPushButton::clicked, this, &MainWindow::handleContinueToWait);
    connect(ui->btnConfirmJoin, &QPushButton::clicked, this, &MainWindow::handleConfirmJoin);
    connect(ui->btnBackJoin, &QPushButton::clicked, this, &MainWindow::handleBackToMenu);
    connect(ui->btnRefresh, &QPushButton::clicked, this, &MainWindow::handleRefreshPlayers,
            Qt::UniqueConnection);
    connect(ui->btnStartGame, &QPushButton::clicked, this, &MainWindow::handleStartGame);

    connect(ui->btnChooseCar, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->page_car);
        updateCarImage();  // para mostrar el primer auto
    });


    // Elegir  autos con el carrousel
    connect(ui->btnNextCar, &QPushButton::clicked, this, [this]() {
        currentCarIndex = (currentCarIndex + 1) % cars.size();
        updateCarImage();
    });
    connect(ui->btnPrevCar, &QPushButton::clicked, this, [this]() {
        currentCarIndex = (currentCarIndex - 1 + cars.size()) % cars.size();
        updateCarImage();
    });
    connect(ui->btnBackToLobby, &QPushButton::clicked, this, [this]() {
        const Car& selectedCar = cars[currentCarIndex];

        player.selectedCar = selectedCar.getType();
        if (protocol) {
            try {
                protocol->sendChooseCar(selectedCar.getName().toStdString());
                // Opcional: podríamos esperar CHOOSE_CAR_OK con receiveActionCode()
            } catch (const std::exception& e) {
                QMessageBox::warning(this, "Choose Car",
                                     QString("Failed to send car: %1").arg(e.what()));
            }
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

void MainWindow::showPage(int page) {
    ui->listRooms->clear();
    if (protocol) {
        try {
            protocol->sendListRooms();
            auto rooms = protocol->receiveRoomList();
            if (!rooms.empty()) {
                allRooms.clear();
                for (const auto& r: rooms) allRooms << QString::fromStdString(r);
            }
        } catch (const std::exception& e) {}
    }
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
    Navigation::goToPage(ui->page_join, ui->stackedWidget, this);
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
    auto* model = qobject_cast<QStandardItemModel*>(ui->comboMaxPlayers->model());

    if (model) {
        QStandardItem* first = model->item(0);
        first->setEnabled(false);
        first->setForeground(QBrush(Qt::gray));
        first->setFont(QFont("Tektur", 12, QFont::StyleItalic));
    }

    // Valores reales
    for (int i = 2; i <= 8; i++) ui->comboMaxPlayers->addItem(QString::number(i));

    ui->comboMaxPlayers->setCurrentIndex(0);
}

void MainWindow::handleContinueToWait() {
    player.maxPlayers = ui->comboMaxPlayers->currentText().toUInt();
    if (protocol) {
        try {
            protocol->sendCreateRoom(player.roomCode.toStdString(), player.maxPlayers);
            ActionCode resp = protocol->receiveActionCode();
            if (resp != ActionCode::ROOM_CREATED) {
                QMessageBox::critical(this, "Create Room", "Failed to create room on server.");
                return;
            }
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Create Room", QString("Error: %1").arg(e.what()));
            return;
        }
    }

    Navigation::goToPage(ui->page_wait, ui->stackedWidget, this);
    ui->labelRoomCode->setText("ROOM CODE: " + player.roomCode.toUpper());
    ui->listPlayers->clear();

    player.currentPlayers = 1;
    updateLobbyStatus();

    ui->btnStartGame->setVisible(true);
    ui->btnRefresh->setVisible(true);

    if (!waitTimer) {
        waitTimer = new QTimer(this);
        connect(
                waitTimer, &QTimer::timeout, this,
                [this]() {
                    if (!protocol || inFlight)
                        return;
                    inFlight = true;
                    try {
                        protocol->sendListState();
                        auto v = protocol->receiveRoomList();
                        if (!v.empty() && v[0] == std::string("started")) {
                            game_started = true;
                            this->close();
                        }
                    } catch (...) {}
                    inFlight = false;
                },
                Qt::UniqueConnection);
    }
    waitTimer->start(1000);
}

void MainWindow::handleConfirmJoin() {
    QString code = ui->inputCode->text().trimmed();
    if (code.isEmpty()) {
        QMessageBox::warning(this, "Join Game", "Please enter a room code.");
        return;
    }

    if (protocol) {
        try {
            protocol->sendJoinRoom(code.toStdString());
            ActionCode resp = protocol->receiveActionCode();
            if (resp != ActionCode::JOIN_OK) {
                QMessageBox::critical(this, "Join Game", "Failed to join room on server.");
                return;
            }
            handleRefreshPlayers();
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Join Game", QString("Error: %1").arg(e.what()));
            return;
        }
    }

    player.roomCode = code;
    ui->labelRoomCode->setText("ROOM CODE: " + code);
    Navigation::goToPage(ui->page_wait, ui->stackedWidget, this);

    ui->listPlayers->clear();

    if (!waitTimer) {
        waitTimer = new QTimer(this);
        connect(
                waitTimer, &QTimer::timeout, this,
                [this]() {
                    if (!protocol || inFlight)
                        return;
                    inFlight = true;
                    try {
                        protocol->sendListState();
                        auto v = protocol->receiveRoomList();
                        if (!v.empty() && v[0] == std::string("started")) {
                            game_started = true;
                            this->close();
                        }
                    } catch (...) {}
                    inFlight = false;
                },
                Qt::UniqueConnection);
    }
    waitTimer->start(1000);

    ui->btnStartGame->setVisible(false);
    ui->btnRefresh->setVisible(true);
}

void MainWindow::updateLobbyStatus() {
    QString text = QString("Players: %1 / %2").arg(player.currentPlayers).arg(player.maxPlayers);
    ui->labelLobbyStatus->setText(text);
}

void MainWindow::handleRefreshPlayers() {
    if (!protocol)
        return;
    if (inFlight)
        return;
    inFlight = true;
    try {
        protocol->sendListPlayers();
        auto players = protocol->receiveRoomList();
        ui->listPlayers->clear();

        if (!players.empty() && players[0].find("maxPlayers:") == 0) {
            QString maxPlayersStr = QString::fromStdString(players[0]).remove("maxPlayers:");
            player.maxPlayers = maxPlayersStr.toUInt();
            players.erase(players.begin());
        }
        for (const auto& p: players) {
            ui->listPlayers->addItem(QString::fromStdString(p));
        }
        player.currentPlayers = static_cast<unsigned>(players.size());
        updateLobbyStatus();
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Refresh Players",
                             QString("Failed to refresh: %1").arg(e.what()));
    }
    inFlight = false;
}

void MainWindow::updateCarImage() {
    const Car& currentCar = cars[currentCarIndex];
    QPixmap pixmap(currentCar.getImagePath());

    ui->labelCarImage->setPixmap(
            pixmap.scaled(550, 350, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::handleStartGame() {
    game_started = true;
    if (protocol) {
        try {
            protocol->sendStartGame();
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Start Game", QString("Error: %1").arg(e.what()));
        }
    }
    this->close();
}

MainWindow::~MainWindow() { delete ui; }
#include <string>
