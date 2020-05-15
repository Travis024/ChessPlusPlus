#ifndef POWERUPBOOT_H
#define POWERUPBOOT_H

#include "powerup.h"

/*
The PowerupBoot gives the player the ability to have two turns in a row
*/
class PowerupBoot : public Powerup
{

    Q_OBJECT

public:
    PowerupBoot(); //Default constructor; set the Pixmap to an image of a boot

    PowerupBoot * Clone(); //Return a copied instance of the PowerupBoot

    int get_powerup_number_() override {return powerup_number_; } //Get the powerup_number_ for identification

    void mousePressEvent(QGraphicsSceneMouseEvent * /*unused*/) override; //Send a signal to the Board that the Powerup has been clicked

private:

    int powerup_number_ = 1; //An identification number for the powerup

    //Store the x and y coordinates of this powerup
    int x_coordinate_;
    int y_coordinate_;
};

#endif // POWERUPBOOT_H
