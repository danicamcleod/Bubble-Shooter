#include "board.h"
#include <QGraphicsScene>
#include <QtGui>
#include <cmath>
#include <iostream>
#include <QMessageBox>

// global variable to file which is the highscore file
QDir dir = QDir::home();
QFile file(dir.filePath(".bubble_highscore"));

// default for file when just using personal computer
// QFile file("/Users/danicamcleod/Desktop/bubble_shooter/bubble_highscore");

// initialize the parameters as appropriate
Board::Board() : shoot_angle(-3.1415/4), current(0)
{
    startTimer(DELAY); // wakeup every DELAY millisec

    // assume file is okay, openFile function will check and change to false if there are errors
    file_good = true;

    // read, sort, and display highscore file if it exists and is properly formatted
    openFile();

    // Have user enter name
    bool ok;
    name = QInputDialog::getText(0, tr("Enter Name"),tr("Enter name:"), QLineEdit::Normal,QDir::home().dirName(), &ok);

    score_label = "Score: ";
    chance_label = "Chances: ";
    mode_condit = "novice";
    mode = "novice";
    score = 0;
    chance = 5;

    pause = false;
    game_over = false;
}

// Tell the area occupied by the track object
QRectF Board::boundingRect() const
{
    return QRectF(0,0, W,H);
}

// Do the initialization tasks. This is where all the buttons are created
void Board::init()
{
    QVBoxLayout *layout = new QVBoxLayout;          // Create a vertical layout for a column of buttons

    QPushButton *b = new QPushButton(tr("Restart")); // create a button for restarting the game
    connect(b, SIGNAL(clicked()), this, SLOT(restartGame()));           // connect restart button to restartGame function
    layout->addWidget(b);        // add the restart button to the button row

    b = new QPushButton(tr("Top 10"));   // Create a button for showing top 10 high scores
    connect(b, SIGNAL(clicked()), this, SLOT(topTen()));   // connect this button to high score function
    layout->addWidget(b);                           // Add it to the row of buttons

    b = new QPushButton(tr("Help"));    // Create a button for opening help window
    connect(b, SIGNAL(clicked()), this, SLOT(help()));      // connect this button to help function
    layout->addWidget(b);                           // Add it to the row of buttons

    QVBoxLayout *vbox = new QVBoxLayout;
    // QRadioButton *rb = new QRadioButton(tr("Novice"));
    nov = new QRadioButton(tr("Novice"));
    nov->setCheckable(true);
    nov->setChecked(mode == "novice");
    connect(nov, SIGNAL(clicked(bool)), this, SLOT(novice()));   // connect this button to novice function
    vbox->addWidget(nov);                           // Add it to the row of buttons

    // rb = new QRadioButton(tr("Expert"));
    exp = new QRadioButton(tr("Expert"));
    exp->setCheckable(true);
    exp->setChecked(mode == "expert");
    connect(exp, SIGNAL(clicked(bool)), this, SLOT(expert()));  // connect this button to expert function
    vbox->addWidget(exp);                           // Add it to the row of buttons

    // rb = new QRadioButton(tr("Master"));
    mast = new QRadioButton(tr("Master"));
    mast->setCheckable(true);
    mast->setChecked(mode == "master");
    connect(mast, SIGNAL(clicked(bool)), this, SLOT(master()));   // connect this button to master function
    vbox->addWidget(mast);                           // Add it to the row of buttons

    QGroupBox *bg = new QGroupBox(tr("Game Mode"));
    bg->setLayout(vbox);
    layout->addWidget(bg);

    QLabel *n = new QLabel(tr("Name: "));       // Label for name
    layout->addWidget(n);
    n = new QLabel(name);
    layout->addWidget(n);

    scoreChange = new QLabel(score_label+QString::number(score,10));    // create label for score
    layout->addWidget(scoreChange);

    chancesChange = new QLabel(chance_label+QString::number(chance,10));    // create label for chances remaining
    layout->addWidget(chancesChange);

    // set space on side menu for timer
    t = new QLabel(tr(""));
    layout->addWidget(t);
    timerChange = new QLabel("");
    layout->addWidget(timerChange);

    QWidget *window = new QWidget;      // create a window to whold the row of buttons
    window->setLayout(layout);          // add the row of buttons to this window
    window->move(W+2*T,0);                // position it at the right edge
    this->scene()->addWidget(window);   // add the button window to the scene

    // initialize the array cells to record the positions a bubble can occupy
    for (int k=0; k<H/(2*Bubble::R); k++)
    {
        int y = T + Bubble::R + 2*Bubble::R*k;
        if (k%2 == 0)
            for (int j=0; j<COLUMNS; j++)
            {
                int x = T + Bubble::R + 2*Bubble::R*j;
                QPoint p(x,y);
                cells[k][j] = p;
            }
        else
            for (int j=0; j<COLUMNS; j++)
            {
                int x = T + 2*Bubble::R + 2*Bubble::R*j;
                QPoint p(x,y);
                cells[k][j] = p;
            }
    }

    // create 9 rows of 17 bubbles to begin game
    for (int k=0; k<ROWS; k++)
    {
        int y=T + Bubble::R + 2*Bubble::R*k;
        if (k%2 == 0)
            for (int j=0; j<COLUMNS; j++)
            {
                int x = T + Bubble::R + 2*Bubble::R*j;
                bubble.push_front(new Bubble(this, shoot_angle, 0));
                bubble.first()->setPos(x,y);
            }
        else
            for (int j=0; j<COLUMNS; j++)
            {
                int x = T + 2*Bubble::R + 2*Bubble::R*j;
                bubble.push_front(new Bubble(this, shoot_angle, 0));
                bubble.first()->setPos(x,y);
            }
    }

    // create the next bubbles for game play
    bubble.push_front(new Bubble(this, shoot_angle, 0));
    bubble.first()->setPos(W+T+Bubble::R,H-Bubble::R);
    bubble.push_front(new Bubble(this, shoot_angle, 0));
    bubble.first()->setPos(W+T+3*Bubble::R,H-Bubble::R);
    current = bubble.at(1);
}

