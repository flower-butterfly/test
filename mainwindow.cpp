#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include "stdio.h"
#include <QString>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QDateTime>
#include <QString>
#include <QVector>
#include <QFile>
#include <QObject>
#include <windows.h>
#include <iostream>
#include <cv.h>
#include <opencv/cxcore.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <qdockwidget.h>
#include <QDesktopWidget>
#include <QPainter>
#include <math.h>
#include <QtCore/QTime>
#include <QMessageBox>
#include <QLibrary>
#include "videoplayer.h"


using namespace cv;
using namespace std;

QVector<CANData> q_CanData;
CANARRAY buffer;
double last_time;
QString strTime_tmp;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),driver(2),
    ui(new Ui::MainWindow)
{
    setFixedSize(1300,700);

    ui->setupUi(this);
    ui->pushButton->setText("回放视频");

    cam = NULL;
    cnt = 0;
    lineCnt = 0;

    timerCamera = new QTimer(this);
    connect(timerCamera,SIGNAL(timeout()),this,SLOT(readFrame()));  //时间到，读取当前摄像头信息

    ui->label->setGeometry(0,0,this->width(),this->height());
//    ui->groupBox->setFixedHeight(150);
//    ui->groupBox->setFixedWidth(240);
//    QRect deskRect = QApplication::desktop()->availableGeometry();
//    ui->groupBox->move(deskRect.right()-250, 0);


    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::blue);
    ui->groupBox->setPalette(pe);
    ui->x->setPalette(pe);
    ui->y->setPalette(pe);
    ui->z->setPalette(pe);
    ui->tar_x->setPalette(pe);
    ui->tar_y->setPalette(pe);
    ui->cam_f->setPalette(pe);

    ui->label->setScaledContents(true);

    memset(&data,0,sizeof(data));
    memset(&buffer,0,sizeof(CANARRAY));

    ui->slider_x->setMinimum(-2000);
    ui->slider_x->setMaximum(2000);
    ui->slider_y->setMinimum(-5000);
    ui->slider_y->setMaximum(5000);
    ui->slider_z->setMinimum(0);
    ui->slider_z->setMaximum(5000);
    ui->slider_tarX->setMinimum(0);
    ui->slider_tarX->setMaximum(2000);
    ui->slider_tarX->setValue(1000);
    ui->slider_tarY->setMinimum(463-5000);
    ui->slider_tarY->setMaximum(463+5000);
    ui->slider_tarY->setValue(463);
    ui->slider_camF->setMinimum(0);
    ui->slider_camF->setMaximum(15000);
    ui->slider_camF->setValue(1300);



    reader = new CanReader(0);
    ZLG_reader = new ReceiveTest(4,0,0);

    connect(ZLG_reader,SIGNAL(mySignal()),this,SLOT(getSignal()));

}

void MainWindow::getSignal()
{
    memcpy(&buffer,&(ZLG_reader->can_array_value),sizeof(CANARRAY));
}


//打开摄像头
void MainWindow::openCamera()
{
    cam = cvCreateCameraCapture(0);
    timerCamera->start(50);
}

//读取视频帧
void MainWindow::readFrame()
{
    frame = cvQueryFrame(cam);  //从摄像头中抓取并返回一帧
    if(!frame)
        return;

    //视频帧处理
    if(driver == 0)
        DataProcess(frame);
    else if(driver == 1)
        DataProcess(frame);

    CvSize size = cvSize(
                (int)cvGetCaptureProperty(cam, CV_CAP_PROP_FRAME_WIDTH),
                (int)cvGetCaptureProperty(cam, CV_CAP_PROP_FRAME_HEIGHT)
                );
    frm = cvCreateImage(size,8,3);
    cvCopy(frame,frm);
    //cvShowImage("aa",frm);

    //将抓取的帧转换为QImage格式
    image = QImage((const uchar*)frame->imageData, frame->width, frame->height, QImage::Format_RGB888).rgbSwapped();
    ui->label->setPixmap(QPixmap::fromImage(image));  // 将图片显示到label上
}

