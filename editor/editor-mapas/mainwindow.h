#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QVector>
#include <QPointF>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void on_loadMapBtn_clicked();
    void on_saveMapBtn_clicked();
    void on_cleanBtn_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QVector<QPointF> checkpoints;

    void drawCheckpoint(const QPointF &p, int id);
    void drawLine(const QPointF &from, const QPointF &to);
};

#endif // MAINWINDOW_H
