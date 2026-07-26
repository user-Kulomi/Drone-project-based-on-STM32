#include "App_FreeRTOS_Task.h"

//电源管理任务
void power_task(void *pvParameters);
#define POWER_TASK_STACK_SIZE  128
#define POWER_TASK_PRIORITY    4
TaskHandle_t power_task_handle;
#define POWER_TASK_PERIOD 10000 //任务周期

//通信任务
void com_task(void *pvParameters);
#define COM_TASK_STACK_SIZE  128
#define COM_TASK_PRIORITY    3
TaskHandle_t com_task_handle;
#define COM_TASK_PERIOD 6 //任务周期

/*
    启动FreeRTOS：
*/
void App_FreeRTOS_start(void)
{
    //创建电源管理任务
    xTaskCreate(power_task, "power_task", POWER_TASK_STACK_SIZE, NULL, POWER_TASK_PRIORITY, &power_task_handle);

    //创建通信任务
    xTaskCreate(com_task, "com_task", COM_TASK_STACK_SIZE, NULL, COM_TASK_PRIORITY, &com_task_handle);

    //启动调度器
    vTaskStartScheduler();
}

void power_task(void *pvParameters)
{
    //获取当前基准时间
    TickType_t LastWakeTime = xTaskGetTickCount();//获取当前基准时间,作为下面vTaskDelayUntil函数的参数
    while (1)
    {
        //由于电源管理芯片会在一段时间后休眠，所以需要每10s启动一次电源，避免电源关闭
        vTaskDelayUntil(&LastWakeTime, 10000);//使用vtaskdelayuntil函数实现延时，精度更高
        //启动电源：
        Int_IP5305T_start();

    }
}

uint8_t com_buf[TX_PLOAD_WIDTH] = {0};
void com_task(void *pvParameters)
{
    //获取当前基准时间
    TickType_t LastWakeTime = xTaskGetTickCount();//获取当前基准时间,作为下面vTaskDelayUntil函数的参数
    while (1)
    {
        //调用SI24R1接口进行数据发送：
        //1.进入发送模式:
        Int_SI24R1_TX_Mode();
        //编辑发送内容：
        com_buf[0] = 'h';
        com_buf[1] = 'e';
        com_buf[2] = 'l';
        com_buf[3] = 'l';
        com_buf[4] = 'o';
        com_buf[5] = '!';
        //发送字节:
        Int_SI24R1_TxPacket(com_buf);

        //退出发送模式(恢复到接收模式):
		Int_SI24R1_RX_Mode();
        vTaskDelayUntil(&LastWakeTime, COM_TASK_PERIOD);//6ms执行一次
    }
}
