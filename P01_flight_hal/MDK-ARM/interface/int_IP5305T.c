#include "int_IP5305T.h"
#include "main.h"
/**
 * @brief 启动IP5305T电源，避免自动关机
 * 
 */
void Int_IP5305T_start(void)
{
    // 拉低POWER_KEY_Pin 100ms 后拉高POWER_KEY_Pin，模拟按键按下，启动电源
    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_RESET);
    vTaskDelay(100);
    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_SET);
}

/**
 * @brief 关闭IP5305T电源，关机
 * 
 */
void Int_IP5305T_shutdown(void)
{
    //模拟1s内按下2次按键，关闭电源
    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_RESET);
    vTaskDelay(100);
    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_SET);
    
    vTaskDelay(200);
    
    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_RESET);
    vTaskDelay(100);
    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_SET);
}

