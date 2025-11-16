#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <QPixmap>
#include <QStackedWidget>
#include <QWidget>

class Navigation {
public:
    static void goToPage(QWidget* page, QStackedWidget* stackedWidget, QWidget* window);
    static void updateBackground(QWidget* window, QWidget* page);

private:
    static QPixmap* originalBackground;
    static QSize lastSize;
};

#endif  // NAVIGATION_H
