#include "myview.h"
#include "board.h"

MyView::MyView(QGraphicsScene *s) : QGraphicsView(s)
{
}

void MyView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);   // need to call base class function to continue processing
    QPoint p = event->pos();                // event->pos() returns screen coordinates as a QPoint object.
    QPointF pf = mapToScene(p);             // Map this to scene coordinate system
    //qDebug("View: %3d,%3d", event->pos().x(), event->pos().y()); // uncomment this to see the coordinates as mouse moves
    board->setAngle(pf);                    // call Board::setAngle() with the mouse coordinates
    board->update();                        // schedule an update to redraw the shooter
}
