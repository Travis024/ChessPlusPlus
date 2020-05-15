#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <utility> //pair

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //The UI must be set up before the scene can be drawn on
    ui->setupUi(this);

    //the QGraphicsView is the UI element that contains the scene that is drawn on
    QGraphicsView *boardView = ui->boardView;
    board_scene_ = new QGraphicsScene;
    boardView -> setScene(board_scene_);
    boardView -> setSceneRect(0,0,boardView->frameSize().width() - 3, boardView -> frameSize().height() - 3); //Subtracting 3 from the width and heigh removes the scroll bars that appear when the view is set

    //Create the board for the game to be played on and connect its signals to the Main Window Slots
    game_board_ = new Board();
    connect(game_board_, &Board::UpdateGrandmaster, this, &MainWindow::UpdateGrandmasterSlot);
    connect(game_board_, &Board::PieceCaptured, this, &MainWindow::PieceCapturedSlot);
    connect(game_board_, &Board::PowerupAcquired, this, &MainWindow::PowerupAcquiredSlot);

    //Connect the list widgets for player one and player two to the board so that their clicks can be responded to
    //Split into separate slots so that player one and player two can only interact with their respective lists.
    connect(ui->playerOneCapturedList, &QListWidget::itemClicked, this, &MainWindow::playerOneCapturedClickSlot);
    connect(ui->playerTwoCapturedList, &QListWidget::itemClicked, this, &MainWindow::playerTwoCapturedClickSlot);
    connect(ui->playerOnePowerUpList, &QListWidget::itemClicked, this, &MainWindow::playerOneListWidgetClickSlot);
    connect(ui->playerTwoPowerUpList, &QListWidget::itemClicked, this, &MainWindow::playerTwoListWidgetClickSlot);

    //Set the Tiles, Pieces, and Powerups, owned by the Board, in their iniaitalized positions
    DrawBoard();
    DrawPieces(false); //false ensures that pieces are added to the scene at initialization
    DrawPowerups(false); //false ensures that pieces are added to the scene at initialization
}

/*
A function to draw every single tile initialized in the board to the screen

@param: An instance of the board class
@return: Every tile with the board is drawn on the board_scene_ in the main window
*/
void MainWindow::DrawBoard(){

    std::vector<std::vector<Tile *>> tile_vector = game_board_->get_tile_vector_();

    for(unsigned int i = 0; i < tile_vector.size(); i++){
        std::vector<Tile *> current_row = tile_vector[i];

        for(unsigned int j = 0; j < current_row.size(); j++){
            board_scene_ -> addItem(current_row[j]);
        }
    }
}

