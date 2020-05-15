#include "piece.h"
#include <QDebug>

Piece::Piece(){

}


/*
Store the new location of a Piece AND update its position on the scene

@param: An x-coordinate and a y-coordinate
@return: Store the new location and update its position on the scene
*/
void Piece::set_coordinates_(int x, int y){
    x_coordinate_ = x;
    y_coordinate_ = y;
    this -> setPos(x_coordinate_, y_coordinate_);
}

/*
Set the row and column of the tile that the piece is currently on

@param: A row and column number
@return: The tile_row_ and tile_column fields in the class are set
*/
void Piece::set_tile_coordinates(int row, int column){
    tile_row_ = row;
    tile_column_ = column;
}

/*
Move the piece off of the screen so it appears captured
This works alongside a signal to update the "Captured" box of the player
All piece but the king reutn 0; king returns 1 to alert the end of the game
*/
int Piece::Captured(){
    this -> set_coordinates_(-100,-100);
    return 0;
}
