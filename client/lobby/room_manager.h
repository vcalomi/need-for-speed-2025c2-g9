#ifndef ROOM_MANAGER_H
#define ROOM_MANAGER_H

#include <QStringList>

class RoomManager {
private:
    static constexpr int PAGE_SIZE = 10; // cantidad de salas por página
    static constexpr int ROOM_CODE_LENGTH = 6;
    static constexpr int MAX_ROOMS = 50;
    static const QString ROOM_CODE_CHARS;
    static const QString ROOM_SUFFIX;
    static QChar randomChar();
public:
    RoomManager() = delete;

    static QString generateRoomCode();
    static QStringList generateRooms(int count = PAGE_SIZE);
};

#endif // ROOM_MANAGER_H
