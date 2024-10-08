#ifndef Snake
#include "snake.h"
#endif

#ifndef Board
#include "board.h"
#endif

#ifndef FOODMANAGER_H
#include "foodManager.h"
#endif

#ifndef ENEMYSNAKE_H
#define ENEMYSNAKE_H
#include <queue>
#include <unordered_set>

struct Node
{
public:
    QPoint position;
    QPoint parent;
    int cost = INT_MAX;
    int heuristicCost = INT_MAX;
    int sumCost = 0;

    Node(QPoint pos = {-1, -1}, QPoint par = {-1, -1}, int cost = INT_MAX, int goalCost = INT_MAX)
        : position(pos), parent(par), cost(cost), heuristicCost(goalCost)
    {
        sumCost = cost + heuristicCost;
    }
    // 最小目标成本为优
    bool operator<(const Node& other) const
    {
        return sumCost > other.sumCost;
    }
};

struct CompareNodes
{
    bool operator()(const Node& a, const Node& b) const
    {
        return a.sumCost > b.sumCost;
    }
};

class EnemySnake : public Snake
{
    Q_OBJECT
public:
    explicit EnemySnake(QObject *parent = nullptr);
    explicit EnemySnake(int initialLength, const QPoint& startPos, QObject *parent = nullptr);
    ~EnemySnake();

    void pathThinking(Board& board, QPoint head, FoodManager& foodList);
    void searchPath(Board& board, QPoint start, QPoint target);

    void tmpSolution();
    void Astar(Board& board, QPoint start, QPoint target);
private:
signals:
    void enemyMoved();
    void operate(QPoint direction);
};

#endif // ENEMYSNAKE_H
