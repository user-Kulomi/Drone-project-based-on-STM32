#include "App_flight.h"

Gyro_Accel_struct gyro_acc_data; // 存储陀螺仪和加速度计数据的结构体
Euler_struct euler_angle_data; // 存储欧拉角数据的结构体
Gyro_struct last_gyro_data; 
/**
 * @brief 获取欧拉角
 * 
 */
void App_flight_get_euler_angle(void)
{
    //1.使用MPU6050硬件接口获取六轴数据：
    Int_MPU6050_Get_Data(&gyro_acc_data);

    //2. 对角速度进行低通滤波：
    //滤波原理：滤波后的值 = 滤波系数 * 当前值 + (1 - 滤波系数) * 上一次滤波后的值，是单片机常用的一种低通滤波方式。
    //x轴：
    gyro_acc_data.gyro_data.gyro_x = Common_Filter_LowPass(gyro_acc_data.gyro_data.gyro_x, last_gyro_data.gyro_x);
    //y轴：
    gyro_acc_data.gyro_data.gyro_y = Common_Filter_LowPass(gyro_acc_data.gyro_data.gyro_y, last_gyro_data.gyro_y);
    //z轴：
    gyro_acc_data.gyro_data.gyro_z = Common_Filter_LowPass(gyro_acc_data.gyro_data.gyro_z, last_gyro_data.gyro_z);
    
    //更新过去值：
    last_gyro_data.gyro_x = gyro_acc_data.gyro_data.gyro_x;
    last_gyro_data.gyro_y = gyro_acc_data.gyro_data.gyro_y;
    last_gyro_data.gyro_z = gyro_acc_data.gyro_data.gyro_z;

    //打印三轴角速度数据：
    // debug_printf(":%d,%d,%d\n", gyro_acc_data.gyro_data.gyro_x, gyro_acc_data.gyro_data.gyro_y, gyro_acc_data.gyro_data.gyro_z);

    //3. 对于波动比较大的加速度，使用更高级的滤波方式进行滤波，即卡尔兹曼滤波：
    //卡尔兹曼滤波器知道怎么用即可，不必深入理解其原理
    gyro_acc_data.acc_data.accel_x = Common_Filter_KalmanFilter(&kfs[0], gyro_acc_data.acc_data.accel_x);
    gyro_acc_data.acc_data.accel_y = Common_Filter_KalmanFilter(&kfs[1], gyro_acc_data.acc_data.accel_y);
    gyro_acc_data.acc_data.accel_z = Common_Filter_KalmanFilter(&kfs[2], gyro_acc_data.acc_data.accel_z);

    //打印三轴加速度数据：
    debug_printf(":%d,%d,%d\n", gyro_acc_data.acc_data.accel_x, gyro_acc_data.acc_data.accel_y, gyro_acc_data.acc_data.accel_z);

}