void Board::setAngle(const QPointF &p)
{
    //qDebug("a: %3.0f,%3.0f", p.y(), p.x());
    if (p.y() < H)
    {
        if (W/2 <= p.x())
            shoot_angle = -atan((H-p.y())/(p.x()-(W/2))); // angle calulated if x is on the right side of P
        else
            shoot_angle = -acos((p.x()-(W/2))/sqrt(((p.x()-(W/2))*(p.x()-(W/2)))+((H-p.y())*(H-p.y())))); // angle calculated if x is on the left side of P
    }
}

// Draw the game board
void Board::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Set drawing parameters
    painter->setBrush(Qt::black);   // Black backround
    painter->setPen(QPen(Qt::gray, T, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin)); // Gray, T pixels wide solid line with flat ends and round joins
    painter->drawRect(0,0,W,H);    // Draw the game board
    painter->drawEllipse(W/2-T,H-T,2*T,2*T); // Draw circle at bottom center

    // Next Bubbles label on board
    painter->drawText(W+T,H-T-2*Bubble::R, "Next Bubbles");

    // Draw shooter
    painter->setPen(QPen(Qt::red, T, Qt::SolidLine, Qt::FlatCap)); // Change pen settings
    painter->drawLine(W/2, H, W/2+(L*cos(shoot_angle)), H+(L*sin(shoot_angle))); // Draw line with length L at angle shootAngle

}

