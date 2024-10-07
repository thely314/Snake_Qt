#ifndef QPoint
#include <QPoint>
#endif

#ifndef vector
#include <vector>
#endif

#ifndef SNAKE_H
#define SNAKE_H

#include <QObject>
#include <QDebug>

class SnakeSegment
{
public:
    SnakeSegment(int x, int y) : position(x, y) {}
    int getX() const;
    int getY() const;

private:
    QPoint position;
};

class Snake : public QObject
{
    Q_OBJECT
public:
    explicit Snake(QObject *parent = nullptr);
    explicit Snake(int initialLength, const QPoint& startPos, QObject *parent = nullptr);
    ~Snake();

    void move(int newHeadX, int newHeadY);
    void grow();
    std::vector<SnakeSegment>& getSegments();
    QPoint getNowVector();
    void setNowVector(int x, int y);
    SnakeSegment getHead();

    void restart(int initialLength, const QPoint& startPos);

private:
    std::vector<SnakeSegment> segments;
    QPoint NowVector; // 前进方向

signals:
    void turn(int value);
};

#endif // SNAKE_H
