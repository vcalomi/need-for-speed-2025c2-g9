#ifndef WAIT_ROOM_CONTROLLER_H
#define WAIT_ROOM_CONTROLLER_H

#include <QObject>
#include <QTimer>

#include "lobby_service.h"
#include "ui_mainwindow.h"

class Ui_Lobby;

class WaitRoomController: public QObject {
    Q_OBJECT
public:
    WaitRoomController(LobbyService& svc, Ui::Lobby* ui, QObject* parent = nullptr);
    void startHost();
    void startGuest();
    void stop();
    void refreshOnce();

signals:
    void gameStarted();

private slots:
    void onTick();

private:
    LobbyService& svc;
    Ui::Lobby* ui;
    QTimer timer;
};

#endif
