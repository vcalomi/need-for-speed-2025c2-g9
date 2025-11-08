#include <QApplication>
#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    MainWindow* w = MainWindow::createDummy();  // usa el modo dummy (sin red)
    w->show();

    return a.exec();
}
