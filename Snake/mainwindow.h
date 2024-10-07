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
#include "foodManager.h"
#include "ObstacleManager.h"
#include "enemysnake.h"

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
    void generateNewObs(Board& board);
    void boardUpdate();
    void gameOverMessage();

    QTimer* getTimer();
    QTimer* setAstarTimer();
    void restartGame();
    CustomBtn* getContinueBtn();
    CustomBtn* getPauseBtn();

private:
    Ui::MainWindow *ui;
    CustomBtn *backBtn;
    CustomBtn *restartBtn;
    CustomBtn *pauseBtn;
    CustomBtn *continueBtn;

    QLabel *maxTitleLab;
    QLabel *maxScoreLab;
    QLabel *nowTitleLab;
    QLabel *nowScoreLab;
    QLabel *obsCountdownTitleLab;
    QLabel *obsUpdateCountdownLab;

    int maxScore;
    int nowScore;
    QTimer *gameLoopTimer;
    QTimer *AstarTimer;
    bool gameOver;
    Snake *player;
    EnemySnake *enemy;
    Board *gameBoard;
    GameBoardView *drawer;

    int timeDelay = 0;
    int foodScore = 0;

    FoodManager *foods;
    ObstacleManager *obsList;
    int obstacleUpdateDelay;
    int obstacleUpdateTimer;
    void resetObsUpdateTimer();
signals:
    void backBtnClick(int newMaxScore);
};
#endif // MAINWINDOW_H
