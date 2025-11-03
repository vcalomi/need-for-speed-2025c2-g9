#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class Loby;
}
QT_END_NAMESPACE

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

//  protected:
    //void resizeEvent(QResizeEvent *event) override;

private:
    Ui::Loby* ui;
};
#endif  // MAINWINDOW_H
