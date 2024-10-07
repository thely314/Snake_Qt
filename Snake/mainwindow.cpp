#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(int sc, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), maxScore(sc), nowScore(0),
    gameOver(true), foods(nullptr), obsList(nullptr), timeDelay(300),
    foodScore(10), obstacleUpdateDelay(100), obstacleUpdateTimer(0), AstarTimer(nullptr)
{
    setWindowTitle("Snake!");
    setFixedSize(800, 600);
    ui->setupUi(this);

    // 历史最大得分
    maxTitleLab = new QLabel("最大得分\nMax Score", this);
    maxTitleLab->setAlignment(Qt::AlignCenter);
    maxTitleLab->setStyleSheet("font-size: 16px; color: black;");
    maxTitleLab->resize(120, 50);
    maxTitleLab->move(670, 100);

    maxScoreLab = new QLabel(QString::number(maxScore), this);
    maxScoreLab->setAlignment(Qt::AlignRight);
    maxScoreLab->setStyleSheet("font-size: 22px; color: blue;");
    maxScoreLab->resize(100, 50);
    maxScoreLab->move(670, 150);

    // 目前得分
    nowTitleLab = new QLabel("目前分数\nNow Score", this);
    nowTitleLab->setAlignment(Qt::AlignCenter);
    nowTitleLab->setStyleSheet("font-size: 16px; color: black;");
    nowTitleLab->resize(120, 50);
    nowTitleLab->move(670, 200);

    nowScoreLab = new QLabel(QString::number(nowScore), this);
    nowScoreLab->setAlignment(Qt::AlignRight);
    nowScoreLab->setStyleSheet("font-size: 22px; color: blue;");
    nowScoreLab->resize(100, 50);
    nowScoreLab->move(670, 250);

    // 障碍刷新倒计时
    obsCountdownTitleLab = new QLabel("障碍物刷新倒计时:\nObstacle update\ncountdown:", this);
    obsCountdownTitleLab->setAlignment(Qt::AlignRight);
    obsCountdownTitleLab->setStyleSheet("font-size: 12px; color: red;");
    obsCountdownTitleLab->resize(100, 50);
    obsCountdownTitleLab->move(670, 300);

    obsUpdateCountdownLab = new QLabel(QString::number(obstacleUpdateDelay - obstacleUpdateTimer), this);
    obsUpdateCountdownLab->setAlignment(Qt::AlignRight);
    obsUpdateCountdownLab->setStyleSheet("font-size: 18px; color: red;");
    obsUpdateCountdownLab->resize(80, 50);
    obsUpdateCountdownLab->move(670, 350);

    // 按钮组件
    backBtn = new CustomBtn("返回\nBack\n(B)", this);
    backBtn->setStyleSheet("font-size: 12px;");
    backBtn->resize(100, 50);
    backBtn->move(680, 40);

    restartBtn = new CustomBtn("重新开始\nRestart\n(R)", this);
    restartBtn->setStyleSheet("font-size: 12px;");
    restartBtn->resize(100, 50);
    restartBtn->move(680, 400);

    pauseBtn = new CustomBtn("暂停\nPause\n(P)", this);
    pauseBtn->setStyleSheet("font-size: 12px;");
    pauseBtn->resize(100, 50);
    pauseBtn->move(680, 500);

    continueBtn = new CustomBtn("继续\nContinue\n(P)", this);
    continueBtn->setStyleSheet("font-size: 12px;");
    continueBtn->resize(100, 50);
    continueBtn->move(680, 500);
    continueBtn->hide();

    // 返回上一级窗口
    connect(backBtn, &QPushButton::clicked, this, [=](){
        restartGame();
        getTimer()->stop();
        // setAstarTimer()->stop();
        emit this->backBtnClick(nowScore>maxScore? nowScore:maxScore);
    });

    // 重新开始
    connect(restartBtn, &QPushButton::clicked, this, [=](){
        restartGame();
    });

    // 暂停
    connect(pauseBtn, &QPushButton::clicked, this, [=](){
        getTimer()->stop();
        // setAstarTimer()->stop();
        pauseBtn->hide();
        continueBtn->show();
    });

    // 继续
    connect(continueBtn, &QPushButton::clicked, this, [=](){
        getTimer()->start(timeDelay);
        // setAstarTimer()->start(timeDelay * 10);
        continueBtn->hide();
        pauseBtn->show();
    });

    // 更新计时器
    gameLoopTimer = new QTimer(this);
    connect(gameLoopTimer, &QTimer::timeout, this, &MainWindow::update);

    // 玩家初始化
    player = new Snake(3, QPoint(17, 16), this);

    // 按键响应
    connect(player, &Snake::turn, this, [=](int value){
        // left
        if(value == -1)
        {
            if(player->getNowVector().x() == 0)
            {
                player->setNowVector(
                    player->getNowVector().y() - player->getNowVector().x(),
                    0
                    );
            }
            else
            {
                player->setNowVector(
                    0,
                    player->getNowVector().y() - player->getNowVector().x()
                    );
            }
        }
        // right
        else if(value == 1)
        {
            if(player->getNowVector().x() == 0)
            {
                player->setNowVector(
                    player->getNowVector().x() - player->getNowVector().y(),
                    0
                    );
            }
            else
            {
                player->setNowVector(
                    0,
                    player->getNowVector().x() - player->getNowVector().y()
                    );
            }
        }
    });

    // 敌人初始化
    enemy = new EnemySnake(4, QPoint(3, 3), this);

    // 敌人寻路
    /*
    AstarTimer = new QTimer(this);
    connect(AstarTimer, &QTimer::timeout, this, [=](){
        qDebug() << "Astar thinking...";
        enemy->pathThinking(*gameBoard, QPoint(enemy->getHead().getX(), enemy->getHead().getY()));
        // setAstarTimer()->start(timeDelay * 10);
    });
    */

    connect(enemy, &EnemySnake::operate, this, [=](QPoint operate){
        if(operate.x() == -1 || operate.y() == -1)
        {
            return;
        }
        enemy->setNowVector(operate.x(), operate.y());
        qDebug() << "changed direction: " << enemy->getNowVector().x() << ", " << enemy->getNowVector().y();
    });

    gameBoard = new Board(33, 29, this);
    drawer = new GameBoardView(this);
    drawer->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    drawer->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    drawer->resize(660, 580);
    drawer->move(10, 10);

    foods = new FoodManager(2, this);
    generateNewFood(*gameBoard);
    obsList = new ObstacleManager(10, this);
    generateNewObs(*gameBoard);
    boardUpdate();
    drawer->drawGameBoard(*gameBoard);
}

