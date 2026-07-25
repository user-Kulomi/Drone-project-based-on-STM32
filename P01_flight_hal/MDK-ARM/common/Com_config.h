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
#endif // __COM_CONFIG_H
