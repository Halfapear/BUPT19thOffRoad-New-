#include "steer.h"

//舵机初始化
void Servo_Init(void)
{
    pwm_init(Servo_PWM_Pin,Servo_Motor_FREQ ,300);
}
//舵机控制输出
void Servo_Ctrl(uint16_t duty)
{
    if (duty >= Servo_Left_Max)                  //限制幅值
     duty = Servo_Left_Max;
    else if (duty <= Servo_Right_Min)            //限制幅值
     duty = Servo_Right_Min;
    CurrentServoDty = duty;
    pwm_set_duty(Servo_PWM_Pin,duty);
}