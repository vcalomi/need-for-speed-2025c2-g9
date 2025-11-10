#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAudioOutput>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QVector>
#include <memory>
#include <string>

#include "../network/client_protocol.h"

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
    MainWindow(ClientProtocol& protocol, bool& game_started_ref, std::string& username,
               QWidget* parent = nullptr, bool isDummy = false);
    // Constructor principal (cliente con red y estado)
    // explicit MainWindow(ClientProtocol& protocol, bool& game_started_ref, QWidget* parent =
    // nullptr);
    // Constructor único: si isDummy = true, entra en modo UI sin red
    // explicit MainWindow(ClientProtocol& protocol, bool& game_started_ref, QWidget* parent =
    // nullptr,
    //                     bool isDummy = false);
    ~MainWindow();
    static MainWindow* createDummy(QWidget* parent = nullptr);

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
    void openEditorMap();
    void handleOpenMapsPage();  // Abre la página con la lista de recorridos
    void handleSelectMaps();    // Selecciona y pasa a page_create

private:
    Ui::Lobby* ui;
    PlayerInfo player;
    ClientProtocol& protocol;  // conexión TCP al servidor
    bool& game_started;
    std::string& username;

    bool isDummy = false;  // agrego

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
    QStringList selectedMaps;

    QString generateRoomCode();
    void loadRooms();
    void showPage(int page);
    void goToPage(QWidget* page);
    void updateCarImage();
    void connectToServer();  // función privada que usará el botón
    void onWaitTimerTickHost();
    void onWaitTimerTickJoin();
};
#endif  // MAINWINDOW_H