// Timer function that handles movement of bubbles
void Board::timerEvent(QTimerEvent *)
{
    if (current)
    {
        if (stop() && current->getDirection()!=0)   // stop returns true if bubble is stopped so current 1 passed to step will cause current direction to be changed to 0
            current->step(DELAY, 1);
        else if (current->getDirection()!=0)    // assuming current bubble isn't being stopped, continues moving normally using step
            current->step(DELAY);
        else if (current != bubble.at(1) && current->getDirection()==0)     // if bubble has been stopped, it must be nested and bubbles may need to be deleted
        {                                                                   // current != bubble.at(1) because that indicates the two functions, nested and delete, have already been executed for the previous shot
            if (current->score_mult == 0)           // score_mult records the number and once bubble is stopped,
                mult = 1;                           // its used to set mult which is multiplied by score increments
            else if (current->score_mult == 1)      // to account for the bounces
                mult = 2;
            else
                mult = 4;
            nestBubble();
            deleteBubbles();
        }
    }

    // checks if bubble is below game line to end game
    if (current)
    {
        if (current->getDirection()==0)     // if current bubble exits and its direction is 0, it will be included in the check for game end
        for (int k=2; k<bubble.count(); k++)
            if (bubble.at(k)->y() >= H-Bubble::R)
                gameOver();
    }
    else
    {
        for (int k=3; k<bubble.count(); k++)       // current is moving so check starts at 3, not including the current bubble (which is at position 2)
            if (bubble.at(k)->y() >= H-Bubble::R)
                gameOver();
    }

    if (mode == "master" && current->getDirection() == 0 && !pause)   // loop to make timer function properly and fire bubble/reset when necessary
    {
        static int second_counter = 0;
        second_counter++;
        if (second_counter == (1000/DELAY))
        {
            second_counter = 0;
            timer--;
            timerChange->setText(QString::number(timer,10));
            timerChange->update();
        }

        // when timer reaches 0, fire bubble
        if (timer == 0)
        {
            current = bubble.at(1);
            current->setPos(W/2,H);
            current->setAngle(shoot_angle);
            current->score_mult = 0;
            current->setDirection(1);
            bubble.first()->setPos(W+T+Bubble::R,H-Bubble::R);
            bubble.push_front(new Bubble(this, shoot_angle, 0));
            bubble.first()->setPos(W+T+3*Bubble::R,H-Bubble::R);

            // reset timer
            timer = 5;
            timerChange->setText(QString::number(timer,10));
            timerChange->update();
        }
    }
}

// slot for the game restart button
void Board::restartGame()
{
    qDebug("clicked Restart");
    pause = true;

    // display window to have user confirm game restart (only if hasn't been asked when changing mode ie. button clicked)
    QMessageBox msg_box;
    QAbstractButton *confirm;

    if (score && !game_over)
    {
        msg_box.setText("Restart?");
        confirm = msg_box.addButton(tr("Confirm"), QMessageBox::ActionRole);
        QAbstractButton *cancel = msg_box.addButton(tr("Cancel"), QMessageBox::ActionRole);
        msg_box.exec();
    }

    if (msg_box.clickedButton() == confirm || !score || game_over)
    {
        // mode is being changed from master to novice or expert and timer labels must be deleted
        if (mode == "master")
        {
            t->setText("");
            t->update();
            timerChange->setText("");
            timerChange->update();
        }

        // game restart so mode must be set
        mode = mode_condit;

        // delete all existing bubbles
        for (int k=0; k<bubble.count(); k++)
        {
            delete bubble.at(k);
            bubble.removeAt(k);
            k--;
        }

        // re-draw board and bubbles and reset score
        resetGame();
    }
    else
        resetMode();

    pause = false;
}

// slot for loading a map file
void Board::topTen()
{
    qDebug("clicked Top 10");
    pause = true;

    if (!file_good || all_scores.isEmpty())
    {
        QMessageBox msg_box;
        msg_box.setText("Invalid highscores or no highscores at this time.");
        msg_box.exec();
    }
    else
        viewFile();

    pause = false;
}

// slot for help button
void Board::help()
{
    qDebug("clicked Help");
    pause = true;

    QMessageBox msg_box;
    msg_box.setText("Game rules and Instructions\n1.\n2.\nYou get the point.");
    msg_box.exec();

    pause = false;
}

