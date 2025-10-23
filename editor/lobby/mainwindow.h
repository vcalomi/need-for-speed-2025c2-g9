#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class Lobby;
}
QT_END_NAMESPACE

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void handleJoinGame();
    void handleCreateGame();
    void handleConfirmJoin();
    void handleBackToMenu();
    void handleStartGame();
    void handleCreateButton();
    void handleContinueToWait();
    void showNextRoom();
    void showPrevRoom();

private:
    Ui::Lobby* ui;
    static constexpr int PAGE_SIZE = 10; // cantidad de salas por página
    int currentPage = 0;
    QStringList allRooms;
    QString currentRoomCode;
    bool isHost = false;

    QString generateRoomCode();
    void loadRooms();
    void showPage(int page);
    void goToPage(QWidget *page);

};
#endif  // MAINWINDOW_H
