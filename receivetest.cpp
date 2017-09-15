#include "receivetest.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QLibrary>


LARGE_INTEGER litmp;
LONGLONG Qpart1,Qpart2,Useingtime;
double dfMinus,dfFreq,dfTime;

ReceiveTest::ReceiveTest(int type,int index,int canIndex):nDevType(type),nDevIndex(index),canNum(canIndex)
{
    QLibrary lib("usbcan.dll");
    if(true == lib.load())
    {
        qDebug() << "load ok!";
    }

    pOpenDevice = (VCI_OpenDevice1 *)lib.resolve("VCI_OpenDevice");
    pCloseDevice = (VCI_CloseDevice1 *)lib.resolve("VCI_CloseDevice");
    pInitCAN = (VCI_InitCAN1 *)lib.resolve("VCI_InitCAN");
    pStartCAN = (VCI_StartCAN1 *)lib.resolve("VCI_StartCAN");
    pTransmitCAN = (VCI_Transmit1 *)lib.resolve("VCI_Transmit");
    pReceive = (VCI_Receive1 *)lib.resolve("VCI_Receive");
    pGetReceiveNum = (VCI_GetReceiveNum1 *)lib.resolve("VCI_GetReceiveNum");
    pClearBuffer = (VCI_ClearBuffer1 *)lib.resolve("VCI_ClearBuffer");
    pReadErrInfoCAN = (VCI_ReadErrInfo1*)lib.resolve("VCI_ReadErrInfo");



    //初始化can结构体
   memset(&init_config,0,sizeof(VCI_INIT_CONFIG));
   init_config.AccCode=0;             //验证码
   init_config.AccMask=0xffffffff;    //屏蔽码 全部接收
   init_config.Filter=1;              //过滤方式
   init_config.Mode=0;                //正常模式
   init_config.Timing0=0x00;          //定时器0  和波特率有关，1M波特率
   init_config.Timing1=0x1c;          //定时器1
   
   memset(&canvalue,0,sizeof(CANValue));

    m_recvCount=0;
    m_testCount=0;
    m_btest=true;    //false;
    m_connect=0;

    memset(&can_array_value.CANDATA,0,sizeof(CANARRAY));
    j=0;

}
ReceiveTest::~ReceiveTest()
{
  m_connect=0;

  //内存清空
  QVector<VCI_CAN_OBJ> pNULL;
  zlg_receivemsg.swap(pNULL);
  QVector<CANARRAY> aNULL;
  zlg_readmsg.swap(aNULL);


  pOpenDevice = NULL;
  pResetCAN = NULL;
  pCloseDevice = NULL;
  pInitCAN = NULL;
  pStartCAN = NULL;
  pTransmitCAN = NULL;
  pReceive = NULL;
  pGetReceiveNum = NULL;
  pClearBuffer = NULL;
  pReadErrInfoCAN = NULL;
  pVCI_ReadCANStatus = NULL;

}
bool ReceiveTest::init_can()
{
    //打开设备
//   dwRel=VCI_OpenDevice(nDevType,nDevIndex,0);
    dwRel = pOpenDevice(nDevType,nDevIndex,0);  //VCI_OpenDevice(VCI_USBCAN2, 0, 0);
   if(dwRel != STATUS_OK)
   {
       qDebug()<<QString(tr("open ZLG_CAN failed！"));
       return false;
   }
   else
       qDebug() << "open ZLG_CAN succeed";

   //读写特殊设置信息
   //VCI_ReadBoardInfo(nDevType,nDevIndex,&);//获取设备信息
   //初始化某路CAN
   dwRel =  pInitCAN(nDevType,nDevIndex,canNum,&init_config);   //VCI_InitCAN(nDevType,nDevIndex,canNum,&init_config);
   if(dwRel == STATUS_ERR)
   {
       pCloseDevice(nDevType,nDevIndex);    //VCI_CloseDevice(0,0);//nDevType,nDevIndex);
       qDebug()<<QString("初始化设备失败！");
       return false;
   }

   qDebug() << "init ZLG_CAN succeed!";
   m_connect = 1;
    //启动某路CAN
    dwRel = pStartCAN(nDevType,nDevIndex,canNum);  //VCI_StartCAN(nDevType,nDevIndex,canNum);

    if(dwRel == STATUS_ERR)
    {
        pCloseDevice(nDevType,nDevIndex);   //VCI_CloseDevice(0,0);//nDevType,nDevIndex);
        qDebug()<<QString("启动设备失败!");
        return false;
   }
    return true;

}
bool ReceiveTest::get_can_status()
{
     dwRel=pCloseDevice(nDevType,nDevIndex);      //VCI_CloseDevice(nDevType,nDevIndex);//0,0
    if(dwRel == STATUS_ERR)
    {
     qDebug()<<QString("Error going bus!");
     return false;
    }
    return true;
}
void ReceiveTest::run()
{
    work();
}







