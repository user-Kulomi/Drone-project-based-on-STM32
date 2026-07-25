#include "App_FreeRTOS_Task.h"

void task1(void *pvParameters);
#define TASK1_STACK_SIZE 128
#define TASK1_PRIORITY 1
TaskHandle_t task1_handle;

void App_FreeRTOS_start(void)//启动FreeRTOS
{
    //创建任务1：
    xTaskCreate(task1, "Task1", TASK1_STACK_SIZE, NULL, TASK1_PRIORITY, &task1_handle);

    //启动调度器
    vTaskStartScheduler();
}

void task1(void *pvParameters)
{
    while (1)
    {
        printf("task1 running\n");
        vTaskDelay(1000);
    }
}