// slot for novice mode
void Board::novice()
{
    qDebug("Novice selected");
    pause = true;

    if (score)
    {
        // message box asking to restart game in novice mode
        QMessageBox msg_box;
        msg_box.setText("Would you like to change to Novice mode?");
        QAbstractButton *confirm = msg_box.addButton(tr("Confirm"), QMessageBox::ActionRole);
        QAbstractButton *cancel = msg_box.addButton(tr("Cancel"), QMessageBox::ActionRole);
        msg_box.exec();

        if (msg_box.clickedButton() == confirm)     // if confirm is clicked, game will be restarted
        {
            mode_condit = "novice";
            restartGame();
        }
        else                    // if user doesn't want to restart game, mode will be reset to current mode of play
            resetMode();
    }
    else            // when score is 0, immediately restart game
    {
        mode_condit = "novice";
        restartGame();
    }

    pause = false;
}

// slot for expert mode
void Board::expert()
{
    qDebug("Expert selected");
    pause = true;

    if (score)
    {
        // message box asking to restart game in expert mode
        QMessageBox msg_box;
        msg_box.setText("Would you like to change to Expert mode?");
        QAbstractButton *confirm = msg_box.addButton(tr("Confirm"), QMessageBox::ActionRole);
        QAbstractButton *cancel = msg_box.addButton(tr("Cancel"), QMessageBox::ActionRole);    
        msg_box.exec();

        if (msg_box.clickedButton() == confirm)     // if confirm is clicked, game will be restarted
        {
            mode_condit = "expert";
            restartGame();
        }
        else                     // if user doesn't want to restart game, mode will be reset to current mode of play
            resetMode();
    }
    else        // when score is 0, immediately restart game
    {
        mode_condit ="expert";
        restartGame();
    }

    pause = false;
}

// slot for master mode
void Board::master()
{
    qDebug("Master selected");
    pause = true;

    if (score)
    {
        // message box asking to restart game in master mode
        QMessageBox msg_box;
        msg_box.setText("Would you like to change to Master mode?");
        QAbstractButton *confirm = msg_box.addButton(tr("Confirm"), QMessageBox::ActionRole);
        QAbstractButton *cancel = msg_box.addButton(tr("Cancel"), QMessageBox::ActionRole);
        msg_box.exec();

        if (msg_box.clickedButton() == confirm)     // if confirm is clicked, game will be restarted
        {
            mode_condit = "master";
            restartGame();
        }
        else                    // if user doesn't want to restart game, mode will be reset to current mode of play
            resetMode();
    }
    else            // when score is 0, immediately restart game
    {
        mode_condit = "master";
        restartGame();
    }

    pause = false;
}

void Board::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);      // call base class function to allow further processing

    // bubble.at(1) is on deck and is set to current to be shot, then the next bubble (first in list) is moved to the on deck position)
    // then a new bubble is created and set to the next position beside on deck
    if (current)
    {
        if ((current->getDirection() == 0 || current == bubble.at(1)) && !pause)
        {
            current = bubble.at(1);
            current->setPos(W/2,H);
            current->setAngle(shoot_angle);
            current->score_mult = 0;
            current->setDirection(1);
            bubble.first()->setPos(W+T+Bubble::R,H-Bubble::R);
            bubble.push_front(new Bubble(this, shoot_angle, 0));
            bubble.first()->setPos(W+T+3*Bubble::R,H-Bubble::R);
        }
    }
}

bool Board::stop()
{
    // check current bubble against each in list to see if rectangles containing bubbles (passed from location) interstect
    for (int k=3; k<bubble.count(); k++)
    {
        if (current->location().intersects(bubble.at(k)->location()))
        {
            // bubble stopped, so reset timer if in master mode
            if (mode == "master")
            {
                timer = 5;
                timerChange->setText(QString::number(timer,10));
                timerChange->update();
            }
            return true;
        }
    }
    return false;
}

