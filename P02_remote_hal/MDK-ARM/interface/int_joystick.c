#include "int_joystick.h"

uint16_t adc_buff[4] = {0};

/**
 * @brief 初始化ADC摇杆,即打开ADC
 * 
 */
void Int_joystick_init(void)
{
    //直接使用HAL库中的函数打开ADC
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buff, 4);//参数1：ADC句柄，参数2：ADC缓冲区地址（16位的数据地址其实是32位，
                                                       //32位的数据地址也是32位），参数3：ADC缓冲区长度
    
}


/**
 * @brief 获取摇杆的ADC数据，保存到结构体地址中 
 * 
 * @param joystick 
 */
void Int_joystick_get(Joystick_struct *joystick)
{
    //DMA转换不依赖于CPU，而且是循环扫描，因此读取的数据是实时保存到adc_buff中的
    //原理图中定义了4个ADC通道，分别对应2个摇杆共4个ADC通道的ADC值，依次是thr,yaw,pit,rol。因此将缓存区的值读取到结构体中的顺序不能搞错
    joystick-> thr = adc_buff[0];
    joystick-> yaw = adc_buff[1];
    joystick-> pit = adc_buff[2];
    joystick-> rol = adc_buff[3];
}
