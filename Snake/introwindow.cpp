#include "introwindow.h"

IntroWindow::IntroWindow(QWidget *parent)
    : QMainWindow{parent}, maxScore(0)
{
    setWindowTitle("Snake?");
    setFixedSize(800, 600);

    startBtn = new CustomBtn("开始\nSTART", this);
    startBtn->resize(100, 50);
    startBtn->move(200, 300);

    maxScore =  readMaxScore();
    // 历史最大得分
    scoreText = new QLabel("历史最大得分\nHistory Max Score", this);
    scoreText->setAlignment(Qt::AlignCenter);
    scoreText->setStyleSheet("font-size: 16px; color: blue;");
    scoreText->resize(150, 50);
    scoreText->move(325, 200);

    maxScorelab = new QLabel(QString::number(maxScore), this);
    maxScorelab->setAlignment(Qt::AlignCenter);
    maxScorelab->setStyleSheet("font-size: 20px; color: blue;");
    maxScorelab->resize(150, 50);
    maxScorelab->move(325, 250);

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
        });
    });

    exitBtn = new CustomBtn("退出\nEXIT", this);
    exitBtn->resize(100, 50);
    exitBtn->move(500, 300);
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