MainWindow::~MainWindow()
{
    delete backBtn;
    delete ui;
}

bool MainWindow::isGameOver()
{
    return gameOver;
}

void MainWindow::setGameOver(bool s)
{
    gameOver = s;
}

// 食物刷新
void MainWindow::generateNewFood(Board& board)
{
    for(auto& food : foods->getfoodList())
    {
        if(food.getX() != -1 && food.getY() != -1)
        {
            continue;
        }
        int x = 0, y = 0;
        do
        {
            x = rand() % board.getWidth();
            y = rand() % board.getHeight();
        }
        while(board.getStateAt(y, x) != 0);
        food.setX(x);
        food.setY(y);
    }
}

// 障碍刷新
void MainWindow::generateNewObs(Board& board)
{
    for(auto& obs : obsList->getObstacleList())
    {
        if(obs.getX() != -1 && obs.getY() != -1)
        {
            continue;
        }
        int x = 0, y = 0;
        do
        {
            x = rand() % board.getWidth();
            y = rand() % board.getHeight();
        }
        while(board.getStateAt(y, x) != 0);
        obs.setX(x);
        obs.setY(y);
    }
}

void MainWindow::resetObsUpdateTimer()
{
    obstacleUpdateTimer = 0;
}

void MainWindow::boardUpdate()
{
    gameBoard->clearMap();

    for(auto& p : player->getSegments())
    {
        gameBoard->setStateAt(p.getY(), p.getX(), 1);
    }
    gameBoard->setStateAt(player->getHead().getY(), player->getHead().getX(), 2);

    for(auto& food : foods->getfoodList())
    {
        gameBoard->setStateAt(food.getY(), food.getX(), 3);
    }
    for(auto& obs : obsList->getObstacleList())
    {
        gameBoard->setStateAt(obs.getY(), obs.getX(), 4);
    }

    for(auto& e : enemy->getSegments())
    {
        gameBoard->setStateAt(e.getY(), e.getX(), 5);
    }
    gameBoard->setStateAt(enemy->getHead().getY(), enemy->getHead().getX(), 6);

    emit enemy->enemyMoved();
}

