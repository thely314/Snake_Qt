#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(int sc, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), maxScore(sc), nowScore(0),
    gameOver(true), food(nullptr), timeDelay(300)
{
    setWindowTitle("Snake!");
    setFixedSize(800, 600);
    ui->setupUi(this);

    // 历史最大得分
    maxTitlelab = new QLabel("最大得分\nMax Score", this);
    maxTitlelab->setAlignment(Qt::AlignCenter);
    maxTitlelab->setStyleSheet("font-size: 16px; color: black;");
    maxTitlelab->resize(120, 50);
    maxTitlelab->move(670, 100);

    maxScorelab = new QLabel(QString::number(maxScore), this);
    maxScorelab->setAlignment(Qt::AlignRight);
    maxScorelab->setStyleSheet("font-size: 22px; color: blue;");
    maxScorelab->resize(100, 50);
    maxScorelab->move(670, 150);

    // 目前得分
    nowTitlelab = new QLabel("目前分数\nNow Score", this);
    nowTitlelab->setAlignment(Qt::AlignCenter);
    nowTitlelab->setStyleSheet("font-size: 16px; color: black;");
    nowTitlelab->resize(120, 50);
    nowTitlelab->move(670, 200);

    nowScorelab = new QLabel(QString::number(nowScore), this);
    nowScorelab->setAlignment(Qt::AlignRight);
    nowScorelab->setStyleSheet("font-size: 22px; color: blue;");
    nowScorelab->resize(100, 50);
    nowScorelab->move(670, 250);

    // 按钮组件
    backBtn = new CustomBtn("返回\nBack\n(B)", this);
    backBtn->setStyleSheet("font-size: 12px;");
    backBtn->resize(100, 50);
    backBtn->move(680, 40);

    restartBtn = new CustomBtn("重新开始\nRestart\n(R)", this);
    restartBtn->setStyleSheet("font-size: 12px;");
    restartBtn->resize(100, 50);
    restartBtn->move(680, 350);

    pauseBtn = new CustomBtn("暂停\nPause\n(P)", this);
    pauseBtn->setStyleSheet("font-size: 12px;");
    pauseBtn->resize(100, 50);
    pauseBtn->move(680, 450);

    continueBtn = new CustomBtn("继续\nContinue\n(P)", this);
    continueBtn->setStyleSheet("font-size: 12px;");
    continueBtn->resize(100, 50);
    continueBtn->move(680, 450);
    continueBtn->hide();

    // 返回上一级窗口
    connect(backBtn, &QPushButton::clicked, this, [=](){
        emit this->backBtnClick(nowScore>maxScore? nowScore:maxScore);
    });

    // 重新开始
    connect(restartBtn, &QPushButton::clicked, this, [=](){
        restartGame();
    });

    // 暂停
    connect(pauseBtn, &QPushButton::clicked, this, [=](){
        getTimer()->stop();
        pauseBtn->hide();
        continueBtn->show();
    });

    // 继续
    connect(continueBtn, &QPushButton::clicked, this, [=](){
        getTimer()->start(timeDelay);
        continueBtn->hide();
        pauseBtn->show();
    });

    // 更新计时器
    gameLoopTimer = new QTimer(this);
    connect(gameLoopTimer, &QTimer::timeout, this, &MainWindow::update);

    player = new Snake(3, QPoint(17, 16), this);
    // 按键响应
    connect(player, &Snake::turn, this, [=](int value){
        // left
        if(value == -1)
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
        // right
        else if(value == 1)
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
    });

    gameBoard = new Board(33, 29, this);
    drawer = new GameBoardView(this);
    drawer->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    drawer->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    drawer->resize(660, 580);
    drawer->move(10, 10);
    food = new QPoint(-1, -1);
    generateNewFood(*gameBoard);
    drawerUpdate();
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

void MainWindow::generateNewFood(Board& board)
{
    if(food->x() != -1 && food->y() != -1)
    {
        return;
    }
    int x = 0, y = 0;
    do
    {
        x = rand() % board.getWidth();
        y = rand() % board.getHeight();
    }
    while(board.getStateAt(x, y) != 0);
    food->setX(x);
    food->setY(y);
}

void MainWindow::drawerUpdate()
{
    for(int i=0; i<gameBoard->getWidth(); i++)
    {
        for(int j=0; j<gameBoard->getHeight(); j++)
        {
            gameBoard->setStateAt(i, j, 0);
        }
    }
    for(auto p : player->getSegments())
    {
        int x = p.getX();
        int y = p.getY();
        gameBoard->setStateAt(x, y, 1);
    }
    gameBoard->setStateAt(player->getHead().getX(), player->getHead().getY(), 2);
    gameBoard->setStateAt(food->x(), food->y(), 3);
    drawer->drawGameBoard(*gameBoard);
}

void MainWindow::update()
{
    if (isGameOver())
    {
        gameLoopTimer->stop();
        return;
    }

    QPoint nowVec = player->getNowVector();
    QPoint tmpHead(player->getHead().getX(), player->getHead().getY());

    // 失败判断
    if(gameBoard->getStateAt(
            tmpHead.x() + nowVec.x(), tmpHead.y() + nowVec.y()) == 1
        || gameBoard->getStateAt(
            tmpHead.x() + nowVec.x(), tmpHead.y() + nowVec.y()) == 2
        || player->getHead().getX() < 0
        || player->getHead().getX() >= gameBoard->getWidth()
        || player->getHead().getY() < 0
        || player->getHead().getY() >= gameBoard->getHeight())
    {
        setGameOver(true);
        gameOverMessage();
        return;
    }

    player->move(tmpHead.x() + nowVec.x(), tmpHead.y() + nowVec.y());
    nowScore++;

    // 吃到食物
    if(gameBoard->getStateAt(
            player->getHead().getX(), player->getHead().getY()) == 3)
    {
        player->grow();
        nowScore += 50;

        food->setX(-1);
        food->setY(-1);
        generateNewFood(*gameBoard);
    }

    // 加速
    if(nowScore > 200 && nowScore <= 500)
    {
        timeDelay = 200;
    }
    else if(nowScore > 500)
    {
        timeDelay = 120;
    }

    // 分数显示更新
    nowScorelab->setText(QString::number(nowScore));
    drawerUpdate();
    gameLoopTimer->start(timeDelay);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_A)
    {
        emit player->turn(-1);
    }
    else if(event->key() == Qt::Key_D)
    {
        emit player->turn(1);
    }
    else if(event->key() == Qt::Key_B)
    {
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
            continueBtn->hide();
            pauseBtn->show();
        }
        else
        {
            getTimer()->stop();
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

void MainWindow::restartGame()
{
    maxScore = maxScore>nowScore? maxScore:nowScore;
    nowScore = 0;
    maxScorelab->setText(QString::number(maxScore));
    player->restart(3, QPoint(17, 16));
    setGameOver(false);
    getTimer()->stop();
    getTimer()->start(timeDelay);
}
