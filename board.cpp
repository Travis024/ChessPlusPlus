#include "board.h"
#include "QDebug"

/*
The defualt constructor for the board class
Initalizes the board with 64 tiles, 32 pieces (16 white, 16 black), and 6 powerups

@param: None
@return: The board has four vectors filled; one for its 64 tiles, one for its 16 white pieces, one for its 16 black pieces, and one for its 6 powerups
*/
Board::Board()
{
    InitializePlayers();
    InitializeTileVector();
    InitializePieceVectors();
    InitializePowerups();
    ConnectPiecesToBoard();
    ConnectPowerupsToBoard();
}

/*
Create two players and set the current_player_ to be player_one_

@param: None
@return: The board class has two players and the current_player_ is set to be player_one_
*/
void Board::InitializePlayers(){

    player_one_ = new Player(0);
    player_two_ = new Player(1);

    current_player_ = player_one_;
}

/*
A function to give the Board 64 total tiles in the form of eight 8-tile vectors
Utilizes the tile Clone method to acheive creation

@param: None
@returnL The board's tile_vector_ is initialized with 64 total tiles in the appropraite chessboard pattern
*/
void Board::InitializeTileVector(){

    //Each tile is 70 by 70
    int tile_width = 70;

    //Track the x position and y position of every tile initialized
    int x_pos = 0;
    int y_pos = 0;

    //The initial tile will be cloned for every following tile
    Tile * initial_tile_ = new Tile(x_pos, y_pos, 0);

    //Outer loop creates rows
    for(int i = 0; i < 8; i++){
        std::vector<Tile *> row_vector;

        //Reset so that tiles don't populate off-screen to the right
        x_pos = 0;

        //Inner loop creates columns
        for(int j = 0; j < 8; j++){

            //Create a new tile copy outside of the push_back call so that all signals and slots can be connected
            Tile * tile_to_create = initial_tile_ -> Clone();
            tile_to_create -> set_x_coordinate_(x_pos);
            tile_to_create -> set_y_coordinate_(y_pos);
            tile_to_create -> set_color_((i + j) % 2); //ensures the classic chess-board pattern of diagonal stripes

            //Update whether or not the tile is occupied by a piece
            //It is known that all tiles in rows 0 and 1 will be occupied by a black piece; all tiles in rows 6 and 7 by a white piece; and all other tiles not occupied by either
            if(i == 0 || i == 1){
                tile_to_create -> set_black_occupied_(true);
                tile_to_create -> set_white_occupied_(false);
            }else if(i == 6 || i == 7){
                tile_to_create -> set_black_occupied_(false);
                tile_to_create -> set_white_occupied_(true);
            }else{
                tile_to_create -> set_black_occupied_(false);
                tile_to_create -> set_white_occupied_(false);
            }

            connect(tile_to_create, &Tile::tileClick, this, &Board::tileClickSlot); //connect each new tile with the board

            row_vector.push_back(tile_to_create);
            x_pos += tile_width;
        }

        tile_vector_.push_back(row_vector);
        y_pos += tile_width;
    }
}

/*
A function to create 32 total pieces, 16 of which are stored in the white_alive_vector_ and 16 of which are stored in the black_alive_vector_
Utilizes the Clone methods of each piece to acheive this creation

This function also connects every piece's signals to the appropriate board slots
This must be completed BEFORE pieces are added to the Piece * vectors, as Qt will not allow casting from base class to derived classes in connect() calls

@param: None
@return: 32 pieces are created, 16 of which are white and 16 of which are black
         Every piece has its signals connected to the appropriate board slots
*/
void Board::InitializePieceVectors(){

    //Start by creating a white and black pawn and cloning each 7 times
    Pawn * white_pawn = new Pawn(0);
    Pawn * black_pawn = new Pawn(1);

    white_alive_vector_.push_back(white_pawn);
    black_alive_vector_.push_back(black_pawn);

    for(unsigned int i = 0; i < 7; i ++){
        Pawn * white_pawn_clone = white_pawn -> Clone();
        Pawn * black_pawn_clone = black_pawn -> Clone();

        white_alive_vector_.push_back(white_pawn_clone);
        black_alive_vector_.push_back(black_pawn_clone);
    }
    //Finish pawn creation

    //Create Bishops
    Bishop * white_bishop = new Bishop(0);
    Bishop * white_bishop_clone = white_bishop -> Clone();

    white_alive_vector_.push_back(white_bishop);
    white_alive_vector_.push_back(white_bishop_clone);

    Bishop * black_bishop = new Bishop (1);
    Bishop * black_bishop_clone = black_bishop -> Clone();

    black_alive_vector_.push_back(black_bishop);
    black_alive_vector_.push_back(black_bishop_clone);
    //Finish bishop creation

    //Create Knights
    Knight * white_knight = new Knight(0);
    Knight * white_knight_clone = white_knight -> Clone();

    white_alive_vector_.push_back(white_knight);
    white_alive_vector_.push_back(white_knight_clone);

    Knight * black_knight = new Knight(1);
    Knight * black_knight_clone = black_knight -> Clone();

    black_alive_vector_.push_back(black_knight);
    black_alive_vector_.push_back(black_knight_clone);
    //Finish knight creation

    //Create Rooks
    Rook * white_rook = new Rook(0);
    Rook * white_rook_clone = white_rook -> Clone();

    white_alive_vector_.push_back(white_rook);
    white_alive_vector_.push_back(white_rook_clone);

    Rook * black_rook = new Rook(1);
    Rook * black_rook_clone = black_rook -> Clone();

    black_alive_vector_.push_back(black_rook);
    black_alive_vector_.push_back(black_rook_clone);
    //Finish rook creation

    //Create Queens
    Queen * white_queen = new Queen(0);
    white_alive_vector_.push_back(white_queen);

    Queen * black_queen = new Queen(1);
    black_alive_vector_.push_back(black_queen);
    //Finish queen creation

    //Create Kings
    King * white_king = new King(0);
    white_alive_vector_.push_back(white_king);

    King * black_king = new King(1);
    black_alive_vector_.push_back(black_king);
    //Finish king creation

}

/*
Intiailize the board to own six total powerups - two of each type

@param: None
@return; The powerup_vector is filled with two of each powerup
*/
void Board::InitializePowerups(){
    PowerupAxe * axe = new PowerupAxe();
    PowerupAxe * axe_clone = axe -> Clone();

    PowerupBoot * boot = new PowerupBoot();
    PowerupBoot * boot_clone = boot -> Clone();

    PowerupShield * shield = new PowerupShield();
    PowerupShield * shield_clone = shield -> Clone();

    powerup_vector_.push_back(axe);
    powerup_vector_.push_back(axe_clone);
    powerup_vector_.push_back(boot);
    powerup_vector_.push_back(boot_clone);
    powerup_vector_.push_back(shield);
    powerup_vector_.push_back(shield_clone);
}

