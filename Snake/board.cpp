#include "board.h"
#include <qdebug.h>

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
    try
    {
        if(row < 0 || row >= height)
        {
            throw std::runtime_error("get index row out of bounds");
        }
        if(col < 0 || col >= width)
        {
            throw std::runtime_error("get index col out of bounds");
        }
    }
    catch(std::exception &e)
    {
        qDebug() << e.what() <<
            " row: " << row << " col: " << col;
    }
    return map[row][col];
}

void Board::setStateAt(int row, int col, int state)
{
    try
    {
        if(row < 0 || row >= height)
        {
            throw std::runtime_error("set index row out of bounds");
        }
        if(col < 0 || col >= width)
        {
            throw std::runtime_error("set index col out of bounds");
        }
    }
    catch(std::exception &e)
    {
        qDebug() << e.what() <<
            " row: " << row << " col: " << col;
    }
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
            setStateAt(i, j, 0);
        }
    }
}
