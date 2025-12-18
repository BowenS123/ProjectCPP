// Vraag 2: clean main (i.e. nothing in the main that should be in a class)
// Vraag 3: no globals, but statics if needed
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