/*
A function to draw 32 pieces in the appropriate position to start a chess game
16 black pieces occupy rows 0 and 1
16 white pieces occupy rows 6 and 7
Each piece has its tile coordinates set as well

This function is also used when the Start New Game button is pressed, but start_new_game is false so that pieces are not re-added to the scene

@param: None
@ return: Pieces are drawn to the scene and their tile coordinates are updated
*/
void MainWindow::DrawPieces(bool start_new_game){

    //Get each vector of alive pieces
    std::vector<Piece *> white_alive_vector = game_board_ -> get_white_alive_vector_();
    std::vector<Piece *> black_alive_vector = game_board_ -> get_black_alive_vector_();


    //start_new_game is false when MainWindow is iniaitlaized so that pieces are added to the scene
    //start_new_game is true when the Start New game button is pressed so that pieces are not re-added
    if(!start_new_game){
        //Add every item to the scene; from this point on, any setPos() call (within the set_coordinates_ wrapper function) will move the piece
        for(unsigned int i = 0; i < white_alive_vector.size(); i++){
            board_scene_ -> addItem(white_alive_vector[i]);
            board_scene_ -> addItem(black_alive_vector[i]);
        }
    }

   //Begin setting the coordinates, positiion, and tile_coordinates of each piece
   //Note that pieces are offset 5 in the x direction and 10 in the y direction to center them

    //Begin drawing pawns; BOTH WHITE AND BLACK
    for(unsigned int i = 0; i < 8; i++){
        white_alive_vector[i] -> set_coordinates_((70 * i) + 5, 430);
        white_alive_vector[i] -> set_tile_coordinates(6, i);

        black_alive_vector[i] -> set_coordinates_((70 * i) + 5, 80);
        black_alive_vector[i] -> set_tile_coordinates(1, i);
    }
    //End drawing panws; BOTH WHITE AND BLACK

    //Begin drawing bishops; WHITE
    white_alive_vector[8] -> set_coordinates_(145, 500);
    white_alive_vector[8] -> set_tile_coordinates(7, 2);
    white_alive_vector[9] -> set_coordinates_(355, 500);
    white_alive_vector[9] -> set_tile_coordinates(7, 5);
    //End drawing bishops; WHITE

    //Begin drawing knights; WHITE
    white_alive_vector[10] -> set_coordinates_(75, 500);
    white_alive_vector[10] -> set_tile_coordinates(7, 1);
    white_alive_vector[11] -> set_coordinates_(425, 500);
    white_alive_vector[11] -> set_tile_coordinates(7, 6);
    //End drawing knights; WHITE

    //Begin drawing rooks; WHITE
    white_alive_vector[12] -> set_coordinates_(5, 500);
    white_alive_vector[12] -> set_tile_coordinates(7, 0);
    white_alive_vector[13] -> set_coordinates_(495, 500);
    white_alive_vector[13] -> set_tile_coordinates(7, 7);
    //End drawing rooks; WHITE

    //Begin drawing queen; WHITE
    white_alive_vector[14] -> set_coordinates_(215, 500);
    white_alive_vector[14] -> set_tile_coordinates(7, 3);
    //End drawing queen; WHITE

    //Begin drawing king: WHITE
    white_alive_vector[15] -> set_coordinates_(285, 500);
    white_alive_vector[15] -> set_tile_coordinates(7, 4);
    //End drawing king; WHITE

    //Begin drawing bishops; BLACK
    black_alive_vector[8] -> set_coordinates_(145, 10);
    black_alive_vector[8] -> set_tile_coordinates(0, 2);
    black_alive_vector[9] -> set_coordinates_(355, 10);
    black_alive_vector[9] -> set_tile_coordinates(0, 5);
    //End drawing bishopsl BLACK

    //Begin drawing knights; BLACK
    black_alive_vector[10] -> set_coordinates_(75, 10);
    black_alive_vector[10] -> set_tile_coordinates(0, 1);
    black_alive_vector[11] -> set_coordinates_(425, 10);
    black_alive_vector[11] -> set_tile_coordinates(0, 6);
    //End drawing knighs; BLACK

    //Begin drawing rooks; BLACK
    black_alive_vector[12] -> set_coordinates_(5, 10);
    black_alive_vector[12] -> set_tile_coordinates(0, 0);
    black_alive_vector[13] -> set_coordinates_(495, 10);
    black_alive_vector[13] -> set_tile_coordinates(0, 7);
    //End drawing rooks; BLACK

    //Begin drawing queen; BLACK
    black_alive_vector[14] -> set_coordinates_(215, 10);
    black_alive_vector[14] -> set_tile_coordinates(0, 3);
    //End drawing queen; BLACK

    //Begin drawing king; BLACK
    black_alive_vector[15] -> set_coordinates_(285, 10);
    black_alive_vector[15] -> set_tile_coordinates(0, 4);
    //End drawing king; BLACK

}

/*
Draw the board's powerups in random positions on rows 2 - 5
start_new_game is false when the game is initiailizing for the first time
start_new_game is true when the Start New Game button is pressed so that powerups aren't re-added to the scene
*/
void MainWindow::DrawPowerups(bool start_new_game){
    std::vector<Powerup *> powerup_vector = game_board_ -> get_powerup_vector_();
    std::vector<QPair<int,int>> possible_positions;

    //Get one of every possible pair of tile positions that a powerup can be placed on
    //Columns 0 - 7, rows 2 - 5
    for(int i = 0; i < 8; i++){
        for(int j = 2; j < 6; j++){
            QPair<int,int> current_pair;
            current_pair.first = i;
            current_pair.second = j;
            possible_positions.push_back(current_pair);
        }
    }

    //Shuffle the vecotr to effectively randomize it
    std::random_shuffle(possible_positions.begin(), possible_positions.end());

    //If the game is being initialized, add every piece to the scene
    if(!start_new_game){
        for(unsigned int i = 0; i < powerup_vector.size(); i++){
            board_scene_ -> addItem(powerup_vector[i]);
        }
    }

    int powerup_x;
    int powerup_y;
    int x_offset = 5;
    int y_offset = 10;

    //Pull 6 pairs from the randomized vector and set the coordinates of the powerups accoridngly
    //Recall, one tile is 70 by 70
    for(unsigned int i = 0; i < powerup_vector.size(); i++){

        powerup_x = possible_positions[i].first;
        powerup_y = possible_positions[i].second;

        powerup_vector[i] -> set_coordinates_( (powerup_x * 70) + x_offset, (powerup_y * 70) + y_offset);
    }
}

