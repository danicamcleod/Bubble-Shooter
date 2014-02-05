#ifndef CREATURE_H
#define CREATURE_H

#include <Qt>
#include <QGraphicsItem>
#include <QRadialGradient>

// Items to be drawn are derived from QGraphicsItem class
// We must override the boundingRect() function to provide a bounding rectangle (allows framework to draw efficiently)
// We must also override the paint() function which performs the actual drawing


// Bubble object encapsulates a bubble
class Bubble : public QGraphicsItem
{
public:
    Bubble(QGraphicsItem * parent = 0, double shoot_angle = -3.1415/4, int dir = 1); // Constructor takes a parent item (if given) and set it as parent, and the shootAngle is passed

    QRectF boundingRect() const;    // Tells the framework the area that this object occupies
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); // Draw the object

    void step(int delay, bool stop = 0);           // move the bubble

    int getDirection();             // returns direciton of bubble (0 if stopped)
    void setAngle(double shoot_angle);   // sets bubbles angle to shootAngle
    Qt::GlobalColor getColour();    // returns colour of bubble
    void setDirection(int dir);     // sets the direction of the bubble to dir

    int score_mult;                 // variable to keep track of wall bounces of a bubble, so score can be multiplied

    QRectF location() const;        // Returns rectangle the bubble occupies

    enum {R = 15};                  // radius of bubble

private:
    QRadialGradient gradient;       // make a brush to paint a spehere
    Qt::GlobalColor colour;         // colour of bubble
    double angle;                   // direction of movement in radians
    int direction;                  // track direction of horizontal velocity
};

#endif // CREATURE_H
