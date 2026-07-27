#ifndef APP_PROCESS_DATA_H
#define APP_PROCESS_DATA_H

#include "int_key.h"
#include "Com_debug.h"

typedef struct 
{
    int16_t thr;
    int16_t yaw;
    int16_t pit;
    int16_t rol;
    uint8_t shutdown;
    uint8_t fix_height;
} Remote_Data;
/**
 * @brief 处理按键数据：如果有按键按下，则进行对应的记录
 * 
 */
void App_process_key_data(void);


#endif // APP_PROCESS_DATA_H

