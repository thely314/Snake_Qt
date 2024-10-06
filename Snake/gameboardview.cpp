#include "gameboardview.h"

GameBoardView::GameBoardView(QWidget* parent)
    : QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    setScene(scene);
}

void GameBoardView::drawGameBoard(const Board& board)
{
    scene->clear();

    int cellSize = 20;

    for (int row = 0; row < board.getHeight(); row++)
    {
        for (int col = 0; col < board.getWidth(); col++)
        {
            int value = board.getStateAt(row, col);
            QGraphicsRectItem* rect = scene->addRect(
                10 + col * cellSize, 10 + row * cellSize,
                cellSize, cellSize);

            if (value == 0)
            {
                rect->setBrush(Qt::black); // 空empty
            }
            else if (value == 1)
            {
                rect->setBrush(Qt::green); // 蛇身snake segment
            }
            else if (value == 2)
            {
                rect->setBrush(Qt::blue); // 蛇头snake head
            }
            else if (value == 3)
            {
                rect->setBrush(Qt::red); // 食物food
            }
            else if (value == 4)
            {
                rect->setBrush(Qt::darkYellow); //  障碍obstacle
            }
            else if (value == 5)
            {
                rect->setBrush(QBrush(QColor(255, 128, 0))); // 敌对蛇身enemy snake body
            }
            else if (value == 6)
            {
                rect->setBrush(Qt::yellow); //  敌对蛇头enemy snake head
            }
            else
            {
                rect->setBrush(Qt::black);
            }
        }
    }
}