void Board::nestBubble()
{
    // using cells, which contains all valid positions of bubbles on the board, check which position the stopped bubble is closest to and set to that spot
    for (int k=0; k<H/(2*Bubble::R); k++)
    {
        for (int j=0; j<COLUMNS; j++)
            if ((current->y() < cells[k][j].y()+Bubble::R && current->y() > cells[k][j].y()-Bubble::R)&&(current->x() < cells[k][j].x()+Bubble::R && current->x() > cells[k][j].x()-Bubble::R))
                current->setPos(cells[k][j]);
    }
}

void Board::deleteBubbles()
{    
    Bubble *go_away[(H/(2*Bubble::R))*COLUMNS];      //array of pointers to track bubbles to delete (size is all valid positions on board)
    int i=0;                  // variable to track at which array location bubble pointers are stored

    // find bubbles immediately around current bubble that have same colour and set pointers to them
    for (int k=3; k<bubble.count(); k++)
    {
        if ((bubble.at(k)->y() <= current->y()+2*Bubble::R && bubble.at(k)->y() >= current->y()-2*Bubble::R) && (bubble.at(k)->x() <= current->x()+2*Bubble::R && bubble.at(k)->x() >= current->x()-2*Bubble::R))
            if (bubble.at(k)->getColour()==current->getColour()) {
                go_away[i]=bubble.at(k);
                i++;
            }
    }

    // using the bubbles go_away points to, check around them for bubbles of the same colour and if found, point to those as well
    if (i)  // only execute if there are pointers to bubbles in the array
    {
        for (int j=0; j<i; j++)
        {
            for (int k=3; k<bubble.count(); k++)
                if (bubble.at(k)!=go_away[j])
                    if ((bubble.at(k)->y() <= go_away[j]->y()+2*Bubble::R && bubble.at(k)->y() >= go_away[j]->y()-2*Bubble::R) && (bubble.at(k)->x() <= go_away[j]->x()+(2*Bubble::R) && bubble.at(k)->x() >= go_away[j]->x()-(2*Bubble::R)))
                        if (bubble.at(k)->getColour()==go_away[j]->getColour())
                        {
                            bool test=true;
                            for (int z=0; z<i; z++)     // check that bubble isn't already in list of bubbles to delete
                            {
                                if (bubble.at(k) == go_away[z])
                                    test=false;
                            }
                            if (test)                   // bubble isn't already in list, so test is still true and goAway will now point to bubble
                            {
                                go_away[i]=bubble.at(k);
                                i++;
                            }
                        }
        }
    }

    if (i >= 2)     // if we have at least two bubbles of the same colour around current, delete them
    {
        score += mult*10*(i+1);              // increase score 10 points for each bubble of the same colour removed
        scoreChange->setText(score_label+QString::number(score,10));
        scoreChange->update();

        for (int j=0; j<i; j++)
            go_away[j]->setDirection(2);         // setting direction to 2 allows bubbles that should be deleted to be easily identified
        for (int k=3; k<bubble.count(); k++)
            if (bubble.at(k)->getDirection()==2)
            {
                delete bubble.at(k);
                bubble.removeAt(k);
                k--;
            }

        current=bubble.at(1);       // setting current to bubble.at(1) readies board for next shot and indicates bubbles have been deleted
        delete bubble.at(2);        // 2 was the location of the current bubble and it also needs to be deleted
        bubble.removeAt(2);

        deleteIsolated();           // call function that finds disconnected bubbles and deletes them
    }
    else                            // if there weren't at least 3 bubbles of the same colour, chances counter must change
    {
        chance--;
        if (chance == 0)             // once chances is 0, new row added and counter resest
        {
            addRow();
            if (mode == "novice")
                chance=5;
            else
                chance=3;
        }
        chancesChange->setText(chance_label+QString::number(chance,10));
        chancesChange->update();
        current=bubble.at(1);       // setting current to bubble.at(1) readies board for next shot and indicates delete function executed for previous bubble
    }
}