//开始录像--处理后的帧
void MainWindow::StartRecord(IplImage* frame,char *filename)
{
    //CvVideoWriter* video = NULL;
    video = NULL;
    frame = NULL;

    cvNamedWindow("Record_Video",CV_WINDOW_NORMAL);
    HWND hWnd = (HWND)cvGetWindowHandle("Record_Video");
    HWND  hRWnd = ::GetParent(hWnd);
    ShowWindow(hRWnd,0);
    //ShowWindow(hWnd,0);   //隐藏子窗口

    if(!cam)
    {
        //qDebug() << "Can not open the camera.";
        QMessageBox::warning(this,tr("can not open the camera"),
                             tr("Can not open the camera."));
    }
    else
    {
        frame = cvQueryFrame(cam);

        //创建CvVedioWriter对象并分配空间,编解码器格式是XVID，帧率设置为25
        video = cvCreateVideoWriter(filename,CV_FOURCC('X','V','I','D'),
                                    25,cvSize(frm->width,frm->height));

        //保存的文件名为yyyy-MM-dd_hh_mm_ss.ts，编码要在运行程序时选择,大小就是摄像头视频的大小,帧率是25
        if(video)   //如果能创建CvVideoWriter对象则表明成功
        {
            qDebug() << "VideoWriter has created .";
        }

        int i = 0;
        while(1/*i<=200*/)
        {
            frame = cvQueryFrame(cam);
            if(!frame)
            {
                qDebug() << "can not get frame from the capture.";  //判断写入是否成功,如果返回的是1,表示写入成功
                break;
            }
            cvWriteFrame(video,frm);
            i++;
            if(cvWaitKey(2)>0)
                break;          //有其他键盘响应,则退出
        }

        cvDestroyAllWindows();
        cvReleaseVideoWriter(&video);
    }
}


void MainWindow::startRecord_rawFrame(IplImage* frame,char* filename)
{
    //CvVideoWriter* video = NULL;
    video = NULL;
    frame = NULL;

    cvNamedWindow("Record_Video",CV_WINDOW_NORMAL);
    HWND hWnd = (HWND)cvGetWindowHandle("Record_Video");
    HWND  hRWnd = ::GetParent(hWnd);
    ShowWindow(hRWnd,0);
    //ShowWindow(hWnd,0);   //隐藏子窗口

    if(!cam)
    {
        //qDebug() << "Can not open the camera.";
        QMessageBox::warning(this,tr("can not open the camera"),
                             tr("Can not open the camera."));
    }
    else
    {
        frame = cvQueryFrame(cam);



        //创建CvVedioWriter对象并分配空间,编解码器格式是XVID，帧率设置为25
        video = cvCreateVideoWriter(filename,CV_FOURCC('X','V','I','D'),
                                    25,cvSize(frame->width,frame->height));

        //保存的文件名为yyyy-MM-dd_hh_mm_ss.ts，编码要在运行程序时选择,大小就是摄像头视频的大小,帧率是25
        if(video)   //如果能创建CvVideoWriter对象则表明成功
        {
            qDebug() << "VideoWriter has created .";
        }

        int i = 0;
        while(1/*i<=200*/)
        {
            frame = cvQueryFrame(cam);
            if(!frame)
            {
                qDebug() << "can not get frame from the capture.";  //判断写入是否成功,如果返回的是1,表示写入成功
                break;
            }
            //int n = cvWriteFrame(video,frame);
            cvWriteFrame(video,frame);
            i++;
            if(cvWaitKey(2)>0)
                break;          //有其他键盘响应,则退出
        }

        cvDestroyAllWindows();
        cvReleaseVideoWriter(&video);
    }
}


