#include "mainwindow.h"

#include "./ui_mainwindow.h"
#include <QDir>
#include <QPixmap>
#include <QPalette>
#include <QFontDatabase>
#include <QMessageBox>
#include <QScreen>
#include <QGuiApplication>

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent), ui(new Ui::Loby) {
    ui->setupUi(this);

    // Fondo
    QPixmap fondo(":/loby_image.jpg");

    if (!fondo.isNull()) {
        // esto es para que sea responsive
        fondo = fondo.scaled(this->size(), Qt::KeepAspectRatioByExpanding);
        QPalette paleta;
        paleta.setBrush(QPalette::Window, fondo);
        this->setPalette(paleta);
    } else {
        qDebug() << "No se encontró la imagen de fondo en recursos.";
    }

    // Fuente Tektur
    int fontId = QFontDatabase::addApplicationFont(":/Tektur-SemiBold.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont tekturFont(fontFamily, 12);
    qApp->setFont(tekturFont);

    // Placeholders
    ui->hostInput->setPlaceholderText("Host");
    ui->portInput->setPlaceholderText("Port");

    // Esstillos globales
    qApp->setStyleSheet(R"(
        QLineEdit {
            background: #2c2c2c;                /* gris */
            color: white;
            border: 2px solid #ff6600;
            border-radius: 10px;
            padding: 6px;
            font-family: 'Tektur';
            font-size: 17px;
            box-shadow: 0 0 15px #ff6600;
        }

        QLineEdit::placeholder {
            color: rgba(255, 255, 255, 200);   /* blanco brillante */
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
            QMessageBox::warning(this, "Error de conexión", "Por favor ingrese Host y Puerto antes de conectarse.");
            return;
        }
        qDebug() << "Conectando a " << host << ":" << port;
    });

    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    this->move(screenGeometry.center() - this->rect().center());


}

MainWindow::~MainWindow() { delete ui; }