/*
Update the grandmaster label, which effectively acts as a real-time form of communication to the users for piece information, who's turn it is, when the game has ended, etc
By getting the current_player_ from the Board class and adding that to the front of each string, players should always know who's turn it is

@param: The message id for the message to be displayed in the label
@return: The label updates with the appropriate message!
*/
void MainWindow::UpdateGrandmasterSlot(int message){

    //Get the current player and their number to create a string that tells the users who's turn it is whenever the label updates
    Player * current_player = game_board_ -> get_current_player_();

    QString current_player_string;

    if(current_player -> get_player_number_() == 0){
        current_player_string = "PLAYER ONE: ";
    }else{
        current_player_string = "PLAYER TWO: ";
    }

   //List of messages that can appear depending on the user interaction
   if(message == 0){
        ui -> grandmasterLabel -> setText(current_player_string + "A pawn has been selected. A pawn cannot move through other pieces. It can move one or two tiles forward on its initial move. Following that, it can move forward one tile or diagonal one tile to capture another piece.");
   }else if(message == 1){
       ui -> grandmasterLabel -> setText(current_player_string + "A bishop has been selected. A bishop may only move diagonally as many tiles as it wishes, and it cannot move through other pieces.");
   }else if(message == 2){
       ui -> grandmasterLabel -> setText(current_player_string + "A knight has been selected. Knights jump over other pieces. They may either move up/down one and horizontally over two, or horizontally over two and up/down one.");
   }else if(message == 3){
       ui -> grandmasterLabel -> setText(current_player_string + "A rook has been selected. A rook may not move through other pieces. It may move has many tiles as it wishes in the horizontal or vertical direction.");
   }else if(message == 4){
       ui -> grandmasterLabel -> setText(current_player_string + "A queen has been selected. A queen may not move through other pieces, but it can move as many spaces in any direction it pleases.");
   }else if(message == 5){
       ui -> grandmasterLabel -> setText(current_player_string + "A king has been selected. A king may not move through other pieces, and it can move one space in any direction. Do not put your king in danger!");
   }else if(message == 6){
       ui -> grandmasterLabel -> setText(current_player_string + "Moving the piece to the selected location is not allowed. Please click on a different location.");
   }else if(message == 7){
       ui -> grandmasterLabel -> setText(current_player_string + "This piece does not belong to you. Please select a piece on your team.");
   }else if(message == 8){
       ui -> grandmasterLabel -> setText("It is PLAYER ONE'S turn. Select a piece and then click on a highlighted tile to move it.");
   }else if(message == 9){
       ui -> grandmasterLabel -> setText("It is PLAYER TWO'S turn. Select a piece and then click on a highlighted tile to move it.");
   }else if(message == 10){
       ui -> grandmasterLabel -> setText(current_player_string + "The Axe allows you to capture any non-royal piece from your enemy. Press 'Use Power-Up' and then click on an enemy piece to capture.");
   }else if(message == 11){
       ui -> grandmasterLabel -> setText(current_player_string + "The Boot skips the opponent's turn, alowing you to go twice in a row. Press 'Use Power-Up' before taking a turn.");
   }else if(message == 12){
       ui -> grandmasterLabel -> setText(current_player_string + "The Shield allows you to revive one of your captured pieces in a random location. Select a piece from the opponent's 'Captured' list and then press 'Use Power-Up.'");
   }else if(message == 13){
       ui -> grandmasterLabel -> setText("PLAYER ONE WINS! Please press the 'Start New Game' button to play again.");
   }else if(message == 14){
       ui -> grandmasterLabel -> setText("PLAYER TWO WINS! Please press the 'Start New Game' button to play again.");
   }else if(message == 15){
       ui -> grandmasterLabel -> setText(current_player_string + "This list does not belong to you. Please select a Power-Up from your own list or select a piece to move.");
   }else if(message == 16){
       ui -> grandmasterLabel -> setText(current_player_string + "Only the opposing player may interact with the pieces that you've captured.");
   }else if(message == 17){
       ui -> grandmasterLabel -> setText(current_player_string + "You have selected one of your pieces that has been captured by the opponent. If you have a Shield Power-Up, use it to bring this piece back to the board!");
   }else if(message == 18){
       ui -> grandmasterLabel -> setText(current_player_string + "If you have a Power-Up that you would like to use, click on the 'Use Power-Up Button' on your side of the screen.");
   }else if(message == 19){
       ui -> grandmasterLabel -> setText(current_player_string + "You are using the Axe Power-Up. Click on a non-royal enemy piece to capture it without forfeiting your turn!");
   }else if(message == 20){
       ui -> grandmasterLabel -> setText(current_player_string + "You cannot use the Axe Power-Up on your own pieces. Click on a non-royal enemy piece to capture it without forfeiting your turn!");
   }else if(message == 21){
       ui -> grandmasterLabel -> setText(current_player_string + "You cannot capture a royal piece (queen or king) with the Axe Power-Up. Click on another piece to capture.");
   }else if(message == 22){
       ui -> grandmasterLabel -> setText(current_player_string + "No Power-Up has been selected.");
   }else if(message == 23){
       ui -> grandmasterLabel -> setText(current_player_string + "You are using the Boot Power-Up, meaning you get to take TWO turns in a row!");
   }else if(message == 24){
       ui -> grandmasterLabel -> setText(current_player_string + "Please select a piece from the opponent's captured list to use whith the Shield Power-Up.");
   }else if(message == 25){
       ui -> grandmasterLabel -> setText(current_player_string + "You are using the Shield Power-Up. Select an empty tile to place the revived piece.");
   }else if(message == 26){
       ui -> grandmasterLabel -> setText(current_player_string + "You cannot revive a piece in this location. Please select an empty tile instead.");
   }
}

