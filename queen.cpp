#include "queen.h"

/*
The constructor for the Queen class

@param: The team of the Queen
@return: Sets the team of the Queen and sets its two Pixmap fields
*/
Queen::Queen(int team)
{

    team_ = team;

    //Can either be selected or resting, and there is a different image for each
    //Each has a unique image path that must be set
    QString resting_image_path;
    QString selected_image_path;

    if(team == 0){
      resting_image_path = R"(./img/queen_white.png)";
      selected_image_path = R"(./img/queen_white_selected.png)";
    }else{
       resting_image_path = R"(./img/queen_black.png)";
       selected_image_path = R"(./img/queen_black_selected.png)";
    }

    QImage * resting_image_object = new QImage();
    QImage * selected_image_object = new QImage();

    resting_image_object->load(resting_image_path);
    selected_image_object->load(selected_image_path);

    //set the images to be 60 by 60
    QImage resting_image_scaled = resting_image_object->scaled(60,60);
    QImage selected_image_scaled = selected_image_object->scaled(60,60);

    this -> resting_image_ = QPixmap::fromImage(resting_image_scaled);
    this -> selected_image_ = QPixmap::fromImage(selected_image_scaled);

    setPixmap(QPixmap(resting_image_));
}

/*
Return a new Queen instance

@param: None
@return: Returns a new instance of the Queen class of the same team
*/
Queen * Queen::Clone(){
    return new Queen(this->team_);
}

/*
When the Queen is clicked, emit a signal that is received by a slot in the board
The slot will check to see if this is the user selecting a piece to move or attemtping to move a piece onto this Queen

@param: The instance of the Queen class that was clicked
@return: None
*/
void Queen::mousePressEvent(QGraphicsSceneMouseEvent */*unused*/){
    emit PieceClick(this, piece_number_);
}
