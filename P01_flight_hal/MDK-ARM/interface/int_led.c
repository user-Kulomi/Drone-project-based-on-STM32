#include "int_led.h"

void int_led_turn_on(Led_Struct* led)//开灯
{
    HAL_GPIO_WritePin(led->GPIOx, led->GPIO_Pin, GPIO_PIN_SET);//直接修改引脚电平实现开灯
}

void int_led_turn_off(Led_Struct* led)//关灯
{
    HAL_GPIO_WritePin(led->GPIOx, led->GPIO_Pin, GPIO_PIN_RESET);//直接修改引脚电平实现关灯
}

void int_led_toggle(Led_Struct* led)//翻转灯
{
    HAL_GPIO_TogglePin(led->GPIOx, led->GPIO_Pin);//直接调用HAL现成函数修改引脚电平实现翻转灯
}
