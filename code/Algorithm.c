/*
 * Algorithm.c
 *
 *  Created on: 2023��3��19��
 *      Author: ORRN
 */
#include "zf_common_headfile.h"

//===================================================LQ_PID===================================================

pid_param_t PID_GPS;
pid_param_t PID_IMU;
pid_param_t PID_Init;
pid_param_t PID_MOTOR;

/*************************************************************************
 *  �������ƣ�float constrain_float(float amt, float low, float high)
 *  ����˵�����޷�����
 *  ����˵����
  * @param    amt   �� ����
  * @param    low   �� ���ֵ
  * @param    high  �� ���ֵ
 *  �������أ���
 *  �޸�ʱ�䣺2020��4��1��
 *  ��    ע��
 *************************************************************************/
float constrain_float(float amt, float low, float high)
{
    return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}

// pid������ʼ������
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
 *  �������ƣ�float constrain_float(float amt, float low, float high)
 *  ����˵����pidλ��ʽ���������
 *  ����˵����
  * @param    pid     pid����
  * @param    error   pid�������
 *  �������أ�PID������
 *  �޸�ʱ�䣺2020��4��1��
 *  ��    ע��
 *************************************************************************/
float PidLocCtrl(pid_param_t * pid, float error)
{
//    PID_GPS.kp=1;
//    PID_GPS.kd=1.05;


    PID_GPS.kp=1.1;
    PID_GPS.kd=5;

    PID_IMU.kp=1;//1.15
    PID_IMU.kd=7;

    /* �ۻ���� */
    pid->integrator += error;

    /* ����޷� */
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
 *  �������ƣ�float constrain_float(float amt, float low, float high)
 *  ����˵����pid����ʽ���������
 *  ����˵����
  * @param    pid     pid����
  * @param    error   pid�������
 *  �������أ�PID������   ע���������Ѿ��������ϴν��
 *  �޸�ʱ�䣺2020��4��1��
 *  ��    ע��
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


//===================================================GPS��IMU�����˲�===================================================

extern float Daty_Z;//�����ǻ��ֵõ���ֵ(��Ƶ����)

void GPS_IMU_COM_filtering()
{
    float K=0.9;//����ϵ��
    float Fusion_angle;//�ںϺ�ĺ����

    Fusion_angle=K*Daty_Z+(1-K)*gps_tau1201.direction;//�����ֵ�YAW�����GPS��direction���л����ں�

    printf("\r\nFusion_angle---%f",Fusion_angle);

}

//===================================================GPS��IMU�����˲�===================================================


//===================================================����ƽ���˲�===================================================

move_filter_struct GPS_Direction_filter;

//-------------------------------------------------------------------------------------------------------------------
// �������     ����ƽ���˲�����
// ����˵��     move_filter     �ṹ�崫��
// ����˵��     new_data        �����������
// ʹ��ʾ��     (&speed_filter, 400);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void move_filter_calc(move_filter_struct *move_filter, int32 new_data)
{
    // �����µ���ֵ ��ȥ��ĩβ����ֵ ������µĺ�
    move_filter->data_sum = move_filter->data_sum + new_data - move_filter->data_buffer[move_filter->index];
    // ������ƽ��ֵ
    move_filter->data_average = move_filter->data_sum / move_filter->buffer_size;

    // ������д�뻺����
    move_filter->data_buffer[move_filter->index] = new_data;
    move_filter->index ++;
    if(move_filter->buffer_size <= move_filter->index)
    {
        move_filter->index = 0;
    }
}


//-------------------------------------------------------------------------------------------------------------------
// �������     ����ƽ���˲���ʼ��
// ����˵��     move_filter     �ṹ�崫��
// ʹ��ʾ��     move_filter_init(&speed_filter);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void move_filter_init(move_filter_struct *move_filter)
{
    move_filter->data_average   = 0;
    move_filter->data_sum       = 0;
    move_filter->index          = 0;
    //���û�������С
    move_filter->buffer_size    = MOVE_AVERAGE_SIZE;

    uint8 i;
    for(i = 0; i < move_filter->buffer_size; i ++)
    {
        move_filter->data_buffer[i] = 0;
    }
}


//===================================================����ƽ���˲�===================================================

//===================================================���㻺��������Ԫ�ظ���===================================================
int ARRAY_Element_Calculation()
{
    int i = 0;      // ���������
    int count = 0;  // ��������Ԫ�ظ���������

    // �������飬��������Ԫ�ظ���
    while (i < 1024 && flash_union_buffer[i].uint32_type !=(-1)&&flash_union_buffer[i].uint32_type !=(0)) {
        count++;
        i++;
    }
//    printf("\r\n count=%d",count);
    return count;
}



//===================================================��̬PID===================================================
typedef struct//�����ṹ��
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
    float error = pid->setpoint - input;  //Ŀ����-��ǰ��
    float proportional = 0.0;             //����
    float integral = 0.0;                 //����
    float derivative = 0.0;               //΢��

    //���������
    float kp_range = pid->kp_max -pid->kp_min;
    float kp_offset=(error / pid->setpoint)* kp_range;
    proportional = pid->kp_min + kp_offset;

    //���������
    pid->error_sum+= error* dt;
    float ki_range = pid->ki_max - pid->ki_min;
    float ki_offset =(pid->error_sum / pid->setpoint)* ki_range;
    integral = pid->ki_min + ki_offset;

    //����΢����
    float kd_range = pid->kd_max - pid->kd_min;
    float kd_offset =((error-pid->last_error)/ dt / pid->setpoint)* kd_range;
    derivative = pid->kd_min + kd_offset;

    //������һ�ε����
    pid->last_error = error;
    float output = proportional + integral + derivative;

    return output;
}


//===================================================��̬PID===================================================




