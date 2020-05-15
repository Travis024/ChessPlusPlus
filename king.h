#ifndef KING_H
#define KING_H

#include "piece.h"


class King : public Piece
{

     Q_OBJECT
public:
    King(int team); //Create a new instance of the King class
    King * Clone(); //Make a copy of a King instance

    int get_team_() override {return team_; } //return the team number

    int get_piece_number_() override {return piece_number_; } //return identification for the piece

    //used to update state from resting to selected and vice versa when being clicked and moved
    void set_resting_image_() override{this -> setPixmap(resting_image_); }
    void set_selected_image_() override{this -> setPixmap(selected_image_); }

    void mousePressEvent(QGraphicsSceneMouseEvent * /*unused*/) override; //Emit a signal to that is received by the Board class

    int Captured() override; //returns 1 to alert the end of the game;


private:

    int team_; //Team 0 is white, team 1 is black

    int piece_number_ = 5; //an identification for the piece, used for determining the type of piece within the board class

    //Store the x and y coordinates of the King
    int x_coordinate_;
    int y_coordinate_;

    //The row number and column number of the tile the King is on
    //Note: Both are 0 indexed, so 0 is first row (top), 7 is last row (bottom), etc
    int tile_row_;
    int tile_column_;

    //Store the two images that the piece can take on
    QPixmap resting_image_;
    QPixmap selected_image_;

};

#endif //KING_H
