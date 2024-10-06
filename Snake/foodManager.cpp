#include "foodManager.h"

#ifndef QPoint
#include <qpoint.h>
#endif

int Food::getX() const
{
    return position.x();
}

int Food::getY() const
{
    return position.y();
}

void Food::setX(int x)
{
    position.setX(x);
}

void Food::setY(int y)
{
    position.setY(y);
}

FoodManager::FoodManager(QObject *parent)
    : QObject{parent}
{

}

FoodManager::FoodManager(int initialnum, QObject *parent)
    : QObject(parent)
{
    for(int i = 0; i < initialnum; ++i)
    {
        foodList.push_back(Food(-1, -1));
    }
}

std::vector<Food>& FoodManager::getfoodList()
{
    return foodList;
}

void FoodManager::resetFoodList()
{
    for(auto& f : foodList)
    {
        f.setX(-1);
        f.setY(-1);
    }
}
