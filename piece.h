#ifndef PIECE_H
#define PIECE_H

#include <QMouseEvent>
#include <QtWidgets>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>


/*
Pieces are the chess pieces that players move. There are 6 unique pieces, and each player owns 16 total pieces.
*/

class Piece : public QObject, public QGraphicsPixmapItem //Note: QObject must be inherited first. Inherited for the use of slots and signals
{

    Q_OBJECT

public:

    Piece(); //Create an instance of the Piece class

    void set_coordinates_(int x, int y); //Set the x and y coordinates of the class AND utilize the setPos method in the Pixmap class
    void set_tile_coordinates(int row, int column); //Update the current tile that the piece is one

    int get_x_coordinate_(){return x_coordinate_; } //Get the x coordinate of the piece
    int get_y_coordinate_(){return y_coordinate_; } //Get the y coordinate of the piece

    int get_tile_row_(){return tile_row_; } //Get the row that the piece is on
    int get_tile_column_(){return tile_column_; } //Get the column that the piece is on

    virtual int get_team_(){return team_; } //Get the team number of the piece, 0 == white and 1 == black

    virtual int get_piece_number_(){return piece_number_; } //Get the piece number of the piece, 0 ==  pawn, 1 == bishop, 2 == knight, 3 == rook, 4 == queen, 5 == king

    //Functions overriden in the piece subclasses; used to update state from resting to selected and vice versa when being clicked and moved
    virtual void set_resting_image_(){};
    virtual void set_selected_image_(){};

    virtual int Captured(); //Move the piece off the screen so that it appears "Captured"; king return 1 when captured to alert end of game

signals:

    void PieceClick(Piece * clicked_piece, int piece_number); //Send a signal to the board to tell it what piece has been clicked and the number of that piece

private:

    int team_; //an identification for the team, 0 == white and 1 == black

    int piece_number_; //an identification for the piece, used for determining the type of piece within the board class, 0 ==  pawn, 1 == bishop, 2 == knight, 3 == rook, 4 == queen, 5 == king

    //Store the x and y coordinates of the piece
    int x_coordinate_;
    int y_coordinate_;

    //The row number and column number of the tile the Piece is on
    //Note: Both are 0 indexed, so 0 is first row (top), 7 is last row (bottom), etc
    int tile_row_;
    int tile_column_;

};


#endif // PIECE_H

