#ifndef CANREADER_H
#define CANREADER_H
#define CAN_HTONS(x) ((((x) & 0xff) << 8) | (((x) & 0xff00) >> 8))
#define CAN_NTOHS(x) CAN_HTONS(x)
#define CAN_HTONL(x) ((((x) & 0xff) << 24) | \
                     (((x) & 0xff00) << 8) | \
                     (((x) & 0xff0000UL) >> 8) | \
                     (((x) & 0xff000000UL) >> 24))
#define CAN_NTOHL(x) CAN_HTONL(x)


#define CAN_DEAL_LENGTH 64
#include <QThread>
#include <QVector>
#include <QTimer>

#include "include/canlib.h"
#include "canlib_class.h"



#include "datastruct.h"


class CanReader:public QThread
{
  Q_OBJECT

private:
  CanHandle handle;
  bool is_running;
  int can_channel;
  CANValue canvalue;//自定义结构体
  CANARRAY can_array_value;
  Canlib* lib;



  bool work(/*QString fileName*/);
public:
  QVector<CanMessage> v_receivemsg;
  QVector<CANARRAY> v_readmsg;

  QTimer* timer;


public:
  int k=0;
  long long j=0;
  CanReader(int ch);
  bool init_can(void);
  bool get_can_status(void);
  void run(/*QString fileName*/);
  void stopRunning();
  bool saveFile(QString fileName);
  bool AnalyseData(CanMessage* temp,CANValue* tempMsg);//数据解析
  //bool DealFile();    //文件数据解析
  CanReader() {};
  ~CanReader();
signals:
  void gotRx(CanMessage msg);
};

#endif // CANREADER_H