/*
When a piece has been captured, receive a signal from the board and add that piece to the "Captured" list for the appropiate player

@param: The current player and the captured piece
@return: The name of the piece is added to the "Captured" list for the player that captured that piece
*/
void MainWindow::PieceCapturedSlot(Player * player, Piece * captured_piece){

    int player_number = player -> get_player_number_();
    int piece_number = captured_piece -> get_piece_number_();
    QListWidget * list_to_update;

    //Set the "Captured" list based on which player it is
    if(player_number == 0){
        list_to_update = ui -> playerOneCapturedList;
    }else{
        list_to_update = ui -> playerTwoCapturedList;
    }

    //associate piece identification with their corresponding string
    if(piece_number == 0){
        list_to_update -> addItem("Pawn");
    }else if(piece_number == 1){
        list_to_update -> addItem("Bishop");
    }else if(piece_number == 2){
        list_to_update -> addItem("Knight");
    }else if(piece_number == 3){
        list_to_update -> addItem("Rook");
    }else if(piece_number == 4){
        list_to_update -> addItem("Queen");
    }else{
        list_to_update -> addItem("King");
    }

}

/*
When a powerup has been acquired, receive a signal from the board and add that powerup to the "Power-Ups" list for the appropiate player

@param: The current player and the acquired powerup
@return: The name of the powerup is added to the "Power-Ups" list for the player that acquired that powerup
*/
void MainWindow::PowerupAcquiredSlot(Player * player, Powerup * powerup){

    int player_number = player -> get_player_number_();
    int powerup_number = powerup -> get_powerup_number_();
    QListWidget * list_to_update;

    //Set the "Captured" list based on which player it is
    if(player_number == 0){
        list_to_update = ui -> playerOnePowerUpList;
    }else{
        list_to_update = ui -> playerTwoPowerUpList;
    }

    //associate powerup identification with their corresponding string
   if(powerup_number == 0){
       list_to_update -> addItem("Axe");
   }else if(powerup_number == 1){
       list_to_update -> addItem("Boot");
   }else{
       list_to_update -> addItem("Shield");
   }
}

