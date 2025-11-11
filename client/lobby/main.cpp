#include <QApplication>

#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    // MainWindow* w = MainWindow::createDummy();  // usa el modo dummy (sin red)
    MainWindow* w = MainWindow::createNormal();  // usa el modo normal (con red)
    w->showMaximized();
    return a.exec();
}
