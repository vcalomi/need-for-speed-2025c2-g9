#include "styles.h"
#include <QFontDatabase>
#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>

void UIStyles::applyGlobalStyle() {
    // Fuente global
    int fontId = QFontDatabase::addApplicationFont(":/Tektur-SemiBold.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont tekturFont(fontFamily, 12);
    qApp->setFont(tekturFont);

    // --- Estilo global (sin box-shadow, solo colores y bordes) ---
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

        QPushButton:hover {
            background-color: #ffa31a;
        }

        QPushButton:pressed {
            background-color: #cc5200;
        }

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

    // --- Aplicar sombras globales a ciertos widgets ---
    const QList<QWidget*> widgets = qApp->allWidgets();
    for (QWidget *w : widgets) {
        // Sombra para botones
        if (auto btn = qobject_cast<QPushButton*>(w)) {
            auto *shadow = new QGraphicsDropShadowEffect(btn);
            shadow->setBlurRadius(20);
            shadow->setOffset(0, 0);
            shadow->setColor(QColor("#ff6600"));
            btn->setGraphicsEffect(shadow);
        }
        // Sombra para campos de texto
        else if (auto edit = qobject_cast<QLineEdit*>(w)) {
            auto *shadow = new QGraphicsDropShadowEffect(edit);
            shadow->setBlurRadius(15);
            shadow->setOffset(0, 0);
            shadow->setColor(QColor("#ff6600"));
            edit->setGraphicsEffect(shadow);
        }
    }
}