/*
Connect each powerup's signals to the board's slots

@param: None
@return: Signals and slots are connected
*/
void Board::ConnectPowerupsToBoard(){
    for(unsigned int i = 0; i < powerup_vector_.size(); i++){
        connect(powerup_vector_[i], &Powerup::PowerupClick, this, &Board::PowerupClickSlot);
    }
}

/*
Connect each piece's signals to the board's slots

@param: None
@return: Signals and slots are connected
*/
void Board::ConnectPiecesToBoard(){

    for(unsigned int i = 0; i < white_alive_vector_.size(); i++){
        connect(white_alive_vector_[i], &Piece::PieceClick, this, &Board::PieceClickSlot);
        connect(black_alive_vector_[i], &Piece::PieceClick, this, &Board::PieceClickSlot);
    }

}

/*
After a piece has been moved, unhighlight all the pieces that were highlighted as possible moves

@param: None
@return: The color of the tile is set to its original color
*/
void Board::Unhighlight(){
    for(unsigned int i = 0; i < tile_vector_.size(); i++){
        for(unsigned int j = 0; j < tile_vector_[i].size(); j++){
            if(tile_vector_[i][j] -> get_tile_is_highlighted()){
                tile_vector_[i][j] -> set_color_(tile_vector_[i][j] -> get_previous_color_());
                tile_vector_[i][j] -> update();
            }
        }
    }
}

/*
Update the occpied fields of the tile at the given row and column
Used when a piece is captured (occupied fields flip) or a piece moves onto an empty tile( (tile becomes occupied with piece color)

@param: The tile row and column, a boolean value for each white occupation and black occupation
@retun: The occupied fields of the tile are set
*/
void Board::ChangeTileOccupied(int tile_row, int tile_column, bool is_white_occupied, bool is_black_occupied){
    tile_vector_[tile_row][tile_column] -> set_white_occupied_(is_white_occupied);
    tile_vector_[tile_row][tile_column] -> set_black_occupied_(is_black_occupied);
}

/*
When the 'Start New Game' button is pressed, reset the occupied fields of the tiles to their initial states
This is where rows 0 and 1 are occupied by black pieces, rows 6 and 7 by white pieces, and all other rows not occupied by either

@param: None
@return: The occupied fields of every tile in the board are reset
*/
void Board::ResetOccupiedToInitial(){
    for(unsigned int i = 0; i < tile_vector_.size(); i++){
        for(unsigned int j = 0; j < tile_vector_.size(); j++){
            if(i == 0 || i == 1){
                tile_vector_[i][j] -> set_black_occupied_(true);
                tile_vector_[i][j] -> set_white_occupied_(false);
            }else if(i == 6 || i == 7){
                tile_vector_[i][j] -> set_black_occupied_(false);
                tile_vector_[i][j] -> set_white_occupied_(true);
            }else{
                tile_vector_[i][j] -> set_black_occupied_(false);
                tile_vector_[i][j] -> set_white_occupied_(false);
            }
        }
    }
}

/*
Revive a piece when the shield powerup has been used and the player has clicked on an empty tile
Go through the player's vector of pieces, find a captured piece of the appropriate type, and reset its coordinates

@param: The x and y coordinates of the tile that the player selected
*/
void Board::RevivePiece(int tile_x, int tile_y){

    //Get the appropraite vector, where player 1 is white pieces and player 2 is black pieces
    std::vector<Piece *>piece_vector;

    if(current_player_ -> get_player_number_() == 0){
        piece_vector = get_white_alive_vector_();
    }else{
        piece_vector = get_black_alive_vector_();
    }

    int start_index;
    int end_index;
    int x_offset = 5;
    int y_offset = 10;

    //Set the starting and ending index for searching through the pieces
    //0 - 7 is pawns, 8-9 is bishops, 10-11 is knights, 12-13 is rooks, 14 is queen
    //15 will never be reached; the game ends when the king is captured
    if(selected_captured_piece_ == 0){
        start_index = 0;
        end_index = 7;
    }else if(selected_captured_piece_ == 1){
        start_index = 8;
        end_index = 9;
    }else if(selected_captured_piece_ == 2){
        start_index = 10;
        end_index = 11;
    }else if(selected_captured_piece_ == 3){
        start_index = 12;
        end_index = 13;
    }else{
        start_index = 14;
        end_index = 15;
    }

    //A captured piece is one that has coordinates of -100,-100.
    //When one piece is found of the approriate tile, reset its postion to the clicked tile and offset so it is centered.
    for(int i = start_index ; i <= end_index; i++){
        if(piece_vector[i] -> get_x_coordinate_() == -100){
            piece_vector[i] -> set_coordinates_(tile_x + x_offset, tile_y + y_offset);
            piece_vector[i] -> set_tile_coordinates(tile_y / 70, tile_x / 70); //Each tile is 70 by 70, so dividing by 70 gives the row and column
            break;
        }
    }

}

/*
Change the current_player_ after a turn has been completed and update the grandmaster label to show which player's turn it is
If start_new_game is true, then automatically set the current_player_ to player_one_

@param: None
@return: current_player_ changes from player_one_ to player_two_ or vice versa; or, to player_one if starting a new game
*/
void Board::ChangeCurrentPlayer(bool start_new_game){

    if(start_new_game){
        current_player_ = player_one_;
    }else{
        if(current_player_ == player_one_){
            current_player_ = player_two_;
            emit UpdateGrandmaster(9);
        }else{
            current_player_ = player_one_;
            emit UpdateGrandmaster(8);
        }
    }
}

/*
When a king is captured, update the grandmaster label and remove all losing pieces and powerups from the board
This just leaves the remaining pieces for the winning team

@param: The winning player
@return: The grandmaster label updates and all losing pieces and powerups are removed from the board
*/
void Board::EndGame(Player *player){

    if(player -> get_player_number_() == 0){
        emit UpdateGrandmaster(13);
    }else{
        emit UpdateGrandmaster(14);
    }

    //Determine the vector of pieces to clear based on the wimnning player
    for(unsigned int i = 0; i < white_alive_vector_.size(); i++){
        if(player -> get_player_number_() == 0){
            black_alive_vector_[i]->Captured();
        }else{
            white_alive_vector_[i]->Captured();
        }
    }

    //If any powerups remain, remove them from the board
    for(unsigned int i = 0; i < powerup_vector_.size(); i++){
        powerup_vector_[i] -> set_coordinates_(-100,-100);
    }

    //Reset the selected_piece and team to help set up the next game
    selected_piece_ = NULL;
    selected_piece_team_ = -1;
}

