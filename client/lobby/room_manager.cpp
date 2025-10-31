#include "room_manager.h"
#include <QRandomGenerator>

const QString RoomManager::ROOM_CODE_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const QString RoomManager::ROOM_SUFFIX = "XYZ";

QString RoomManager::generateRoomCode() {
    QString code;
    code.reserve(ROOM_CODE_LENGTH);

    for (int i = 0; i < ROOM_CODE_LENGTH; ++i)
        code.append(randomChar());
    return code;
}

QStringList RoomManager::generateRooms(int count) {
    QStringList list;
    list.reserve(count);

    for (int i = 1; i <= count; ++i) {
        QString roomName = QString("Room %1 - Code: %2%3%4")
        .arg(i)
                .arg(QChar('A' + (i % MAX_ROOMS)))
                .arg(i)
                .arg(ROOM_SUFFIX);
        list << roomName;
    }
    return list;
}

QChar RoomManager::randomChar() {
    int index = QRandomGenerator::global()->bounded(ROOM_CODE_CHARS.length());
    return ROOM_CODE_CHARS.at(index);
}
