#ifndef CAR_SELECTION_CONTROLLER_H
#define CAR_SELECTION_CONTROLLER_H

#include <QVector>

#include "car.h"
#include "lobby_service.h"
#include "ui_mainwindow.h"

class LobbyService;
class Ui_Lobby;

class CarSelectionController {
public:
    CarSelectionController(LobbyService& svc, Ui::Lobby* ui);
    void initCars();
    void next();
    void prev();
    bool submitSelectedCar();  // true si CHOOSE_CAR_OK
    void updateImage();
    CarType currentCarType() const;

private:
    LobbyService& svc;
    Ui::Lobby* ui;
    QVector<Car> cars;
    int currentIndex = 0;
};

#endif
