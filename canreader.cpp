#include "canreader.h"
#include <QDebug>
#include "include/canlib.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>

extern volatile bool processingFlag;
CanMessage canmsg;
//------------------------------------------------------------------------------
CanReader::CanReader(int ch)
{
    can_channel = ch;
    lib=new Canlib();
    memset(&canmsg,0,sizeof(CanMessage));
    memset(&canvalue,0,sizeof(CANValue));
}
CanReader::~CanReader()
{
    delete lib;
    lib=NULL;

    //内存清空
    QVector<CanMessage> pNULL;
    v_receivemsg.swap(pNULL);
    QVector<CANARRAY> aNULL;
    v_readmsg.swap(aNULL);
}

//------------------------------------------------------------------------------
void CanReader::run(/*QString fileName*/)
{
    work(/*fileName*/); 
}


bool CanReader::init_can(void)
{
    //2.读取数据   初始化-》打开通道-》读取数据-》关闭通道
    canStatus stat;
    lib->canInitializeLibrary();

    stat =lib->canOpenChannel(0,0);
    //channel 通道0 1无关   获取正在开启的通道标号
    if (stat != canOK)
    {
        qDebug() << QString("Error opening channel ") << stat;
        return false;
    }
    stat = lib->canBusOn();
    if(stat != canOK)
    {
        qDebug() << QString("Error going buson").arg(stat);
        return false;
    }

    return true;
}

/*void  CanReader::swap32(void)
{
//处理msg的大小端问题
//for(int i=0;i<2;i++)
//{
//  canmsg.msg[i]=CAN_NTOHL(canmsg.msg[i]);
//}
}*/
//------------------------------------------------------------------------------
bool CanReader::work(/*QString fileName*/)
{

    int static i = 0;
    int idx = 0;
    canStatus stat;
    stat = lib->canRead(&canmsg);

    if(stat == canOK)
    {
        v_receivemsg.push_back(canmsg);

        CanMessage aa = v_receivemsg.takeFirst();
        CanMessage *bb = &aa;
        if(bb == NULL) return false;

        AnalyseData(bb/*&v_receivemsg.takeFirst()*/,&canvalue);

        idx = i%64;

        memcpy(&(can_array_value.CANDATA[idx]),&canvalue,sizeof(CANValue));
        //

        //处理数据 保存到数组
        if((i>0)&&(idx == 0))
        {
            v_readmsg.push_back(can_array_value);
            j++;                                       //????????
//            //文件存储数据
//            if(!saveFile("ESRcan"))
//            {
//                qDebug() << QString("Couldn't save File.");
//                return false;
//            }
        }
        i = i+1;

     }
    else
    {
        qDebug()<<"未收到数据";
    }
    return true;
}

bool CanReader::get_can_status(void)
{
     canStatus state;
    //wait for the messages to be sent before going busoff
    state = canOK;  //lib->canReadSync(1000);   //CAN 一直等待读取数据
    if(state != canOK)
    {
        qDebug() << QString("Couldn't receive the messages within the 1000ms.");
        return false;
    }
    state = lib->canBusOff(/*handle*/);
      if(state != canOK)
      {
         qDebug()<< QString("Error going busoff").arg(state);
      }
      return true;
}


//------------------------------------------------------------------------------
void CanReader::stopRunning()
{
    is_running = false;
}

bool CanReader::saveFile(QString fileName)
{
    //文件名：系统时间  保存为txt格式    只写形式打开   将文件原来内容清空
//    int static j=0;//标记元素个数
    fileName=fileName+".txt";
    QFile data(fileName);
    if(data.open(QIODevice::ReadWrite|QFile::Append|QIODevice::Text))//|QFile::Truncate
    {
        QTextStream out(&data);

        //if((!v_readmsg.isEmpty())&& (j <= v_readmsg.count()))

        while(!v_readmsg.isEmpty())
        {
            can_array_value=v_readmsg.takeFirst();//.at(j);
            //if(j==0) out<<"ID  val   ang   rang  isTar"; //窗口存文件待试   自行存文件重复64
            //数据保存到文件
            for(int k=0;k<TAR_NUM_MAX;k++)
            {
                out << endl;
                out << left << can_array_value.CANDATA[k].ID << " ";
                out << can_array_value.CANDATA[k].valocity << " ";
                out << can_array_value.CANDATA[k].angle << " ";
                out << can_array_value.CANDATA[k].range << " ";
                out << can_array_value.CANDATA[k].isTarExsit;
            }

           // if(v_readmsg.isEmpty())
           // {
            //    sleep(50);
           // }
//             qDebug() << j;
        }


    }

    data.close();  //close file
    return true;
}

//
bool CanReader::AnalyseData(CanMessage* temp,CANValue* tempMsg)
{
    if(tempMsg == NULL) return false;

    short valocity = (temp->msg[0]&0xff)<<8 | (temp->msg[1]&0xff);
    short angle = (temp->msg[2]&0xff)<<8 | (temp->msg[3]&0xff);
    unsigned short range = (temp->msg[4]<<8&0xff00) | (temp->msg[5]&0xff);
    short isTar = (temp->msg[6]&0xff)<<8 |(temp->msg[7]&0xff);
    tempMsg->ID = temp->id;
    tempMsg->angle = angle*0.01;
    tempMsg->valocity = valocity*0.01;
    tempMsg->range = range*0.01;
    tempMsg->isTarExsit = isTar *0.01;
    return true;
}
