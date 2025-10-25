#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <QWidget>
#include <QStackedWidget>

class Navigation {
public:
    Navigation() = delete;
    static void goToPage(QWidget* page, QStackedWidget* stackedWidget, QWidget* window);
};

#endif // NAVIGATION_H
