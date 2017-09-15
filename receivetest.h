#ifndef RECEIVCETEST_H
#define RECEIVCETEST_H
#define TAR_NUM_MAX 64

#include <include/ControlCAN.h>
#include <windows.h>
#include <QtCore>
#include <QThread>
#include <QVector>
#include "datastruct.h"

//QSemaphore freeBytes(sizeof(CANARRAY));//64*CANMsg   空闲区域
//QSemaphore usedBytes(0);//已经使用区域
//
typedef DWORD(__stdcall VCI_OpenDevice1)(DWORD,DWORD,DWORD);
typedef DWORD(__stdcall VCI_ResetCAN1)(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);
typedef DWORD(__stdcall VCI_CloseDevice1)(DWORD DeviceType,DWORD DeviceInd);
typedef DWORD(__stdcall VCI_InitCAN1)(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, PVCI_INIT_CONFIG pInitConfig);
typedef DWORD(__stdcall VCI_StartCAN1)(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);
typedef ULONG(__stdcall VCI_Transmit1)(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,PVCI_CAN_OBJ pSend,ULONG Len);
typedef ULONG(__stdcall VCI_Receive1)(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,PVCI_CAN_OBJ pReceive,ULONG Len,INT WaitTime/*=-1*/);
typedef ULONG(__stdcall VCI_GetReceiveNum1)(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);
typedef DWORD(__stdcall VCI_ClearBuffer1)(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);
typedef DWORD(__stdcall VCI_ReadErrInfo1)(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,PVCI_ERR_INFO pErrInfo);
typedef DWORD(__stdcall VCI_ReadCANStatus1)(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,PVCI_CAN_STATUS pCANStatus);

//


class ReceiveTest:public QThread
{
    Q_OBJECT

public:
    ~ReceiveTest();
    ReceiveTest(){}
    ReceiveTest(int type,int index,int canIndex);
    bool init_can(void);
    bool get_can_status(void);
    void run();
    //bool Send(int i);
    bool saveFile(QString fileName,LONGLONG usetime);
    bool DealData(VCI_CAN_OBJ* temp,CANValue* tempMsg);//数据解析
    //bool DealFile();//文件数据解析

    //////
    VCI_OpenDevice1     *pOpenDevice;
    VCI_ResetCAN1       *pResetCAN;
    VCI_CloseDevice1    *pCloseDevice;
    VCI_InitCAN1        *pInitCAN;
    VCI_StartCAN1       *pStartCAN;
    VCI_Transmit1       *pTransmitCAN;
    VCI_Receive1        *pReceive;
    VCI_GetReceiveNum1  *pGetReceiveNum;
    VCI_ClearBuffer1    *pClearBuffer;
    VCI_ReadErrInfo1    *pReadErrInfoCAN;
    VCI_ReadCANStatus1  *pVCI_ReadCANStatus;  ////
    
public:
    VCI_INIT_CONFIG init_config;//初始化can的结构体

    CANValue canvalue;
    CANARRAY can_array_value;
    QVector<_VCI_CAN_OBJ> zlg_receivemsg;//接收的缓冲区
    QVector<CANARRAY> zlg_readmsg;//读取文件的缓冲区
    int j;
private:
    bool m_btest;//读写标识
    int m_recvCount;//接收计数
    ULONG m_testCount;//发送计数
    int m_connect;//连接标识
    DWORD nDevType;//设备类型号
    DWORD nDevIndex;//设备索引号
    DWORD dwRel;
    int canNum;//第几路CAN

    bool work(/*QString fileName*/);
signals:
    void mySignal();


};

#endif // RECEIVCETEST_H
