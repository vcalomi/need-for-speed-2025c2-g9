#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "player_info.h"
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
    void showNextRoom();
    void showPrevRoom();
    void handleRefreshPlayers();
    void updateLobbyStatus();

private:
    Ui::Lobby* ui;
    PlayerInfo player;
    static constexpr int PAGE_SIZE = 10; // cantidad de salas por página
    int currentPage = 0;
    QStringList allRooms;

    QString generateRoomCode();
    void loadRooms();
    void showPage(int page);
    void goToPage(QWidget *page);

};
#endif  // MAINWINDOW_H
