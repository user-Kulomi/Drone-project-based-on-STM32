#include "int_mpu6050.h"

/**
 * @brief 向MPU6050指定寄存器写入数据
 * 
 * @param reg 寄存器地址
 * @param data 写入数据
 */
void Int_MPU6050_Write_Reg(uint8_t reg, uint8_t data)
{
    // 使用HAL库的I2C写入函数向MPU6050指定寄存器写入数据
    //参数1：I2C句柄，参数2：从设备地址，参数3：寄存器地址(带有读写位)，参数4：寄存器地址位数（8位）
    //参数5：要写入的数据的首地址（支持写入多字节），参数6：数据长度，参数7：超时写入时间
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR_WRITE, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 1000);
}

/**
 * @brief 从MPU6050指定寄存器读取数据
 * 
 * @param reg 寄存器地址
 * @param data 存放读取到的数据的首地址
 */
void Int_MPU6050_Read_Reg(uint8_t reg, uint8_t *data)
{
    // 使用HAL库的I2C读取函数从MPU6050指定寄存器读取数据
    //参数1：I2C句柄，参数2：从设备地址，参数3：寄存器地址(带有读写位)，参数4：寄存器地址位数（8位）
    //参数5：存放读取到的数据的首地址（支持读取多字节），参数6：数据长度，参数7：超时读取时间
    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR_READ, reg, I2C_MEMADD_SIZE_8BIT, data, 1, 1000);
}

/**
 * @brief 初始化MPU6050
 * 
 */
void Int_MPU6050_Init(void)
{
    //1.重启芯片：

    //1.1 重启MPU6050芯片:

    //通过写电源管理寄存器1的设备复位(0x80)位来重置所有寄存器值，从而重启MPU6050芯片:
    Int_MPU6050_Write_Reg(MPU_PWR_MGMT1_REG, 0x80);

    //1.2 等待重启完成：

    //通过判断电源管理寄存器1的值是否为0x40来确认芯片是否已经重启完成（重启完成，0x6B值会变为0x40，芯片睡眠）:
    uint8_t reg_value = 0;
    do
    {
        Int_MPU6050_Read_Reg(MPU_PWR_MGMT1_REG, &reg_value);
    } while (reg_value == 0x40);

    //1.3 唤醒MPU6050使之进入到正常工作状态：

    //将电源管理寄存器1的睡眠位(0x40)清零，使MPU6050从睡眠模式唤醒，进入正常工作状态:
    Int_MPU6050_Write_Reg(MPU_PWR_MGMT1_REG, 0x00);

    //2.设置合适量程（尽量选择允许范围内尽可能精准的量程）：

    //2.1 设置陀螺仪量程为 ±2000°/s：
    Int_MPU6050_Write_Reg(MPU_GYRO_CFG_REG, 0x18);

    //2.2 设置加速度计量程为 ±8g：
    Int_MPU6050_Write_Reg(MPU_ACCEL_CFG_REG, 0x10);

    //3.关闭中断使能，因为用不到：
    Int_MPU6050_Write_Reg(MPU_INT_EN_REG, 0x00);

    //4.设置用户配置寄存器为0，因为用不到里面的FIFO和拓展I2C功能：
    Int_MPU6050_Write_Reg(MPU_USER_CTRL_REG, 0x00);

    //5.设置采样率：
    //根据香农定理，采样率必须大于等于使用频率的2倍，否则会失真，由于该函数对应的任务周期为6ms，即1s内
    //会使用1000 / 6 ≈ 166.67次数据，因此采样率必须大于等于2 * 166.67 ≈ 333.34Hz，选择采样率为500Hz即可满足要求
    //采样率 = 陀螺仪输出率 / (1 + SMPLRT_DIV)，而陀螺仪默认输出率=1000Hz，因此设置SMPLRT_DIV = 1即可得到500Hz的采样率:
    Int_MPU6050_Write_Reg(MPU_SAMPLE_RATE_REG, 0x01);

    //6.设置低通滤波器：
    //将加速度计和角速度计的低通滤波值分别设为184Hz与188Hz，往0x1A寄存器(低通滤波器)写入0x01即可：
    Int_MPU6050_Write_Reg(MPU_CFG_REG, 1);

    //7.选择使用的系统时钟：
    //选择一个添加了PLL（倍频器）的时钟作为时钟源：
    Int_MPU6050_Write_Reg(MPU_PWR_MGMT1_REG, 0x01);// 写0x01表示选择X轴陀螺仪的PLL作为时钟源

    //8.使能加速度计和陀螺仪：
    Int_MPU6050_Write_Reg(MPU_USER_CTRL_REG, 0x00);
}
