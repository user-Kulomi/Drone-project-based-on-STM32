#ifndef INT_JOYSTICK_H
#define INT_JOYSTICK_H

#include "adc.h"
typedef struct
{
    int16_t thr;//油门
    int16_t yaw;//偏航角
    int16_t pit;//俯仰角
    int16_t rol;//滚转角
} Joystick_struct;

/**
 * @brief 初始化ADC摇杆，打开ADC
 * 
 */
void Int_joystick_init(void);

/**
 * @brief 获取摇杆的ADC数据，保存到结构体地址中 
 * 
 * @param x 
 * @param y 
 */
void Int_joystick_get(Joystick_struct *joystick);

#endif // INT_JOYSTICK_H