//开始记录数据文件和视频文件
void MainWindow::on_StartRecord_triggered()
{
    //    if(driver==0?(reader->v_readmsg.isEmpty()):(ZLG_reader->zlg_readmsg.isEmpty()))
    if(driver == 2/*clickNum == 0*/ )
    {
        QMessageBox::warning(this,tr("未选择CAN卡"),tr("未选择CAN卡"));
        return;
    }
    switch (driver) {
    case 0:
        if(reader->v_readmsg.isEmpty())
        {
            QMessageBox::warning(this,tr("不存储文件"),tr("CAN没有数据"));
            return;
        }
        break;
    case 1:
        if(ZLG_reader->zlg_readmsg.isEmpty())
        {
            QMessageBox::warning(this,tr("不存储文件"),tr("CAN没有数据"));
            return;
        }
    default:
        QMessageBox::warning(this,tr("未选择CAN卡"),tr("未选择CAN卡"));
        return;
        break;
    }

    time = QDateTime::currentDateTime();            //获取系统现在时间
    strTime = time.toString("yy-MM-dd_hh_mm_ss");   //设置显示格式
    RecordTime = strTime;

    QDir *t = new QDir;
    bool exist = t->exists(strTime);
    if(exist)
    {
        QMessageBox::warning(this,tr("create a file"),tr("file had existed!"));
    }
    else
    {
        bool ok = t->mkdir(strTime);
        if(ok)
        {
            //QMessageBox::warning(this,tr("create a file"),tr("file create succeed!"));
            //strTime = t->currentPath() + "/" + strTime;
        }

    }

    gDirectory = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("select save file path"), t->currentPath()/*QDir::currentPath()*/));
    if (!gDirectory.isEmpty())
    {
        strTime = gDirectory + "\\" + strTime;
    }
    else{}


    QByteArray tmp[3] ;
    tmp[0]= strTime.toLatin1();
    QString strTmp =  strTime + "_processFrame.ts";
    tmp[1] = strTmp.toLatin1();
    QString strTmp1 =  strTime + "_rowFrame.ts";
    tmp[2] = strTmp1.toLatin1();


    //处理数据 保存到文件
//    if(driver==0)
//        reader->saveFile(tmp[0].data());
//    else
//        ZLG_reader->saveFile(tmp[0].data(),Useingtime);


    StartRecord(frame,tmp[1].data());
    //startRecord_rawFrame(frame,tmp[2].data());   //???原始帧和处理帧同时保存存在问题



}
//停止记录数据文件和视频文件
void MainWindow::on_StopRecord_triggered()
{
    on_CloseCamera_triggered();

    if(driver==0)
    {
        reader->v_readmsg.clear();
    }
    else
    {
        ZLG_reader->zlg_readmsg.clear();
    }
}


//回放本地视频文件
void MainWindow::on_OpenFile_triggered()
{



    IplImage *frame;
    QImage image;
    CvCapture *pCapture;
    QString filename;
    QTime t;
    QTime t_t;

    filename = QFileDialog::getOpenFileName(this,tr("选择文件"),"/",tr("Vedio(*.avi *.mp4 *.ts)"));
    if(filename.isEmpty())
    {
        return;
    }

    QByteArray aa = filename.toLatin1();
     char *file = aa.data();

    pCapture = cvCaptureFromFile(file);
    cvNamedWindow("PlayBack",CV_WINDOW_NORMAL);
    HWND hWnd = (HWND)cvGetWindowHandle("PlayBack");
    HWND  hRWnd = ::GetParent(hWnd);
    ShowWindow(hRWnd,0);
    int aviFramePerSec       = (int) cvGetCaptureProperty(pCapture, CV_CAP_PROP_FPS);
    int aviTotalFrames = (int) cvGetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_COUNT);

    aviTotalFrames = aviTotalFrames - 1;//会比实际多1帧，why？
    qDebug()<<aviFramePerSec;
    qDebug()<<aviTotalFrames;

    int canCurFrame = 0;
    int static canTotalFrames = q_CanData.count();

    t_t.start();


    while(1)
    {
        t.start();
        frame = cvQueryFrame(pCapture);
        if(frame){

            int aviCurFrame = (int) cvGetCaptureProperty(pCapture, CV_CAP_PROP_POS_FRAMES);
//            qDebug()<<"aviCurFrame:"<<aviCurFrame;

            double coffe = (double)aviCurFrame/(double)aviTotalFrames;

            canCurFrame = int(double(canTotalFrames)*double(coffe));
            canCurFrame = canCurFrame/64;
//            qDebug()<<"coffe:"<<coffe<<"canCurFrame:"<<canCurFrame;
            process(frame,canCurFrame);

            image = QImage((const uchar*)frame->imageData, frame->width, frame->height,QImage::Format_RGB888).rgbSwapped();
            image = image.scaled(ui->label->width(), ui->label->height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            ui->label->setPixmap(QPixmap::fromImage(image));

            //cvShowImage("PlayBack",frame);
            cvWaitKey(1);
            qDebug("Time elapsed: %d ms", t.elapsed());
        }
    }

    qDebug("avi avi avi tatol Time elapsed: %d ms", t_t.elapsed());
    qDebug()<<"视频回放结束";
    cvDestroyAllWindows();
    cvReleaseCapture(&pCapture);
}


