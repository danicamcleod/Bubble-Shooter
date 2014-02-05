#ifndef TRACK_H
#define TRACK_H

#include <QGraphicsItem>
#include <QObject>
#include <QLabel>
#include <QtGui>
#include "bubble.h"

class QPushButton;                          // forward declaration for the button pointer

// Items to be drawn are derived from QGraphicsItem class
// We must override the boundingRect() function to provide a bounding rectangle (allows framework to draw efficiently)
// We must also override the paint() function which performs the actual drawing
// Derive from QObject class as well for the timer functionality

// Board object handles drawing game board, creating bubbles and moving them around

class Board : public QObject, public QGraphicsItem
{
    Q_OBJECT                                // add support for signals and slots
    Q_INTERFACES(QGraphicsItem)             // needed for signals and slots

public:
    Board();

    void init();                            // Do the initialization tasks
    void setAngle(const QPointF &p);        // set angle based on mouse coordinates
    QRectF boundingRect() const;            // Tell the framework the area occupied by this object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); // How this object is drawn

    bool stop();                            // Checks whether the moving bubble hits another

    enum {DELAY=30, W=36*Bubble::R-(Bubble::R/2), H=30*Bubble::R, T=5, L=40, ROWS=9, COLUMNS=17, MAXCHAR=10};     // Timer fires every 20 ms, Width fits 17 bubbles, Height fits 15 bubbles, Thickness is 5, Length of Shooter is 40
                                                                                                      // rows and columns for bubbles drawn at game start up
    QPointF cells[H/(2*Bubble::R)][COLUMNS];    // stores the coordinates of each cell a bubble can occupy

    void deleteBubbles();                   // function to be called once direction of current bubble is set to 0
    void nestBubble();                      // set just stopped bubble into nested position
    void deleteIsolated();                  // called from deleteBubbles to find the disconnected bubbles on the board and delete them

    void gameOver();                        // function to execute once bubble crosses bottom board line

    void addRow();                          // add row of bubbles when no more remaining chances

    void resetGame();                       // resets game and redraws bubbles

    void resetMode();                       // resets the mode of the game if radio button clicked but user does not want to restart game

    QString name;                           // record users name for highscore purposes
    QLabel *scoreChange;                    // points to score label so it can be accessed, changed, and updated
    QString score_label;
    int score;                              // keeps track of score fo label update
    int mult;                               // depending on the bubble member variable score_mult, value set to multiply score by 2 for 1 bounce and 4 for 2 or more bounces

    QLabel *chancesChange;                  // points to remaining chances label so it can be accessed, changed, and updated
    QString chance_label;
    int chance;                             // records number of chances during game play

    int timer;                              // keeps track of timer value and countdown
    QLabel *timerChange;
    QLabel *t;

    bool file_good;                         // bool to flag if file is acceptable and there are no problems with name and score records
    QString error;                          // records error, if any, for high score entries
    QStringList all_names;                  // hold ordered names of high scores
    QList<int> all_scores;                  // hold ordered scores of high scores
    bool checkEntry(int score, QString name);   // function to check if name and score follow guidlines
    void openFile();                        // to open, read, parse, and check file
    void addToFile(int score, QString name);    // called to record highscore
    void viewFile();                        // called to display the file (top ten only)

public slots:
    void restartGame();                     // Slot for restarting game
    void topTen();                          // Slot for showing top 10 high scores
    void help();                            // Slot for when help button clicked, displays instructions
    void novice();                          // Slot when novice button checked, resets mode and game if user confirms
    void expert();                          // Slot when expert button checked, resets mode and game if user confirms
    void master();                          // Slot when master button checked, resets mode and game if user confirms

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    void timerEvent(QTimerEvent *event);    // Timer function handles movement of bubbles. Called every DELAY msec.
    double shoot_angle;
    Bubble *current;                        // store the moving bubble
    QList <Bubble *> bubble;                // stores all bubbles on board

    QRadioButton *nov;                      // points to novice radio button
    QRadioButton *mast;                     // points to master radio button
    QRadioButton *exp;                      // points to expert radio button

    QString mode;                           // variable to store in which mode the game is being played
    QString mode_condit;                    // if game actually restarted after clicking new mode, mode is set to mode_condit, otherwise mode remains the same and game continues as normal

    bool pause;                             // used to indicate when window is open so game play cannot resume

    bool game_over;                         // used to indicate once game is over, for restart function
};

#endif // TRACK_H
