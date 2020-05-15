#ifndef POWERUPSHIELD_H
#define POWERUPSHIELD_H

#include "powerup.h"

/*
The PowerupShield gives the player the ability to automatically revive any of their captured pieces
*/
class PowerupShield : public Powerup
{

    Q_OBJECT

public:
    PowerupShield(); //Deafult constructor; sets the Pixmap to an image of a shield

    PowerupShield * Clone(); //Return a copied instance of the PowerupShield

    int get_powerup_number_() override {return powerup_number_; } //Get the powerup_number_ for identification

    void mousePressEvent(QGraphicsSceneMouseEvent * /*unused*/) override; //Send a signal to the Board that the Powerup has been clicked

private:

    int powerup_number_ = 2; //An identification number for the powerup

    //Store the x and y coordinates of this powerup
    int x_coordinate_;
    int y_coordinate_;
};

#endif // POWERUPSHIELD_H
