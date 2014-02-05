// Bubble shooter template created by Jagath Samarabandu, Oct. 16, 2012.
// This applications uses the GraphicsView framework in Qt.
// See http://doc.trolltech.com/4.6/graphicsview.html for a nice introduction

#include <QtGui>
#include "board.h"
#include "bubble.h"
#include "myview.h"

#include <math.h>

int main(int argc, char **argv)
{
    // Initialize random seed (used for colour choice)
    srand(time(NULL));

    QApplication app(argc, argv);

    // Scene object takes care of drawing all the items in the scene
    QGraphicsScene scene;
    scene.setSceneRect(0, 0, Board::W+130, Board::H+20); // Top left coordinate is (0,0). Size is (board width+130, board height+20

    // Track is a subclass of QGraphicsItem which defines how it is drawn
    Board *b = new Board;   // create the object
    b->setPos(Board::T,Board::T);     // Set its position relative to scene
    scene.addItem(b);       // Add the board object to scene
    b->init();              // Initialize only after adding to scene

    // Create a viewing area for the scene
    MyView view(&scene);
    view.setRenderHint(QPainter::Antialiasing); // anti-aliasing makes it draw nice smooth lines
    view.setBackgroundBrush(Qt::black);         // Set the backtround color to black
    view.viewport()->setMouseTracking(true);    // Start keeping track of mouse
    view.setBoard(b);

    // Set the window title
    view.setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Bubble Shooter"));
    view.show();        // show the viewing area on screen



    return app.exec();  // Start the application
}
