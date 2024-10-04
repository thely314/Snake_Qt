#ifndef GAMEBOARDVIEW_H
#define GAMEBOARDVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>

#include "board.h"

class GameBoardView : public QGraphicsView
{
    Q_OBJECT
public:
    GameBoardView(QWidget* parent = nullptr);
    void drawGameBoard(const Board& board);

private:
    QGraphicsScene* scene;
};

#endif // GAMEBOARDVIEW_H
