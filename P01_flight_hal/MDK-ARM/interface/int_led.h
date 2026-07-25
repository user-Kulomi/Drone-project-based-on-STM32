#ifndef INT_LED_H
#define INT_LED_H

#include "main.h"
//定义led灯结构体：
typedef struct
{
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_Pin;
}Led_Struct;

//开关灯：
void int_led_turn_on(Led_Struct* led);
void int_led_turn_off(Led_Struct* led);

//翻转灯：
void int_led_toggle(Led_Struct* led);

#endif // INT_LED_H
