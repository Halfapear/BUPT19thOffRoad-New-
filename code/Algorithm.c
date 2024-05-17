/*
 * Algorithm.c
 *
 *  Created on: 2023年3月19日
 *      Author: ORRN
 */
#include "zf_common_headfile.h"

//===================================================LQ_PID===================================================

pid_param_t PID_GPS;
pid_param_t PID_IMU;
pid_param_t PID_Init;
pid_param_t PID_MOTOR;

/*************************************************************************
 *  函数名称：float constrain_float(float amt, float low, float high)
 *  功能说明：限幅函数
 *  参数说明：
  * @param    amt   ： 参数
  * @param    low   ： 最低值
  * @param    high  ： 最高值
 *  函数返回：无
 *  修改时间：2020年4月1日
 *  备    注：
 *************************************************************************/
float constrain_float(float amt, float low, float high)
{
    return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}

// pid参数初始化函数
void PidInit(pid_param_t * pid)
{
    pid->kp        = 0;
    pid->ki        = 0;
    pid->kd        = 0;
    pid->imax      = 0;
    pid->out_p     = 0;
    pid->out_i     = 0;
    pid->out_d     = 0;
    pid->out       = 0;
    pid->integrator= 0;
    pid->last_error= 0;
    pid->last_derivative   = 0;
    pid->last_t    = 0;
}

/*************************************************************************
 *  函数名称：float constrain_float(float amt, float low, float high)
 *  功能说明：pid位置式控制器输出
 *  参数说明：
  * @param    pid     pid参数
  * @param    error   pid输入误差
 *  函数返回：PID输出结果
 *  修改时间：2020年4月1日
 *  备    注：
 *************************************************************************/
float PidLocCtrl(pid_param_t * pid, float error)
{
//    PID_GPS.kp=1;
//    PID_GPS.kd=1.05;


    PID_GPS.kp=1.1;
    PID_GPS.kd=5;

    PID_IMU.kp=1;//1.15
    PID_IMU.kd=7;

    /* 累积误差 */
    pid->integrator += error;

    /* 误差限幅 */
    constrain_float(pid->integrator, -pid->imax, pid->imax);


    pid->out_p = pid->kp * error;
    pid->out_i = pid->ki * pid->integrator;
    pid->out_d = pid->kd * (error - pid->last_error);

    pid->last_error = error;

    pid->out = pid->out_p + pid->out_i + pid->out_d;

//    if(pid->out>SERVO_MOTOR_LMAX)
//    {pid->out=SERVO_MOTOR_LMAX;}
//    if(pid->out<SERVO_MOTOR_RMIN)
//    {pid->out=SERVO_MOTOR_RMIN;}
    return pid->out;
//    printf("OUT:%d\n",pid->out);
}
/*************************************************************************
 *  函数名称：float constrain_float(float amt, float low, float high)
 *  功能说明：pid增量式控制器输出
 *  参数说明：
  * @param    pid     pid参数
  * @param    error   pid输入误差
 *  函数返回：PID输出结果   注意输出结果已经包涵了上次结果
 *  修改时间：2020年4月1日
 *  备    注：
 *************************************************************************/
float PidIncCtrl(pid_param_t * pid, float error)
{
    PID_MOTOR.kp=110;
    PID_MOTOR.ki=6;
    PID_MOTOR.kd=5;



    pid->out_p = pid->kp * (error - pid->last_error);
    pid->out_i = pid->ki * error;
    pid->out_d = pid->kd * ((error - pid->last_error) - pid->last_derivative);

    pid->last_derivative = error - pid->last_error;
    pid->last_error = error;

    pid->out += pid->out_p + pid->out_i + pid->out_d;
    return pid->out;
}

//===================================================LQ_PID===================================================


//===================================================GPS与IMU互补滤波===================================================

extern float Daty_Z;//陀螺仪积分得到的值(高频噪声)

