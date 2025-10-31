#ifndef CAR_H
#define CAR_H

#include <QString>

enum class CarType {
    FIAT_600,
    FERRARI_F40,
    PORSCHE_911,
    SEDAN,
    SHEEP_4X4,
    FORD_F100,
    TRUCK
};

class Car {
public:
    Car(CarType type, const QString& name, const QString& imagePath)
            : type(type), name(name), imagePath(imagePath) {}

    CarType getType() const { return type; }
    QString getName() const { return name; }
    QString getImagePath() const { return imagePath; }

private:
    CarType type;
    QString name;
    QString imagePath;
};

#endif // CAR_H
