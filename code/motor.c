#include "zf_common_headfile.h"
#include "motor.h"

void motorAndencouder_init(void)
{
    pwm_init(PWM_CH1, 1000, 0);                                                 // PWM ͨ��1 ��ʼ��Ƶ��1KHz ռ�ձȳ�ʼΪ0
    gpio_init(DIR_CH1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                          // ��ʼ����������������
    encoder_dir_init(ENCODER1_TIM, ENCODER1_PLUS, ENCODER1_DIR);                // ��ʼ����������ֵ���ż���ʱ��
}

void PWM_OUT(int16 duty)
{
    if(duty>=MAX_DUTY)
        duty=MAX_DUTY;
    else if(duty<=-MAX_DUTY)
        duty=-MAX_DUTY;
    
    if(duty>=0)
    {
        pwm_set_duty(PWM_CH1,duty);
        gpio_set_level(DIR_CH1,1);
    }
    else
    {
        pwm_set_duty(PWM_CH1,-duty);
        gpio_set_level(DIR_CH1,0);
    }
    
          
}

int16 GetSpeed(void)
{
    int16 encoderdata=0;
    encoderdata=encoder_get_count(ENCODER1_TIM);
    encoder_clear_count(ENCODER1_TIM);
    return encoderdata;
}