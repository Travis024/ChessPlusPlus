#include "powerup.h"

Powerup::Powerup()
{

}

/*
Update the coordinate fields of the powerup AND utilize the Pixmap setPos function to update the position on the scene

@param: An x-coordianate and a y-coordinate
@return: The corrdinates are set and the position of the powerup on the scene is updated
*/
void Powerup::set_coordinates_(int x, int y){
    x_coordinate_ = x;
    y_coordinate_ = y;
    setPos(x_coordinate_, y_coordinate_);
}

