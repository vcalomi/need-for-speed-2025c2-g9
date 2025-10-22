#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDir>
#include <QPixmap>
#include <QPalette>
#include <QFontDatabase>
#include <QMessageBox>
#include <QScreen>
#include <QGuiApplication>
#include <QDebug>
#include <QRandomGenerator>
#include <QPainter>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::Lobby) {
    ui->setupUi(this);

    // Fuente Tektur
    int fontId = QFontDatabase::addApplicationFont(":/Tektur-SemiBold.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont tekturFont(fontFamily, 12);
    qApp->setFont(tekturFont);

    // Placeholders
    ui->hostInput->setPlaceholderText("Host");
    ui->portInput->setPlaceholderText("Port");

    // Estilos globales
    qApp->setStyleSheet(R"(
        QLineEdit {
            background: #2c2c2c;
            color: white;
            border: 2px solid #ff6600;
            border-radius: 10px;
            padding: 6px;
            font-family: 'Tektur';
            font-size: 17px;
            box-shadow: 0 0 15px #ff6600;
        }

        QLineEdit::placeholder {
            color: rgba(255, 255, 255, 200);
            font-style: italic;
        }

        QPushButton {
            background-color: #ff6600;
            color: white;
            border: none;
            border-radius: 10px;
            padding: 15px 30px;
            font-family: 'Tektur';
            font-size: 15px;
            font-weight: bold;
            box-shadow: 0px 0px 15px #ff6600;
        }

        QPushButton:hover {
            background-color: #ffa31a;
            box-shadow: 0px 0px 25px #ffa31a;
        }

        QPushButton:pressed {
            background-color: #cc5200;
        }
    )");


    // Acción del botón
    connect(ui->connectButton, &QPushButton::clicked, this, [this]() {
        QString host = ui->hostInput->text();
        QString port = ui->portInput->text();
        if (host.isEmpty() || port.isEmpty()) {
            QMessageBox::warning(this, "Connection error",
                                 "Please enter Host and Port before connecting");
            return;
        }
        qDebug() << "Conectando a " << host << ":" << port;

        // Si la conexión fuera exitosa
        nextPage(ui->page_menu);
    });

    // Página inicial
    nextPage(ui->page_connection);

    // Botones del menú principal
    connect(ui->btnCreate, &QPushButton::clicked, this, &MainWindow::handleCreateGame);
    connect(ui->btnJoin, &QPushButton::clicked, this, &MainWindow::handleJoinGame);

    connect(ui->btnCreate, &QPushButton::clicked, this, &MainWindow::handleCreateButton);
    connect(ui->btnBackCreate, &QPushButton::clicked, this, &MainWindow::handleContinueToWait);

    // Página Join
    connect(ui->btnConfirmJoin, &QPushButton::clicked, this, &MainWindow::handleConfirmJoin);
    connect(ui->btnBackJoin, &QPushButton::clicked, this, &MainWindow::handleBackToMenu);

    // Página Wait
    connect(ui->btnStartGame, &QPushButton::clicked, this, &MainWindow::handleStartGame);

    // Centrar
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    this->move(screenGeometry.center() - this->rect().center());

}

// Navegar entre páginas
void MainWindow::nextPage(QWidget* page) {
    ui->stackedWidget->setCurrentWidget(page);
    QPixmap background(":/loby_image.jpg");
    if (background.isNull()) {
        qDebug() << "No se encontró la imagen de background en recursos.";
        return;
    }
    background = background.scaled(this->size(), Qt::KeepAspectRatioByExpanding);

    // Si estamos en la página de conexión --> imagen normal
    if (page == ui->page_connection) {
        QPalette paleta;
        paleta.setBrush(QPalette::Window, background);
        this->setPalette(paleta);
        return;
    }

    // En cualquier otra página --> aplicar overlay negro translúcido sobre la imagen
    QPixmap backgroundDark = background;
    QPainter painter(&backgroundDark);
    painter.fillRect(backgroundDark.rect(), QColor(0, 0, 0, 160)); // negro con 160/255 de opacidad
    painter.end();

    QPalette palette;
    palette.setBrush(QPalette::Window, backgroundDark);
    this->setPalette(palette);
}

// Generar código aleatorio
QString MainWindow::generateRoomCode() {
    const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    QString code;
    for (int i = 0; i < 6; ++i) {
        int index = QRandomGenerator::global()->bounded(chars.length());
        code.append(chars.at(index));
    }
    return code;
}

// Volver al menú
void MainWindow::handleBackToMenu() {
    nextPage(ui->page_menu);
}

// Unirse a partida
void MainWindow::handleJoinGame() {
    isHost = false;
    nextPage(ui->page_join);
}

// Crear partida
void MainWindow::handleCreateGame() {
    isHost = true;
    nextPage(ui->page_create);
}

void MainWindow::handleCreateButton() {
    // Generar código de sala
    QString code = generateRoomCode();
    this->currentRoomCode = code;
    nextPage(ui->page_create);
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

    // Mostrar el mensaje de compartir
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

    ui->btnBackCreate->setText("Continue");
    ui->btnBackCreate->show();
    isHost = true;
}

void MainWindow::handleContinueToWait() {
    nextPage(ui->page_wait);
    ui->labelRoomCode->setText("ROOM CODE: " + currentRoomCode.toUpper());
    ui->listPlayers->clear();
    ui->listPlayers->addItem("You (Host)");
    ui->btnStartGame->setVisible(true); // solo host
}

void MainWindow::handleConfirmJoin() {
    QString code = ui->inputCode->text().trimmed();
    if (code.isEmpty()) {
        QMessageBox::warning(this, "Join Game", "Please enter a room code.");
        return;
    }

    qDebug() << "Joining room" << code;

    this->currentRoomCode = code;
    ui->labelRoomCode->setText("ROOM CODE: " + code);
    nextPage(ui->page_wait);

    // Limpia y muestra jugadores
    ui->listPlayers->clear();
    ui->listPlayers->addItem("You");
    ui->listPlayers->addItem("Host");

    // Los invitados no ven el botón "Start Game"
    ui->btnStartGame->setVisible(false);
}

// Botón Start Game (solo host)
void MainWindow::handleStartGame() {
    this->close(); // Cierra la ventana
}

MainWindow::~MainWindow() { delete ui; }