bool ReceiveTest::work(/*QString fileName*/)
{
    //接收数据
    static int i=0;
    int idx=0;
    VCI_ERR_INFO errinfo;
    int buf_frame_length=0;      //缓冲区没有被读取的帧数据个数
    int rcv_length=0;         //接收到的数据
    bool static isFirstIn = true;
     _VCI_CAN_OBJ frameinfo[TAR_NUM_MAX];//接收的数据帧
    while(1)
    {
        memset(&frameinfo,0,sizeof(_VCI_CAN_OBJ)*TAR_NUM_MAX);

        //sleep(10);
        //if(m_connect==0)  break;
         /*//复位   重新初始化CAN控制器
        dwRel=VCI_ResetCAN(nDevType,nDevIndex,canNum);
        dwRel=VCI_StartCAN(nDevType,nDevIndex,canNum);*/
        buf_frame_length = pGetReceiveNum(nDevType,nDevIndex,canNum); //查看缓冲区有多少数据帧
        //VCI_GetReceiveNum(nDevType,nDevIndex,canNum);

        //缓冲区中有未读取的数据，开始读数据
        if(buf_frame_length!=0)
        {
            //用来接收数据帧数组的首指针    数组长度   等待超时时间
            rcv_length = pReceive(nDevType,nDevIndex,canNum,frameinfo,TAR_NUM_MAX,20);  //VCI_Receive(nDevType,nDevIndex,canNum,frameinfo,100,20);
            //qDebug()<<rcv_length;
            if(rcv_length == 0xFFFFFFFF)//0XFFFFFFFF有错误发生   VCI_ReadErrInfo
            {
                //输出错误码
                pReadErrInfoCAN(nDevType,nDevIndex,canNum,&errinfo);   //VCI_ReadErrInfo(nDevType,nDevIndex,canNum,&errinfo);
                QString key;//转成16进制显示
                key=QString("%1").arg(errinfo.ErrCode,8,16,QLatin1Char('0'));
                qDebug() << key;
                qDebug() << QString("读取错误");

            }
            else
            {
               //接收信息放入缓冲区
                for(int m=0;m<rcv_length;m++)
                {
                  zlg_receivemsg.push_back(frameinfo[m]);

                  VCI_CAN_OBJ aa=zlg_receivemsg.takeFirst();
                  VCI_CAN_OBJ* bb=&aa;
                  if(bb==NULL)return false;

                  DealData(bb,&canvalue);

                  idx=i%64;
                  memcpy(&(can_array_value.CANDATA[idx]),&canvalue,sizeof(CANValue));

                  //处理数据，保存到数组
                  if((i>0)&&(idx==0))
                  {
                      emit mySignal();
                      msleep(10);
                      zlg_readmsg.push_back(can_array_value);

                      if(isFirstIn){
                          //获得CPU计时器的时钟频率
                          QueryPerformanceFrequency(&litmp);//取得高精度运行计数器的频率f,单位是每秒多少次（n/s），
                          dfFreq = (double)litmp.QuadPart;

                          QueryPerformanceCounter(&litmp);//取得高精度运行计数器的数值
                          Qpart1 = litmp.QuadPart; //开始计时

                          Qpart2 = Qpart1;

                          dfMinus = (double)(Qpart1 - Qpart2);//计算计数器值
                          dfTime = dfMinus / dfFreq;//获得对应时间，单位为秒,可以乘1000精确到毫秒级（ms）
                          Useingtime = dfTime*1000;
                          isFirstIn = false;
                      }else{
                          //获得CPU计时器的时钟频率
                          QueryPerformanceFrequency(&litmp);//取得高精度运行计数器的频率f,单位是每秒多少次（n/s），
                          dfFreq = (double)litmp.QuadPart;

                          QueryPerformanceCounter(&litmp);//取得高精度运行计数器的数值
                          Qpart1 = litmp.QuadPart; //开始计时
                          dfMinus = (double)(Qpart1 - Qpart2);//计算计数器值
                          dfTime = dfMinus / dfFreq;//获得对应时间，单位为秒,可以乘1000精确到毫秒级（ms）
                          Useingtime = dfTime*1000;
                      }

                      saveFile("ZLG",Useingtime);

//                      Qpart2 = Qpart1;
                      qDebug() << Useingtime << "帧号：" <<j;
//                      qDebug() << "xxy "<< j;
                      j++;
                  }
                  i++;

                }
              //qDebug() << i;
        }

        }
    }
    return true;
}

