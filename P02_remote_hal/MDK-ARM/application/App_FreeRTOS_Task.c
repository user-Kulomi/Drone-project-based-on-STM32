#include "App_FreeRTOS_Task.h"

//电源管理任务
void power_task(void *pvParameters);
#define POWER_TASK_STACK_SIZE  128
#define POWER_TASK_PRIORITY    4
TaskHandle_t power_task_handle;

void App_FreeRTOS_start(void)//启动FreeRTOS
{
    //创建电源管理任务
    xTaskCreate(power_task, "power_task", POWER_TASK_STACK_SIZE, NULL, POWER_TASK_PRIORITY, &power_task_handle);
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