/*
Allow player two to interact with player one's captured list
This is only for use with the Shield powerup that revives a piece in a random location
Set the selected_captured_piece_ field in Main Window AND the board itself

@param: The piece that was clciked on
@return: The selected_captured_piece_ field of the Main Window and the board is set to an id corresponding to that of the selected piece and the grandmaster label updates
*/
void MainWindow::playerOneCapturedClickSlot(QListWidgetItem * captured_piece){

    Player * current_player = game_board_ -> get_current_player_();

    //If player one attempts to interact with this list, alert them through the grandmaster label and clear the selection
    if(current_player -> get_player_number_() == 0){
        UpdateGrandmasterSlot(16);
        ui -> playerOneCapturedList -> clearSelection();
    }

    //If player two is interacting with the list, set the selected_captured_piece_ and update them to use the Shield powerup
    else{

        UpdateGrandmasterSlot(17);

        QString captured_piece_number = captured_piece -> text();

        if(captured_piece_number == "Pawn"){
            selected_captured_piece_ = 0;
            game_board_ -> set_selected_captured_piece(0);
        }else if(captured_piece_number == "Bishop"){
            selected_captured_piece_ = 1;
            game_board_ -> set_selected_captured_piece(1);
        }else if(captured_piece_number == "Knight"){
            selected_captured_piece_ = 2;
            game_board_ -> set_selected_captured_piece(2);
        }else if(captured_piece_number == "Rook"){
            selected_captured_piece_ = 3;
            game_board_ -> set_selected_captured_piece(3);
        }else if(captured_piece_number == "Queen"){
            selected_captured_piece_ = 4;
            game_board_ -> set_selected_captured_piece(4);
        }else{
            selected_captured_piece_ = 5;
            game_board_ -> set_selected_captured_piece(5);
        }
    }

}

/*
This slot is functionally similar to the above slot, but it only allows player one to interact with player two's captured list

@param: The piece that was clciked on
@return: The selected_captured_piece_ field of the Main Window is set to an id corresponding to that of the selected piece and the grandmaster label updates
*/
void MainWindow::playerTwoCapturedClickSlot(QListWidgetItem * captured_piece){

    Player * current_player = game_board_ -> get_current_player_();

    //If player two attempts to interact with this list, alert them through the grandmaster label and clear the selection
    if(current_player -> get_player_number_() == 1){
        UpdateGrandmasterSlot(16);
        ui -> playerTwoCapturedList -> clearSelection();
    }

    //If player one is interacting with the list, set the selected_captured_piece_ and update them to use the Shield powerup
    else{

        UpdateGrandmasterSlot(17);

        QString captured_piece_number = captured_piece -> text();

        if(captured_piece_number == "Pawn"){
            selected_captured_piece_ = 0;
            game_board_ -> set_selected_captured_piece(0);
        }else if(captured_piece_number == "Bishop"){
            selected_captured_piece_ = 1;
            game_board_ -> set_selected_captured_piece(1);
        }else if(captured_piece_number == "Knight"){
            selected_captured_piece_ = 2;
            game_board_ -> set_selected_captured_piece(2);
        }else if(captured_piece_number == "Rook"){
            selected_captured_piece_ = 3;
            game_board_ -> set_selected_captured_piece(3);
        }else if(captured_piece_number == "Queen"){
            selected_captured_piece_ = 4;
            game_board_ -> set_selected_captured_piece(4);
        }else{
            selected_captured_piece_ = 5;
            game_board_ -> set_selected_captured_piece(5);
        }
    }
}

/*
Allow the MainWindow to respond to an acquired powerup being clicked on in the list widget.
First, give the user a description of what the powerup does and how to use it in the grandmaster label
Then, set the Mian Window selected_powerup_ field to the identification number of the powerup selected

The list can only be selected if the current player is player one.

@param: The powerup that was clicked on, as a list item
@return: The grandmaster label updates with a description of the powerup
*/
void MainWindow::playerOneListWidgetClickSlot(QListWidgetItem * powerup){

    Player * current_player = game_board_ -> get_current_player_();

    //If the current player is player two, alert them not to use the list and de-select the item.
    if(current_player -> get_player_number_() == 1){
        UpdateGrandmasterSlot(15);
        ui -> playerOnePowerUpList -> clearSelection();
    }

    //Otherwise, update the grandmaster label with the appropriate description.
    else{
        if(powerup -> text() == "Axe"){
            UpdateGrandmasterSlot(10);
            selected_powerup_ = 0;
        }else if(powerup -> text() == "Boot"){
            UpdateGrandmasterSlot(11);
            selected_powerup_ = 1;
        }else{
            UpdateGrandmasterSlot(12);
            selected_powerup_ = 2;
        }
    }
}

