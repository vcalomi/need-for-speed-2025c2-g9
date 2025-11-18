#include <QApplication>

#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow* w = MainWindow::createNormal();
    w->showMaximized();
    return a.exec();
}