/*
Highlight all tiles that are in valid locations for the selected piece to move to

@param: The piece's row, column, team, and number
@return: Depending on the piece's team and type, all valid tiles are highlighted
*/
void Board::ValidMoves(int piece_row, int piece_column, int piece_team, int piece_number){

    //Valid moves for a pawn
    //A pawn can move forward one or two spaces on their first turn
    //Following that, they may only move one space forward OR capture one space diagonal
    if(piece_number == 0){

        //Start by checing white pawns
        if(piece_team == 0){

                //Can move forward one
                if(piece_row - 1 >= 0 && tile_vector_[piece_row - 1][piece_column] -> get_white_occupied_() == false && tile_vector_[piece_row - 1][piece_column] -> get_black_occupied_() == false){
                    tile_vector_[piece_row - 1][piece_column] -> set_color_(2);
                    tile_vector_[piece_row - 1][piece_column] -> update();

                   //Check for this being the first move, in which case can move forward two spaces
                    if(piece_row == 6){

                        //Cannot move through pieces, so the first tile must be empty for the second one to be checked
                        if(piece_row - 2 >= 0 && tile_vector_[piece_row - 2][piece_column] -> get_white_occupied_() == false && tile_vector_[piece_row - 2][piece_column] -> get_black_occupied_() == false){
                            tile_vector_[piece_row - 2][piece_column] -> set_color_(2);
                            tile_vector_[piece_row - 2][piece_column] -> update();
                        }
                    }
                }

                //Check diagonal captures, meaning there must be a black piece diagonally forward one
                if(piece_row - 1 >= 0 && piece_column - 1 >= 0 && tile_vector_[piece_row - 1][piece_column - 1] -> get_black_occupied_() == true){
                    tile_vector_[piece_row - 1][piece_column - 1] -> set_color_(2);
                    tile_vector_[piece_row - 1][piece_column - 1] -> update();
                }

                //Check diagonal captures, meaning there must be a black piece diagonally forward one
                if(piece_row - 1 >= 0 && piece_column + 1 <= 7 && tile_vector_[piece_row - 1][piece_column + 1] -> get_black_occupied_() == true){
                    tile_vector_[piece_row - 1][piece_column + 1] -> set_color_(2);
                    tile_vector_[piece_row - 1][piece_column + 1] -> update();
                }
            }

        //Make the same checks for black pawns, but now initial move is on row 1 and rows are ADDITION
        else{

            //Can move forward one
            if(piece_row + 1 <= 7 && tile_vector_[piece_row + 1][piece_column] -> get_white_occupied_() == false && tile_vector_[piece_row + 1][piece_column] -> get_black_occupied_() == false){
                tile_vector_[piece_row + 1][piece_column] -> set_color_(2);
                tile_vector_[piece_row + 1][piece_column] -> update();

               //Check for this being the first move, in which case can move forward two spaces
                if(piece_row == 1){

                    //Cannot move through pieces, so the first tile must be empty for the second one to be checked
                    if(piece_row + 2 <= 7 && tile_vector_[piece_row + 2][piece_column] -> get_white_occupied_() == false && tile_vector_[piece_row + 2][piece_column] -> get_black_occupied_() == false){
                        tile_vector_[piece_row + 2][piece_column] -> set_color_(2);
                        tile_vector_[piece_row + 2][piece_column] -> update();
                    }
                }
            }

            //Check diagonal captures, meaning there must be a black piece diagonally forward one
            if(piece_row + 1 <= 7 && piece_column - 1 >= 0 && tile_vector_[piece_row + 1][piece_column - 1] -> get_white_occupied_() == true){
                tile_vector_[piece_row + 1][piece_column - 1] -> set_color_(2);
                tile_vector_[piece_row + 1][piece_column - 1] -> update();
            }

            //Check diagonal captures, meaning there must be a black piece diagonally forward one
            if(piece_row + 1 <= 7 && piece_column + 1 <= 7 && tile_vector_[piece_row + 1][piece_column + 1] -> get_white_occupied_() == true){
                tile_vector_[piece_row + 1][piece_column + 1] -> set_color_(2);
                tile_vector_[piece_row + 1][piece_column + 1] -> update();
            }
        }
    }

    //Valid moves for a bishop AND a queen (since a queen has the same diagonal components)
    //Must check diagonal in every direction, meaning row++ & col++; row-- & col--; row++ & col--; row-- & col++
    //Bisop cannot go through pieces, so upon reaching ANY piece it must stop checking in that directio
    if(piece_number == 1 || piece_number == 4){

        //For each while loop, the counters are set so as not to check the tile itself
        int diagonal_row_counter = piece_row + 1;
        int diagonal_column_counter = piece_column + 1;

        while(diagonal_row_counter <= 7 && diagonal_column_counter <= 7){

            //The first check is for a black bishop captruing a white piece
            if(tile_vector_[diagonal_row_counter][diagonal_column_counter] -> get_white_occupied_() == true && piece_team == 1){
                tile_vector_[diagonal_row_counter][diagonal_column_counter] -> set_color_(2);
                tile_vector_[diagonal_row_counter][diagonal_column_counter] -> update();
                break;
            }

            //The next check is for a white bishop capturing a black piece
            else if(tile_vector_[diagonal_row_counter][diagonal_column_counter] -> get_black_occupied_() == true && piece_team == 0){
                tile_vector_[diagonal_row_counter][diagonal_column_counter] -> set_color_(2);
                tile_vector_[diagonal_row_counter][diagonal_column_counter] -> update();
                break;
            }

            //The next check is for any empty tile
            else if(tile_vector_[diagonal_row_counter][diagonal_column_counter] -> get_white_occupied_() == false && tile_vector_[diagonal_row_counter][diagonal_column_counter] -> get_black_occupied_() == false){
                tile_vector_[diagonal_row_counter][diagonal_column_counter] -> set_color_(2);
                tile_vector_[diagonal_row_counter][diagonal_column_counter] -> update();
            }

            //The next check is for a white bishop encountering a white piece
            else if(tile_vector_[diagonal_row_counter][diagonal_column_counter] -> get_white_occupied_() == true && piece_team == 0){
                break;
            }

            //The final check is for a black bishop encountering a black piece
            else if(tile_vector_[diagonal_row_counter][diagonal_column_counter] -> get_black_occupied_() == true && piece_team == 1){
                break;
            }

            diagonal_row_counter++;
            diagonal_column_counter++;
        }

        //Repeat the same process as laid out above three more times but in different directions
        diagonal_row_counter = piece_row - 1;
        diagonal_column_counter = piece_column - 1;

        while(diagonal_row_counter >= 0 && diagonal_column_counter >= 0){
            if(tile_vector_[diagonal_row_counter][diagonal_column_counter] -> get_white_occupied_() == true && piece_team == 1){
                tile_vector_[diagonal_row_counter][diagonal_column_counter] -> set_color_(2);
                tile_vector_[diagonal_row_counter][diagonal_column_counter] -> update();
                break;
            }else if(tile_vector_[diagonal_row_counter][diagonal_column_counter] -> get_black_occupied_() == true && piece_team == 0){
                tile_vector_[diagonal_row_counter][diagonal_column_counter] -> set_color_(2);
                tile_vector_[diagonal_row_counter][diagonal_column_counter] -> update();
                break;
            }else if(tile_vector_[diagonal_row_counter][diagonal_column_counter] -> get_white_occupied_() == false && tile_vector_[diagonal_row_counter][diagonal_column_counter] -> get_black_occupied_() == false){
                tile_vector_[diagonal_row_counter][diagonal_column_counter] -> set_color_(2);
                tile_vector_[diagonal_row_counter][diagonal_column_counter] -> update();
            }else if(tile_vector_[diagonal_row_counter][diagonal_column_counter] -> get_white_occupied_() == true && piece_team == 0){
                break;
            }else if(tile_vector_[diagonal_row_counter][diagonal_column_counter] -> get_black_occupied_() == true && piece_team == 1){
                break;
            }

            diagonal_row_counter--;
            diagonal_column_counter--;
        }

        diagonal_row_counter = piece_row - 1;
        diagonal_column_counter = piece_column + 1;

        while(diagonal_row_counter >= 0 && diagonal_column_counter <= 7){
            if(tile_vector_[diagonal_row_counter][diagonal_column_counter] -> get_white_occupied_() == true && piece_team == 1){
                tile_vector_[diagonal_row_counter][diagonal_column_counter] -> set_color_(2);
                tile_vector_[diagonal_row_counter][diagonal_column_counter] -> update();
                break;
            }else if(tile_vector_[diagonal_row_counter][diagonal_column_counter] -> get_black_occupied_() == true && piece_team == 0){
                tile_vector_[diagonal_row_counter][diagonal_column_counter] -> set_color_(2);
                tile_vector_[diagonal_row_counter][diagonal_column_counter] -> update();
                break;
            }else if(tile_vector_[diagonal_row_counter][diagonal_column_counter] -> get_white_occupied_() == false && tile_vector_[diagonal_row_counter][diagonal_column_counter] -> get_black_occupied_() == false){
                tile_vector_[diagonal_row_counter][diagonal_column_counter] -> set_color_(2);
                tile_vector_[diagonal_row_counter][diagonal_column_counter] -> update();
            }else if(tile_vector_[diagonal_row_counter][diagonal_column_counter] -> get_white_occupied_() == true && piece_team == 0){
                break;
            }else if(tile_vector_[diagonal_row_counter][diagonal_column_counter] -> get_black_occupied_() == true && piece_team == 1){
                break;
            }

            diagonal_row_counter--;
            diagonal_column_counter++;
        }

        diagonal_row_counter = piece_row + 1;
        diagonal_column_counter = piece_column - 1;

        while(diagonal_row_counter <= 7 && diagonal_column_counter >= 0){
            if(tile_vector_[diagonal_row_counter][diagonal_column_counter] -> get_white_occupied_() == true && piece_team == 1){
                tile_vector_[diagonal_row_counter][diagonal_column_counter] -> set_color_(2);
                tile_vector_[diagonal_row_counter][diagonal_column_counter] -> update();
                break;
            }else if(tile_vector_[diagonal_row_counter][diagonal_column_counter] -> get_black_occupied_() == true && piece_team == 0){
                tile_vector_[diagonal_row_counter][diagonal_column_counter] -> set_color_(2);
                tile_vector_[diagonal_row_counter][diagonal_column_counter] -> update();
                break;
            }else if(tile_vector_[diagonal_row_counter][diagonal_column_counter] -> get_white_occupied_() == false && tile_vector_[diagonal_row_counter][diagonal_column_counter] -> get_black_occupied_() == false){
                tile_vector_[diagonal_row_counter][diagonal_column_counter] -> set_color_(2);
                tile_vector_[diagonal_row_counter][diagonal_column_counter] -> update();
            }else if(tile_vector_[diagonal_row_counter][diagonal_column_counter] -> get_white_occupied_() == true && piece_team == 0){
                break;
            }else if(tile_vector_[diagonal_row_counter][diagonal_column_counter] -> get_black_occupied_() == true && piece_team == 1){
                break;
            }

            diagonal_row_counter++;
            diagonal_column_counter--;
        }

    }

    //Valid moves for a knight
    //A knight has eight possible moves; up 2 over 1 (either direction), down 2 over 1(either directon), two left up/down, two right up/down
    //The knoght can go through pieces, so the only thing that matters is that the tile it wants to move to is empty or of a different color
    if(piece_number == 2){

        //Each possible moves has three components; check for black piece moving on white piece, white piece moving on black piece, or either colored piece moving on empty tile

        //Check up two, left one
        if(piece_row + 2 <= 7 && piece_column - 1 >= 0 && tile_vector_[piece_row + 2][piece_column - 1] -> get_white_occupied_() && piece_team == 1){
            tile_vector_[piece_row + 2][piece_column - 1] -> set_color_(2);
            tile_vector_[piece_row + 2][piece_column - 1] -> update();
        }

        if(piece_row + 2 <= 7 && piece_column - 1 >= 0 && tile_vector_[piece_row + 2][piece_column - 1] -> get_black_occupied_() && piece_team == 0){
            tile_vector_[piece_row + 2][piece_column - 1] -> set_color_(2);
            tile_vector_[piece_row + 2][piece_column - 1] -> update();
        }

        if(piece_row + 2 <= 7 && piece_column - 1 >= 0 && tile_vector_[piece_row + 2][piece_column - 1] -> get_white_occupied_() == false && tile_vector_[piece_row + 2][piece_column - 1] -> get_black_occupied_() == false){
            tile_vector_[piece_row + 2][piece_column - 1] -> set_color_(2);
            tile_vector_[piece_row + 2][piece_column - 1] -> update();
        }

        //Check up two, right one
        if(piece_row + 2 <= 7 && piece_column + 1 <= 7 && tile_vector_[piece_row + 2][piece_column + 1] -> get_white_occupied_() && piece_team == 1){
            tile_vector_[piece_row + 2][piece_column + 1] -> set_color_(2);
            tile_vector_[piece_row + 2][piece_column + 1] -> update();
        }

        if(piece_row + 2 <= 7 && piece_column + 1 <= 7 && tile_vector_[piece_row + 2][piece_column + 1] -> get_black_occupied_() && piece_team == 0){
            tile_vector_[piece_row + 2][piece_column + 1] -> set_color_(2);
            tile_vector_[piece_row + 2][piece_column + 1] -> update();
        }

        if(piece_row + 2 <= 7 && piece_column + 1 <= 7 && tile_vector_[piece_row + 2][piece_column + 1] -> get_white_occupied_() == false && tile_vector_[piece_row + 2][piece_column + 1] -> get_black_occupied_() == false){
            tile_vector_[piece_row + 2][piece_column + 1] -> set_color_(2);
            tile_vector_[piece_row + 2][piece_column + 1] -> update();
        }

        //Check down two, left one
        if(piece_row - 2 >= 0 && piece_column - 1 >= 0 && tile_vector_[piece_row - 2][piece_column - 1] -> get_white_occupied_() && piece_team == 1){
            tile_vector_[piece_row - 2][piece_column - 1] -> set_color_(2);
            tile_vector_[piece_row - 2][piece_column - 1] -> update();
        }

        if(piece_row - 2 >= 0 && piece_column - 1 >= 0 && tile_vector_[piece_row - 2][piece_column - 1] -> get_black_occupied_() && piece_team == 0){
            tile_vector_[piece_row - 2][piece_column - 1] -> set_color_(2);
            tile_vector_[piece_row - 2][piece_column - 1] -> update();
        }

        if(piece_row - 2 >= 0 && piece_column - 1 >= 0 && tile_vector_[piece_row - 2][piece_column - 1] -> get_white_occupied_() == false && tile_vector_[piece_row - 2][piece_column - 1] -> get_black_occupied_() == false){
            tile_vector_[piece_row - 2][piece_column - 1] -> set_color_(2);
            tile_vector_[piece_row - 2][piece_column - 1] -> update();
        }

        //Check down two, right one
        if(piece_row - 2 >= 0 && piece_column + 1 <= 7 && tile_vector_[piece_row - 2][piece_column + 1] -> get_white_occupied_() && piece_team == 1){
            tile_vector_[piece_row - 2][piece_column + 1] -> set_color_(2);
            tile_vector_[piece_row - 2][piece_column + 1] -> update();
        }

        if(piece_row - 2 >= 0 && piece_column + 1 <= 7 && tile_vector_[piece_row - 2][piece_column + 1] -> get_black_occupied_() && piece_team == 0){
            tile_vector_[piece_row - 2][piece_column + 1] -> set_color_(2);
            tile_vector_[piece_row - 2][piece_column + 1] -> update();
        }

        if(piece_row - 2 >= 0 && piece_column + 1 <= 7 && tile_vector_[piece_row - 2][piece_column + 1] -> get_white_occupied_() == false && tile_vector_[piece_row - 2][piece_column + 1] -> get_black_occupied_() == false){
            tile_vector_[piece_row - 2][piece_column + 1] -> set_color_(2);
            tile_vector_[piece_row - 2][piece_column + 1] -> update();
        }

        //Check right two, up one
        if(piece_row + 1 <= 7 && piece_column + 2 <= 7 && tile_vector_[piece_row + 1][piece_column + 2] -> get_white_occupied_() && piece_team == 1){
            tile_vector_[piece_row + 1][piece_column + 2] -> set_color_(2);
            tile_vector_[piece_row + 1][piece_column + 2] -> update();
        }

        if(piece_row + 1 <= 7 && piece_column + 2 <= 7 && tile_vector_[piece_row + 1][piece_column + 2] -> get_black_occupied_() && piece_team == 0){
            tile_vector_[piece_row + 1][piece_column + 2] -> set_color_(2);
            tile_vector_[piece_row + 1][piece_column + 2] -> update();
        }

        if(piece_row + 1 <= 7 && piece_column + 2 <= 7 && tile_vector_[piece_row + 1][piece_column + 2] -> get_white_occupied_() == false && tile_vector_[piece_row + 1][piece_column + 2] -> get_black_occupied_() == false){
            tile_vector_[piece_row + 1][piece_column + 2] -> set_color_(2);
            tile_vector_[piece_row + 1][piece_column + 2] -> update();
        }

        //Check right two, down one
        if(piece_row - 1 >= 0 && piece_column + 2 <= 7 && tile_vector_[piece_row - 1][piece_column + 2] -> get_white_occupied_() && piece_team == 1){
            tile_vector_[piece_row - 1][piece_column + 2] -> set_color_(2);
            tile_vector_[piece_row - 1][piece_column + 2] -> update();
        }

        if(piece_row - 1 >= 0 && piece_column + 2 <= 7 && tile_vector_[piece_row - 1][piece_column + 2] -> get_black_occupied_() && piece_team == 0){
            tile_vector_[piece_row - 1][piece_column + 2] -> set_color_(2);
            tile_vector_[piece_row - 1][piece_column + 2] -> update();
        }

        if(piece_row - 1 >= 0 && piece_column + 2 <= 7 && tile_vector_[piece_row - 1][piece_column + 2] -> get_white_occupied_() == false && tile_vector_[piece_row - 1][piece_column + 2] -> get_black_occupied_() == false){
            tile_vector_[piece_row - 1][piece_column + 2] -> set_color_(2);
            tile_vector_[piece_row - 1][piece_column + 2] -> update();
        }

        //Check left two, up one
        if(piece_row + 1 <= 7 && piece_column - 2 >= 0 && tile_vector_[piece_row + 1][piece_column - 2] -> get_white_occupied_() && piece_team == 1){
            tile_vector_[piece_row + 1][piece_column - 2] -> set_color_(2);
            tile_vector_[piece_row + 1][piece_column - 2] -> update();
        }

        if(piece_row + 1 <= 7 && piece_column - 2 >= 0 && tile_vector_[piece_row + 1][piece_column - 2] -> get_black_occupied_() && piece_team == 0){
            tile_vector_[piece_row + 1][piece_column - 2] -> set_color_(2);
            tile_vector_[piece_row + 1][piece_column - 2] -> update();
        }

        if(piece_row + 1 <= 7 && piece_column - 2 >= 0 && tile_vector_[piece_row + 1][piece_column - 2] -> get_white_occupied_() == false && tile_vector_[piece_row + 1][piece_column - 2] -> get_black_occupied_() == false){
            tile_vector_[piece_row + 1][piece_column - 2] -> set_color_(2);
            tile_vector_[piece_row + 1][piece_column - 2] -> update();
        }

        //Check left two, down one
        if(piece_row - 1 >= 0 && piece_column - 2 >= 0 && tile_vector_[piece_row - 1][piece_column - 2] -> get_white_occupied_() && piece_team == 1){
            tile_vector_[piece_row - 1][piece_column - 2] -> set_color_(2);
            tile_vector_[piece_row - 1][piece_column - 2] -> update();
        }

        if(piece_row - 1 >= 0 && piece_column - 2 >= 0 && tile_vector_[piece_row - 1][piece_column - 2] -> get_black_occupied_() && piece_team == 0){
            tile_vector_[piece_row - 1][piece_column - 2] -> set_color_(2);
            tile_vector_[piece_row - 1][piece_column - 2] -> update();
        }

        if(piece_row - 1 >= 0 && piece_column - 2 >= 0 && tile_vector_[piece_row - 1][piece_column - 2] -> get_white_occupied_() == false && tile_vector_[piece_row - 1][piece_column - 2] -> get_black_occupied_() == false){
            tile_vector_[piece_row - 1][piece_column - 2] -> set_color_(2);
            tile_vector_[piece_row - 1][piece_column - 2] -> update();
        }
    }

    //Valid moves for a rook AND a queen (since queen as the same vertical and horizontal components)
    //Rook can move horizontally or vertically in either direction (4 total possibiliites)
    //Rook cannot move through other pieces
    if(piece_number == 3 || piece_number == 4){

        //For each while loop, the counters are set so as not to check the tile itself
        int vertical_counter = piece_row + 1;
        int horizontal_counter = piece_column;

        while(vertical_counter <= 7){

            //The first check is for a black rook captruing a white piece
            if(tile_vector_[vertical_counter][horizontal_counter] -> get_white_occupied_() == true && piece_team == 1){
                tile_vector_[vertical_counter][horizontal_counter] -> set_color_(2);
                tile_vector_[vertical_counter][horizontal_counter] -> update();
                break;
            }

            //The next check is for a white rook capturing a black piece
            else if(tile_vector_[vertical_counter][horizontal_counter] -> get_black_occupied_() == true && piece_team == 0){
                tile_vector_[vertical_counter][horizontal_counter] -> set_color_(2);
                tile_vector_[vertical_counter][horizontal_counter] -> update();
                break;
            }

            //The next check is for any empty tile
            else if(tile_vector_[vertical_counter][horizontal_counter] -> get_white_occupied_() == false && tile_vector_[vertical_counter][horizontal_counter] -> get_black_occupied_() == false){
                tile_vector_[vertical_counter][horizontal_counter] -> set_color_(2);
                tile_vector_[vertical_counter][horizontal_counter] -> update();
            }

            //The next check is for a white rook encountering a white piece
            else if(tile_vector_[vertical_counter][horizontal_counter] -> get_white_occupied_() == true && piece_team == 0){
                break;
            }

            //The final check is for a black rook encountering a black piece
            else if(tile_vector_[vertical_counter][horizontal_counter] -> get_black_occupied_() == true && piece_team == 1){
                break;
            }

            vertical_counter++;
        }

        vertical_counter = piece_row - 1;
        horizontal_counter = piece_column;

        while(vertical_counter >= 0){

            //The first check is for a black rook captruing a white piece
            if(tile_vector_[vertical_counter][horizontal_counter] -> get_white_occupied_() == true && piece_team == 1){
                tile_vector_[vertical_counter][horizontal_counter] -> set_color_(2);
                tile_vector_[vertical_counter][horizontal_counter] -> update();
                break;
            }

            //The next check is for a white rook capturing a black piece
            else if(tile_vector_[vertical_counter][horizontal_counter] -> get_black_occupied_() == true && piece_team == 0){
                tile_vector_[vertical_counter][horizontal_counter] -> set_color_(2);
                tile_vector_[vertical_counter][horizontal_counter] -> update();
                break;
            }

            //The next check is for any empty tile
            else if(tile_vector_[vertical_counter][horizontal_counter] -> get_white_occupied_() == false && tile_vector_[vertical_counter][horizontal_counter] -> get_black_occupied_() == false){
                tile_vector_[vertical_counter][horizontal_counter] -> set_color_(2);
                tile_vector_[vertical_counter][horizontal_counter] -> update();
            }

            //The next check is for a white rook encountering a white piece
            else if(tile_vector_[vertical_counter][horizontal_counter] -> get_white_occupied_() == true && piece_team == 0){
                break;
            }

            //The final check is for a black rook encountering a black piece
            else if(tile_vector_[vertical_counter][horizontal_counter] -> get_black_occupied_() == true && piece_team == 1){
                break;
            }

            vertical_counter--;
        }

        vertical_counter = piece_row ;
        horizontal_counter = piece_column + 1;

        while(horizontal_counter <= 7){

            //The first check is for a black rook captruing a white piece
            if(tile_vector_[vertical_counter][horizontal_counter] -> get_white_occupied_() == true && piece_team == 1){
                tile_vector_[vertical_counter][horizontal_counter] -> set_color_(2);
                tile_vector_[vertical_counter][horizontal_counter] -> update();
                break;
            }

            //The next check is for a white rook capturing a black piece
            else if(tile_vector_[vertical_counter][horizontal_counter] -> get_black_occupied_() == true && piece_team == 0){
                tile_vector_[vertical_counter][horizontal_counter] -> set_color_(2);
                tile_vector_[vertical_counter][horizontal_counter] -> update();
                break;
            }

            //The next check is for any empty tile
            else if(tile_vector_[vertical_counter][horizontal_counter] -> get_white_occupied_() == false && tile_vector_[vertical_counter][horizontal_counter] -> get_black_occupied_() == false){
                tile_vector_[vertical_counter][horizontal_counter] -> set_color_(2);
                tile_vector_[vertical_counter][horizontal_counter] -> update();
            }

            //The next check is for a white rook encountering a white piece
            else if(tile_vector_[vertical_counter][horizontal_counter] -> get_white_occupied_() == true && piece_team == 0){
                break;
            }

            //The final check is for a black rook encountering a black piece
            else if(tile_vector_[vertical_counter][horizontal_counter] -> get_black_occupied_() == true && piece_team == 1){
                break;
            }

            horizontal_counter++;
        }

        vertical_counter = piece_row ;
        horizontal_counter = piece_column - 1;

        while(horizontal_counter >= 0){

            //The first check is for a black rook captruing a white piece
            if(tile_vector_[vertical_counter][horizontal_counter] -> get_white_occupied_() == true && piece_team == 1){
                tile_vector_[vertical_counter][horizontal_counter] -> set_color_(2);
                tile_vector_[vertical_counter][horizontal_counter] -> update();
                break;
            }

            //The next check is for a white rook capturing a black piece
            else if(tile_vector_[vertical_counter][horizontal_counter] -> get_black_occupied_() == true && piece_team == 0){
                tile_vector_[vertical_counter][horizontal_counter] -> set_color_(2);
                tile_vector_[vertical_counter][horizontal_counter] -> update();
                break;
            }

            //The next check is for any empty tile
            else if(tile_vector_[vertical_counter][horizontal_counter] -> get_white_occupied_() == false && tile_vector_[vertical_counter][horizontal_counter] -> get_black_occupied_() == false){
                tile_vector_[vertical_counter][horizontal_counter] -> set_color_(2);
                tile_vector_[vertical_counter][horizontal_counter] -> update();
            }

            //The next check is for a white rook encountering a white piece
            else if(tile_vector_[vertical_counter][horizontal_counter] -> get_white_occupied_() == true && piece_team == 0){
                break;
            }

            //The final check is for a black rook encountering a black piece
            else if(tile_vector_[vertical_counter][horizontal_counter] -> get_black_occupied_() == true && piece_team == 1){
                break;
            }

            horizontal_counter--;
        }
    }

    //Valid moves for a king
    //A king may move one space in any direction
    //It may not move through pieces
    if (piece_number == 5){

        //Checks the threee tile above, three tiles on same level, and three tiles below
        for(int i = piece_row - 1; i <= piece_row + 1; i++){
            for(int j = piece_column - 1; j <= piece_column + 1; j++){

                //must make sure the tile actually exists before trying to access it
                if(i >= 0 && i <= 7 && j >= 0 && j<= 7){

                    //A black king attacking a white piece
                    if(tile_vector_[i][j] -> get_white_occupied_() == true && piece_team == 1){
                        tile_vector_[i][j] -> set_color_(2);
                        tile_vector_[i][j] -> update();
                    }

                    //A white king attacking a black piece
                    else if(tile_vector_[i][j] -> get_black_occupied_() == true && piece_team == 0){
                        tile_vector_[i][j] -> set_color_(2);
                        tile_vector_[i][j] -> update();
                    }

                    //An empty tile
                    else if(tile_vector_[i][j] -> get_white_occupied_() == false && tile_vector_[i][j] -> get_black_occupied_() == false){
                        tile_vector_[i][j] -> set_color_(2);
                        tile_vector_[i][j] -> update();
                    }
                }
            }
        }
    }

}

