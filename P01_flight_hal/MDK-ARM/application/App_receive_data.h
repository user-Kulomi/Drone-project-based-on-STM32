#ifndef APP_RECEIVE_DATA_H
#define APP_RECEIVE_DATA_H

#include "Int_SI24R1.h"
#include "Com_config.h"
#include "string.h"
//定义帧头校验值：
#define FRAME_HEAD_CHECK_VALUE_1 'k'
#define FRAME_HEAD_CHECK_VALUE_2 'l'
#define FRAME_HEAD_CHECK_VALUE_3 'm'

/** 
* @brief 接收遥控器发送的数据
*
* @return uint8_t: 处理结果。0表示校验通过，数据正确。1表示校验失败或者未接收到数据
*/
uint8_t App_receive_data(void);

#endif // APP_RECEIVE_DATA_H
