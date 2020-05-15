#include "mainwindow.h"

#include <QApplication>
#include <ctime>

int main(int argc, char *argv[])
{
    srand(time(NULL)); //used for randomizing the location of powerups when drawn on the board
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
