#include "introwindow.h"

IntroWindow::IntroWindow(QWidget *parent)
    : QMainWindow{parent}, maxScore(0)
{
    setWindowTitle("Snake?");
    setFixedSize(800, 600);

    startBtn = new CustomBtn("开始\nSTART", this);
    startBtn->setStyleSheet("font-size: 14px; color: green;");
    startBtn->resize(100, 50);
    startBtn->move(200, 325);

    maxScore =  readMaxScore();
    // 历史最大得分
    scoreText = new QLabel("历史最大得分\nHistory Max Score", this);
    scoreText->setAlignment(Qt::AlignCenter);
    scoreText->setStyleSheet("font-size: 16px; color: blue;");
    scoreText->resize(150, 50);
    scoreText->move(175, 125);

    maxScorelab = new QLabel(QString::number(maxScore), this);
    maxScorelab->setAlignment(Qt::AlignCenter);
    maxScorelab->setStyleSheet("font-size: 20px; color: blue;");
    maxScorelab->resize(150, 50);
    maxScorelab->move(175, 200);

    // 操作说明
    manual = new QLabel(
        "操作声明\nManual\n\n"
        "按A(或D)向左(右转向)\nPress A(or D) to turn\n"
        "按B返回，按R重新开始，按P暂停\nPress B to go back, R to restart,\n P to pause\n"
        "控制蛇吃到更多食物以取得更高分数\nControl the snake to gain\n more food for higher scores"
        , this);
    manual->setAlignment(Qt::AlignCenter);
    manual->setStyleSheet("font-size: 14px; color: black;");
    manual->resize(250, 200);
    manual->move(425, 125);

    mainWinPtr = new MainWindow(maxScore);
    // 监听返回信号，处理
    connect(mainWinPtr, &MainWindow::backBtnClick, this, [=](int newMaxScore){
        // 更新最大得分
        this->setMaxScore(newMaxScore);
        maxScorelab->setText(QString::number(maxScore));
        // 延时300ms
        QTimer::singleShot(300, this, [=](){
            mainWinPtr->hide();
            this->show();
        });
    });

    // 切换窗口
    connect(startBtn, &QPushButton::clicked, this,[=](){
        // 延时300ms
        QTimer::singleShot(300, this, [=](){
            this->hide();
            mainWinPtr->show();
            mainWinPtr->setGameOver(false);
            mainWinPtr->getTimer()->start(300);
            mainWinPtr->getContinueBtn()->hide();
            mainWinPtr->getPauseBtn()->show();
        });
    });

    exitBtn = new CustomBtn("退出\nEXIT", this);
    exitBtn->setStyleSheet("font-size: 14px; color: red;");
    exitBtn->resize(100, 50);
    exitBtn->move(500, 325);
    // 关闭窗口结束程序
    connect(exitBtn, &QPushButton::clicked, this, &MainWindow::close);
}

IntroWindow::~IntroWindow()
{
    writeMaxScore(maxScore);
    delete [] maxScorelab;
    delete [] scoreText;
    delete [] exitBtn;
    delete [] startBtn;
    if(mainWinPtr != nullptr)
    {
        delete [] mainWinPtr;
    }
}

int IntroWindow::getMaxScore()
{
    return maxScore;
}

void IntroWindow::setMaxScore(int newScore)
{
    maxScore = newScore;
}

int IntroWindow::readMaxScore()
{
    QFile file("./maxScore_Snake.dat");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Could not open file for reading";
        return 0;
    }

    QTextStream in(&file);
    int maxScore = 0;
    if(!in.atEnd())
    {
        maxScore = in.readLine().toInt();
    }
    file.close();
    return maxScore;
}

void IntroWindow::writeMaxScore(int score)
{
    QFile file("./maxScore_Snake.dat");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        qDebug() << "Could not open file for writing";
        return;
    }

    QTextStream out(&file);
    out << score;
    file.close();
}
