#include "mainwindow.h"
#include <QApplication>
#include <qdatetime.h>
#include <QDebug>
#include "receivetest.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
