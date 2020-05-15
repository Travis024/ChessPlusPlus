#include "tile.h"


/*
The constructor for the tile class

@param: An x-coordinate, a y-coordinate, and a patter indetifier
        0 == assign color to brown
        1 == assign color to tan
@return: A new instance of the tile class is created with the specified coordinates and color
*/
Tile::Tile(int x, int y, int pattern)
{
    x_coordinate_ = x;
    y_coordinate_ = y;

    if (pattern == 0)
        color_ = brown_;
    else
        color_ = tan_;

    //Ensures that the tiles show up behind the chess pieces
    setZValue(-1);
}

/*
Make a copy of a tile instance

@param: None
@return: A copy of the tile that calls the method is created
*/
Tile * Tile::Clone(){
    return new Tile(this -> x_coordinate_, this -> y_coordinate_, 0);
}

//Creates a rectangle around the Tile to let Qt know where to paint
QRectF Tile::boundingRect() const
{
    return QRectF(x_coordinate_, y_coordinate_, width_, width_);
}

//Tells Qt the shape to create when the addItem() function is called
QPainterPath Tile::shape() const
{
    QPainterPath path;
    path.addRect(x_coordinate_, y_coordinate_, width_, width_);
    return path;
}

//Tells Qt how to paint the Tile
void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem */*unused*/, QWidget *widget)
{
    Q_UNUSED(widget);


    QBrush b = painter->brush();
    painter->setBrush(QBrush(color_));

    painter->drawRect(QRect(this->x_coordinate_, y_coordinate_, this->width_, this->width_));
    painter->setBrush(b);
}

/*
Set the color of the tile
Used at initialization to create a chessboard pattern (0 = brown and 1 = tan)
Used when highlighting possible moves to show where a piece can be moved

Also sets the previous color of a tile so that the tile can rever to that color after is has been highlighted

@param: A number, where 0 = brown, 1 = tan, and 2  = highlighted
@return: The color of the Tile is set
*/
void Tile::set_color_(int pattern){
    if(pattern == 0){
        color_ = brown_;
        previous_color_ = 0;
        tile_is_highlighted_ = false;
    }else if(pattern == 1){
        color_ = tan_;
        previous_color_ = 1;
        tile_is_highlighted_ = false;
    }else{
        color_ = highlighted_;
        tile_is_highlighted_ = true;
    }
}


/*
When the user presses on an empty tile, emit a signal that is received by a slot in the board class

@param: The instance of the Tile class that has been clicked
@return: None
*/
void Tile::mousePressEvent(QGraphicsSceneMouseEvent */*unused*/){
    emit tileClick(this);
}

