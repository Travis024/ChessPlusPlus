#include "bishop.h"
#include <QDebug>

/*
The constructor for the Bishop class

@param: The team of the Bishop
@return: Sets the team of the Bishop and sets its two Pixmap fields
*/
Bishop::Bishop(int team)
{

    team_ = team;

    //Can either be selected or resting, and there is a different image for each
    //Each has a unique image path that must be set
    QString resting_image_path;
    QString selected_image_path;

    if(team == 0){
      resting_image_path = R"(./img/bishop_white.png)";
      selected_image_path = R"(./img/bishop_white_selected.png)";
    }else{
       resting_image_path = R"(./img/bishop_black.png)";
       selected_image_path = R"(./img/bishop_black_selected.png)";
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
Return a new Bishop instance

@param: None
@return: Returns a new instance of the Bishop class of the same team
*/
Bishop * Bishop::Clone(){
    return new Bishop(this->team_);
}

/*
When the Bishop is clicked, emit a signal that is received by a slot in the board
The slot will check to see if this is the user selecting a piece to move or attemtping to move a piece onto this Bishop

@param: The instance of the Bishop class that was clicked
@return: None
*/
void Bishop::mousePressEvent(QGraphicsSceneMouseEvent * /*unused*/){
    emit PieceClick(this, piece_number_);
}
