#ifndef ROOMS_PAGER_H
#define ROOMS_PAGER_H

#include <QStringList>

#include "lobby_service.h"
#include "ui_mainwindow.h"

class RoomsPager {
public:
    RoomsPager(LobbyService& svc, Ui::Lobby* ui);
    void showPage(int page);
    void showNext();
    void showPrev();

private:
    LobbyService& svc;
    Ui::Lobby* ui;
    static constexpr int PAGE_SIZE = 10;
    int currentPage = 0;
    QStringList allRooms;
};

#endif
