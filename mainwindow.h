#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QImage>
#include <QGraphicsItem>

#include "howtoplay.h"
#include "board.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void DrawBoard(); //Draws 64 tiles from the game_board_ field; used at the start of a new game
    void DrawPieces(bool start_new_game); //Draws 32 pieces from the game_board_ field; used at initialization (start_new_game == false) OR when the Start New Game button is pressed (start_new_game == true)
    void DrawPowerups(bool start_new_game); //Draws 6 powerups from the game_board_ field in random positions on rows 2-5; used at initailization (start_new_game == false) OR when the Start New Game button is pressed (start_new_game == true)

private slots:
    void on_howToPlayButton_clicked(); //Shows a small dialog on how to play the game. The dialog has no other options than exiting.

    void on_newGameButton_clicked(); //Resets the position of the pieces using the DrawPieces(true) call

    void UpdateGrandmasterSlot(int message); //Receive a signal from the board to update the "Grandmaster Says"; 1-5 designate pieces, 6 desginates an invalid move

    void PieceCapturedSlot(Player * player, Piece * captured_piece); //Receive a signal from the board to update a player's "Captured" box with the piece they captured

    void PowerupAcquiredSlot(Player * player, Powerup * powerup); //Receive a signal from the board to update the player's "Power-Ups" box with the powerup they acquired

    //Respond to the "Captured" lists being interacted with; only allow interactions from the player who owns the list
    void playerOneCapturedClickSlot(QListWidgetItem * captured_piece);
    void playerTwoCapturedClickSlot(QListWidgetItem * captured_piece);

    //Respond to the "Power-Ups" lists being interacted with; only allow interactions from the player who owns the list
    void playerOneListWidgetClickSlot(QListWidgetItem * powerup);
    void playerTwoListWidgetClickSlot(QListWidgetItem * powerup);

    //Respond to "Use Power-Up" buttons being used
    void on_playerOnePowerUpButton_clicked();
    void on_playerTwoPowerUpButton_clicked();

private:
    Ui::MainWindow *ui;
    HowToPlay *instructions_window_; //The dialog with instructions that pops up
    QGraphicsScene *board_scene_; //The scene where the interaction with pieces occurs

    Board * game_board_; //The game board that stores 2 players, 64 tiles, and 32 pieces

    int selected_captured_piece_ = -1; //Track the identification number for the piece that has been selected to be revived. Set to -1 originally in case no piece is selected

    int selected_powerup_ = -1; //Track the identification number for the powerup that has been selected to be uses. Set to -1 orginally in case no powerup is selected
};
#endif // MAINWINDOW_H
