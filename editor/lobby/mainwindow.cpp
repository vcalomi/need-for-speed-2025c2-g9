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
    loadRooms();

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

        // Simular carga de salas disponibles --> esto va a tener que recibir la info del socket con el protocolo
        ui->listRooms->clear();
        ui->listRooms->addItem("Room A - Code: 123ABC");
        ui->listRooms->addItem("Room B - Code: 987XYZ");
        ui->listRooms->addItem("Room C - Code: 555NFS");

        loadRooms();
        goToPage(ui->page_rooms);
    });

    // Página inicial
    goToPage(ui->page_connection);

    // Botones del menú principal
    connect(ui->btnCreate, &QPushButton::clicked, this, &MainWindow::handleCreateButton);
    connect(ui->btnJoin,   &QPushButton::clicked, this, &MainWindow::handleJoinGame);

    // Botones de paginación (solo cambian las salas mostradas)
    connect(ui->btnNextRooms, &QPushButton::clicked, this, &MainWindow::showNextRoom);
    connect(ui->btnPrevRooms, &QPushButton::clicked, this, &MainWindow::showPrevRoom);


    // Botón Continue → va al menú principal
    connect(ui->btnContinueRooms, &QPushButton::clicked, this, [this]() {
        goToPage(ui->page_menu);
    });

    // Página Create (nuevo botón Continue)
    connect(ui->btnContinue, &QPushButton::clicked, this, &MainWindow::handleContinueToWait);

    // Página Join
    connect(ui->btnConfirmJoin, &QPushButton::clicked, this, &MainWindow::handleConfirmJoin);
    connect(ui->btnBackJoin, &QPushButton::clicked, this, &MainWindow::handleBackToMenu);

    // Página Wait
    connect(ui->btnStartGame, &QPushButton::clicked, this, &MainWindow::handleStartGame);

    // Centrar
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    this->move(screenGeometry.center() - this->rect().center());

}

// Navegar entre páginas + color fondo
void MainWindow::goToPage(QWidget* page) {
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

void MainWindow::loadRooms() {
    allRooms.clear();

            // Ejemplo: simulamos 20 salas
    for (int i = 1; i <= 20; ++i)
        allRooms << QString("Room %1 - Code: %2%3%4")
                            .arg(i)
                            .arg(QChar('A' + (i % 26)))
                            .arg(i)
                            .arg("XYZ");

    showPage(0); // Empieza en la primera página
}

void MainWindow::showPage(int page) {
    ui->listRooms->clear();

    int totalPages = qCeil(allRooms.size() / static_cast<double>(PAGE_SIZE));
    currentPage = qBound(0, page, totalPages - 1);

    int start = currentPage * PAGE_SIZE;
    int end = qMin(start + PAGE_SIZE, allRooms.size());
    for (int i = start; i < end; ++i)
        ui->listRooms->addItem(allRooms[i]);

    // Actualizamos el texto de página con las nuevas salas
    ui->labelPageInfo->setText(QString("Page %1 / %2")
                                       .arg(currentPage + 1)
                                       .arg(totalPages));

    // Habilitar/deshabilitar botones de prev/next según corresponda
    ui->btnPrevRooms->setEnabled(currentPage > 0);
    ui->btnNextRooms->setEnabled(currentPage < totalPages - 1);
}

void MainWindow::showNextRoom() {
    showPage(currentPage + 1);
}

void MainWindow::showPrevRoom() {
    showPage(currentPage - 1);
}


// Volver al menú
void MainWindow::handleBackToMenu() {
    goToPage(ui->page_menu);
}

// Unirse a partida
void MainWindow::handleJoinGame() {
    isHost = false;
    goToPage(ui->page_join);
}

// Crear partida
void MainWindow::handleCreateGame() {
    isHost = true;
    goToPage(ui->page_create);
}

void MainWindow::handleCreateButton() {
    // Generar código de sala
    QString code = generateRoomCode();
    this->currentRoomCode = code;
    goToPage(ui->page_create);

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

    ui->btnContinue->setText("Continue");
    ui->btnContinue->show();
    isHost = true;
}

void MainWindow::handleContinueToWait() {
    goToPage(ui->page_wait);
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
    goToPage(ui->page_wait);

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

