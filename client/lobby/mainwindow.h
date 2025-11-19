#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAudioOutput>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QVector>
#include <memory>
#include <string>

#include "../network/client_protocol.h"

#include "car.h"
#include "lobby_api.h"
#include "lobby_service.h"
#include "player_info.h"

class RoomsPager;
class WaitRoomController;
class CarSelectionController;
class BackgroundMusic;


QT_BEGIN_NAMESPACE
namespace Ui {
class Lobby;
}
QT_END_NAMESPACE

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(ClientProtocol& protocol, bool& game_started_ref, std::string& username_ref,
                        QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent* e) override;

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
    void onMapItemChanged(QListWidgetItem* item);
    void updateMapItemStyle(QListWidgetItem* item);

private:
    Ui::Lobby* ui;
    PlayerInfo player;
    ClientProtocol& protocol;  // conexión TCP al servidor
    bool& game_started;
    std::string& username;

    QTimer* refreshTimer;
    QMediaPlayer* backgroundMusic;
    QAudioOutput* audioOutput;

    static constexpr int PAGE_SIZE = 10;  // cantidad de salas por página
    int currentPage = 0;
    QStringList allRooms;
    QStringList selectedMaps;
    bool inFlight = false;

    QString generateRoomCode();
    void loadRooms();
    void showPage(int page);
    void goToPage(QWidget* page);
    void updateCarImage();
    void connectToServer();  // función privada que usará el botón
    void onWaitTimerTickHost();
    void onWaitTimerTickJoin();

    // Controladores
    std::unique_ptr<LobbyApi> lobbyApi;
    std::unique_ptr<LobbyService> lobbySvc;
    std::unique_ptr<RoomsPager> roomsPager;
    std::unique_ptr<WaitRoomController> waitCtrl;
    std::unique_ptr<CarSelectionController> carCtrl;
    std::unique_ptr<BackgroundMusic> music;
};
#endif  // MAINWINDOW_H
