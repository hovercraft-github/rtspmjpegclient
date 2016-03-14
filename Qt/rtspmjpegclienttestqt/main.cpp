#include "rtspmjpegclienttestmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RTSPMJPEGClientTestMainWindow w;
    w.show();

    return a.exec();
}