void Board::gameOver()
{
    qDebug("Game Over");
    game_over = true;
    pause = true;
    QMessageBox msg_box;
    msg_box.setText("Game Over!");
    msg_box.exec();

    // have user enter name for score, only if valid highscore file
    if (file_good)
    {
        name = "";
        bool ok = true;
        while (name=="" && ok)  // if nothing entered to name and confirm still clicked, window reappears asking for name again
        {
            name = QInputDialog::getText(0, tr("Record Score"),("Score: "+QString::number(score,10)+"\nEnter name:"), QLineEdit::Normal,QDir::home().dirName(), &ok);
        }

        if (name!="")   // name inputed (cancel was not clicked) so score will be recorded
        {
            QString short_name = "";
            if (name.size() > MAXCHAR)   // if name is longer than 10 characters, it must be shortened
            {
                for (int k=0; k<MAXCHAR; k++)
                    short_name.append(name[k]);
                qDebug() << short_name;
            }
            else
                short_name = name;
            qDebug("record score");
            addToFile(score, short_name);   // addToFile function will append entry to file
        }
    }

    restartGame();      // ready board for next game
}

void Board::addRow()
{
    qDebug("Add Row");
    // shift rows down (all bubbles y + 2*R)
    double row_stagger;
    for (int k=2; k<bubble.count(); k++)    // start at k=2 since first bubbles in list are next and on deck
    {
        if (bubble.at(k)->y() == T + Bubble::R)     // find the x value of the bubbles in the top list so offset of added row can be found
            row_stagger = bubble.at(k)->x();
        int y = bubble.at(k)->y() + (2*Bubble::R);
        int x = bubble.at(k)->x();
        bubble.at(k)->setPos(x,y);
    }
    // create another row
    row_stagger = (row_stagger-T)/Bubble::R;          // row_stagger now indicates how many R's to x position
    int y=T + Bubble::R;
    if ((int)row_stagger%2 != 0)                     // if number R's not even, new row will be offset
        for (int j=0; j<COLUMNS; j++)
        {
            int x = T + 2*Bubble::R + 2*Bubble::R*j;
            bubble.push_back(new Bubble(this, shoot_angle, 0));
            bubble.last()->setPos(x,y);
        }
    else                                            // else new row starts flush with left side of board
        for (int j=0; j<COLUMNS; j++)
        {
            int x = T + Bubble::R + 2*Bubble::R*j;
            bubble.push_back(new Bubble(this, shoot_angle, 0));
            bubble.last()->setPos(x,y);
        }

    // must change array cells to account for row addition (x values will add or subtract R depending)
    for (int k=0; k<H/(2*Bubble::R); k++)
    {
        int y = T + Bubble::R + 2*Bubble::R*k;
        for (int j=0; j<COLUMNS; j++)
        {
            if ((int)((cells[k][j].x()-T)/Bubble::R) %2 == 0)
            {
                int x = cells [k][j].x() - Bubble::R;
                QPoint p(x,y);
                cells[k][j] = p;
            }
            else
            {
                int x = cells[k][j].x() + Bubble::R;
                QPoint p(x,y);
                cells[k][j] = p;
            }
        }
    }
}

void Board::deleteIsolated()
{
    QList <Bubble *> connected;

    // add all top row bubbles to connected list since there is no way they will be isolated
    for (int k=0; k<bubble.count(); k++)
        if (bubble.at(k)->y() == T + Bubble::R)
            connected.append(bubble.at(k));

    // check around each connected bubble to find other bubbles that are also connected to board
    for (int j=0; j<connected.count(); j++)
    {
        for (int k=0; k<bubble.count(); k++)
            if ((bubble.at(k)->x() <= connected.at(j)->x()+(2*Bubble::R) && bubble.at(k)->x() >= connected.at(j)->x()-(2*Bubble::R)) && (bubble.at(k)->y() <= connected.at(j)->y()+(2*Bubble::R) && bubble.at(k)->y() >= connected.at(j)->y()-(2*Bubble::R)))
            {
                if (!connected.contains(bubble.at(k)))
                    connected.append(bubble.at(k));
            }
    }

    // find bubbles that arent in the connected list, they are the isolated bubbles that need to be deleted (add to deleteMe list)
    QList <Bubble *> delete_me;

    for (int k=2; k<bubble.count(); k++)
    {
            if (!connected.contains(bubble.at(k)))
                delete_me.append(bubble.at(k));
    }

    score += mult*20*delete_me.count();           // increase score by 20 for each bubble to be deleted
    scoreChange->setText(score_label+QString::number(score,10));
    scoreChange->update();

    // find isolated bubbles in bubble list using deleteMe list and delete
    for (int k=0; k<bubble.count(); k++)
    {
        if (delete_me.contains(bubble.at(k)))
        {
            delete bubble.at(k);
            bubble.removeAt(k);
            k--;
        }
    }
}

