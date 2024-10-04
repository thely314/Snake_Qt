#include "snake.h"

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
    : QObject{parent}, NowVector(0, 1)
{
    for (int i = 0; i < initialLength; ++i)
    {
        segments.push_back(SnakeSegment(startPos.x(), startPos.y() - i));
    }
}

Snake::~Snake()
{

}

void Snake::move(int newHeadX, int newHeadY)
{
    // Remove the tail segment
    segments.pop_back();

    // Add a new head segment
    segments.insert(segments.begin(), SnakeSegment(newHeadX, newHeadY));
}

void Snake::grow()
{
    // Add a new head segment to simulate growth
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
    for (int i = 0; i < initialLength; ++i)
    {
        segments.push_back(SnakeSegment(startPos.x(), startPos.y() - i));
    }
}
