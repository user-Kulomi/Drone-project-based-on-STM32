#include "App_receive_data.h"

extern Remote_Data remote_data ; //定义遥控器数据结构体变量

uint8_t rx_buff[TX_PLOAD_WIDTH] = {0}; //接收数据缓冲区

extern Remote_State remote_state;
uint8_t Try_count = 0; //尝试连接次数

/** 
* @brief 接收遥控器发送的数据
*
* @return uint8_t: 处理结果。0表示校验通过，数据正确。1表示校验失败或者未接收到数据
*/
uint8_t App_receive_data(void)
{
    memset(rx_buff, 0, TX_PLOAD_WIDTH);

    // //原始版本：
    // //调用SI24R1接收数据包：
    // Int_SI24R1_RxPacket(rx_buff);
    // if(strlen((char*)rx_buff) == 0)
    // {
    //     debug_printf(":未接收到数据");
    //     return 1; //未接收到数据
    // }

    //更标准的写法：
    uint8_t rec_res = Int_SI24R1_RxPacket(rx_buff);
    if(rec_res == 1)
    {
        // debug_printf(":未接收到数据\r\n");
        return 1;
    }

    //对接收到的数据进行校验：

    //1.帧头校验：
    if(rx_buff[0] != FRAME_HEAD_CHECK_VALUE_1 || rx_buff[1] != FRAME_HEAD_CHECK_VALUE_2 || rx_buff[2] != FRAME_HEAD_CHECK_VALUE_3)
    {
        debug_printf(":帧头校验失败");
        return 1; //帧头校验失败
    }

    //2.帧尾校验：
    uint32_t sum = 0;//计算接收到的数据的和
    uint32_t sum_check = 0;//接收到的数据的和的标准校验值

    for(uint8_t i = 0; i < 13; i++)
    {
        sum += rx_buff[i];
    }
    //按高位在前解析标准校验值：
    sum_check = rx_buff[13] << 24 | rx_buff[14] << 16 | rx_buff[15] << 8 | rx_buff[16];
    if(sum != sum_check)
    {
        debug_printf(":帧尾校验失败");
        return 1; //帧尾校验失败
    }

    //3.保存数据：
    remote_data.thr = (rx_buff[3] << 8) | rx_buff[4];
    remote_data.yaw = (rx_buff[5] << 8) | rx_buff[6];
    remote_data.pit = (rx_buff[7] << 8) | rx_buff[8];
    remote_data.rol = (rx_buff[9] << 8) | rx_buff[10];
    remote_data.shutdown = rx_buff[11];
    remote_data.fix_height = rx_buff[12];

    debug_printf(":%d,%d,%d,%d,%d,%d\n",remote_data.thr, remote_data.yaw, remote_data.pit, remote_data.rol, remote_data.shutdown, remote_data.fix_height);
    return 0; //数据接收并校验成功
}

void process_connect_state(uint8_t res)
{
    if(res == 0)
    {
        remote_state = REMOTE_CONNECT; //连接成功
        Try_count = 0; //连接成功，重置尝试连接次数
    }
    else
    {
        Try_count++; //增加尝试连接次数
        if(Try_count >= MAX_RETRY_CONNECT_COUNT)
        {
            remote_state = REMOTE_DISCONNECT; //连接失败
            Try_count = 0; //重置尝试连接次数
        }
    }
}