/*
This slot responds to a piece being clicked by a player, of which there are numerous different options

First, the user could be using an axe or shield powerup, which occurs when selected_piece is NULL and selected_piece_team is -1 (inbetween turns)
It is viable to use the axe powerup on another piece of the opposite team; it is NOT viable to use the shield powerup an another piece at all

Next, it is checked to see if a new piece is being selected. If so, this piece must be on the current_player_'s team (current_player does NOT change)
Next, it is check to see if the user is clicking the same pice twice. If so, the piece is de-selected (current_player does NOT change)
Next, it is checked to see if the user is clicking on a piece of their same team. If so, the user is alerted that this is an invalid move. (current_player does NOT change)
Next, it is checked to see if the user is clicking on a piece of the opposite team. If so, that piece is captured and current_player_ updates (it does NOT update if a shield powerup is being used).
*/
void Board::PieceClickSlot(Piece * clicked_piece, int piece_number){

    //Check to see if the axe powerup is being used; this powerup is used inbetween turns, so selected_piece_ and selectd_piece_team cannot be used.
    //If a user makes a mistake when using the powerup, their turn is not forfeited. They can continue until the powerup is used.
    if(using_axe_powerup_){

        //If the player is attemtpting to use the powerup on one of their own pieces, do not allow them
        if(clicked_piece -> get_team_() == current_player_ -> get_player_number_()){
            emit UpdateGrandmaster(20);
        }

        //If the user is attempting to use the powerup on a royal (queen or king) piece, do not allow them
        else if(clicked_piece -> get_piece_number_() == 4 || clicked_piece -> get_piece_number_() == 5){
            emit UpdateGrandmaster(21);
        }

        //If the user is attemtping to use the powerup correctly, capture the piece, set using_axe_powerup_ to false, and reset the grandmaster label
        //Also, update the tile to reflect that it is no longer occupied.
        else{
            clicked_piece -> Captured();
            emit PieceCaptured(current_player_, clicked_piece);
            using_axe_powerup_ = false;

            if(current_player_ -> get_player_number_() == 0){
                emit UpdateGrandmaster(8);
            }else{
                emit UpdateGrandmaster(9);
            }

            tile_vector_[clicked_piece -> get_tile_row_()][clicked_piece -> get_tile_column_()] -> set_black_occupied_(false);
            tile_vector_[clicked_piece -> get_tile_row_()][clicked_piece -> get_tile_column_()] -> set_white_occupied_(false);
        }

     }

    //Check to see if the shield powerup is being used; if so, it can only be used on an empty tile, so alert the user and skip everything else. It remains their turn
    else if(using_shield_powerup_){
        emit UpdateGrandmaster(26);
    }

    //This is the piece a user wants to move; set the piece as selected and update the team
    //Also highlight the tiles that are valid for this piece to move to
     else if(selected_piece_team_ == -1){

        if( (current_player_ == player_one_ && clicked_piece -> get_team_() == 0) || (current_player_ == player_two_ && clicked_piece -> get_team_() == 1) ){ //Ensure the player is clicking on a piece on their team

             selected_piece_ = clicked_piece;
             selected_piece_team_ = clicked_piece -> get_team_();

             clicked_piece -> set_selected_image_();

             emit UpdateGrandmaster(piece_number);

             ValidMoves(selected_piece_ -> get_tile_row_(), selected_piece_ -> get_tile_column_(), selected_piece_ -> get_team_(), selected_piece_ -> get_piece_number_());

         }else{
             emit UpdateGrandmaster(7);
         }

     }

     //The user wishes to deselect the piece by clicking on it again.
     else if(clicked_piece == selected_piece_){

         selected_piece_ = NULL;
         selected_piece_team_ = -1;

         clicked_piece -> set_resting_image_();

         //Unhighlight the possible moves the piece had before it was unselected
         Unhighlight();
     }

     //The user is attempting to move a piece onto a piece on the same team; this is an invalid move.
     //Do not change the selected piece nor the selected piece team
     else if(selected_piece_team_ == clicked_piece -> get_team_()){
         emit UpdateGrandmaster(6);
     }

     //The user is moving a piece onto an enemy's piece, thus capturng it
     //After the move is completed, reset the selected piece and selected piece team
     //Also, update the current_player_
     else{

        //If the tile underneath the piece is not highlighted, this is not a valid move.
        //Stop the player from moving and exit the function
        if(!tile_vector_[clicked_piece -> get_tile_row_()][clicked_piece -> get_tile_column_()] -> get_tile_is_highlighted()){
            emit UpdateGrandmaster(6);
            return;
         }

         //Before the piece moves, update the old piece is was on to not be occupied at all
         ChangeTileOccupied(selected_piece_ -> get_tile_row_(), selected_piece_ -> get_tile_column_(), false, false);

         selected_piece_ -> set_coordinates_(clicked_piece -> get_x_coordinate_(), clicked_piece -> get_y_coordinate_());
         selected_piece_ -> set_tile_coordinates(clicked_piece -> get_tile_row_(), clicked_piece -> get_tile_column_());
         selected_piece_ -> set_resting_image_();

         //Update the new tile that the piece is on with the correct occupied fields
         if(selected_piece_ -> get_team_() == 0){
             ChangeTileOccupied(clicked_piece -> get_tile_row_(), clicked_piece -> get_tile_column_(), true, false);
         }else{
             ChangeTileOccupied(clicked_piece -> get_tile_row_(), clicked_piece -> get_tile_column_(), false, true);
         }

         //Unhighlight the possible moves the piece had before being moved
         Unhighlight();

         int is_king = clicked_piece -> Captured();

         //When the king is captured, completley end the game.
         if(is_king){
            EndGame(current_player_);
            return;
         }

         emit PieceCaptured(current_player_, clicked_piece);

         //If the boot powerup is in use, a double-call to ChangeCurrentPlayer will effectively skip the opponent's turn and return control to the player who used the powerup
         if(using_boot_powerup_){
             ChangeCurrentPlayer(false); //parameter is false to denote that a new game is not being started
             using_boot_powerup_ = false;
         }

         ChangeCurrentPlayer(false); //parameter is false to denote that a new game is not being started

         selected_piece_ = NULL;
         selected_piece_team_ = -1;
     }
}

