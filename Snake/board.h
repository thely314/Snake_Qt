#ifndef vector
#include <vector>
#endif // vector

#ifndef BOARD_H
#define BOARD_H

#include <QObject>

class Board : public QObject
{
    Q_OBJECT
public:
    explicit Board(QObject *parent = nullptr);
    explicit Board(int w, int h, QObject *parent = nullptr);
    ~Board();

    int getWidth() const;
    int getHeight() const;
    int getStateAt(int row, int col) const;
    void setStateAt(int row, int col, int state);
    void clearMap();
private:
    int width = 0;
    int height = 0;
    std::vector<std::vector<int> > map;
signals:
};

#endif // BOARD_H
