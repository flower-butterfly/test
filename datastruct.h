#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#define TAR_NUM_MAX 64
#include <windows.h>
typedef struct _CANValue{
    //UINT time;                  //时间戳
    int ID;                     //ID号
    double valocity;            //速度
    double angle;               //角度
    double range;               //距离  无符号
    double isTarExsit;          //目标存在
} CANValue;

class CANARRAY{
public:
    CANValue CANDATA[TAR_NUM_MAX];
};

#endif // DATASTRUCT_H
