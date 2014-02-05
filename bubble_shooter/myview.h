#ifndef MYVIEW_H
#define MYVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>

class Board; // forward declaration for pointer to Board object
// we need to subclass from QGraphicsView and override mouseMoveEvent() in order to
// catch the mouse move events
class MyView : public QGraphicsView
{
public:
    MyView(QGraphicsScene * s);
    void setBoard(Board *b) {board = b;}
protected:
    void mouseMoveEvent (QMouseEvent * event);

private:
    Board *board;
};

#endif // MYVIEW_H
