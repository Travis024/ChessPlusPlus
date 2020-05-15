#ifndef POWERUP_H
#define POWERUP_H

#include <QMouseEvent>
#include <QtWidgets>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>


/*
Powerups give players abilities outside of the normal moving of pieces. There are 3 unique powerups and 6 total powerups on the board.
*/

class Powerup : public QObject, public QGraphicsPixmapItem //Note: QObject must be inherited first. Inherited for the use of slots and signals
{

    Q_OBJECT

public:

    Powerup(); //Default constructor

    void set_coordinates_(int x, int y); //Set the x and y coordinate fields AND call the Pixmap -> setPost() function

    int get_x_coordinate_(){return x_coordinate_; } //Get the x-coordinate of the powerup
    int get_y_coordinate_(){return y_coordinate_; } //Get the y-coordinate of the powerup

    virtual int get_powerup_number_(){return powerup_number_; } //Get the powerup_number of the powerup, where 0 == powerup_axe, 1 == powerup_boot, 2 == powerup_shield

signals:

    void PowerupClick(Powerup * clicked_powerup, int powerup_number); //Send a signal to the board when a powerup is clicked

private:

    int powerup_number_; //An idenitifcation number for the powerup, where 0 == powerupaxe, 1 == powerupboot, 2 == powerupshield

    //Store the x and y coordinates of this powerup
    int x_coordinate_;
    int y_coordinate_;

};

#endif // POWERUP_H
