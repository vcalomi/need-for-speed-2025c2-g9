#include <cmath>
#include "navigation.h"

#include <QDebug>
#include <QPainter>
#include <QPalette>
#include <QPixmap>
#include <QSizeF>

void Navigation::goToPage(QWidget* page, QStackedWidget* stackedWidget, QWidget* window) {
    stackedWidget->setCurrentWidget(page);

    // Cargar imagen de fondo
    QPixmap background(":/loby_image.jpg");
    if (background.isNull()) {
        qDebug() << "No se encontró la imagen de background en recursos.";
        return;
    }
    window->setAutoFillBackground(true);
    const QSize canvasSize = window->size();
    QPixmap canvas(canvasSize);

    QPixmap scaled =
            background.scaled(canvasSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPainter composePainter(&canvas);
    const QSize scaledSz = scaled.size();
    const int offsetX = (canvas.width() - scaledSz.width()) / 2;
    int offsetY;
    if (scaledSz.height() <= canvas.height()) {
        offsetY = (canvas.height() - scaledSz.height()) / 2;
    } else {
        const double focal = 0.20;
        const double desiredCenter = scaledSz.height() * focal;
        int top = static_cast<int>(std::round(desiredCenter - canvas.height() / 2.0));
        const int maxTop = scaledSz.height() - canvas.height();
        if (top < 0)
            top = 0;
        if (top > maxTop)
            top = maxTop;
        offsetY = top;
    }
    const QPoint topLeft(offsetX, offsetY);
    composePainter.drawPixmap(topLeft, scaled);
    composePainter.end();

    // Si estamos en la página de conexión --> imagen normal
    if (page->objectName() == "page_connection") {
        QPalette palette;
        palette.setBrush(QPalette::Window, canvas);
        window->setPalette(palette);
        if (QWidget* cw = window->findChild<QWidget*>("centralwidget")) {
            cw->setAutoFillBackground(true);
            cw->setPalette(palette);
            cw->update();
        }
        window->update();
        return;
    }

    // En cualquier otra página --> aplicar overlay negro translúcido sobre la imagen
    QPixmap backgroundDark = canvas;
    QPainter painter(&backgroundDark);
    painter.fillRect(backgroundDark.rect(), QColor(0, 0, 0, 160));
    painter.end();

    QPalette palette;
    palette.setBrush(QPalette::Window, backgroundDark);
    window->setPalette(palette);
    if (QWidget* cw = window->findChild<QWidget*>("centralwidget")) {
        cw->setAutoFillBackground(true);
        cw->setPalette(palette);
        cw->update();
    }
    window->update();
}
