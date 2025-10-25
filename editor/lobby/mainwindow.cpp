#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "player_info.h"

#include "styles.h"
#include "navigation.h"
#include "room_manager.h"

#include <QMessageBox>
#include <QScreen>
#include <QGuiApplication>
#include <QDebug>
#include <QtMath>
#include <QStandardItemModel>
#include <QStandardItem>


MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent), ui(new Ui::Lobby) {
    ui->setupUi(this);

    // Estilos y fuente global
    UIStyles::applyGlobalStyle();

    // Placeholders
    ui->hostInput->setPlaceholderText("Host");
    ui->portInput->setPlaceholderText("Port");

    // Cargar salas simuladas --> acá le voy a pedir el sockt que me diga cuantas salas hay
    allRooms = RoomManager::generateRooms(20);
    showPage(0);

    // Página inicial
    Navigation::goToPage(ui->page_connection, ui->stackedWidget, this);

    // Conexión al servidor
    connect(ui->connectButton, &QPushButton::clicked, this, [this]() {
        player.host = ui->hostInput->text();
        player.port = ui->portInput->text().toInt();

        if (player.host.isEmpty() || player.port <= 0) {
            QMessageBox::warning(this, "Connection error",
                                 "Please enter Host and Port before connecting");
            return;
        }

        showPage(0);

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
        // enviar al socket

        // Ir a la pantalla de salas dispo
        Navigation::goToPage(ui->page_rooms, ui->stackedWidget, this);
    });


    // 6Botones del menú principal
    connect(ui->btnCreate, &QPushButton::clicked, this, &MainWindow::handleCreateButton);
    connect(ui->btnJoin,   &QPushButton::clicked, this, &MainWindow::handleJoinGame);

    // Paginación salas disponibles
    connect(ui->btnNextRooms, &QPushButton::clicked, this, &MainWindow::showNextRoom);
    connect(ui->btnPrevRooms, &QPushButton::clicked, this, &MainWindow::showPrevRoom);

    // Otros botones
    connect(ui->btnContinueRooms, &QPushButton::clicked, this, [this]() {
        Navigation::goToPage(ui->page_menu, ui->stackedWidget, this);
    });

    connect(ui->btnContinue, &QPushButton::clicked, this, &MainWindow::handleContinueToWait);
    connect(ui->btnConfirmJoin, &QPushButton::clicked, this, &MainWindow::handleConfirmJoin);
    connect(ui->btnBackJoin, &QPushButton::clicked, this, &MainWindow::handleBackToMenu);
    connect(ui->btnRefresh, &QPushButton::clicked, this, &MainWindow::handleRefreshPlayers, Qt::UniqueConnection);
    connect(ui->btnStartGame, &QPushButton::clicked, this, &MainWindow::handleStartGame);

    // Centrar ventana
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    this->move(screenGeometry.center() - this->rect().center());
}

void MainWindow::showPage(int page) {
    ui->listRooms->clear();

    int totalPages = qCeil(allRooms.size() / static_cast<double>(PAGE_SIZE));
    currentPage = qBound(0, page, totalPages - 1);

    int start = currentPage * PAGE_SIZE;
    int end = qMin(start + PAGE_SIZE, allRooms.size());
    for (int i = start; i < end; ++i)
        ui->listRooms->addItem(allRooms[i]);

    ui->labelPageInfo->setText(QString("Page %1 / %2")
                                       .arg(currentPage + 1)
                                       .arg(totalPages));

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
            font-size: 16px;
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
    auto *model = qobject_cast<QStandardItemModel*>(ui->comboMaxPlayers->model());

    if (model) {
        QStandardItem* first = model->item(0);
        first->setEnabled(false);
        first->setForeground(QBrush(Qt::gray));
        first->setFont(QFont("Tektur", 12, QFont::StyleItalic));
    }

    // Valores reales
    for(int i = 2; i <= 8; i++)
        ui->comboMaxPlayers->addItem(QString::number(i));

    ui->comboMaxPlayers->setCurrentIndex(0);
}

void MainWindow::handleContinueToWait() {
    player.maxPlayers = ui->comboMaxPlayers->currentText().toUInt();

    Navigation::goToPage(ui->page_wait, ui->stackedWidget, this);
    ui->labelRoomCode->setText("ROOM CODE: " + player.roomCode.toUpper());
    ui->listPlayers->clear();
    ui->listPlayers->addItem(player.username + " (Host)");

    player.currentPlayers = 1;
    updateLobbyStatus();

    ui->btnStartGame->setVisible(true);
    ui->btnRefresh->setVisible(true);
}

void MainWindow::handleConfirmJoin() {
    QString code = ui->inputCode->text().trimmed();
    if (code.isEmpty()) {
        QMessageBox::warning(this, "Join Game", "Please enter a room code.");
        return;
    }

    player.roomCode = code;
    ui->labelRoomCode->setText("ROOM CODE: " + code);
    Navigation::goToPage(ui->page_wait, ui->stackedWidget, this);

    ui->listPlayers->clear();
    ui->listPlayers->addItem("You");
    ui->listPlayers->addItem("Host");

    // Mostrar el nombre del jugador local y el del host
    ui->listPlayers->addItem(player.username);
    ui->listPlayers->addItem(player.username + " Host");

    ui->btnStartGame->setVisible(false);
    ui->btnRefresh->setVisible(false);
}

void MainWindow::updateLobbyStatus() {
    QString text = QString("Players: %1 / %2")
    .arg(player.currentPlayers)
            .arg(player.maxPlayers);
    ui->labelLobbyStatus->setText(text);
}

void MainWindow::handleRefreshPlayers() {
    if (!player.isHost) return;

    if (player.currentPlayers < player.maxPlayers) {
        player.currentPlayers++;
        ui->listPlayers->addItem("New Player"); // Luego vendrá del servidor
    }

    updateLobbyStatus();
}

void MainWindow::handleStartGame() {
    this->close();
}

MainWindow::~MainWindow() {
    delete ui;
}
