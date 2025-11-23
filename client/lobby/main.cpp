#include <QApplication>

#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    ClientProtocol protocol;
    bool game_started = false;
    std::string username;
    MainWindow* w = new MainWindow(protocol, game_started, username, nullptr);
    w->showMaximized();
    return a.exec();
}