void GPS_IMU_COM_filtering()
{
    float K=0.9;//互补系数
    float Fusion_angle;//融合后的航向角

    Fusion_angle=K*Daty_Z+(1-K)*gps_tau1201.direction;//将积分的YAW和逐飞GPS的direction进行互补融合

    printf("\r\nFusion_angle---%f",Fusion_angle);

}

//===================================================GPS与IMU互补滤波===================================================


//===================================================滑动平均滤波===================================================

move_filter_struct GPS_Direction_filter;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     滑动平均滤波计算
// 参数说明     move_filter     结构体传参
// 参数说明     new_data        新输入的数据
// 使用示例     (&speed_filter, 400);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void move_filter_calc(move_filter_struct *move_filter, int32 new_data)
{
    // 加上新的数值 减去最末尾的数值 求得最新的和
    move_filter->data_sum = move_filter->data_sum + new_data - move_filter->data_buffer[move_filter->index];
    // 重新求平均值
    move_filter->data_average = move_filter->data_sum / move_filter->buffer_size;

    // 将数据写入缓冲区
    move_filter->data_buffer[move_filter->index] = new_data;
    move_filter->index ++;
    if(move_filter->buffer_size <= move_filter->index)
    {
        move_filter->index = 0;
    }
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     滑动平均滤波初始化
// 参数说明     move_filter     结构体传参
// 使用示例     move_filter_init(&speed_filter);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void move_filter_init(move_filter_struct *move_filter)
{
    move_filter->data_average   = 0;
    move_filter->data_sum       = 0;
    move_filter->index          = 0;
    //设置缓冲区大小
    move_filter->buffer_size    = MOVE_AVERAGE_SIZE;

    uint8 i;
    for(i = 0; i < move_filter->buffer_size; i ++)
    {
        move_filter->data_buffer[i] = 0;
    }
}


//===================================================滑动平均滤波===================================================

//===================================================计算缓冲区数组元素个数===================================================
int ARRAY_Element_Calculation()
{
    int i = 0;      // 定义计数器
    int count = 0;  // 定义数组元素个数计数器

    // 遍历数组，计算数组元素个数
    while (i < 1024 && flash_union_buffer[i].uint32_type !=(-1)&&flash_union_buffer[i].uint32_type !=(0)) {
        count++;
        i++;
    }
//    printf("\r\n count=%d",count);
    return count;
}



//===================================================动态PID===================================================
typedef struct//参数结构体
{
   float kp_min;
   float kp_max;
   float ki_min;
   float ki_max;
   float kd_min;
   float kd_max;
   float setpoint;
   float error_sum;
   float last_error;
}PIDContorller;

void pid_init(PIDContorller*pid,float kp_min,float kp_max,float ki_min,float ki_max,float kd_min,float kd_max,float setpoint)
{
    pid->kp_min=kp_min;
    pid->kp_max=kp_max;
    pid->ki_min=ki_min;
    pid->ki_max=ki_max;
    pid->kd_min=kd_min;
    pid->kd_max=kd_max;
    pid->setpoint=setpoint;
    pid->error_sum=0.0;
    pid->last_error=0.0;

}

float pid_update(PIDContorller *pid, float input, float dt)
{
    float error = pid->setpoint - input;  //目标量-当前量
    float proportional = 0.0;             //比例
    float integral = 0.0;                 //积分
    float derivative = 0.0;               //微分

    //计算比例顶
    float kp_range = pid->kp_max -pid->kp_min;
    float kp_offset=(error / pid->setpoint)* kp_range;
    proportional = pid->kp_min + kp_offset;

    //计算积分项
    pid->error_sum+= error* dt;
    float ki_range = pid->ki_max - pid->ki_min;
    float ki_offset =(pid->error_sum / pid->setpoint)* ki_range;
    integral = pid->ki_min + ki_offset;

    //计算微分项
    float kd_range = pid->kd_max - pid->kd_min;
    float kd_offset =((error-pid->last_error)/ dt / pid->setpoint)* kd_range;
    derivative = pid->kd_min + kd_offset;

    //更新上一次的误差
    pid->last_error = error;
    float output = proportional + integral + derivative;

    return output;
}


//===================================================动态PID===================================================




