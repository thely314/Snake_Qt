#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <QKeyEvent>
#include <QMessageBox>
#include <QLabel>

#include "CustomBtn.h"
#include "snake.h"
#include "gameboardview.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int sc, QWidget *parent = nullptr);
    ~MainWindow();
    bool isGameOver();
    void setGameOver(bool s);
    void update();
    void keyPressEvent(QKeyEvent *event) override;
    void generateNewFood(Board& board);
    void drawerUpdate();
    void gameOverMessage();
    QTimer* getTimer();
    void restartGame();
    CustomBtn* getContinueBtn();
    CustomBtn* getPauseBtn();

    int timeDelay = 0;

private:
    Ui::MainWindow *ui;
    CustomBtn *backBtn;
    CustomBtn *restartBtn;
    CustomBtn *pauseBtn;
    CustomBtn *continueBtn;

    QLabel *maxTitlelab;
    QLabel *maxScorelab;
    QLabel *nowTitlelab;
    QLabel *nowScorelab;

    int maxScore;
    int nowScore;
    QTimer *gameLoopTimer;
    bool gameOver;
    Snake *player;
    Board *gameBoard;
    GameBoardView *drawer;
    QPoint *food;
signals:
    void backBtnClick(int newMaxScore);
};
#endif // MAINWINDOW_H
