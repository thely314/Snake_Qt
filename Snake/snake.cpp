#include "snake.h"
#include <qdebug.h>

int SnakeSegment::getX() const
{
    return position.x();
}

int SnakeSegment::getY() const
{
    return position.y();
}

Snake::Snake(QObject *parent)
    : QObject{parent}
{

}

Snake::Snake(int initialLength, const QPoint& startPos, QObject *parent)
    : QObject{parent}, NowVector(1, 0)
{
    for(int i = 0; i < initialLength; ++i)
    {
        segments.push_back(SnakeSegment(startPos.x() - i, startPos.y()));
    }
}

Snake::~Snake()
{

}

void Snake::move(int newHeadX, int newHeadY)
{
    segments.pop_back();

    segments.insert(segments.begin(), SnakeSegment(newHeadX, newHeadY));
}

void Snake::grow()
{
    // 添加体节
    SnakeSegment newHead = segments.front();
    segments.insert(segments.begin(), newHead);
}

const std::vector<SnakeSegment>& Snake::getSegments() const
{
    return segments;
}

QPoint Snake::getNowVector()
{
    return NowVector;
}

void Snake::setNowVector(int x, int y)
{
    try
    {
        if(x < -1 || x > 1)
        {
            throw std::runtime_error("unexcepted x");
        }
        if(y < -1 || y > 1)
        {
            throw std::runtime_error("unexcepted y");
        }
    }
    catch(std::exception &e)
    {
        qDebug() << e.what() <<
            " x: " << x << " y: " << y;
    }
    NowVector.setX(x);
    NowVector.setY(y);
}

SnakeSegment Snake::getHead()
{
    return getSegments().front();
}

void Snake::restart(int initialLength, const QPoint& startPos)
{
    segments.clear();
    for(int i = 0; i < initialLength; ++i)
    {
        segments.push_back(SnakeSegment(startPos.x() - i, startPos.y()));
    }
    setNowVector(1, 0);
}
