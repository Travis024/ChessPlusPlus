#ifndef BOARD_H
#define BOARD_H

#include <vector>

#include "tile.h"

#include "player.h"

#include "powerupaxe.h"
#include "powerupboot.h"
#include "powerupshield.h"

#include "knight.h"
#include "bishop.h"
#include "pawn.h"
#include "rook.h"
#include "queen.h"
#include "king.h"

/*
The board class contains all piecs, tiles, powerups, and players.
It make interactions between these elements possible.

The board class also interacts with button presses, labels, and lists in the MainWindow.
*/

class Board: public QObject
{

    Q_OBJECT


public:
    Board(); //Deafult constructor; intializes the board with 2 players, 64 tiles, and 32 pieces (16 white, 16 black)

    void InitializePlayers(); //Create player_one_ and player_two; also, set the current_player_ to player_one_;
    void InitializePieceVectors(); //Sets up the white_alive and black_alive vectors with 16 pieces each; also, connect signals and slots from pieces to the board
    void InitializeTileVector(); //Sets up the board with 64 total tiles
    void InitializePowerups(); //Creates six total powerups - two of each type - that exist in the game

    void ConnectPiecesToBoard(); //Connect the signals of each piece to the slots on the board
    void ConnectPowerupsToBoard(); //Connect the signals of the powerups to the slots on the board

    std::vector<std::vector<Tile *>> get_tile_vector_(){return tile_vector_; } //Get the private vector of vectors of tiles
    std::vector<Piece *> get_white_alive_vector_(){return white_alive_vector_; } //Get the private vector of alive white pieces
    std::vector<Piece *> get_black_alive_vector_(){return black_alive_vector_; } //Get the private vector of alive black pieces
    std::vector<Powerup *> get_powerup_vector_(){return  powerup_vector_; } //Get the private vector of powerups
    Player * get_current_player_(){return current_player_; } //Retreive the current player to be used by the Main Window

    //Update the powerup fields after the "Use Power-Up" button has been pressed and again after the powerup's function is complete
    void set_using_axe_powerup_(bool in_use){using_axe_powerup_ = in_use; }
    void set_using_boot_powerup_(bool in_use){using_boot_powerup_ = in_use; }
    void set_using_shield_powerup_(bool in_use){using_shield_powerup_ = in_use; }

    void set_selected_captured_piece(int piece_number){selected_captured_piece_ = piece_number; } //Update the selected_captured_piece field when a player chooses a piece to revive in the Main Window

    void Unhighlight(); //Reset all tiles from highlighted to their intial colors

    void ChangeTileOccupied(int tile_row, int tile_column, bool is_white_occupied, bool is_black_occupied); //Update the occupied fields of a tile at a given row and column
    void ResetOccupiedToInitial(); //When the 'Start New Game' button is pressed, reset the occupied fields of every tile

    void ValidMoves(int piece_row, int piece_column, int piece_team, int piece_number); //Highlight tiles that are valid moves after a piece is clicked

    void RevivePiece(int tile_x, int tile_y); //Move a piece from -100, -100 to the location of the tile selected

    void ChangeCurrentPlayer(bool start_new_game); //Update the current player and reflect the change in the grandmaster label; if starting a new game, the parameter is set to true;

    void EndGame(Player * player); //When the king is captured, the grandmaster label updates and all losing pieces and powerups are removed from the board


signals:

    void UpdateGrandmaster(int message); //Send a signal to the Main Window to update the "Grandmaster Says"; 1-5 designate pieces, 6 desginates an invalid move

    void PieceCaptured(Player * player, Piece * captured_piece); //Send a signal to the Main Window to update the "Captured" box of the player

    void PowerupAcquired(Player * Player, Powerup * powerup); //Send a signal to the Main Window to update the "Power-Ups" box of the player

private slots:

    void tileClickSlot(Tile * selected_tile_); //Slot to respond to any open tile being clicked by a user

    void PieceClickSlot(Piece * clicked_piece, int piece_number); //Slot to respond to any piece being clicked by a user

    void PowerupClickSlot(Powerup * clicked_powerup); //Slot to respond to any powerup being clicked by a user

private:

    //This is a two player game only; keep track of each player and whose turn it is
    Player * player_one_;
    Player * player_two_;
    Player * current_player_;

    //Store 8 vectors each consisting of 8 tiles
    std::vector<std::vector<Tile *>> tile_vector_;

    /*
     Vectors to store the alive pieces for each color
     0-7 are pawns
     8,9 are bishops
     10,11 are knights
     12, 13 are rooks
     14 is queen
     15 is king
      */
    std::vector<Piece *> white_alive_vector_;
    std::vector<Piece *> black_alive_vector_;

    //Keep track of the powerups that have been created
    std::vector<Powerup *> powerup_vector_;

    //Store the piece and team that a user has selected for moving
    //Set to NULL and -1 at the start of the fgame and anytime a successful move is completed
    Piece * selected_piece_ = NULL;
    int selected_piece_team_ = -1;

   //Flags to determine if a powerup is being used when the player clicks on the board
   //Originally set to false, as now powerups will bes used on the initial move
   bool using_axe_powerup_ = false;
   bool using_boot_powerup_ = false;
   bool using_shield_powerup_ = false;

   int selected_captured_piece_ = -1; //Works in conjunction with selected_captured_piece_ in the Main Window class to track the piece being revived by the shield powerup

    //The board is 560 by 560
    int width_ = 560;
};

#endif // BOARD_H
