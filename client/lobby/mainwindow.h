#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAudioOutput>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QVector>
#include <memory>

#include "../client_protocol.h"

#include "car.h"
#include "player_info.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class Lobby;
}
QT_END_NAMESPACE

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    MainWindow(ClientProtocol& protocol, bool& game_started_ref, QWidget* parent = nullptr);
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
    void setupBackgroundMusic();

private:
    Ui::Lobby* ui;
    PlayerInfo player;
    ClientProtocol& protocol;  // conexión TCP al servidor
    bool& game_started;
    QTimer* waitTimer;
    QTimer* refreshTimer;
    QMediaPlayer* backgroundMusic;
    QAudioOutput* audioOutput;

    static constexpr int PAGE_SIZE = 10;  // cantidad de salas por página
    int currentPage = 0;
    QStringList allRooms;
    QVector<Car> cars;
    int currentCarIndex = 0;
    bool inFlight = false;  // serializa lecturas del protocolo en wait

    QString generateRoomCode();
    void loadRooms();
    void showPage(int page);
    void goToPage(QWidget* page);
    void updateCarImage();
    void connectToServer();  // función privada que usará el botón
};
#endif  // MAINWINDOW_H
