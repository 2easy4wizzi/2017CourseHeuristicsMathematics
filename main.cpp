#include "mainwindow.h"
#include <QApplication>
#include "BandB.h"

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
    Q_UNUSED(argc);
    Q_UNUSED(argv);
//    MainWindow w;
//    w.show();
    BandB* inc = new BandB();
    Q_UNUSED(inc);
    return 0;
//    return a.exec();
}
