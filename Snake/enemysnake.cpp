#include "enemysnake.h"

EnemySnake::EnemySnake(QObject *parent)
    : Snake(parent)
{

}

EnemySnake::EnemySnake(int initialLength, const QPoint& startPos, QObject *parent)
    : Snake(initialLength, startPos, parent)
{

}

EnemySnake::~EnemySnake()
{

}

void EnemySnake::pathThinking(Board& board, QPoint head, FoodManager& foodList)
{
    QPoint closestTarget(-1, -1);
    int distance = INT_MAX;
    for(auto& f : foodList.getfoodList())
    {
        int tmpDistance = abs(head.y() - f.getY()) + abs(head.x() - f.getX());
        if(tmpDistance < distance)
        {
            distance = tmpDistance;
            closestTarget.setY(f.getY());
            closestTarget.setX(f.getX());
        }
    }

    if(closestTarget.y() != -1 && closestTarget.x() != -1)
    {
        qDebug() << "start search path";
        Astar(board, head, closestTarget);
    }
}

// 记忆队列搜索
int findInClosedSet(std::vector<int>& clo, Node parentNode)
{
    for(auto& x : clo)
    {
        if(x == parentNode.position.y() * 100 + parentNode.position.x())
        {
            return x;
        }
    }
    return -1;
}

// 回溯队列查找节点
Node* findInNodeList(QList<Node>& NodeList, Node currentNode)
{
    for(auto& x : NodeList)
    {
        if(x.position == currentNode.parent)
        {
            return &x;
        }
    }
    return nullptr;
}

// 计算代价
int heuristicCostCalculate(QPoint current, QPoint goal)
{
    return abs(current.y() - goal.y()) + abs(current.x() - goal.x());
}

void EnemySnake::Astar(Board& m_board, QPoint m_start, QPoint m_target)
{
    std::priority_queue<Node, std::vector<Node>, CompareNodes> openSet;
    std::unordered_set<int> closedSet;
    QList<Node> nodeList;

    Node startNode(m_start, QPoint(-1, -1),
                   0, heuristicCostCalculate(m_start, m_target));
    openSet.push(startNode);

    while(!openSet.empty())
    {
        Node currentNode = openSet.top();
        nodeList.append(currentNode);
        openSet.pop();

        if(m_board.getStateAt(currentNode.position.y(), currentNode.position.x()) == 3)
        {
            // 回溯路径
            std::vector<QPoint> path;
            while (currentNode.parent != QPoint(-1, -1))
            {
                path.insert(path.begin(), currentNode.position);
                currentNode = *findInNodeList(nodeList, currentNode);
            }

            QPoint firstStep = path[0];
            qDebug() << "firstStep " << path[0];
            int dx = firstStep.x() - m_start.x();
            int dy = firstStep.y() - m_start.y();
            QPoint direction(dx, dy);
            emit operate(direction);
            qDebug() << "found path, direction " << direction;
            return;
        }
        closedSet.insert(currentNode.position.y() * 100 + currentNode.position.x());

        // 邻居节点
        std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (const auto& dir : directions)
        {
            int newX = currentNode.position.x() + dir.first;
            int newY = currentNode.position.y() + dir.second;
            if (newX >= 0 && newX < m_board.getWidth()
                && newY >= 0 && newY < m_board.getHeight()
                && closedSet.find(newY * 100 + newX) == closedSet.end())
            {
                int newCost = currentNode.cost + 1;
                if(m_board.getStateAt(newY, newX) == 5)
                {
                    newCost += 10;
                }
                int newH = heuristicCostCalculate(QPoint(newX, newY), m_target);
                Node newNode(QPoint(newX, newY), currentNode.position,
                             newCost, newH);

                openSet.push(newNode);
            }
        }
    }

    if(openSet.empty())
    {
        qDebug() << "not found path, keep direction " << this->getNowVector();
    }
}
