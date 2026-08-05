#ifndef COM_PID_H
#define COM_PID_H

#include "Com_config.h"
#include "Com_debug.h"

extern Remote_Data remote_data;         //遥控器数据
extern Euler_struct euler_angle_data;   // 欧拉角数据
extern Gyro_Accel_struct gyro_acc_data; //陀螺仪和加速度计数据

#define PERIOD 0.006f
//PID结构体：(CPU性能够强，可使用double类型提高精度)
//其中kp，ki，kd需要在初始化时设置，目标值desire与测量值measure需要在计算时传递
typedef struct
{
    float kp;// 比例部分，值越大响应越快
    float ki;// 积分部分，用来解决稳态误差。无人机控制中，积分项一般不使用。
    float kd;// 微分部分，值越大，抑制效果越好，用来解决过调震荡
    float err;//误差值
    float desire;//目标值
    float measure;//测量值
    float last_err;//上次误差值
    float integral;//积分误差乘积
    float output;//输出结果
} PID_Struct;

//俯仰角PID结构体，对应俯仰角的外环：(后续需要进行专业的PID调参)
extern PID_Struct pitch_pid;
//Y轴角速度结构体，对应俯仰角的内环：
extern PID_Struct gyro_y_pid;

//单次PID计算:
void Com_PID_Calc(PID_Struct* pid);

//串级PID计算:
void Com_PID_Calc_chain(PID_Struct* out_pid,PID_Struct* in_pid);


/**
* @brief 限制电机速度
*
* @param 原速度
* @param 最大速度
* @param 最小速度
* @return 最终速度
*/
int16_t com_limit(int16_t speed, int16_t max_speed, int16_t min_speed);

#endif 
