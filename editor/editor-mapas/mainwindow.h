#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsItem>
#include <QMainWindow>
#include <QStack>

#include "mapview.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct Command {
    enum Type { Add, Remove } type;
    QGraphicsItem* item = nullptr;
};

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadMapBtn_clicked();
    void on_saveMapBtn_clicked();
    void on_cleanBtn_clicked();
    void onUndo();

    void toolSelect();
    void toolCheckpoint();
    void toolHint();
    void toolSpawn();
    void toolStart();
    void toolFinish();
    void toolErase();

private:
    Ui::MainWindow* ui;
    MapView* view;
    QStack<Command> history;

    QString currentCityId() const;
    QPixmap mapForCity(const QString& city) const;
};

#endif  // MAINWINDOW_H