void MainWindow::update()
{
    if (isGameOver())
    {
        gameLoopTimer->stop();
        // AstarTimer->stop();
        return;
    }
    // qDebug() << setAstarTimer()->remainingTime() << " left";

    QPoint nowVec = player->getNowVector();
    QPoint tmpHead(player->getHead().getX(), player->getHead().getY());

    int enemyDesX = (enemy->getHead().getX() + enemy->getNowVector().x());
    int enemyDesY = (enemy->getHead().getY() + enemy->getNowVector().y());

    // qDebug() << "head at x:" <<tmpHead.x() << " y: "<< tmpHead.y();
    // 失败判断
    if(tmpHead.x() + nowVec.x() < 0
        || tmpHead.x() + nowVec.x() >= gameBoard->getWidth()
        || tmpHead.y() + nowVec.y() < 0
        || tmpHead.y() + nowVec.y() >= gameBoard->getHeight()
        || gameBoard->getStateAt(
               tmpHead.y() + nowVec.y(), tmpHead.x() + nowVec.x()) == 1
        || gameBoard->getStateAt(
               tmpHead.y() + nowVec.y(), tmpHead.x() + nowVec.x()) == 2
        || gameBoard->getStateAt(
               tmpHead.y() + nowVec.y(), tmpHead.x() + nowVec.x()) == 4
        || gameBoard->getStateAt(
               tmpHead.y() + nowVec.y(), tmpHead.x() + nowVec.x()) == 5
        || gameBoard->getStateAt(
               tmpHead.y() + nowVec.y(), tmpHead.x() + nowVec.x()) == 6)
    {
        setGameOver(true);
        gameOverMessage();
        return;
    }

    // 敌人死亡
    if(enemyDesX < 0
        || enemyDesX >= gameBoard->getWidth()
        || enemyDesY < 0
        || enemyDesY >= gameBoard->getHeight()
        || gameBoard->getStateAt(enemyDesY, enemyDesX) == 1
        || gameBoard->getStateAt(enemyDesY, enemyDesX) == 2
        || gameBoard->getStateAt(enemyDesY, enemyDesX) == 4
        || gameBoard->getStateAt(enemyDesY, enemyDesX) == 5
        || gameBoard->getStateAt(enemyDesY, enemyDesX) == 6)
    {
        int x = 0, y = 0;
        do
        {
            x = rand() % (gameBoard->getWidth() - 1);
            y = rand() % gameBoard->getHeight();
        }
        while(gameBoard->getStateAt(y, x) != 0 || gameBoard->getStateAt(y, x + 1) != 0);
        enemy->restart(4, QPoint(x, y));
        enemyDesX = (enemy->getHead().getX() + enemy->getNowVector().x());
        enemyDesY = (enemy->getHead().getY() + enemy->getNowVector().y());
    }

    // 移动
    player->move(tmpHead.x() + nowVec.x(), tmpHead.y() + nowVec.y());

    // 敌人寻路 临时占位
    enemy->pathThinking(*gameBoard, QPoint(enemy->getHead().getX(), enemy->getHead().getY()));
    enemy->move(enemyDesX, enemyDesY);

    nowScore++;

    // 玩家吃到食物
    if(gameBoard->getStateAt(
            player->getHead().getY(), player->getHead().getX()) == 3)
    {
        player->grow();
        nowScore += foodScore;

        for(auto& food : foods->getfoodList())
        {
            if(player->getHead().getY() == food.getY()
                && player->getHead().getX() == food.getX())
            {
                food.setX(-1);
                food.setY(-1);
                break;
            }
        }
    }

    // 敌人吃到食物
    if(gameBoard->getStateAt(
            enemy->getHead().getY(), enemy->getHead().getX()) == 3)
    {
        enemy->grow();

        for(auto& food : foods->getfoodList())
        {
            if(enemy->getHead().getY() == food.getY()
                && enemy->getHead().getX() == food.getX())
            {
                food.setX(-1);
                food.setY(-1);
                break;
            }
        }
    }

    // 食物刷新调用
    generateNewFood(*gameBoard);

    // 障碍定时刷新
    obstacleUpdateTimer++;
    obsUpdateCountdownLab->setText(QString::number(obstacleUpdateDelay - obstacleUpdateTimer));
    if(obstacleUpdateTimer >= obstacleUpdateDelay)
    {
        resetObsUpdateTimer();
        obsList->resetObstacleList(10);
        generateNewObs(*gameBoard);
    }

    // 加速
    if(nowScore > 200 && nowScore <= 500)
    {
        timeDelay = 200;
        foodScore = 15;
    }
    else if(nowScore > 500)
    {
        timeDelay = 120;
        foodScore = 20;
    }

    // 分数显示更新
    nowScoreLab->setText(QString::number(nowScore));
    boardUpdate();
    drawer->drawGameBoard(*gameBoard);
    gameLoopTimer->start(timeDelay);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_A)
    {
        // right
        emit player->turn(-1);
    }
    else if(event->key() == Qt::Key_D)
    {
        // left
        emit player->turn(1);
    }
    else if(event->key() == Qt::Key_B)
    {
        restartGame();
        getTimer()->stop();
        // setAstarTimer()->stop();
        emit this->backBtnClick(nowScore>maxScore? nowScore:maxScore);
    }
    else if(event->key() == Qt::Key_R)
    {
        restartGame();
    }
    else if(event->key() == Qt::Key_P)
    {
        if(pauseBtn->isHidden())
        {
            getTimer()->start(timeDelay);
            // setAstarTimer()->start(timeDelay * 10);
            continueBtn->hide();
            pauseBtn->show();
        }
        else
        {
            getTimer()->stop();
            // setAstarTimer()->stop();
            pauseBtn->hide();
            continueBtn->show();
        }
    }
}

