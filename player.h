#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include "piece.h"


/*
A basic class to represent a player. This allows the board to determine who's turn it is.
*/

class Player
{
public:

    Player(int player_number); //Default constructor; takes only the player number where 0 == player_one_ and 1 == player_two_

    int get_player_number_(){return player_number_; } //Get the player_number_ of the player

private:

    int player_number_; //An identification number for the player, where 0 == player_one_ and 1 == player_two_

    std::vector<Piece *> captured_pieces_; //Store the pieces that this player has captured
};

#endif // PLAYER_H
