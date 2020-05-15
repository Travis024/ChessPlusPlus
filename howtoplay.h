#ifndef HOWTOPLAY_H
#define HOWTOPLAY_H

#include <QDialog>

/*
No changes made to this class other that what Qt gives the dialog files at initialization
This just pops up a little window explaning how to play the game
*/
namespace Ui {
class HowToPlay;
}

class HowToPlay : public QDialog
{
    Q_OBJECT

public:
    explicit HowToPlay(QWidget *parent = nullptr);
    ~HowToPlay();

private:
    Ui::HowToPlay *ui;
};

#endif // HOWTOPLAY_H
