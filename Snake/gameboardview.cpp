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
                rect->setBrush(Qt::black); // empty
            }
            else if (value == 1)
            {
                rect->setBrush(Qt::green); // snake segment
            }
            else if (value == 2)
            {
                rect->setBrush(Qt::blue); // snake head
            }
            else if (value == 3)
            {
                rect->setBrush(Qt::red); // food item
            }
        }
    }
}
