#ifndef TILE_H
#define TILE_H

#include <QObject>
#include <QColor>
#include <QGraphicsItem>
#include <QtWidgets>


/*
 Tiles are the underlying elements of the board class. Not only do they make up the design of the board, but they also interact with pieces to help determine viable moves.
*/

class Tile : public QObject, public QGraphicsItem
{

    Q_OBJECT


public:

    Tile(int x, int y, int pattern); //Create a new instance of the Tile class
    Tile * Clone(); //Make a copy of a Tile instance

    //Override these functions so that the Tile can be created in a specific method for this program
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem */*unused*/, QWidget *widget) override;

    void set_x_coordinate_(int x){x_coordinate_ = x;  } //set the x coordinate of a tile
    void set_y_coordinate_(int y){y_coordinate_ = y; } //set the y coordinate of a tile

    void set_color_(int pattern); //set the color of a tile; 0 = brown, 1 = tan, and 2 = highlighted

    int get_x_coordinate(){return x_coordinate_; } //Return the x_coordinate_ of the Tile
    int get_y_coordinate(){return y_coordinate_; } //Return the y_coordinate_ of the Tile

    void set_white_occupied_(bool is_occupied){white_occupied_ = is_occupied; } //Set the value of the white_occupied_ field
    void set_black_occupied_(bool is_occupied){black_occupied_ = is_occupied; } //Set the value of the black_occupied_ field

    bool get_white_occupied_(){return white_occupied_; } //Return the value of the white_occupied_ field
    bool get_black_occupied_(){return black_occupied_; } //Return the value of the black_occupied_ field

    bool get_tile_is_highlighted(){return tile_is_highlighted_; } //Return whether or not the current tile is highlighted

    int get_previous_color_(){return previous_color_; } //Get the previous color of the tile so that it can be reset

    void mousePressEvent(QGraphicsSceneMouseEvent */*unused*/) override; //Emit a signal to that is received by the Board class

signals:

    void tileClick(Tile * selected_tile); //Signal emitted when any Tile is clicked, tied to a mousePressEvent

private:

    //Three possible colors for a tile
    //These colors are included for readability
    QColor color_;
    QColor tan_ = QColor(248,217,170);
    QColor brown_ = QColor(165, 79, 24);
    QColor highlighted_ = QColor(165, 241, 242);

    //Keep track of whether or not a tile is highlighted so that ir can be reset
    bool tile_is_highlighted_ = false;

    //Stores the old color of the Tile when it is highlighted so that it can revert; 0 == brown and 1 == tan
    int previous_color_;

    //X and Y coordinates of the Tile on the scene
    int x_coordinate_;
    int y_coordinate_;

    //Store whether or not the tile is occupied by a black or white piece
    bool white_occupied_;
    bool black_occupied_;

    //The board is 560 by 560, so 70 by 70 tiles give 8 rows and 8 columns
    int width_ = 70;
};

#endif // TILE_H
