#include "navigation.h"
#include <QPixmap>
#include <QPalette>
#include <QPainter>
#include <QDebug>

void Navigation::goToPage(QWidget* page, QStackedWidget* stackedWidget, QWidget* window) {
    stackedWidget->setCurrentWidget(page);

    // Cargar imagen de fondo
    QPixmap background(":/loby_image.jpg");
    if (background.isNull()) {
        qDebug() << "No se encontró la imagen de background en recursos.";
        return;
    }

    // Escalar la imagen al tamaño de la ventana
    background = background.scaled(window->size(), Qt::KeepAspectRatioByExpanding);

    // Si estamos en la página de conexión --> imagen normal
    if (page->objectName() == "page_connection") {
        QPalette palette;
        palette.setBrush(QPalette::Window, background);
        window->setPalette(palette);
        return;
    }

    // En cualquier otra página --> aplicar overlay negro translúcido sobre la imagen
    QPixmap backgroundDark = background;
    QPainter painter(&backgroundDark);
    painter.fillRect(backgroundDark.rect(), QColor(0, 0, 0, 160)); // negro con 160/255 de opacidad
    painter.end();

    QPalette palette;
    palette.setBrush(QPalette::Window, backgroundDark);
    window->setPalette(palette);
}
