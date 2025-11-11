#include "rooms_pager.h"

#include <QtMath>

#include "lobby_service.h"
#include "ui_mainwindow.h"

RoomsPager::RoomsPager(LobbyService& svc, Ui::Lobby* ui): svc(svc), ui(ui) {}

void RoomsPager::showPage(int page) {
    ui->listRooms->clear();
    auto rooms = svc.listRooms();
    if (!rooms.isEmpty()) {
        allRooms = rooms;
    }
    int totalPages = qMax(1, qCeil(allRooms.size() / static_cast<double>(PAGE_SIZE)));
    currentPage = qBound(0, page, totalPages - 1);

    int start = currentPage * PAGE_SIZE;
    int end = qMin(start + PAGE_SIZE, allRooms.size());
    for (int i = start; i < end; ++i) ui->listRooms->addItem(allRooms[i]);

    ui->labelPageInfo->setText(QString("Page %1 / %2").arg(currentPage + 1).arg(totalPages));
    ui->btnPrevRooms->setEnabled(currentPage > 0);
    ui->btnNextRooms->setEnabled(currentPage < totalPages - 1);
}

void RoomsPager::showNext() { showPage(currentPage + 1); }
void RoomsPager::showPrev() { showPage(currentPage - 1); }
