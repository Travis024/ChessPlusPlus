#include "powerupshield.h"

PowerupShield::PowerupShield()
{
    //Set the image path
    QString image_path;
    image_path = R"(./img/powerup_shield.png)";

    //create an image object and scale it to be 60 by 60
    QImage * image_object = new QImage();
    image_object->load(image_path);
    QImage image_scaled = image_object->scaled(60,60);

    //create the Pixamp and set this powerup to that pixmap
    QPixmap final_pixmap = QPixmap::fromImage(image_scaled);
    setPixmap(QPixmap(final_pixmap));
}

/*
Returns a copyied insatnce of the PowerupShield

@param: None
@return: A pointer to a copied instance of the PowerupShield
*/
PowerupShield * PowerupShield::Clone(){
    return new PowerupShield();
}

/*
Send a signal to the board to alert it that the powerup has been clicked

@param: none
@return: emit a signal to the board
*/
void PowerupShield::mousePressEvent(QGraphicsSceneMouseEvent * /*usnused*/){
    emit PowerupClick(this, powerup_number_);
}
