#include "ObstacleManager.h"

#ifndef QPoint
#include <qpoint.h>
#endif

int Obstacle::getX() const
{
    return position.x();
}

int Obstacle::getY() const
{
    return position.y();
}

void Obstacle::setX(int x)
{
    position.setX(x);
}

void Obstacle::setY(int y)
{
    position.setY(y);
}

ObstacleManager::ObstacleManager(QObject *parent)
    : QObject{parent}
{

}

ObstacleManager::ObstacleManager(int initialNum, QObject *parent)
    : QObject(parent)
{
    for(int i = 0; i < initialNum; ++i)
    {
        ObstacleList.push_back(Obstacle(-1, -1));
    }
}

std::vector<Obstacle>& ObstacleManager::getObstacleList()
{
    return ObstacleList;
}

void ObstacleManager::resetObstacleList(int initialNum)
{
    if(!ObstacleList.empty())
    {
        ObstacleList.clear();
    }
    for(int i = 0; i < initialNum; ++i)
    {
        ObstacleList.push_back(Obstacle(-1, -1));
    }
}
