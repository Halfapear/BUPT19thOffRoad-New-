#ifndef STEER_H_
#define STEER_H_
#include "zf_common_headfile.h"
#include "common.h"
#define Servo_Delta              (90)//ת����Χ
#define Servo_Center_Mid         (730)//�����ֵ
#define Servo_Left_Max           (Servo_Center_Mid+Servo_Delta) //�����ת���ֵ
#define Servo_Right_Min          (Servo_Center_Mid-Servo_Delta) //�����ת���ֵ

#define Servo_Motor_FREQ         (50)//���Ƶ��
#define Servo_PWM_Pin            ( TCPWM_CH13_P00_3)//����ӿ�

void Servo_Init(void);//�����ʼ��
void Servo_Ctrl(uint16_t duty);//����������
#endif