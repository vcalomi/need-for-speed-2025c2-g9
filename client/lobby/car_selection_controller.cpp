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
    updateSpecs();
}

void CarSelectionController::next() {
    if (cars.isEmpty())
        return;
    currentIndex = (currentIndex + 1) % cars.size();
    updateImage();
    updateSpecs();
}

void CarSelectionController::prev() {
    if (cars.isEmpty())
        return;
    currentIndex = (currentIndex - 1 + cars.size()) % cars.size();
    updateImage();
    updateSpecs();
}

void CarSelectionController::updateImage() {
    if (cars.isEmpty())
        return;
    QPixmap pix(cars[currentIndex].getImagePath());
    ui->labelCarImage->setPixmap(
            pix.scaled(550, 350, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void CarSelectionController::updateSpecs() {
    if (cars.isEmpty())
        return;

    QString name;
    QString speed;
    QString health_points;
    QString handling;
    switch (cars[currentIndex].getType()) {
        case CarType::FIAT_600:
            name = "Fiat 600";
            speed = "120 km/h";
            health_points = "5000";
            handling = "Medium";
            break;
        case CarType::FERRARI_F40:
            name = "Ferrari F40";
            speed = "324 km/h";
            health_points = "20000";
            handling = "High";
            break;
        case CarType::PORSCHE_911:
            name = "Porsche 911";
            speed = "310 km/h";
            health_points = "3000";
            handling = "High";
            break;
        case CarType::SEDAN:
            name = "Sedan";
            speed = "180 km/h";
            health_points = "3500";
            handling = "Medium";
            break;
        case CarType::JEEP_4X4:
            name = "Jeep 4x4";
            speed = "150 km/h";
            health_points = "100";
            handling = "Offroad";
            break;
        case CarType::FORD_F100:
            name = "Ford F100";
            speed = "140 km/h";
            health_points = "5000";
            handling = "Low";
            break;
        case CarType::TRUCK:
            name = "Truck";
            speed = "120 km/h";
            health_points = "10000";
            handling = "Low";
            break;
    }

    QString html =
            QString("<div style='text-align:center; font-size:28px; font-weight:800; "
                    "color:#1cd3dc; margin-bottom:12px; text-shadow:0px 0px 6px #00eaff;'>%1</div>"

                    "<div style='font-size:16px; display:flex; justify-content:center; "
                    "gap:32px; letter-spacing:0.5px;'>"

                    "<span style='color:white; display:flex; align-items:center;'>"
                    "<span style='color:#1cd3dc; margin-left:6px;'>Speed: </span>"
                    "<span style='font-weight:900; color:white; margin-left:4px;'>%2</span>"
                    "</span>"

                    "<span style='color:white; display:flex; align-items:center;'>"
                    " - <span style='color:#1cd3dc; margin-left:6px;'>Health points: </span>"
                    "<span style='font-weight:900; color:white; margin-left:4px;'>%3</span>"
                    "</span>"


                    "<span style='color:white; display:flex; align-items:center;'>"
                    " - <span style='color:#1cd3dc; margin-left:6px;'>Handling: </span>"
                    "<span style='font-weight:900; color:white; margin-left:4px;'>%4</span>"
                    "</span>"
                    "</div>")
                    .arg(name, speed, health_points, handling);


    ui->labelCarSpecs->setTextFormat(Qt::RichText);
    ui->labelCarSpecs->setText(html);
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
