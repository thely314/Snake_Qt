#ifndef QPoint
#include <QPoint>
#endif

#ifndef vector
#include <vector>
#endif

#ifndef FOODMANAGER_H
#define FOODMANAGER_H

#include <QObject>

class Food
{
public:
    Food(int x, int y) : position(x, y) {}
    int getX() const;
    int getY() const;
    void setX(int x);
    void setY(int y);

private:
    QPoint position;
};

class FoodManager : public QObject
{
    Q_OBJECT
public:
    explicit FoodManager(QObject *parent = nullptr);
    explicit FoodManager(int initialnum, QObject *parent = nullptr);

    std::vector<Food>& getfoodList();
    void resetFoodList();
private:
    std::vector<Food> foodList;
};

#endif // FOODMANAGER_H