/*
This slot responds to an empty tile being clicked by the user as a location they wish to move a piece.
First, the slot check to see if a piece has been selected. If not, no move is made.
If a piece has been selected, then it is moved to the tile.

It must be checked to see if the boot powerup is in use; if so, then call ChangeCurrentPlayer() an extra time

It must be chcecked to see if the shield powerup is in use; if so, revive the selected piece and the turn does NOT change

@param: The instance of the Tile class that was clicked on
@return: None
*/
void Board::tileClickSlot(Tile * selected_tile){

    //Check to see if the shield powerup is in use; if so, revive a piece in this location and reset the approriate variables
    //Also, update the tile to reflect that it is now occupied
    if(using_shield_powerup_){
        RevivePiece(selected_tile -> get_x_coordinate(), selected_tile -> get_y_coordinate());
        selected_captured_piece_ = -1;
        using_shield_powerup_ = false;

        //Update the grandmaster label to reflect the current player's turn
        if (current_player_ -> get_player_number_() == 0){
            emit UpdateGrandmaster(8);
        }else{
            emit UpdateGrandmaster(9);
        }

        if(current_player_ -> get_player_number_() == 0){
            selected_tile -> set_white_occupied_(true);
            selected_tile -> set_black_occupied_(false);
        }else{
            selected_tile -> set_white_occupied_(false);
            selected_tile -> set_black_occupied_(true);
        }
    }

    //The player is just clicking on a tile; do nothing
    if(selected_piece_ == NULL){
        return;
    }

    //The player is moving a piece onto a tile; move the piece to the tile's coordinates and offset so it is centered
    else{

        //If the selected tile is not highlighted, it is not a valid move.
        //Stop the player from moving and exit the function
        if(!selected_tile -> get_tile_is_highlighted()){
            emit UpdateGrandmaster(6);
            return;
        }

        //Pieces should be offset by 5 in the x direction and 10 in the y direction to center them within the tile
        int fixed_x_coordinate = selected_tile -> get_x_coordinate() + 5;
        int fixed_y_coordinate = selected_tile -> get_y_coordinate() + 10;

        //Update the old tile the piece was on to not be occupied
        ChangeTileOccupied(selected_piece_ -> get_tile_row_(), selected_piece_ -> get_tile_column_(), false, false);

        //This also calls the PixMap -> setPos function that updated the position.
        selected_piece_ -> set_coordinates_(fixed_x_coordinate, fixed_y_coordinate);
        selected_piece_ -> set_tile_coordinates(selected_tile -> get_y_coordinate() / 70, selected_tile -> get_x_coordinate() / 70); //Each tile is 70 by 70, so dividing by 70 gives the row and column

        //Return the piece to its resting state
        selected_piece_ -> set_resting_image_();

        //Update the occupied fields of the new tile the piece is on
        if(selected_piece_ -> get_team_() == 0){
            ChangeTileOccupied(selected_piece_ -> get_tile_row_(), selected_piece_ -> get_tile_column_(), true, false);
        }else{
            ChangeTileOccupied(selected_piece_ -> get_tile_row_(), selected_piece_ -> get_tile_column_(), false, true);
        }

        //Unihighlight the possible moves the piece had before being moved
        Unhighlight();

        //A successful move has been completed, so change the player and allow the new player to select a new piece.
        selected_piece_ = NULL;
        selected_piece_team_ = -1;

        //If the boot powerup is in use, a double-call to ChangeCurrentPlayer will effectively skip the opponent's turn and return control to the player who used the powerup
        if(using_boot_powerup_){
            ChangeCurrentPlayer(false); //parameter is false to denote that a new game is not being started
            using_boot_powerup_ = false;
        }

        ChangeCurrentPlayer(false); //parameter is false to denote that a new game is not being started
    }

}

