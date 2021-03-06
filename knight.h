#ifndef KNIGHT_H
#define KNIGHT_H

#include "piece.h"


class Knight : public Piece
{

     Q_OBJECT
public:
    Knight(int team); //Create a new instance of the Knight class
    Knight * Clone(); //Make a copy of a Knight instance

    int get_team_() override {return team_; } //return the team number

    int get_piece_number_() override {return piece_number_; }

    //used to update state from resting to selected and vice versa when being clicked and moved
    void set_resting_image_() override{this -> setPixmap(resting_image_); }
    void set_selected_image_() override{this -> setPixmap(selected_image_); }

    void mousePressEvent(QGraphicsSceneMouseEvent * /*unused*/) override; //Emit a signal to that is received by the Board class

private:

    int team_; //Team 0 is white, team 1 is black

    int piece_number_ = 2; //an identification for the number, used for determining the type of piece within the board class

    //Store the x and y coordinates of the Knight
    int x_coordinate_;
    int y_coordinate_;

    //The row number and column number of the tile the Knight is on
    //Note: Both are 0 indexed, so 0 is first row (top), 7 is last row (bottom), etc
    int tile_row_;
    int tile_column_;

    //Store the two images that the piece can take on
    QPixmap resting_image_;
    QPixmap selected_image_;


};

#endif // KNIGHT_H
