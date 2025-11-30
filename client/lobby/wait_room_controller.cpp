#include "wait_room_controller.h"

#include "lobby_service.h"
#include "ui_mainwindow.h"
#include "../../common/common_codes.h"

WaitRoomController::WaitRoomController(LobbyService& svc, Ui::Lobby* ui, QObject* parent):
        QObject(parent), svc(svc), ui(ui) {
    connect(&timer, &QTimer::timeout, this, &WaitRoomController::onTick);
}

void WaitRoomController::startHost() { timer.start(1000); }
void WaitRoomController::startGuest() { timer.start(1000); }
void WaitRoomController::stop() { timer.stop(); }

void WaitRoomController::onTick() {
    auto state = svc.pollState();
    if (state.started) {
        emit gameStarted();
        timer.stop();
        return;
    }
}
