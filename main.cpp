#include "mainwindow.h"

#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
	// Set icon
	w.setWindowIcon(QIcon("logo.ico"));

    w.show();

    return a.exec();
}
