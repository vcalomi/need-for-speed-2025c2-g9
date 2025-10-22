#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class Lobby;
}
QT_END_NAMESPACE

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void handleJoinGame();
    void handleCreateGame();
    void handleConfirmJoin();
    void handleBackToMenu();
    void handleStartGame();
    void handleCreateButton();
    void handleContinueToWait();

private:
    Ui::Lobby* ui;
    void nextPage(QWidget *page);
    QString generateRoomCode();
    QString currentRoomCode;
    bool isHost = false;
};
#endif  // MAINWINDOW_H
