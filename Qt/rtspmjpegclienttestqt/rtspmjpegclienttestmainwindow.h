#ifndef RTSPMJPEGCLIENTTESTMAINWINDOW_H
#define RTSPMJPEGCLIENTTESTMAINWINDOW_H

#include <QMainWindow>
#include "rtspmjpegclient.hpp"
#include "rtspmjpegclient_interface.hpp"

namespace Ui {
class RTSPMJPEGClientTestMainWindow;
}

class RTSPMJPEGClientTestMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RTSPMJPEGClientTestMainWindow(QWidget *parent = 0);
    ~RTSPMJPEGClientTestMainWindow();

private slots:
    void on_ConnectButton_clicked();

private:
    Ui::RTSPMJPEGClientTestMainWindow *ui;
    RTSPMJPEGClientData *data;
    int timerId;
    int fps;

protected:
    void timerEvent(QTimerEvent *event);
};

#endif // RTSPMJPEGCLIENTTESTMAINWINDOW_H
