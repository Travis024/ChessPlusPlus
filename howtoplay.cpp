#include "howtoplay.h"
#include "ui_howtoplay.h"

/*
Nothing has been changed in this file
This dialog just shows a litte text on how to play the game
*/
HowToPlay::HowToPlay(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HowToPlay)
{
    ui->setupUi(this);
}

HowToPlay::~HowToPlay()
{
    delete ui;
}
