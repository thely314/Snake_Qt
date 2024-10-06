#include <QFile>
#include <QTextStream>

#ifndef MAINWINDOW_H
#include "mainwindow.h"
#endif

#ifndef QTimer
#include <QTimer>
#endif

#ifndef INTROWINDOW_H
#define INTROWINDOW_H

#include <QMainWindow>

class IntroWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit IntroWindow(QWidget *parent = nullptr);
    ~IntroWindow();

    int getMaxScore();
    void setMaxScore(int newScore);

    int readMaxScore();
    void writeMaxScore(int score);
private:
    MainWindow * mainWinPtr = nullptr;
    CustomBtn *startBtn;
    CustomBtn *exitBtn;
    QLabel *scoreText;
    QLabel *maxScorelab;
    QLabel *manual;
    QLabel *signal;

    int maxScore;
signals:
};

#endif // INTROWINDOW_H