void Board::resetGame()
{
    qDebug("resetting game");
    score = 0;
    scoreChange->setText(score_label+QString::number(score,10));    // create label for score
    scoreChange->update();

    if (mode == "novice")
        chance = 5;
    else
        chance = 3;
    chancesChange->setText(chance_label+QString::number(chance,10));
    chancesChange->update();

    if (mode == "master")   // add timer to the board
    {
        timer = 5;
        t->setText("Timer: ");
        t->update();
        timerChange->setText(QString::number(timer,10));
        timerChange->update();
    }

    // set mode radio buttons to correct mode
    resetMode();

    // initialize the array cells to record the positions a bubble can occupy
    for (int k=0; k<H/(2*Bubble::R); k++)
    {
        int y = T + Bubble::R + 2*Bubble::R*k;
        if (k%2 == 0)
            for (int j=0; j<COLUMNS; j++)
            {
                int x = T + Bubble::R + 2*Bubble::R*j;
                QPoint p(x,y);
                cells[k][j] = p;
            }
        else
            for (int j=0; j<COLUMNS; j++)
            {
                int x = T + 2*Bubble::R + 2*Bubble::R*j;
                QPoint p(x,y);
                cells[k][j] = p;
            }
    }

    // create 9 rows of 17 bubbles to begin game
    for (int k=0; k<ROWS; k++)
    {
        int y=T + Bubble::R + 2*Bubble::R*k;
        if (k%2 == 0)
            for (int j=0; j<COLUMNS; j++)
            {
                int x = T + Bubble::R + 2*Bubble::R*j;
                bubble.push_front(new Bubble(this, shoot_angle, 0));
                bubble.first()->setPos(x,y);
            }
        else
            for (int j=0; j<COLUMNS; j++)
            {
                int x = T + 2*Bubble::R + 2*Bubble::R*j;
                bubble.push_front(new Bubble(this, shoot_angle, 0));
                bubble.first()->setPos(x,y);
            }
    }

    // create the next bubbles for game play
    bubble.push_front(new Bubble(this, shoot_angle, 0));
    bubble.first()->setPos(W+T+Bubble::R,H-Bubble::R);
    bubble.push_front(new Bubble(this, shoot_angle, 0));
    bubble.first()->setPos(W+T+3*Bubble::R,H-Bubble::R);
    current = bubble.at(1);
}

void Board::resetMode()
{
    // nov, exp, and mast point to the radio mode buttons and depending on the mode variable, the correct one will be selected
    nov->setChecked(mode == "novice");
    exp->setChecked(mode == "expert");
    mast->setChecked(mode == "master");
}

