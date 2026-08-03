#include "App_flight.h"

Gyro_Accel_struct gyro_acc_data; // 存储陀螺仪和加速度计数据的结构体
Euler_struct euler_angle_data; // 存储欧拉角数据的结构体

/**
 * @brief 获取欧拉角
 * 
 */
void App_flight_get_euler_angle(void)
{
    //直接使用MPU6050硬件接口获取数据
    Int_MPU6050_Get_Data(&gyro_acc_data);

    // //打印三轴角速度数据：
    // debug_printf(":%d,%d,%d\n", gyro_acc_data.gyro_data.gyro_x, gyro_acc_data.gyro_data.gyro_y, gyro_acc_data.gyro_data.gyro_z);

    // //打印三轴加速度数据：
    // debug_printf(":%d,%d,%d\n", gyro_acc_data.acc_data.accel_x, gyro_acc_data.acc_data.accel_y, gyro_acc_data.acc_data.accel_z);
}
