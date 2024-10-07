#include "enemysnake.h"

EnemySnake::EnemySnake(QObject *parent)
    : Snake(parent)
{

}

EnemySnake::EnemySnake(int initialLength, const QPoint& startPos, QObject *parent)
    : Snake(initialLength, startPos, parent), searchThread(nullptr)
{
    for(int i = 0; i < initialLength; ++i)
    {
        getSegments().push_back(SnakeSegment(startPos.x(), startPos.y()));
    }
}

EnemySnake::~EnemySnake()
{

}

void EnemySnake::pathThinking(Board& board, QPoint head)
{
    QPoint closestTarget(-1, -1);
    int distance = INT_MAX;
    for(int row = 0; row < board.getHeight(); row++)
    {
        for(int col = 0; col < board.getWidth(); col++)
        {
            if(board.getStateAt(row, col) == 3)
            {
                int tmpDistance = abs(head.y() - row) + abs(head.x() - col);
                if(tmpDistance < distance)
                {
                    distance = tmpDistance;
                    closestTarget.setY(row);
                    closestTarget.setX(col);
                }
            }
        }
    }

    if(closestTarget.y() != -1 && closestTarget.x() != -1)
    {
        int chance = rand() % 100;
        if(chance < 10)
        {
            // left
            if(getNowVector().x() == 0)
            {
                setNowVector(
                    getNowVector().y() - getNowVector().x(),
                    0
                    );
            }
            else
            {
                setNowVector(
                    0,
                    getNowVector().y() - getNowVector().x()
                    );
            }
        }
        else if(chance > 89)
        {
            // right
            if(getNowVector().x() == 0)
            {
                setNowVector(
                    getNowVector().x() - getNowVector().y(),
                    0
                    );
            }
            else
            {
                setNowVector(
                    0,
                    getNowVector().x() - getNowVector().y()
                    );
            }
        }
        // qDebug() << "start search path";
        // searchPath(board, head, closestTarget);
    }
}

void EnemySnake::searchPath(Board& board, QPoint start, QPoint target)
{
    if(searchThread != nullptr)
    {
        return;
    }
    searchThread = new SearchPathThread(board, start, target, this);
    connect(searchThread, &SearchPathThread::pathFound, this, [=](QPoint direction){
        emit this->operate(direction);
    });
    connect(searchThread, &SearchPathThread::finish, this, &QObject::deleteLater);
    searchThread->start();
}

SearchPathThread::SearchPathThread(Board& board, QPoint start, QPoint target, QObject *parent)
    : QThread(parent), m_board(&board), m_start(start), m_target(target), m_isRunning(true)
{

}
SearchPathThread::~SearchPathThread()
{
    qDebug() << "thread interrupt";
    requestInterruption();
    wait();
}

int SearchPathThread::goalCostCalculate(QPoint current, QPoint goal)
{
    return abs(current.y() - goal.y()) + abs(current.x() - goal.x());
}

// 回溯队列查找节点
Node* findInQueue(std::vector<Node>& queue, QPoint position)
{
    for(auto& x : queue)
    {
        if(x.position == position)
        {
            return &x;
        }
    }
    return nullptr;
}

// 获取邻居节点
QList<QPoint> getNeighbors(QPoint position, const Board& board)
{
    QList<QPoint> neighbors;
    int x = position.x();
    int y = position.y();
    int h = board.getHeight();
    int w = board.getWidth();

    // 上 下 左 右
    int direct[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    for(int i=0; i<4; i++)
    {
        int nx = x + direct[i][0];
        int ny = y + direct[i][1];
        if(nx >= 0 && nx < w && ny >= 0 && ny < h && board.getStateAt(ny, nx) == 0)
        {
            neighbors.append(QPoint(nx, ny));
        }
    }
    return neighbors;
}

void SearchPathThread::run()
{
    qDebug() << "Astar thread running";
    std::vector<std::vector<bool> > visited(
        m_board->getHeight(), std::vector<bool>(m_board->getWidth(), false));
    std::vector<Node> queue;
    std::vector<Node> historyNode;
    Node startNode(m_start, QPoint(-1, -1),
                   0, goalCostCalculate(m_start, m_target));
    queue.push_back(startNode);

    while(!queue.empty() && !isInterruptionRequested())
    {
        Node currentNode = queue.front();
        historyNode.push_back(currentNode);
        // 访问标记
        visited[currentNode.position.y()][currentNode.position.x()] = true;

        if(m_board->getStateAt(currentNode.position.y(), currentNode.position.x()) == 3)
        {
            // 回溯父路径
            QList<QPoint> path;
            while(currentNode.parent != QPoint(-1, -1))
            {
                path.prepend(currentNode.position);
                currentNode = *findInQueue(historyNode, currentNode.parent);
            }
            // 起点
            path.prepend(currentNode.position);

            QPoint firstStep = path.at(1);
            int dx = firstStep.x() - m_start.x();
            int dy = firstStep.y() - m_start.y();
            QPoint direction(dx, dy);
            emit pathFound(direction);
            qDebug() << "found path case 1";
            return;
        }

        // 邻居节点
        QList<QPoint> neighbors = getNeighbors(currentNode.position, *m_board);

        for(auto n : neighbors)
        {
            if(visited[n.y()][n.x()])
            {
                continue;
            }
            visited[n.y()][n.x()] = true;
            Node nNode(n, currentNode.position,
                       currentNode.goalCost + 1, goalCostCalculate(n, m_target));
            queue.push_back(nNode);
        }
    }

    if(queue.empty())
    {
        QList<QPoint> neighbors = getNeighbors(m_start, *m_board);
        if(!neighbors.empty())
        {
            int dx = neighbors[0].x() - m_start.x();
            int dy = neighbors[0].y() - m_start.y();
            QPoint direction(dx, dy);
            emit pathFound(direction);
            qDebug() << "found path case 2";
        }
    }
    emit finish();
}
