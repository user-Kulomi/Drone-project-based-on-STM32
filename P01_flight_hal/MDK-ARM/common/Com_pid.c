#include "Com_pid.h"

//说明：PID的调参是先调内环再调外环

//俯仰角PID结构体，对应俯仰角的外环：(后续需要进行专业的PID调参)
PID_Struct pitch_pid = {.kp = 0.0, .ki = 0.0, .kd = 0.0};
//Y轴角速度结构体，对应俯仰角的内环：
PID_Struct gyro_y_pid = {.kp = 1.0, .ki = 0.0, .kd = 0.0};

//单次PID计算:
void Com_PID_Calc(PID_Struct* pid)
{
    //1.计算误差值 = 目标值 - 测量值
    pid -> err = pid -> measure - pid -> desire;

    //2.计算积分误差 = 积分系数 * 误差值 * 间隔时间
    pid -> integral += pid -> err ;//先将误差值加起来，后面再乘积分系数与间隔时间

    //3.计算微分误差 = (误差值 - 上一次误差值) / 间隔时间
    if(pid -> last_err == 0)//上次误差第一次为0，故第一次不计算微分误差，提高稳定性
    {
        pid -> last_err = pid -> err;
    }

    float der = pid -> err - pid -> last_err;//先将前后两次误差值相减的结果累加，后面再乘微分系数，除积分时间

    //4.计算输出结果 = 比例部分*比例部分系数 + 积分误差部分*积分部分系数 + 微分误差部分*微分部分系数
    pid -> output = (pid -> kp * pid -> err) + (pid -> ki * pid -> integral * PERIOD) + (pid -> kd * der / PERIOD);

    //5.更新上一次误差值
    pid -> last_err = pid -> err;
}

//串级PID计算:
void Com_PID_Calc_chain(PID_Struct* out_pid,PID_Struct* in_pid)
{
    //1. 先计算外环：
    Com_PID_Calc(out_pid);

    //2. 再将外环输出值作为内环目标值：
    in_pid -> desire = out_pid -> output;

    //3. 最后计算内环：
    Com_PID_Calc(in_pid);
}


/**
* @brief 限制电机速度
*
* @param 原速度
* @param 最大速度
* @param 最小速度
* @return 最终速度
*/
int16_t com_limit(int16_t speed, int16_t max_speed, int16_t min_speed)
{
    if(speed >= max_speed)
    {
        return max_speed;
    }
    else if (speed <= min_speed)
    {
        return min_speed;
    }
    else
    {
        return speed;
    }
}

