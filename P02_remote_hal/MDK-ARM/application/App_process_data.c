#include "App_process_data.h"


Remote_Data remote_Data = {0};//结构体存储遥控器数据值

//区分一下摇杆的控制值和按键的微调值:

int16_t key_pit_offset = 0;//取前为正
int16_t key_rol_offset = 0;//取右为正

/**
 * @brief 处理按键数据：如果有按键按下，则进行对应的记录
 * 
 */
void App_process_key_data(void)
{
    Key_type Key = Int_key_get();
    //根据按键的值进行记录：
    if(Key == KEY_UP)
    {
        //向前飞微调，俯仰角+10
        key_pit_offset += 10;
    }
    else if(Key == KEY_DOWN)
    {
        //向后飞微调，俯仰角-10
        key_pit_offset -= 10;
    }
    else if(Key == KEY_LEFT)
    {
        //向左飞微调，滚转角-10
        key_rol_offset -= 10;
    }
    else if(Key == KEY_RIGHT)
    {
        //向右飞微调，滚转角+10
        key_rol_offset += 10;
    }
    else if(Key == KEY_LEFT_X)
    {
        //左上角按键：关机
        remote_Data.shutdown = 1;
    }
    else if(Key == KEY_RIGHT_X)
    {
        //右上角按键：定高
        remote_Data.fix_height = 1;
    }
    else if(Key == KEY_RIGHT_X_LONG)
    {
        //右上角按键长按：更新摇杆校准数据，并清零微调值
    }
}

