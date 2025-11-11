#include "car_selection_controller.h"

#include <QPixmap>

#include "lobby_service.h"
#include "ui_mainwindow.h"

CarSelectionController::CarSelectionController(LobbyService& svc, Ui::Lobby* ui):
        svc(svc), ui(ui) {}

void CarSelectionController::initCars() {
    cars = {Car(CarType::FIAT_600, "Fiat 600", ":/fiat_600.png"),
            Car(CarType::FERRARI_F40, "Ferrari F40", ":/ferrari.png"),
            Car(CarType::PORSCHE_911, "Porsche 911", ":/porsche.png"),
            Car(CarType::SEDAN, "Sedan", ":/sedan.png"),
            Car(CarType::JEEP_4X4, "Jeep 4x4", ":/jeep_4x4.png"),
            Car(CarType::FORD_F100, "Ford F100", ":/ford_f100.png"),
            Car(CarType::TRUCK, "Truck", ":/truck.png")};
    updateImage();
}

void CarSelectionController::next() {
    if (cars.isEmpty())
        return;
    currentIndex = (currentIndex + 1) % cars.size();
    updateImage();
}

void CarSelectionController::prev() {
    if (cars.isEmpty())
        return;
    currentIndex = (currentIndex - 1 + cars.size()) % cars.size();
    updateImage();
}

void CarSelectionController::updateImage() {
    if (cars.isEmpty())
        return;
    QPixmap pix(cars[currentIndex].getImagePath());
    ui->labelCarImage->setPixmap(
            pix.scaled(550, 350, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

CarType CarSelectionController::currentCarType() const {
    if (cars.isEmpty())
        return CarType::FIAT_600;
    return cars[currentIndex].getType();
}

bool CarSelectionController::submitSelectedCar() {
    QString key;
    switch (currentCarType()) {
        case CarType::FIAT_600:
            key = "fiat_600";
            break;
        case CarType::FERRARI_F40:
            key = "ferrari_F40";
            break;
        case CarType::PORSCHE_911:
            key = "porsche_911";
            break;
        case CarType::SEDAN:
            key = "sedan";
            break;
        case CarType::JEEP_4X4:
            key = "jeep";
            break;
        case CarType::FORD_F100:
            key = "f100";
            break;
        case CarType::TRUCK:
            key = "truck";
            break;
    }
    return svc.chooseCar(key);
}
