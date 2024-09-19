#include "mainwindow.h"
#include <QWidget>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget qwidget1;
    qwidget1.setWindowTitle("my first program");
    qwidget1.setMinimumHeight(500);
    qwidget1.setMinimumWidth(500);


    qwidget1.show();
    return a.exec();
}
