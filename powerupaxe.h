#ifndef POWERUPAXE_H
#define POWERUPAXE_H

#include "powerup.h"

/*
The PowerupAxe gives the player the ability to automatically capture any non-royal piece of the other player
*/
class PowerupAxe : public Powerup
{

        Q_OBJECT

public:
    PowerupAxe(); //Default constructor; set the Pixmap to an image of a boot

    PowerupAxe * Clone(); //Return a copied instance of the PowerupAxe

    int get_powerup_number_() override {return powerup_number_; } //Get the powerup_number_ for identification

    void mousePressEvent(QGraphicsSceneMouseEvent * /*unused*/) override; //Send a signal to the Board that the Powerup has been clicked

private:

    int powerup_number_ = 0; //An identification number for the powerup

    //Store the x and y coordinates of this powerup
    int x_coordinate_;
    int y_coordinate_;
};

#endif // POWERUPAXE_H
