#include "steer.h"

//�����ʼ��
void Servo_Init(void)
{
    pwm_init(Servo_PWM_Pin,Servo_Motor_FREQ ,300);
}
//����������
void Servo_Ctrl(uint16_t duty)
{
    if (duty >= Servo_Left_Max)                  //���Ʒ�ֵ
     duty = Servo_Left_Max;
    else if (duty <= Servo_Right_Min)            //���Ʒ�ֵ
     duty = Servo_Right_Min;
    CurrentServoDty = duty;
    pwm_set_duty(Servo_PWM_Pin,duty);
}