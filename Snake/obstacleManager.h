#ifndef QPoint
#include <QPoint>
#endif

#ifndef vector
#include <vector>
#endif

#ifndef OBSTACLEMANAGER_H
#define OBSTACLEMANAGER_H

#include <QObject>

class Obstacle
{
public:
    Obstacle(int x, int y) : position(x, y) {}
    int getX() const;
    int getY() const;
    void setX(int x);
    void setY(int y);

private:
    QPoint position;
};

class ObstacleManager : public QObject
{
    Q_OBJECT
public:
    explicit ObstacleManager(QObject *parent = nullptr);
    explicit ObstacleManager(int initialNum, QObject *parent = nullptr);

    std::vector<Obstacle>& getObstacleList();
    void resetObstacleList(int initialNum);
private:
    std::vector<Obstacle> ObstacleList;
};

#endif // OBSTACLEMANAGER_H
