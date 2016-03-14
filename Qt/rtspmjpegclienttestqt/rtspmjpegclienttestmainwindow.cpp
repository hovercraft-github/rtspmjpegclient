#include <string>
#include <chrono>
#include <thread>
#include "rtspmjpegclienttestmainwindow.h"
#include "ui_rtspmjpegclienttestmainwindow.h"

#define TEST_ADDRESS "rtsp://192.168.1.54/live/mjpeg"

RTSPMJPEGClientTestMainWindow::RTSPMJPEGClientTestMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RTSPMJPEGClientTestMainWindow)
{
    ui->setupUi(this);

    data = new RTSPMJPEGClientData;
    data->state = RTSPMJPEGCLIENT_STATE_EMPTY;

    rtspmjpegclient_start_log("", "");
    rtspmjpegclient_set_log_level(10000); // DEBUG_LOG_LEVEL

    fps = 2;

    ui->StateLabel->setText(rtspmjpegclientns::state_to_string(data->state));
    ui->AddressLineEdit->setText(TEST_ADDRESS);
    ui->FpsLineEdit->setText(QString::number(fps));
}

RTSPMJPEGClientTestMainWindow::~RTSPMJPEGClientTestMainWindow()
{
    delete ui;

    if (data->state == RTSPMJPEGCLIENT_STATE_LOOPING)
        rtspmjpegclient_stop(0);

    rtspmjpegclient_destroy(0);

    rtspmjpegclient_stop_log();
}

void RTSPMJPEGClientTestMainWindow::on_ConnectButton_clicked()
{
    data->state = rtspmjpegclient_get_state(0);

    if (data->state < RTSPMJPEGCLIENT_STATE_INITIALIZING)
    {
        rtspmjpegclient_start(0, TEST_ADDRESS, 1);

        std::this_thread::sleep_for(std::chrono::seconds(1));

        fps = ui->FpsLineEdit->text().toInt();
        fps = std::max(fps, 1);
        timerId = startTimer(1000 / fps);

        ui->ConnectButton->setText("Disconnect");
    }
    else if (data->state == RTSPMJPEGCLIENT_STATE_LOOPING)
    {
        rtspmjpegclient_stop(0);

        killTimer(timerId);

        ui->ConnectButton->setText("Connect");
    }

    data->state = rtspmjpegclient_get_state(0);
    ui->StateLabel->setText(rtspmjpegclientns::state_to_string(data->state));
}

void RTSPMJPEGClientTestMainWindow::timerEvent(QTimerEvent *event)
{
    if (fps != ui->FpsLineEdit->text().toInt())
    {
        fps = ui->FpsLineEdit->text().toInt();
        fps = std::max(fps, 1);
        ui->FpsLineEdit->setText(QString::number(fps));

        // restart timer with the new interval
        killTimer(timerId);
        timerId = startTimer(1000 / fps);
    }

    data->state = rtspmjpegclient_get_state(0);
    ui->StateLabel->setText(rtspmjpegclientns::state_to_string(data->state));

    // stop reading
    if (rtspmjpegclient_wait(0) != 0)
        return;

    // get frame
    rtspmjpegclient_get_data(0, 1, 0, data);

    // resume reading
    rtspmjpegclient_resume(0);

    if (data->framesRead <= 0)
        return;

    // show frame
    QPixmap frame;
    if (frame.loadFromData(data->frameQueue, data->frameSizes[0]), "JPG")
        ui->FrameLabel->setPixmap(frame.scaled(frame.width() / 2, frame.height() / 2, Qt::KeepAspectRatio));
}
