/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *OpenFile;
    QAction *StartRecord;
    QAction *StopRecord;
    QAction *OpenCamera;
    QAction *CloseCamera;
    QAction *OpenDataFile;
    QAction *ESRCan;
    QAction *ZLGCan;
    QWidget *centralWidget;
    QLabel *label;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *x;
    QSlider *slider_x;
    QHBoxLayout *horizontalLayout_2;
    QLabel *y;
    QSlider *slider_y;
    QHBoxLayout *horizontalLayout_3;
    QLabel *z;
    QSlider *slider_z;
    QHBoxLayout *horizontalLayout_4;
    QLabel *tar_x;
    QSlider *slider_tarX;
    QHBoxLayout *horizontalLayout_5;
    QLabel *tar_y;
    QSlider *slider_tarY;
    QHBoxLayout *horizontalLayout_6;
    QLabel *cam_f;
    QSlider *slider_camF;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QMenu *menuRecord;
    QMenu *menuPlayback;
    QMenu *menuCameraCfg;
    QMenu *menuDriver;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(518, 416);
        MainWindow->setDockNestingEnabled(false);
        OpenFile = new QAction(MainWindow);
        OpenFile->setObjectName(QStringLiteral("OpenFile"));
        StartRecord = new QAction(MainWindow);
        StartRecord->setObjectName(QStringLiteral("StartRecord"));
        StopRecord = new QAction(MainWindow);
        StopRecord->setObjectName(QStringLiteral("StopRecord"));
        OpenCamera = new QAction(MainWindow);
        OpenCamera->setObjectName(QStringLiteral("OpenCamera"));
        CloseCamera = new QAction(MainWindow);
        CloseCamera->setObjectName(QStringLiteral("CloseCamera"));
        OpenDataFile = new QAction(MainWindow);
        OpenDataFile->setObjectName(QStringLiteral("OpenDataFile"));
        ESRCan = new QAction(MainWindow);
        ESRCan->setObjectName(QStringLiteral("ESRCan"));
        ZLGCan = new QAction(MainWindow);
        ZLGCan->setObjectName(QStringLiteral("ZLGCan"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 0, 511, 391));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(20);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setAutoFillBackground(true);
        label->setAlignment(Qt::AlignCenter);
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setEnabled(true);
        groupBox->setGeometry(QRect(95, 110, 211, 141));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        x = new QLabel(groupBox);
        x->setObjectName(QStringLiteral("x"));
        QFont font1;
        font1.setFamily(QStringLiteral("Times New Roman"));
        font1.setPointSize(10);
        x->setFont(font1);

        horizontalLayout->addWidget(x);

        slider_x = new QSlider(groupBox);
        slider_x->setObjectName(QStringLiteral("slider_x"));
        slider_x->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(slider_x);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        y = new QLabel(groupBox);
        y->setObjectName(QStringLiteral("y"));
        y->setFont(font1);

        horizontalLayout_2->addWidget(y);

        slider_y = new QSlider(groupBox);
        slider_y->setObjectName(QStringLiteral("slider_y"));
        slider_y->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(slider_y);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        z = new QLabel(groupBox);
        z->setObjectName(QStringLiteral("z"));
        z->setFont(font1);

        horizontalLayout_3->addWidget(z);

        slider_z = new QSlider(groupBox);
        slider_z->setObjectName(QStringLiteral("slider_z"));
        slider_z->setOrientation(Qt::Horizontal);

        horizontalLayout_3->addWidget(slider_z);


        gridLayout->addLayout(horizontalLayout_3, 2, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        tar_x = new QLabel(groupBox);
        tar_x->setObjectName(QStringLiteral("tar_x"));
        tar_x->setFont(font1);

        horizontalLayout_4->addWidget(tar_x);

        slider_tarX = new QSlider(groupBox);
        slider_tarX->setObjectName(QStringLiteral("slider_tarX"));
        slider_tarX->setOrientation(Qt::Horizontal);

        horizontalLayout_4->addWidget(slider_tarX);


        gridLayout->addLayout(horizontalLayout_4, 3, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        tar_y = new QLabel(groupBox);
        tar_y->setObjectName(QStringLiteral("tar_y"));
        tar_y->setFont(font1);

        horizontalLayout_5->addWidget(tar_y);

        slider_tarY = new QSlider(groupBox);
        slider_tarY->setObjectName(QStringLiteral("slider_tarY"));
        slider_tarY->setOrientation(Qt::Horizontal);

        horizontalLayout_5->addWidget(slider_tarY);


        gridLayout->addLayout(horizontalLayout_5, 4, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        cam_f = new QLabel(groupBox);
        cam_f->setObjectName(QStringLiteral("cam_f"));
        cam_f->setFont(font1);

        horizontalLayout_6->addWidget(cam_f);

        slider_camF = new QSlider(groupBox);
        slider_camF->setObjectName(QStringLiteral("slider_camF"));
        slider_camF->setOrientation(Qt::Horizontal);

        horizontalLayout_6->addWidget(slider_camF);


        gridLayout->addLayout(horizontalLayout_6, 5, 0, 1, 1);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(110, 20, 75, 23));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 518, 23));
        menuRecord = new QMenu(menuBar);
        menuRecord->setObjectName(QStringLiteral("menuRecord"));
        menuPlayback = new QMenu(menuBar);
        menuPlayback->setObjectName(QStringLiteral("menuPlayback"));
        menuCameraCfg = new QMenu(menuBar);
        menuCameraCfg->setObjectName(QStringLiteral("menuCameraCfg"));
        menuDriver = new QMenu(menuBar);
        menuDriver->setObjectName(QStringLiteral("menuDriver"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuCameraCfg->menuAction());
        menuBar->addAction(menuDriver->menuAction());
        menuBar->addAction(menuPlayback->menuAction());
        menuBar->addAction(menuRecord->menuAction());
        menuRecord->addAction(StartRecord);
        menuRecord->addAction(StopRecord);
        menuPlayback->addAction(OpenFile);
        menuPlayback->addAction(OpenDataFile);
        menuCameraCfg->addAction(OpenCamera);
        menuCameraCfg->addAction(CloseCamera);
        menuDriver->addAction(ESRCan);
        menuDriver->addAction(ZLGCan);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Camera", 0));
        OpenFile->setText(QApplication::translate("MainWindow", "OpenFile", 0));
        StartRecord->setText(QApplication::translate("MainWindow", "StartRecord", 0));
        StopRecord->setText(QApplication::translate("MainWindow", "StopRecord", 0));
        OpenCamera->setText(QApplication::translate("MainWindow", "OpenCamera", 0));
        CloseCamera->setText(QApplication::translate("MainWindow", "CloseCamera", 0));
        OpenDataFile->setText(QApplication::translate("MainWindow", "OpenDataFile", 0));
        ESRCan->setText(QApplication::translate("MainWindow", "ESRCan", 0));
        ZLGCan->setText(QApplication::translate("MainWindow", "ZLGCan", 0));
        label->setText(QString());
        groupBox->setTitle(QApplication::translate("MainWindow", "CameraConfig", 0));
        x->setText(QApplication::translate("MainWindow", "\346\221\204\345\203\217\345\244\264\346\250\252\345\220\221\350\260\203\346\225\264", 0));
        y->setText(QApplication::translate("MainWindow", "\346\221\204\345\203\217\345\244\264\347\272\265\345\220\221\350\260\203\346\225\264", 0));
        z->setText(QApplication::translate("MainWindow", "\346\221\204\345\203\217\345\244\264\351\253\230\344\275\216\350\260\203\346\225\264", 0));
        tar_x->setText(QApplication::translate("MainWindow", "\347\224\273\351\235\242\346\250\252\345\220\221\350\260\203\346\225\264", 0));
        tar_y->setText(QApplication::translate("MainWindow", "\347\224\273\351\235\242\347\272\265\345\220\221\350\260\203\346\225\264", 0));
        cam_f->setText(QApplication::translate("MainWindow", "\347\204\246\350\267\235F", 0));
        pushButton->setText(QApplication::translate("MainWindow", "PushButton", 0));
        menuRecord->setTitle(QApplication::translate("MainWindow", "Record", 0));
        menuPlayback->setTitle(QApplication::translate("MainWindow", "Playback", 0));
        menuCameraCfg->setTitle(QApplication::translate("MainWindow", "CameraCfg", 0));
        menuDriver->setTitle(QApplication::translate("MainWindow", "Driver", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