/*
This slot responds to a powerup being clicked by a user as they move their piece to that position
The player will gain the powerup, which will disappear and show up in their "Power-Ups" box

It must be checked to see if the boot powerup is in use; if so, then call ChangeCurrentPlayer() an extra time

It must be checked to see if the shield powerup is in use; if so, the alert the player that they cannot revive a piece in this location

@param: The insatnce of the powerup that was clicked on
@return: The powerup is given to the current player and the powerup disappears from the board; the selected piece takes its position
*/
void Board::PowerupClickSlot(Powerup * clicked_powerup){

    //Check to see if the shield powerup is being used; if so, it can only be used on an empty tile, so alert the user and skip everything else. It remains their turn
    if(using_shield_powerup_){
        emit UpdateGrandmaster(26);
    }

    //The player is clicking on a powerup without a piece selectd; do nothing
    else if(selected_piece_ == NULL){
        return;
    }

    //The player is moving their piece onto a powerup; they aqcuire the powerup, it disappears from the screen, and their piece takes its position
    else{

        //If the tile underneath the powerup is not highlighted, it is not a valid move
        //Stop the player from moving and exit the function
        if(!tile_vector_[clicked_powerup -> get_y_coordinate_() / 70][clicked_powerup -> get_x_coordinate_() / 70] -> get_tile_is_highlighted()){
            emit UpdateGrandmaster(6);
            return;
        }

        //Update the old tile the piece was on to not be occupied
        ChangeTileOccupied(selected_piece_ -> get_tile_row_(), selected_piece_ -> get_tile_column_(), false, false);

        //This also calls the PixMap -> setPos function that updated the position.
        //Note that the powerups are already in the correct, offset position, so their coordinates can be used for the piece
        selected_piece_ -> set_coordinates_(clicked_powerup -> get_x_coordinate_(), clicked_powerup -> get_y_coordinate_());
        selected_piece_ -> set_tile_coordinates(clicked_powerup -> get_y_coordinate_() / 70, clicked_powerup -> get_x_coordinate_() / 70); //Each tile is 70 by 70, so dividing by 70 gives the row and column
        //Return the piece to its resting state
        selected_piece_ -> set_resting_image_();

        //Update the tile that the piece is now on to being occupied
        if(selected_piece_ -> get_team_() == 0){
            ChangeTileOccupied(selected_piece_ -> get_tile_row_(), selected_piece_ -> get_tile_column_(), true, false);
        }else{
            ChangeTileOccupied(selected_piece_ -> get_tile_row_(), selected_piece_ -> get_tile_column_(), false, true);
        }

        //Unhighlight the possible moves the piece had before being moved
        Unhighlight();

        //Hide the powerup and emit a signal to update the player's "Power-Ups" list
        clicked_powerup -> set_coordinates_(-100,-100);
        emit PowerupAcquired(current_player_, clicked_powerup);

        //A successful move has been completed, so change the player and allow the new player to select a new piece.
        selected_piece_ = NULL;
        selected_piece_team_ = -1;

        //If the boot powerup is in use, a double-call to ChangeCurrentPlayer will effectively skip the opponent's turn and return control to the player who used the powerup
        if(using_boot_powerup_){
            ChangeCurrentPlayer(false); //parameter is false to denote that a new game is not being started
            using_boot_powerup_ = false;
        }

        ChangeCurrentPlayer(false); //parameter is false to denote that a new game is not being started
    }
}
