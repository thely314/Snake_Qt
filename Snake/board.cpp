#include "board.h"

Board::Board(QObject *parent)
    : QObject{parent}
{
    map.resize(height, std::vector<int>(width, 0));
}

Board::Board(int w, int h, QObject *parent)
    : QObject{parent}, width(w), height(h)
{
    map.resize(height, std::vector<int>(width, 0));
}

Board::~Board()
{

}

int Board::getWidth() const
{
    return width;
}

int Board::getHeight() const
{
    return height;
}

int Board::getStateAt(int row, int col) const
{
    return map[row][col];
}

void Board::setStateAt(int row, int col, int state)
{
    if(row >= 0 && row < height && col >= 0 && col < width)
    {
        map[row][col] = state;
    }
}

void Board::clearMap()
{
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            map[i][j] = 0;
        }
    }
}