/*
This slot is functionally the same as the above slot, but the list can only be selected if the current player is player one.

@param: The powerup that was clicked on, as a list item
@return: The grandmaster label updates with a description of the powerup
*/
void MainWindow::playerTwoListWidgetClickSlot(QListWidgetItem * powerup){

    Player * current_player = game_board_ -> get_current_player_();

    //If the current player is player one, alert them not to use the list and de-select the item.
    if(current_player -> get_player_number_() == 0){
        UpdateGrandmasterSlot(15);
        ui -> playerTwoPowerUpList -> clearSelection();
    }

    //Otherwise, update the grandmaster label with the appropriate description
    else{
        if(powerup -> text() == "Axe"){
            UpdateGrandmasterSlot(10);
            selected_powerup_ = 0;
        }else if(powerup -> text() == "Boot"){
            UpdateGrandmasterSlot(11);
            selected_powerup_ = 1;
        }else{
            UpdateGrandmasterSlot(12);
            selected_powerup_ = 2;
        }
    }
}

/*
Open a dialouge that explains the instructions of the game to the user.

@param: None
@return: A new window is created using the model-less method, meaning that both windows can be used at the same time.
*/
void MainWindow::on_howToPlayButton_clicked()
{
    instructions_window_ = new HowToPlay(this);
    instructions_window_ -> show();
}

/*
Start a new game by resetting the position of the pieces that have been moved, resetting the powerups, and clearing both the captured and powerups lists; the game restarts on player one's turn

@param: None
@return: The pieces are set to their initial position and it becomes Player One's turn
*/
void MainWindow::on_newGameButton_clicked()
{
    DrawPieces(true); //Passing true ensures the pieces are not re-added to the scene.

    DrawPowerups(true); //Passing true ensures the powerups are not re-added to the scene.

    //Clear all lists, which effectiively removes all items from them
    ui -> playerOneCapturedList -> clear();
    ui -> playerTwoCapturedList -> clear();
    ui -> playerOnePowerUpList -> clear();
    ui -> playerTwoPowerUpList -> clear();

    game_board_ -> ChangeCurrentPlayer(true); //parameter is true to denote that a new game IS being started

    game_board_ -> ResetOccupiedToInitial(); //Reset the occupied fields of the tiles to their initial states

    selected_powerup_ = -1; //Reset the game so that no powerup is selected

    selected_captured_piece_ = -1; //Reset the game so that no piece from either captured list is selected

    UpdateGrandmasterSlot(8); //Because this is a slot within the MainWindow class, no signal is needed. It is called like a regular function.
}

