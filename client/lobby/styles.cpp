#include "styles.h"
#include <QFontDatabase>
#include <QApplication>

void UIStyles::applyGlobalStyle() {
    int fontId = QFontDatabase::addApplicationFont(":/Tektur-SemiBold.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont tekturFont(fontFamily, 12);
    qApp->setFont(tekturFont);

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

        QComboBox {
            background-color: rgba(0, 0, 0, 0.6);
            color: white;
            font-family: 'Tektur';
            font-size: 18px;
            border: 2px solid #ff6600;
            border-radius: 10px;
            padding: 6px 12px;
        }

        QComboBox::down-arrow {
            width: 14px;
            height: 14px;
            color: white;
        }

        // Mostramos la flecha nativa
        QComboBox::down-arrow:!pressed {
            image: none;
        }

        // Evitar que el fondo la tape
        QComboBox::drop-down {
            background: transparent;
            border: none;
            width: 20px;
        }

        QComboBox QAbstractItemView {
            background-color: #1f1f1f;
            color: white;
            selection-background-color: #ff6600;
            selection-color: black;
            border: 2px solid #ff6600;
            font-family: 'Tektur';
        }


    )");
}
