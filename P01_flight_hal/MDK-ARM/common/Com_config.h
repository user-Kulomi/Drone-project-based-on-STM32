#ifndef __COM_CONFIG_H
#define __COM_CONFIG_H
#include "main.h"

//遥控器状态枚举：
typedef enum
{
    REMOTE_CONNECT = 0,
    REMOTE_DISCONNECT,
}Remote_State;

//飞行状态枚举：
typedef enum
{
    IDLE = 0,//空闲状态
    NORMAL,  //正常飞行状态
    FIX_HIGH,//定高状态
    FAIL,    //故障状态
}Flight_State;

//结构体存储遥控器数据值:
typedef struct 
{
    int16_t thr;        //油门
    int16_t yaw;        //偏航
    int16_t pit;        //俯仰
    int16_t rol;        //翻滚
    uint8_t shutdown;   //关机（默认为0，为0代表不改变状态，为1代表执行关机操作）
    uint8_t fix_height; //定高（默认为0，为0代表不改变状态，为1代表执行定高操作）
} Remote_Data;          

#endif // __COM_CONFIG_H