//打开摄像头
void MainWindow::on_OpenCamera_triggered()
{
    openCamera();
}

//关闭摄像头
void MainWindow::on_CloseCamera_triggered()
{
    timerCamera->stop();           //停止读取数据
    cvReleaseCapture(&cam);  // 释放内存
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label->setText(tr("No Video"));
}


//打开CAN数据文件
void MainWindow::OpenDataFile(CANData data)
{
    QString filename;
    filename = QFileDialog::getOpenFileName(NULL,QObject::tr("选择文件"),"/",QObject::tr("Text(*.txt)"));
    QFile file(filename);
    if(filename == NULL)
    {
        return;
    }

    //按行读取文件
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        char buf[2048];
        while(!file.atEnd())
        {
            qint64 lineLen = file.readLine(buf,sizeof(buf));
            if(lineLen != -1)
            {
                QString str = QString(QLatin1String(buf));
                if(&data == NULL)
                    return;

                str = str.simplified();
                int len = str.length();

                QVector<QString> s;
                int start = 0, end = 0;
                for(int i=0; i<len; i++)
                {
                    if(str.at(i).isSpace())
                    {
                        end = i;
                        s.push_back(str.mid(start,end-start));   //
                        start = i + 1;
                    }
                }
                s.push_back(str.mid(start,len));
                data.timestamp = s.takeFirst().toLongLong();
                data.id = s.takeFirst().toInt();
                data.speed = s.takeFirst().toDouble();
                data.angle = s.takeFirst().toDouble();
                data.distance = s.takeFirst().toDouble();
                data.isTarExsit = (bool)(s.takeFirst().toDouble());

                s.clear();
                q_CanData.push_back(data);
            }
        }
    }

    lineCnt = q_CanData.count();
}

//本地视频回放----叠加的数据文件的选择
void MainWindow::on_OpenDataFile_triggered()
{
    OpenDataFile(data);
}

//回放时的数据叠加
void MainWindow::process(IplImage *frame,int canCurFrame)
{
    pos_horizontal = ui->slider_x->value();
    pos_depth = ui->slider_y->value();
    pos_vertical = ui->slider_z->value();
    tar_x = ui->slider_tarX->value();
    tar_y = ui->slider_tarY->value();
    cam_f = ui->slider_camF->value();

    drawTarInfo(frame,canCurFrame);

}


