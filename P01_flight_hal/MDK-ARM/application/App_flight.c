#include "App_flight.h"

Gyro_Accel_struct gyro_acc_data; // 存储陀螺仪和加速度计数据的结构体
Euler_struct euler_angle_data; // 存储欧拉角数据的结构体
Gyro_struct last_gyro_data; 

extern Flight_State flight_state;//飞行状态
extern Remote_Data remote_data;//遥控器数据


//四个方位的电机初始化：
Motor_Struct left_top_motor = {.tim = &htim3, .channel = TIM_CHANNEL_1 ,.speed = 0};
Motor_Struct left_bottom_motor = {.tim = &htim4, .channel = TIM_CHANNEL_4 ,.speed = 0};
Motor_Struct right_top_motor = {.tim = &htim2, .channel = TIM_CHANNEL_2 ,.speed = 0};
Motor_Struct right_bottom_motor = {.tim = &htim1, .channel = TIM_CHANNEL_3,.speed = 0};

float gyro_z_sum = 0;

/**
 * @brief 飞行任务初始化，内含电机启动与MPU6050初始化
 * 
 */
void App_flight_init(void)
{
    //MPU6050初始化：
    Int_MPU6050_Init();

    //启动电机：
    Int_motor_start(&left_top_motor);
    Int_motor_start(&left_bottom_motor);
    Int_motor_start(&right_top_motor);
    Int_motor_start(&right_bottom_motor);

}

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

    //4. 利用加速度与角速度得到飞机倾斜的角度，即姿态解算：

    //使用互补解算计算欧拉角:

    //优先考虑使用加速度解算。由于偏航角无法使用加速度解算，故偏航角使用角速度积分计算，俯仰角与横滚角使用加速度解算：
    //加速度解算的大致原理: 通过反正切函数计算加速度向量与重力方向的夹角来得到俯仰和横滚角
    // euler_angle_data.pitch = atan2(gyro_acc_data.acc_data.accel_x * 1.0 , gyro_acc_data.acc_data.accel_z) / 3.1415926 * 180;//要将角度转为弧度
    // euler_angle_data.roll = atan2(gyro_acc_data.acc_data.accel_y * 1.0 , gyro_acc_data.acc_data.accel_z) / 3.1415926 * 180;//要将角度转为弧度

    // //偏航角使用角速度积分计算：
    // //角速度积分的大致原理是用瞬时角速度乘时间再累加。这里的任务周期是6ms，所以时间间隔是0.006s
    // euler_angle_data.yaw += (gyro_acc_data.gyro_data.gyro_z * 2000 / 32768) * 0.006;//需要将16位ADC值换算为°/s，且量程为±2000°/s
    // //为了保证精度，需要使用浮点数进行一下中转；
    // gyro_z_sum += (gyro_acc_data.gyro_data.gyro_z * 2000 / 32768) * 0.006;
    // euler_angle_data.yaw = gyro_z_sum;


    //也可以用四元数解算：

    //四元数解算的大致原理是用角速度积分得到四元数，再通过四元数转换为欧拉角。了解即可，不必深究
    Common_IMU_GetEulerAngle(&gyro_acc_data,&euler_angle_data,0.006);
    //打印三轴加速度数据：
    // debug_printf(":%d,%d,%d\n", gyro_acc_data.acc_data.accel_x, gyro_acc_data.acc_data.accel_y, gyro_acc_data.acc_data.accel_z);

    // //打印欧拉角：
    // debug_printf(":%.2f,%.2f,%.2f\n", euler_angle_data.pitch, euler_angle_data.roll, euler_angle_data.yaw);
}

/*
* @brief 根据欧拉角计算出PID的目标值
*/
void App_flight_pid_process(void)
{
    //俯仰角：

    //1.需要赋值目标值与测量值
    //处理外环：
    //俯仰角PID的目标值等于遥控器传递的值：
    //需要将遥控器的数据(0~1000)转换为±10°的角度值。遥控器的初始位置对应的值为500
    //所以要看遥控器目前的值与初始值的差值来判断用户想要飞机产生的俯仰角（前后飞行的速度）：
    pitch_pid.desire = (remote_data.pit - 500) / 50.0; //remote_data.pit - 500 代表差值，除50可以把0~1000的数据映射到±10
    //俯仰角PID的测量值等于测量得到的欧拉角对应的pitch数值：
    pitch_pid.measure = euler_angle_data.pitch;

    //处理内环：
    //俯仰角速度（Y轴角速度），即内环的测量值等于测量出的Y轴角速度
    //赋值时需注意单位换算，即将int16转化为0~2000
    gyro_y_pid.measure = (gyro_acc_data.gyro_data.gyro_y * 2000.0 / 32768.0);

    //2.进行PID计算
    Com_PID_Calc_chain(&pitch_pid, &gyro_y_pid);

    //打印结果
    // debug_printf(":%.2f,%.2f\n",gyro_y_pid.err, gyro_y_pid.output);
}

/**
 * @brief 控制电机
 * 
 */
void App_flight_control_motor(void)
{
    //1.判断飞机飞行状态：
    switch (flight_state)
    {
        case IDLE:
        {
            //空闲状态，电机上锁，速度为0
            left_top_motor.speed = 0;
            left_bottom_motor.speed = 0; 
            right_top_motor.speed = 0; 
            right_bottom_motor.speed = 0; 
            break;
        }
        case NORMAL:
        {
            //处理俯仰角：
            //飞机向前飞，会在角速度上产生一个正的误差。为了抵抗向前飞的趋势，需要施加一个向后飞的趋势
            //将前两个电机调快，后两个电机调慢即可。调快与调慢的值均等于PID输出值。
            //通过修改遥控器的油门数据来实现电机的转速调整：
            left_top_motor.speed = remote_data.thr + gyro_y_pid.output;
            left_bottom_motor.speed = remote_data.thr - gyro_y_pid.output; 
            right_top_motor.speed = remote_data.thr + gyro_y_pid.output; 
            right_bottom_motor.speed = remote_data.thr - gyro_y_pid.output; 
            break;
        }
        case FIX_HEIGHT:
        {

            break;
        }
        case FAIL:
        {

            break;
        }
        default:
            break;
    }

    //2.设置电机速度：

    //限速：
    left_top_motor.speed = com_limit(left_top_motor.speed, 400, 0);
    left_bottom_motor.speed = com_limit(left_bottom_motor.speed, 400, 0); 
    right_top_motor.speed = com_limit(right_top_motor.speed, 400, 0); 
    right_bottom_motor.speed = com_limit(right_bottom_motor.speed, 400, 0); 

    //设置速度：
    Int_motor_set_speed(&left_top_motor);
    Int_motor_set_speed(&left_bottom_motor);
    Int_motor_set_speed(&right_top_motor);
    Int_motor_set_speed(&right_bottom_motor);
}

