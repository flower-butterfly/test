#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QTimer>
#include <highgui.h>
#include <cv.h>
#include <opencv.hpp>
#include <cxcore.h>
#include "opencv2/core/types_c.h"
#include <QPaintEvent>
#include <canreader.h>
#include <receivetest.h>
#include <QDateTime>
#include <stdio.h>
#include <QThread>
#include "videoplayer.h"


#define eps (0.0001)
using namespace std;



typedef struct CANData{
    LONGLONG timestamp;
    int id;
    double speed;
    double angle;
    double distance;
    bool isTarExsit;
}CANData;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:
    QTimer *timerCamera;
    CvCapture *cam;
    IplImage *frame;
private:
    Ui::MainWindow *ui;

    IplImage *frm;
    CvPoint pt1;
    CvPoint pt2;
    bool visible;

    CANData data;

    int pos_horizontal;
    int pos_depth;
    int pos_vertical;
    int cam_f;
    int tar_x;
    int tar_y;
    int cnt;
    qint64  lineCnt;
    QString filename;
    QDateTime time;

    QString gDirectory;
    QString RecordTime;

    int driver;//0--ESRCAN黑盒   1--ZLGCan白盒

private:
    void openCamera();
    void OpenDataFile(CANData data);
    void StartRecord(IplImage* frame,char* filename);
    void startRecord_rawFrame(IplImage* frame,char* filename);
    void drawTarInfo(IplImage* frame,int frameth);
    void close();
    void process(IplImage *frame,int canCurFrame);
private slots:
    void readFrame();
    void on_OpenFile_triggered();
    void on_StartRecord_triggered();
    void on_StopRecord_triggered();
    void on_CloseCamera_triggered();
    void on_OpenCamera_triggered();
//    void process(IplImage *frame,int canCurFrame);
    void on_slider_x_valueChanged(int value);
    void on_slider_y_valueChanged(int value);
    void on_slider_z_valueChanged(int value);
    void on_slider_tarX_valueChanged(int value);
    void on_slider_tarY_valueChanged(int value);
    void on_slider_camF_valueChanged(int value);
    void DataProcess(IplImage *frame);
    void on_OpenDataFile_triggered();
    void on_ESRCan_triggered();
    void on_ZLGCan_triggered();

    void on_pushButton_clicked();

public slots:
    void getSignal();
public:

    CvVideoWriter* video;
    CanReader* reader;         //黑盒
    ReceiveTest* ZLG_reader;   //白盒
    QString strTime;
    QImage image;
    QImage image2;
    VideoPlayer player;
};




#endif // MAINWINDOW_H