void Board::openFile()
{
    if (!file.exists())     // if file doesn't exist, empty one is created and reading silently skipped
    {
        qDebug("file DNE, creating file.");
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        file.close();
    }
    else
    {
        qDebug("read highscore file");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream t( &file ); // use a text stream
        QString line;
        while ( !t.atEnd() )    // loop until end of file
        {
            QString check_name;
            int check_score;
            line = t.readLine();
            if (line.isEmpty())      // if line is empty, skip over and proceed to check others
                continue;
            QStringList fields = line.split(':');
            check_name = fields.first();           // the first field will be the name
            check_score = fields.last().toInt(); // convert the second field, the score, to int and add to list
            all_names.append(fields.first());       // add the names and score to the end of their lists (same indexes!)
            all_scores.append(check_score);

            if (!checkEntry(check_score, check_name) || line.count(':') != 1) // checkEntry will return false if there is an issue with the score or name and count(":") ensures there is a :, and only 1, in the entry
            {
                file_good = false;
                break;
            }
        }

        // sort name and score lists now that all added
        for (int k=0; k<all_scores.size(); k++)
        {
            int min = k;
            for (int i=k+1; i<all_scores.size(); i++)
            {
                if (all_scores[i]<all_scores[min])
                    min = i;
            }
            all_scores.swap(k, min);        // when swapping scores, also swap names so correct scores and names are always together
            all_names.swap(k, min);
        }

        if (!file_good)     // display error message if file isn't properly formatted
        {
            QMessageBox warning;
            warning.setText("Error: Invalid Highscore File\n"+error);
            warning.exec();

            // remove file and create a new one
            file.remove();
            file.open(QIODevice::ReadWrite);
            file.close();

            // reset the all_scores and all_names lists
            all_scores.clear();
            all_names.clear();

            // file is now good
            file_good = true;
        }
        else        // otherwise display top ten scores
            topTen();
    }

    file.close();
}

void Board::addToFile(int score, QString name)
{
    // only add to file if it is valid and contains proper entries
    if (file_good)
    {
        if (checkEntry(score, name))
        {
            // add entry to file
            file.open(QIODevice::Append);
            QTextStream t(&file);
            t<<"\n"<<name<<":"<<score;
            file.close();

            // add name and score to all_names and all_scores lists in proper positions
            if (all_scores.size()==0)       // if there is nothing in the lists, just add the score and name
            {
                all_scores.append(score);
                all_names.append(name);
            }
            for (int k=0; k<all_scores.size(); k++)
            {
                if (score<all_scores[k])
                {
                    all_scores.insert(k, score);
                    all_names.insert(k, name);
                    break;
                }
            }
        }
    }
}

void Board::viewFile()
{
    if (file_good)
    {
        QString print_highscores = "Top Ten Scores!\n";
        int index = all_scores.size()-1;
        // if there are less than ten scores, you can only print as many as the list contains
        if (all_scores.size() < 10)
        {
            int loops = all_scores.size();      // use to count how many iterations of for loop to go through (size of all_scores)
            for (int k=1; k<=loops; k++)
            {
                QString rank = QString::number(k,10);
                QString print_score = QString::number(all_scores[index],10);
                QString add_string = rank+". "+all_names[index]+":"+print_score+"\n";
                print_highscores += add_string;
                index--;
            }
        }
        else
            for (int k=1; k<=10; k++)    // only add highest ten to print_highscores string
            {
                QString rank = QString::number(k,10);
                QString print_score = QString::number(all_scores[index],10);
                QString add_string = rank+". "+all_names[index]+":"+print_score+"\n";
                print_highscores += add_string;
                index--;
            }

        // Create message box that will display top ten scores
        QMessageBox top_ten;
        top_ten.setText(print_highscores);
        top_ten.exec();

    }
}

bool Board::checkEntry(int score, QString name)
{
    bool good = true;       // record whether entry is acceptable or not (changed to false during checks if necessary)

    // check if name in file is too long
    if (name.size() > MAXCHAR)
    {
        good = false;
        error = "Name is longer than 10 characters.";
    }
    // check if score contains anything other than 0-9
    QString check = QString::number(score,10);
    for (int i=0; i<check.size(); i++)
    {
        if (!(check[i] > 47 && check[i] < 58))
        {
            good = false;
            error = "Score contains non numeric characters.";
        }
    }
    // check if score is less than 0 or greater than 2147483647
    if (score < 0 || score > 2147483647)
    {
        good = false;
        error = "Score is out of range.";
    }

    return good;
}
