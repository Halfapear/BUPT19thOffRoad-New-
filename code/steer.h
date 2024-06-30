#ifndef STEER_H_
#define STEER_H_
#include "zf_common_headfile.h"
#include "common.h"
#define Servo_Delta              (90)//转动范围
#define Servo_Center_Mid         (730)//舵机中值
#define Servo_Left_Max           (Servo_Center_Mid+Servo_Delta) //舵机左转最大值
#define Servo_Right_Min          (Servo_Center_Mid-Servo_Delta) //舵机右转最大值

#define Servo_Motor_FREQ         (50)//舵机频率
#define Servo_PWM_Pin            ( TCPWM_CH13_P00_3)//舵机接口

void Servo_Init(void);//舵机初始化
void Servo_Ctrl(uint16_t duty);//舵机控制输出
#endif