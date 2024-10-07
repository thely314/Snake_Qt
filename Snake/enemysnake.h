#ifndef Snake
#include "snake.h"
#endif

#ifndef Board
#include "board.h"
#endif

#ifndef QThread
#include <QThread>
#endif

#ifndef ENEMYSNAKE_H
#define ENEMYSNAKE_H

struct Node
{
public:
    QPoint position;
    QPoint parent;
    int cost = INT_MAX;
    int goalCost = INT_MAX;

    Node(QPoint pos = {-1, -1}, QPoint par = {-1, -1}, int cost = INT_MAX, int goalCost = INT_MAX)
        : position(pos), parent(par), cost(cost), goalCost(goalCost)
    {

    }
    // 最小目标成本为优
    bool operator<(const Node& other) const
    {
        return goalCost > other.goalCost;
    }
};

class SearchPathThread : public QThread
{
    Q_OBJECT
public:
    SearchPathThread(Board& board, QPoint start, QPoint target, QObject *parent = nullptr);
    ~SearchPathThread();

    int goalCostCalculate(QPoint current, QPoint goal);
protected:
    void run() override;
private:
    Board *m_board;
    QPoint m_start;
    QPoint m_target;
    bool m_isRunning;
signals:
    void pathFound(QPoint direction);
    void finish();
};

class EnemySnake : public Snake
{
    Q_OBJECT
public:
    explicit EnemySnake(QObject *parent = nullptr);
    explicit EnemySnake(int initialLength, const QPoint& startPos, QObject *parent = nullptr);
    ~EnemySnake();

    void pathThinking(Board& board, QPoint head);
    void searchPath(Board& board, QPoint start, QPoint target);
private:
    SearchPathThread *searchThread;
signals:
    void enemyMoved();
    void operate(QPoint direction);
};

#endif // ENEMYSNAKE_H