void MainWindow::drawTarInfo(IplImage *frame,int frameth)
{
    bool isTarExsit = false;

    for(int i=0; i < TAR_NUM_MAX; i++){

        isTarExsit = q_CanData.at(frameth*64).isTarExsit;

        if(isTarExsit){

            float theta = q_CanData.at(i + frameth*TAR_NUM_MAX).angle * M_PI/180;
            float dis = q_CanData.at(i + frameth*TAR_NUM_MAX).distance;
//            qDebug()<<dis;
//            qDebug()<<theta;
//            qDebug()<<q_CanData.at(i + frameth*TAR_NUM_MAX).speed;
//            qDebug()<<q_CanData.at(i + frameth*TAR_NUM_MAX).isTarExsit;
//            qDebug()<<q_CanData.at(i + frameth*TAR_NUM_MAX).id;
            float x3 = dis*cos(theta) + float(pos_depth/1000.0f) + eps;
            float x1 = dis*sin(theta) + float(pos_horizontal/1000.0f) + eps;
            float x2 = pos_vertical/1000.0f + eps;

            float k = cam_f/x3;

            int y1 = round(k*x1);
            int y2 = round(k*x2);

            int x = tar_x + y1;
            int y = tar_y + y2;
            int z = 20;

            Point pt1 = cv::Point(x-z/2,y-z/2);
            Point pt2 = cv::Point(x+z/2,y+z/2);
            if(abs(q_CanData.at(i + frameth*64).speed) >= 0.5f)
            {
                cvRectangle(frame,pt1,pt2,cv::Scalar(0,0,200),2,8,0);
            }
            else
            {
                cvLine(frame,cv::Point(x-6,y),cv::Point(x+6,y),cv::Scalar(255,0,0),1,8,0);
                cvLine(frame,cv::Point(x,y-6),cv::Point(x,y+6),cv::Scalar(255,0,0),1,8,0);
            }
        }
    }
}
//视频实时的数据叠加
void MainWindow::DataProcess(IplImage *frame)
{
    //---------------视频帧基数、数据帧计数、存取文件时间的显示----------------//
    CvFont font;
    cvInitFont(&font,CV_FONT_HERSHEY_COMPLEX,0.5,0.5,0,0.2,8);

    //视频帧计数
    QString str = QString::number(cnt++,10);
    QByteArray ch = str.toLatin1();
    const char *p = ch.data();
    cvPutText(frame,"Video_FrameCnt: ",cvPoint(10,30),&font,CV_RGB(255,0,0));
    cvPutText(frame,p,cvPoint(160,30),&font,CV_RGB(255,0,0));

    //数据帧计数
    QString str1 ;//= QString::number(driver==0?reader->j:0,10);//driver==0?(reader->v_readmsg.count()):(ZLG_reader->zlg_readmsg.count())

    if(driver == 0)
    {
        str1 = QString::number(reader->j,10);
    }
    else if(driver == 1)
    {
        str1 = QString::number(ZLG_reader->j,10);
    }
    else
    {
        str1 = QString::number(0,10);
    }

    QByteArray ch1 = str1.toLatin1();
    const char *p1 = ch1.data();
    cvPutText(frame,"Data_FrameCnt: ",cvPoint(10,60),&font,CV_RGB(255,0,0));
    cvPutText(frame,p1,cvPoint(160,60),&font,CV_RGB(255,0,0));

    //数据开始存储时间
    QByteArray ch2 = RecordTime.toLatin1();
    const char *p2 = ch2.data();
    cvPutText(frame,"Timer: ",cvPoint(10,90),&font,CV_RGB(255,0,0));
    cvPutText(frame,p2,cvPoint(70,90),&font,CV_RGB(255,0,0));
    //-----------------------------------------------------------//



   // if(v_readmsg.isEmpty()) return;   //判断结构体数组是否为空??
    //if(buffer == NULL) return;

    CANValue canVal;
    CANValue zlg_canVal;

    pos_horizontal = ui->slider_x->value();
    pos_depth = ui->slider_y->value();
    pos_vertical = ui->slider_z->value();
    tar_x = ui->slider_tarX->value();
    tar_y = ui->slider_tarY->value();
    cam_f = ui->slider_camF->value();


    for(int i = 0;i < TAR_NUM_MAX;i++)
    {
        bool isTarget = 0;
        float theta = 0;
        float dis = 0;
        if(driver == 0)
        {
            //memcpy(&canVal,&(v_readmsg.last().CANDATA[i]),sizeof(CANValue));
            memcpy(&canVal,&(buffer.CANDATA[i]),sizeof(CANValue));
            isTarget = bool(canVal.isTarExsit);
            if(isTarget != 0)
            {
                theta = canVal.angle * M_PI/180;
                dis = canVal.range;
            }
        }
        else if(driver == 1)
        {
            //memcpy(&zlg_canVal,&(v_readmsg.last().CANDATA[i]),sizeof(CANValue));
            memcpy(&zlg_canVal,&(buffer.CANDATA[i]),sizeof(CANValue));
            isTarget = bool(zlg_canVal.isTarExsit);
            if(isTarget != 0)
            {
                theta = canVal.angle * M_PI/180;
                dis = canVal.range;
            }
        }
        float x3 = dis*cos(theta) + float(pos_depth/1000.0f) + eps;
        float x1 = dis*sin(theta) + float(pos_horizontal/1000.0f) + eps;
        float x2 = pos_vertical/1000.0f + eps;

        float k = cam_f/x3;

        int y1 = round(k*x1);
        int y2 = round(k*x2);

        int x = tar_x + y1;
        int y = tar_y + y2;
        int z = 20;

        Point pt1 = cv::Point(x-z/2,y-z/2);
        Point pt2 = cv::Point(x+z/2,y+z/2);

       // qDebug()<<"aaa";
        double valocity=(driver==0?(canVal.valocity):(zlg_canVal.valocity));
        if(abs(valocity) >=0.5f)
        {
            cvRectangle(frame,pt1,pt2,cv::Scalar(255,0,0),2,8,0);
            //qDebug()<<"zjl1";
        }
        else
        {
            cvLine(frame,cv::Point(x-6,y),cv::Point(x+6,y),cv::Scalar(255,0,0),1,8,0);
            cvLine(frame,cv::Point(x,y-6),cv::Point(x,y+6),cv::Scalar(255,0,0),1,8,0);
            //qDebug()<<"zjl2";
        }


    }

    //delete buffer;
    //buffer = NULL;
}


