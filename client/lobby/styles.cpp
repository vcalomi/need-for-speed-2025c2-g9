#include <QDebug>
#include "styles.h"

#include <QApplication>
#include <QComboBox>
#include <QFontDatabase>
#include <QGraphicsDropShadowEffect>
#include <QLineEdit>
#include <QPushButton>

void UIStyles::applyGlobalStyle() {
    // Cargar fuente global
    int fontId = QFontDatabase::addApplicationFont(":/Tektur-SemiBold.ttf");
    if (fontId == -1) {
        qWarning() << "[UIStyles] Failed to load font Tektur-SemiBold.ttf";
    } else {
        QStringList families = QFontDatabase::applicationFontFamilies(fontId);
        if (!families.isEmpty()) {
            QFont tekturFont(families.at(0), 12);
            qApp->setFont(tekturFont);
            qDebug() << "[UIStyles] Loaded font family:" << families.at(0);
        } else {
            qWarning() << "[UIStyles] Font loaded but no families found.";
        }
    }

    // --- Estilo global ---
    qApp->setStyleSheet(R"(
        QLineEdit {
            background: #2c2c2c;
            color: white;
            border: 2px solid #ff6600;
            border-radius: 10px;
            padding: 6px;
            font-family: 'Tektur';
            font-size: 17px;
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
        }

        QPushButton:hover { background-color: #ffa31a; }
        QPushButton:pressed { background-color: #cc5200; }

        QComboBox {
            background-color: rgba(0, 0, 0, 0.6);
            color: white;
            font-family: 'Tektur';
            font-size: 18px;
            border: 2px solid #ff6600;
            border-radius: 10px;
            padding: 6px 12px;
        }
    )");

    // --- Aplicar sombras globales ---
    const QList<QWidget*> widgets = qApp->allWidgets();
    for (QWidget* w: widgets) {
        if (auto btn = qobject_cast<QPushButton*>(w)) {
            auto* shadow = new QGraphicsDropShadowEffect(btn);
            shadow->setBlurRadius(20);
            shadow->setOffset(0, 0);
            shadow->setColor(QColor("#ff6600"));
            btn->setGraphicsEffect(shadow);
        } else if (auto edit = qobject_cast<QLineEdit*>(w)) {
            auto* shadow = new QGraphicsDropShadowEffect(edit);
            shadow->setBlurRadius(15);
            shadow->setOffset(0, 0);
            shadow->setColor(QColor("#ff6600"));
            edit->setGraphicsEffect(shadow);
        }
    }
}
