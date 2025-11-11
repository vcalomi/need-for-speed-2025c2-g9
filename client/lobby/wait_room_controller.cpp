#include "wait_room_controller.h"

#include "lobby_service.h"
#include "ui_mainwindow.h"

WaitRoomController::WaitRoomController(LobbyService& svc, Ui::Lobby* ui, QObject* parent):
        QObject(parent), svc(svc), ui(ui) {
    connect(&timer, &QTimer::timeout, this, &WaitRoomController::onTick);
}

void WaitRoomController::startHost() { timer.start(1000); }
void WaitRoomController::startGuest() { timer.start(1000); }
void WaitRoomController::stop() { timer.stop(); }

void WaitRoomController::refreshOnce() {
    auto vm = svc.listPlayers();
    ui->listPlayers->clear();
    if (vm.maxPlayers > 0) {
        ui->labelLobbyStatus->setText(
                QString("Players: %1 / %2").arg(ui->listPlayers->count()).arg(vm.maxPlayers));
    }
    for (const auto& p: vm.players) ui->listPlayers->addItem(p);
    ui->labelLobbyStatus->setText(
            QString("Players: %1 / %2").arg(ui->listPlayers->count()).arg(vm.maxPlayers));
}

void WaitRoomController::onTick() {
    auto state = svc.pollState();
    if (state.started) {
        emit gameStarted();
        timer.stop();
        return;
    }
    refreshOnce();
}