/*
There are multiple possibilities when the "Use Power-Up" button is clicked
First, if the opposite player is clicking the button, they are alerted to click their own button
Next, if no powerup has been selected, the user is notified to select a powerup
Finally, the function checks to see which powerup has been selected and updates variables appropriately

If a powerup HAS been used, it is removed from the list, deleted approriately, and variables are reset

@param: None
@return: The grandmaster label updates, and if a powerup has been used, it is removed from the list
*/
void MainWindow::on_playerOnePowerUpButton_clicked()
{
    Player * current_player = game_board_ -> get_current_player_();

    //The player clicking the powerup button is NOT the correct player
    if(current_player -> get_player_number_() == 1){
        UpdateGrandmasterSlot(18);
        return;
    }

    //The player clicking the powerup button IS the correct player
    else{

        //No powerup has been selected
        if(selected_powerup_ == -1){
            UpdateGrandmasterSlot(22);
            return;
        }

        //The axe powerup has been selected
        else if(selected_powerup_ == 0){
            UpdateGrandmasterSlot(19);
            game_board_ -> set_using_axe_powerup_(true);
       }

        //The boot powwerup has been selected
        else if(selected_powerup_ == 1){
            UpdateGrandmasterSlot(23);
            game_board_ -> set_using_boot_powerup_(true);
       }

       //The shield powerup has been selected
       else{

            //No piece has been selected to revive
            if(selected_captured_piece_ == -1){
                UpdateGrandmasterSlot(24);
                ui -> playerOnePowerUpList -> clearSelection();
                return;
            }

            //A piece has been selected to revive
            else{
                UpdateGrandmasterSlot(25);
                game_board_ -> set_using_shield_powerup_(true);

                //Remove the captured piece from the board, remove the selection, and reset the variable

                //Get the piece that has been selected and delete it; the takeItem function does NOT free up memory
                int revived_piece_row = ui -> playerTwoCapturedList -> currentRow();
                QListWidgetItem * piece_to_delete = ui -> playerTwoCapturedList -> takeItem(revived_piece_row);
                delete piece_to_delete;

                //Clear the selection so that no field is highlighted
                ui -> playerTwoCapturedList -> clearSelection();

                //Finally, reset the variable
                selected_captured_piece_ = -1;
            }
       }

       //When a powerup is used, remove it from the list and reset the appropriate fields

       //Get the powerup that has been selected and delete it; the takeItem function does NOT free up memory
       int used_powerup_row = ui -> playerOnePowerUpList -> currentRow();
       QListWidgetItem * item_to_delete = ui -> playerOnePowerUpList -> takeItem(used_powerup_row);
       delete item_to_delete;

       //Clear the selection so that no field is highlighted
       ui -> playerOnePowerUpList ->clearSelection();

       //Finally, reset the variables
       selected_powerup_ = -1;

    }
}

/*
The slot functions in a similar manner to the above slot, but it checks to make sure that only player two can use it

@param: None
@return: The grandmaster label updates, and if a powerup has been used, it is removed from the list
*/
void MainWindow::on_playerTwoPowerUpButton_clicked()
{
    Player * current_player = game_board_ -> get_current_player_();

    //The player clicking the powerup button is NOT the correct player
    if(current_player -> get_player_number_() == 0){
        UpdateGrandmasterSlot(18);
        return;
    }

    //The player clicking the powerup button IS the correct player
    else{

        //No powerup has been selected
        if(selected_powerup_ == -1){
            UpdateGrandmasterSlot(22);
            return;
        }

        //The axe powerup has been selected
        else if(selected_powerup_ == 0){
            UpdateGrandmasterSlot(19);
            game_board_ -> set_using_axe_powerup_(true);
       }

        //The boot powwerup has been selected
        else if(selected_powerup_ == 1){
            UpdateGrandmasterSlot(23);
            game_board_ -> set_using_boot_powerup_(true);
       }

        //The shield powerup has been selected
        else{

             //No piece has been selected to revive
             if(selected_captured_piece_ == -1){
                 UpdateGrandmasterSlot(24);
                 ui -> playerTwoPowerUpList -> clearSelection();
                 return;
             }

             //A piece has been selected to revive
             else{
                 UpdateGrandmasterSlot(25);
                 game_board_ -> set_using_shield_powerup_(true);

                 //Remove the captured piece from the board, remove the selection, and reset the variable

                 //Get the piece that has been selected and delete it; the takeItem function does NOT free up memory
                 int revived_piece_row = ui -> playerOneCapturedList -> currentRow();
                 QListWidgetItem * piece_to_delete = ui -> playerOneCapturedList -> takeItem(revived_piece_row);
                 delete piece_to_delete;

                 //Clear the selection so that no field is highlighted
                 ui -> playerOneCapturedList -> clearSelection();

                 //Finally, reset the variable
                 selected_captured_piece_ = -1;
             }
        }

        //When a powerup is used, remove it from the list and reset the appropriate fields

        //Get the powerup that has been selected and delete it; the takeItem function does NOT free up memory
        int used_powerup_row = ui -> playerTwoPowerUpList -> currentRow();
        QListWidgetItem * item_to_delete = ui -> playerTwoPowerUpList -> takeItem(used_powerup_row);
        delete item_to_delete;

        //Clear the selection so that no field is highlighted
        ui -> playerTwoPowerUpList ->clearSelection();

        //Finally, reset the variables
        selected_powerup_ = -1;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

