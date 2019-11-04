#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("French Conjugator v.01");
    w.resize(400,200);
    w.show();
    return a.exec();
}