void MainWindow::gameOverMessage()
{
    QMessageBox msgBox(this);
    msgBox.setText("游戏结束\nGame Over!");
    msgBox.setInformativeText("点击Ok返回主菜单\nClick Ok to return to Main Menu");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.show();

    // 处理用户选择
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok)
    {
        restartGame();
        getTimer()->stop();
        // setAstarTimer()->stop();
        emit this->backBtnClick(nowScore>maxScore? nowScore:maxScore);
    }
    else
    {
        restartGame();
    }
    msgBox.close();
}

QTimer* MainWindow::getTimer()
{
    return gameLoopTimer;
}

QTimer* MainWindow::setAstarTimer()
{
    return AstarTimer;
}

void MainWindow::restartGame()
{
    maxScore = maxScore>nowScore? maxScore:nowScore;
    nowScore = 0;
    maxScoreLab->setText(QString::number(maxScore));

    // 初始化
    gameBoard->clearMap();
    player->restart(3, QPoint(17, 16));
    foods->resetFoodList();
    obsList->resetObstacleList(10);
    generateNewObs(*gameBoard);
    enemy->restart(4, QPoint(3, 3));
    resetObsUpdateTimer();
    timeDelay = 300;
    foodScore = 10;
    setGameOver(false);
    getTimer()->stop();
    // setAstarTimer()->stop();

    if(pauseBtn->isHidden())
    {
        getTimer()->start(timeDelay);
        // setAstarTimer()->start(timeDelay * 10);
        continueBtn->hide();
        pauseBtn->show();
    }
    getTimer()->start(timeDelay);
    // setAstarTimer()->start(timeDelay * 10);
}

CustomBtn* MainWindow::getContinueBtn()
{
    return continueBtn;
}

CustomBtn* MainWindow::getPauseBtn()
{
    return pauseBtn;
}