/*********************************************************/
/*----------------------滑动条功能代码---------------------*/
/********************************************************/
//camera_x
void MainWindow::on_slider_x_valueChanged(int value)
{
    pos_horizontal = value;
}

//camera_y
void MainWindow::on_slider_y_valueChanged(int value)
{
    pos_depth = value;
}

//camera_z
void MainWindow::on_slider_z_valueChanged(int value)
{
    pos_vertical = value;
}

//target_x
void MainWindow::on_slider_tarX_valueChanged(int value)
{
    tar_x = value;
}

//target_y
void MainWindow::on_slider_tarY_valueChanged(int value)
{
    tar_y = value;
}

//Camera_f
void MainWindow::on_slider_camF_valueChanged(int value)
{
    cam_f = value;
}

MainWindow::~MainWindow()
{
    if(driver==0)
    {
        reader->get_can_status();//关闭通道
    }
    else if(driver == 1)
    {
        ZLG_reader->get_can_status();
    }

    if(reader->isRunning())
    {
        reader->terminate();
    }
    delete reader;
    reader = NULL;

    if(ZLG_reader->isRunning())
    {
        ZLG_reader->terminate();
    }
    delete ZLG_reader;
    ZLG_reader = NULL;

    cvReleaseCapture(&cam);
    delete frame;

    timerCamera->stop();

    delete ui;
}


//黑盒
void MainWindow::on_ESRCan_triggered()
{
    driver = 0;
    qDebug()<< "ESRCAN";
    if(reader->init_can())
    {
        reader->start();
    }
}

//白盒
void MainWindow::on_ZLGCan_triggered()
{
    driver = 1;
    qDebug() << "ZLGCAN";
    if(ZLG_reader->init_can())
    {
        ZLG_reader->start();
    }

   // ZLG_reader->stop();
 }


void MainWindow::close()
{

    exit(0);
}

void MainWindow::on_pushButton_clicked()
{

    const QRect availableGeometry = QApplication::desktop()->availableGeometry(&player);
    player.resize(availableGeometry.width() , availableGeometry.height() );
    player.show();

}
