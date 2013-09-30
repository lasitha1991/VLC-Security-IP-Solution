//Lasitha Weerasinghe
//100577A

#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;    
    w.show(); //this will show the window

    return a.exec();
}
