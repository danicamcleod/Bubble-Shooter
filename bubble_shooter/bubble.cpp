#include "bubble.h"
#include "board.h"
#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <cmath>

// Initialize the colour and direction of movement
Bubble::Bubble(QGraphicsItem *parent, double shoot_angle, int dir) : QGraphicsItem(parent), gradient(-R, -R, 2*R), angle(shoot_angle), direction(dir)
{
    Qt::GlobalColor chooseColour[6] = {Qt::cyan, Qt::red, Qt::magenta, Qt::yellow, Qt::green, Qt::blue}; // array to choose bubble colour from
    colour = chooseColour[rand()%6]; // assign colour as a random selection from array

    gradient.setColorAt(1, QColor(colour).lighter(30));
    gradient.setColorAt(0, QColor(colour).lighter(150));
}

// Tell the area of the object to the framework
QRectF Bubble::boundingRect() const
{
    return QRectF(-R, -R, 2*R, 2*R);
}

QRectF Bubble::location() const
{
    return QRectF(x()-R, y()-R, 2*R, 2*R);
}

// Draw the object. Framework calls this function whenever the object needs to be drawn.
// We have no control over when this actually gets called
// Add code here to make the bubble look the way you want
void Bubble::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Set a radial gradient to make it look like a sphere
    painter->setBrush(gradient);            // set this as the brush
    painter->setPen(QPen(Qt::black, 0));    // set a zero-width border

    painter->drawEllipse(-R,-R,R*2,R*2);// Draw the circle.
}

// This gets called periodically and avoids a separate timer for each creature.
// Add code here that handles the position of the bubbles
void Bubble::step(int delay, bool stop)
{
    const double VEL(0.2);      // Velocity of the bubble
    double x = pos().x(); double y = pos().y();     // get current x,y coordinates

    // Calculate new position based on 'VEL' and 'angle', uncomment following line and pass appropriate values

    if (x > Board::W - R || x < Board::T + R) // switch direction when wall is reached
    {
        direction = -direction;
        score_mult += 1;
    }

    if (stop)                   // if current bubble collided with bubbles and stopped (established from board stop function), a 1 is passed as bool stop
    {
        direction = 0;
    }

    if (y < (Board::T+R))       // stop bubble if it has reached top
    {
        direction = 0;
    }

    double new_x = x + direction*(VEL*cos(angle)*delay);
    double new_y = y + abs(direction)*(VEL*sin(angle)*delay);

    setPos(new_x, new_y);       // Move 'step' pixels toward the next vertex

}

int Bubble::getDirection()
{
    return direction;
}

void Bubble::setAngle(double shoot_angle)
{
    angle = shoot_angle;
}

Qt::GlobalColor Bubble::getColour()
{
    return colour;
}

void Bubble::setDirection(int dir)
{
    direction = dir;
}