/*//自测使用
bool ReceiveTest::Send(int i)
{
    VCI_CAN_OBJ frameSend;//[50];//待发送的数据帧
    ZeroMemory(&frameSend,sizeof(VCI_CAN_OBJ));
    if(m_btest)
    {
       //要发送数据帧数组的首指针    数组长度  返回实际发送的帧数
//        for(int i=0;i<50;i++)
//        {
//            frameSend[i].ID=0X00000080;
//            frameSend[i].RemoteFlag=0;//数据帧
//            frameSend[i].ExternFlag=0;//标准帧
//            frameSend[i].SendType=0;
//            frameSend[i].DataLen=8;
       }
        frameSend.ID=0X00000080;
        frameSend.SendType=2;//0正常发送  1单次发送   2自发自收   3单次自发自收
        frameSend.RemoteFlag=0;//数据帧
        frameSend.ExternFlag=0;//标准帧
        frameSend.DataLen=8;
        //frameSend.Data={0x10,0x01,0x02,0x03,0x04,0x05,0x06,0x07};//{01,02,03,04,05,06,07,08};
            frameSend.Data[0]=i;
        dwRel=VCI_ClearBuffer(nDevType,nDevIndex,canNum);
        ULONG ret=VCI_Transmit(nDevType,nDevIndex,canNum,&frameSend,1);
        if(ret==1)
        {
            m_testCount++;
            //qDebug()<<QString("发送成功");
            return true;
        }     
    }
    return false;
}*/

bool ReceiveTest::saveFile(QString fileName,LONGLONG usetime)
{
    if(fileName==NULL){
        return false;
    }
    fileName=fileName+".txt";
    QFile data(fileName);
    if(data.open(QIODevice::ReadWrite|QFile::Append|QIODevice::Text))
    {
        QTextStream out(&data);

        while(!zlg_readmsg.isEmpty())
        {
            can_array_value=zlg_readmsg.takeFirst();
            //can_array_value=zlg_readmsg.at(j);
            //if(j==0)out<<"ID  val ang  range isTar";//窗口保存文件时待试
            QDateTime dt = QDateTime::currentDateTime();
            QString str = dt.toString("yy-MM-dd ddd hh:mm:ss.zzz");
//            out<<str<<endl;
            for(int k=0;k<TAR_NUM_MAX;k++)
            {

                //out<<can_array_value.CANDATA[k].time<<" ";//!!!!
                out<<left<<usetime<<" ";
                out<<can_array_value.CANDATA[k].ID<<" ";
                out<<can_array_value.CANDATA[k].valocity<<" ";
                out<<can_array_value.CANDATA[k].angle<<" ";
                out<<can_array_value.CANDATA[k].range<<" ";
                out<<can_array_value.CANDATA[k].isTarExsit<<" ";
                out<<endl;
            }
        }

    }
    data.close();
    return true;
}
bool ReceiveTest::DealData(VCI_CAN_OBJ* temp,CANValue* tempMsg)
{
    if(tempMsg == NULL)return false;

    short valocity = (temp->Data[0]&0xff)<<8 | (temp->Data[1]&0xff);
    short angle = (temp->Data[2]&0xff)<<8 | (temp->Data[3]&0xff);
    unsigned short range = (temp->Data[4]<<8&0xff00) | (temp->Data[5]&0xff);
    short isTar = (temp->Data[6]<<8&0xff) | (temp->Data[7]&0xff);
    //tempMsg->time = temp->TimeStamp;
    tempMsg->ID = temp->ID&0xff;
    tempMsg->angle = angle*0.01;
    tempMsg->valocity = valocity*0.01;
    tempMsg->range = range*0.01;
    tempMsg->isTarExsit = isTar*0.01;
    return true;
}

